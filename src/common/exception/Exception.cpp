#include "eurora/common/exception/Exception.h"

#include <sstream>


namespace eurora::common {

    std::string trimFilePath(const std::string& filePath) {
        size_t pos = filePath.find_last_of("/\\");
        return (pos == std::string::npos) ? filePath : filePath.substr(pos + 1);
    }

    Exception::Exception(ErrorCode code, const std::string& description, const std::string& file, int line)
        : mErrorCode(code), mDescription(description), mFile(trimFilePath(file)), mLine(line) {}

    const char* Exception::what() const noexcept {
        if (mCachedWhat.empty()) {
            std::ostringstream oss;
            oss << getErrorMessage(mErrorCode);
            if (!mDescription.empty()) {
                oss << ": " << mDescription;
            }
            if (!mFile.empty()) {
                oss << " (in " << mFile << ":" << mLine << ")";
            }
            mCachedWhat = oss.str();
        }

        return mCachedWhat.c_str();
    }

    ErrorCode Exception::code() const noexcept {
        return mErrorCode;
    }

    const std::string& Exception::description() const noexcept {
        return mDescription;
    }

} // namespace eurora::common
