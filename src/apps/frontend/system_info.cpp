#include <boost/asio.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <system_error>
#include <vector>

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
#include <sys/sysctl.h>
#include <unistd.h>
#endif

namespace eurora::fe {

constexpr std::string_view IsmrmrdVersion() { return "0.0.0-alpha"; }

constexpr std::string_view EuroraVersion() { return ""; }

constexpr std::string_view EuroraBuild() { return ""; }

size_t SystemMemory() {
#if defined(_WIN32)
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    return static_cast<size_t>(status.ullTotalPhys);
#elif defined(__unix__) || defined(__APPLE__)
#if defined(CTL_HW) && defined(HW_MEMSIZE)              // macOS
    int mib[2]   = {CTL_HW, HW_MEMSIZE};
    int64_t size = 0;
    size_t len   = sizeof(size);
    if (sysctl(mib, 2, &size, &len, nullptr, 0) == 0) {
        return static_cast<size_t>(size);
    }
#elif defined(_SC_PHYS_PAGES) && defined(_SC_PAGESIZE)  // Linux
    return static_cast<size_t>(sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGESIZE));
#endif
#endif
    return 0;
}

bool PythonSupport() { return true; }

bool MatlabSupport() { return false; }

bool JuliaSupport() { return false; }

boost::asio::ip::tcp TcpProtocol() {
#ifdef __linux__
    try {
        std::ifstream ipv6_status("/proc/sys/net/ipv6/conf/all/disable_ipv6");
        if (ipv6_status) {
            int status = 1;
            ipv6_status >> status;
            if (status == 0) {
                return boost::asio::ip::tcp::v6();
            }
        }
    } catch (...) {}
#endif
    return boost::asio::ip::tcp::v4();
}

void PrintSystemInfo(std::ostream& os) {
    os << "Eurora Version Info" << std::endl;
    os << "  -- Version            : " << EuroraVersion() << std::endl;
    os << "  -- Git SHA1           : " << EuroraBuild() << std::endl;
    os << "  -- System Memory Size : " << SystemMemory() / (1024 * 1024) << " MB" << std::endl;
    os << "  -- Python Support     : " << (PythonSupport() ? "Yes" : "No") << std::endl;
    os << "  -- Matlab Support     : " << (MatlabSupport() ? "Yes" : "No") << std::endl;
    os << "  -- Julia Support      : " << (JuliaSupport() ? "Yes" : "No") << std::endl;
    os << std::endl;
}

}  // namespace eurora::fe
