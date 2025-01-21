#pragma once

#include <array>
#include <boost/asio.hpp>
#include <boost/asio/experimental/parallel_group.hpp>
#include <chrono>
#include <coroutine>
#include <string>
#include <variant>

namespace eurora::client {

template <typename Awaitable>
boost::asio::awaitable<typename Awaitable::value_type> WithTimeout(boost::asio::io_context::executor_type executor, Awaitable awaitable,
                                                                   std::chrono::milliseconds timeout) {
    boost::asio::steady_timer timer(executor);
    timer.expires_after(timeout);

    std::variant<typename Awaitable::value_type, boost::system::error_code> result =
        co_await boost::asio::experimental::make_parallel_group(std::move(awaitable), timer.async_wait(boost::asio::use_awaitable))
            .async_wait(boost::asio::experimental::wait_for_one(), boost::asio::use_awaitable);

    if (std::holds_alternative<typename Awaitable::value_type>(result)) {
        co_return std::get<typename Awaitable::value_type>(result);
    } else {
        throw std::runtime_error("Operation timed out.");
    }
}

class ServerConnector {
public:
    ServerConnector(const std::string& host, const std::string& port, std::chrono::milliseconds timeout = std::chrono::seconds(5));

    boost::asio::awaitable<void> Connect();
    void Close();

    void SetTimeout(std::chrono::milliseconds timeout);
    std::chrono::milliseconds GetTimeout();

    boost::asio::awaitable<void> Send(const std::string& data);
    boost::asio::awaitable<std::string> Receive();
    boost::asio::awaitable<std::string> ReceiveFixedSize(std::size_t message_size);

private:
    std::string host_;
    std::string port_;
    std::chrono::milliseconds timeout_;
    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::resolver resolver_;
    boost::asio::ip::tcp::socket socket_;
};

}  // namespace eurora::client
