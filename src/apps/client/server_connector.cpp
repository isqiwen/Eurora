#include "server_connector.h"

#include "eurora/utils/logger.h"

using namespace eurora::utils;

namespace eurora::client {

ServerConnector::ServerConnector(const std::string& host, const std::string& port)
    : io_context_(), resolver_(io_context_), socket_(io_context_), host_(host), port_(port), timeout_(timeout) {}

boost::asio::awaitable<void> ServerConnector::Connect() {
    try {
        auto endpoints = co_await resolver_.async_resolve(host_, port_, boost::asio::use_awaitable);

        co_await boost::asio::async_connect(socket_, endpoints, boost::asio::use_awaitable);
        STREAM_INFO() << "Connection established.";
    } catch (const std::exception& e) {
        STREAM_ERROR() << "Connection failed: " << e.what();
    }
}

boost::asio::awaitable<void> ServerConnector::Send(const std::string& data) {
    co_await boost::asio::async_write(socket_, boost::asio::buffer(data), boost::asio::use_awaitable);
    STREAM_INFO() << "Data sent.";
}

boost::asio::awaitable<std::string> ServerConnector::Receive() {
    std::array<char, 1024> buffer;
    auto n = co_await WithTimeout(socket_.get_executor(), socket_.async_read_some(boost::asio::buffer(buffer), boost::asio::use_awaitable), timeout_);
    co_return std::string(buffer.data(), n);
}

boost::asio::awaitable<std::string> ServerConnector::ReceiveFixedSize(std::size_t message_size) {
    std::string message(message_size, '\0');
    std::size_t total_received = 0;

    while (total_received < message_size) {
        total_received += co_await WithTimeout(
            socket_.get_executor(),
            socket_.async_read_some(boost::asio::buffer(message.data() + total_received, message_size - total_received), boost::asio::use_awaitable), timeout_);
    }

    co_return message;
}

void ServerConnector::SetTimeout(std::chrono::milliseconds timeout) { timeout_ = timeout; }

std::chrono::milliseconds ServerConnector::GetTimeout() const { return timeout_; }

void ServerConnector::Close() {
    if (socket_.is_open()) {
        socket_.close();
        STREAM_INFO() << "Connection closed.";
    }
}

}  // namespace eurora::client
