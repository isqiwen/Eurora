#ifndef EURORA_COMMON_UTILS_STRING_UTILS_
#define EURORA_COMMON_UTILS_STRING_UTILS_

#pragma once

#include <list>
#include <numeric>
#include <string>
#include <vector>

namespace eurora::common {

    class StringUtils {
    public:
        static bool
        EqualsIgnoreCase(
            const std::string&                                      left,
            const std::string&                                      right
        );

        static std::string
        WCharToString(
            const wchar_t*                                          lpcwstrStr
        );

        static std::string
        JoinString(
            const std::vector<std::string>&                         aVec,
            const std::string&                                      aSeparator
        );

        static std::string
        ReplaceAll(
            const std::string&                                      aSource,
            const std::string&                                      aOld,
            const std::string&                                      aNew
        );

        static void
        ReplacedAll(
            std::string&                                            aSource,
            const std::string&                                      aOld,
            const std::string&                                      aNew
        );

        static std::string
        ReplaceAllByRegex(
            const std::string&                                      aSource,
            const std::string&                                      aReplacedPattern,
            const std::string&                                      aNew
        );

        static void
        ReplacedAllByRegex(
            std::string&                                            aSource,
            const std::string&                                      aReplacedPattern,
            const std::string&                                      aNew
        );

        static std::wstring
        ConvertStringToWString(
            const std::string&                                      aString,
            uint64_t                                                aEncodingFlag = 0
        );

        static std::string
        ConvertWStringToString(
            const std::wstring&                                     aString,
            uint64_t                                                aEncodingFlag = 0
        );

        static bool
        StartsWith(
            const std::string&                                      aStr,
            const std::string&                                      aPrefix
        );

        static bool
        EndsWith(
            const std::string&                                      aStr,
            const std::string&                                      aSuffix
        );

        static std::vector<std::string>
        SplitByRegex(
            const std::string&                                      aString,
            const std::string&                                      aPattern
        );

        static void
        TrimLeft(
            std::string&                                            aString,
            const std::string&                                      aChars
        );

        static std::string
        TrimedLeft(
            const std::string&                                      aString,
            const std::string&                                      aChars
        );

        static void
        TrimRight(
            std::string&                                            aString,
            const std::string&                                      aChars
        );

        static std::string
        TrimedRight(
            const std::string&                                      aString,
            const std::string&                                      aChars
        );

        static std::vector<std::string>
        SplitString(
            const std::string&                                      aStr,
            const std::string&                                      aDelimiter
        );

        static bool
        IsMatch(
            const std::string&                                      aString,
            const std::string&                                      aPattern
        );

        static std::string
        ToLowCase(
            const std::string&                                      aString
        );

        static std::string
        ToUpperCase(
            const std::string&                                      aString
        );

        static bool
        IsAllNumber(
            const std::string&                                      aString
        );

        static bool
        IsIDString(
            const  std::string&                                     aString
        );
    };

}

#endif // EURORA_COMMON_UTILS_STRING_UTILS_
