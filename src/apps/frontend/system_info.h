#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <ostream>

namespace eurora::fe {

void PrintSystemInfo(std::ostream&);

boost::asio::ip::tcp TcpProtocol();

constexpr std::string_view IsmrmrdVersion();
constexpr std::string_view EuroraVersion();
constexpr std::string_view EuroraBuild();

size_t SystemMemory();

bool PythonSupport();
bool MatlabSupport();
bool JuliaSupport();

}  // namespace eurora::fe
