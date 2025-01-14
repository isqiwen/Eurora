#include "common/utils/string_utils.h"

#include <memory>
#include <regex>

namespace eurora::common {

bool StringUtils::StartsWith(const std::string& source_str, const std::string& prefix) {
  return source_str.find(prefix) == 0;
}

}  // namespace eurora::common
