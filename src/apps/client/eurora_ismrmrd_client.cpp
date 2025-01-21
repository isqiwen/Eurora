#include <ismrmrd/dataset.h>
#include <ismrmrd/ismrmrd.h>
#include <ismrmrd/meta.h>
#include <ismrmrd/waveform.h>
#include <ismrmrd/xml.h>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>

#include <concepts>
#include <condition_variable>
#include <coroutine>
#include <iomanip>
#include <iostream>
#include <memory>
#include <numeric>
#include <optional>
#include <ranges>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

// Concepts for type constraints
template <typename T>
concept Numeric = std::is_arithmetic_v<T> || requires(T t) {
    {std::complex<typename T::value_type>()};
};

template <typename T>
concept HasData = requires(T t) {
    { t.getDataPtr() } -> std::convertible_to<float*>;
    { t.getDataSize() } -> std::convertible_to<size_t>;
};

// NoiseStatistics structure with optional state
struct NoiseStatistics {
    uint16_t channels;
    float sigma_min;
    float sigma_max;
    float sigma_mean;
    float noise_dwell_time_us;

    static std::optional<NoiseStatistics> fromMeta(const ISMRMRD::MetaContainer& meta) {
        if (meta.as_str("status") != "success") {
            return std::nullopt;
        }
        return NoiseStatistics{static_cast<uint16_t>(meta.as_long("channels")), meta.as_double("min_sigma"), meta.as_double("max_sigma"),
                               meta.as_double("mean_sigma"), meta.as_double("noise_dwell_time_us")};
    }
};

// Asynchronous connection using coroutine
boost::asio::awaitable<void> async_connect(boost::asio::ip::tcp::socket& socket, boost::asio::ip::tcp::resolver& resolver, const std::string& host,
                                           const std::string& port) {
    auto endpoints = co_await resolver.async_resolve(host, port, boost::asio::use_awaitable);
    co_await boost::asio::async_connect(socket, endpoints, boost::asio::use_awaitable);
}

class EuroraClient {
public:
    EuroraClient(const std::string& host, const std::string& port, unsigned int timeout_ms = 10000)
        : host_(host), port_(port), timeout_ms_(timeout_ms), io_context_(), socket_(io_context_) {}

    ~EuroraClient() {
        if (socket_.is_open()) {
            socket_.close();
        }
    }

    void SetTimeout(unsigned int timeout_ms) { timeout_ms_ = timeout_ms; }

    void RegisterReader(uint16_t message_id, std::shared_ptr<EuroraClientMessageReader> reader) { readers_[message_id] = std::move(reader); }

    void SendConfiguration(const std::string& config) {
        GadgetMessageIdentifier id{GADGET_MESSAGE_CONFIG_SCRIPT};
        GadgetMessageScript conf{static_cast<uint32_t>(config.size())};

        boost::asio::write(socket_, boost::asio::buffer(&id, sizeof(id)));
        boost::asio::write(socket_, boost::asio::buffer(&conf, sizeof(conf)));
        boost::asio::write(socket_, boost::asio::buffer(config));
    }

    void Connect() {
        boost::asio::co_spawn(io_context_, async_connect(socket_, resolver_, host_, port_), boost::asio::detached);
        io_context_.run();
    }

    void SendAcquisition(const ISMRMRD::Acquisition& acq) {
        GadgetMessageIdentifier id{GADGET_MESSAGE_ISMRMRD_ACQUISITION};
        boost::asio::write(socket_, boost::asio::buffer(&id, sizeof(id)));
        boost::asio::write(socket_, boost::asio::buffer(&acq.getHead(), sizeof(ISMRMRD::AcquisitionHeader)));

        size_t data_size = acq.getDataSize();
        boost::asio::write(socket_, boost::asio::buffer(acq.getDataPtr(), data_size));
    }

    void Receive() {
        while (true) {
            GadgetMessageIdentifier id;
            boost::asio::read(socket_, boost::asio::buffer(&id, sizeof(id)));

            if (id.id == GADGET_MESSAGE_CLOSE)
                break;

            if (auto it = readers_.find(id.id); it != readers_.end()) {
                it->second->read(socket_);
            } else {
                throw std::runtime_error("Unknown message ID received");
            }
        }
    }

private:
    std::string host_;
    std::string port_;
    unsigned int timeout_ms_;

    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::ip::tcp::resolver resolver_{io_context_};
    std::unordered_map<uint16_t, std::shared_ptr<EuroraClientMessageReader>> readers_;
};

// Abstract base class for message readers
class EuroraClientMessageReader {
public:
    virtual ~EuroraClientMessageReader()                    = default;
    virtual void read(boost::asio::ip::tcp::socket& socket) = 0;
};

// Example reader for ISMRMRD images
class EuroraClientImageReader : public EuroraClientMessageReader {
public:
    explicit EuroraClientImageReader(const std::string& output_file) : output_file_(output_file) {}

    void read(boost::asio::ip::tcp::socket& socket) override {
        ISMRMRD::ImageHeader header;
        boost::asio::read(socket, boost::asio::buffer(&header, sizeof(header)));

        // Handle image data
        std::vector<char> image_data(header.channels * header.matrix_size[0] * header.matrix_size[1]);
        boost::asio::read(socket, boost::asio::buffer(image_data));

        // Save the image to file
        std::ofstream file(output_file_, std::ios::binary);
        file.write(image_data.data(), image_data.size());
    }

private:
    std::string output_file_;
};

int main(int argc, char** argv) {
    namespace po = boost::program_options;

    std::string host, port, input_file, output_file, config_file;
    unsigned int timeout_ms = 10000;

    po::options_description desc("Allowed options");
    desc.add_options()("help,h", "Show help message")("host", po::value<std::string>(&host)->default_value("localhost"), "Gadgetron host")(
        "port", po::value<std::string>(&port)->default_value("9002"), "Gadgetron port")("input,i", po::value<std::string>(&input_file), "Input ISMRMRD file")(
        "output,o", po::value<std::string>(&output_file)->default_value("output.h5"), "Output file")(
        "config,c", po::value<std::string>(&config_file)->default_value("default.xml"), "Configuration file")(
        "timeout,t", po::value<unsigned int>(&timeout_ms)->default_value(10000), "Connection timeout in ms");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }

    try {
        EuroraClient client(host, port, timeout_ms);

        // Example: Register an image reader
        client.RegisterReader(GADGET_MESSAGE_ISMRMRD_IMAGE, std::make_shared<EuroraClientImageReader>(output_file));

        client.connect();
        client.SendConfiguration(config_file);

        // Send acquisitions and receive responses (simplified)
        client.receive();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
