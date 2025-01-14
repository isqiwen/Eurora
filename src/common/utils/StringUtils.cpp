#include "common/utils/StringUtils.h"

#include <memory>
#include <regex>

namespace eurora::common {

std::string StringUtils::JoinString(const std::vector<std::string> &aVec, const std::string &aSeparator)
{
    std::string res;

    auto func = [&aSeparator](const std::string &aA, const std::string &aB) { return aA + aSeparator + aB; };
    res = std::accumulate(aVec.begin(), aVec.end(), res, func);

    return res;
}

std::string StringUtils::ReplaceAll(const std::string &aSource, const std::string &aOld, const std::string &aNew)
{
    std::string result = aSource;
    ReplacedAll(result, aOld, aNew);
    return result;
}

void StringUtils::ReplacedAll(std::string &aSource, const std::string &aOld, const std::string &aNew)
{
    if (aOld == aNew)
    {
        return;
    }
    size_t offset = 0;
    while (true)
    {
        size_t pos = aSource.find(aOld, offset);
        if (std::string::npos == pos)
        {
            break;
        }
        aSource.replace(pos, aOld.length(), aNew);
        offset = pos + aNew.length();
    }
}

std::string StringUtils::ReplaceAllByRegex(const std::string &aSource, const std::string &aReplacedPattern, const std::string &aNew)
{
    std::string result;
    std::string remainString = aSource;
    std::smatch match;
    std::regex regex(aReplacedPattern);
    while (std::regex_search(remainString, match, regex))
    {
        result += match.prefix();
        result += aNew;
        remainString = match.suffix().str();
    }
    result += remainString;
    return result;
}

void StringUtils::ReplacedAllByRegex(std::string &aSource, const std::string &aReplacedPattern, const std::string &aNew)
{
    aSource = ReplaceAllByRegex(aSource, aReplacedPattern, aNew);
}

bool StringUtils::StartsWith(const std::string &aStr, const std::string &aPrefix)
{
    return aStr.find(aPrefix) == 0;
}

bool StringUtils::EndsWith(const std::string &aStr, const std::string &aSuffix)
{
    return aStr.rfind(aSuffix) == aStr.length() - aSuffix.length();
}

std::vector<std::string> StringUtils::SplitByRegex(const std::string &aString, const std::string &aPattern)
{
    std::vector<std::string> results;
    std::string remainString = aString;
    std::smatch match;
    std::regex regex(aPattern);
    while (std::regex_search(remainString, match, regex))
    {
        results.push_back(match.prefix());
        remainString = match.suffix().str();
    }
    results.push_back(remainString);
    return results;
}

void StringUtils::TrimLeft(std::string &aString, const std::string &aChars)
{
    size_t pos = aString.find_first_not_of(aChars);
    if (std::string::npos != pos)
    {
        aString = aString.substr(pos);
    }
}

std::string StringUtils::TrimedLeft(const std::string &aString, const std::string &aChars)
{
    std::string result = aString;
    TrimLeft(result, aChars);
    return result;
}

void StringUtils::TrimRight(std::string &aString, const std::string &aChars)
{
    size_t pos = aString.find_last_not_of(aChars);
    if (std::string::npos != pos)
    {
        if (pos + 1 != aString.length())
        {
            aString = aString.substr(0, pos + 1);
        }
    }
    else
    {
        aString.clear();
    }
}

std::string StringUtils::TrimedRight(const std::string &aString, const std::string &aChars)
{
    std::string result = aString;
    TrimRight(result, aChars);
    return result;
}

std::vector<std::string> StringUtils::SplitString(const std::string &aStr, const std::string &aDelimiter)
{
    size_t posStart = 0, posEnd, delimLen = aDelimiter.length();
    std::string token;
    std::vector<std::string> res;

    if (aStr.empty())
    {
        return res;
    }

    while ((posEnd = aStr.find(aDelimiter, posStart)) != std::string::npos)
    {
        token = aStr.substr(posStart, posEnd - posStart);
        posStart = posEnd + delimLen;
        res.push_back(token);
    }

    res.push_back(aStr.substr(posStart));
    return res;
}

bool StringUtils::IsMatch(const std::string &aString, const std::string &aPattern)
{
    return std::regex_match(aString, std::regex(aPattern));
}

std::string StringUtils::ToLowCase(const std::string &aString)
{
    int32_t len = aString.length();
    std::string newString(aString);
    for (std::string::size_type i = 0; i < len; i++)
    {
        newString[i] = tolower(aString[i]);
    }
    return newString;
}

std::string StringUtils::ToUpperCase(const std::string &aString)
{
    int32_t len = aString.length();
    std::string newString(aString);
    for (std::string::size_type i = 0; i < len; i++)
    {
        newString[i] = toupper(aString[i]);
    }
    return newString;
}

bool StringUtils::IsAllNumber(const std::string &aString)
{
    for (std::string::size_type i = 0; i < aString.length(); i++)
    {
        char c = aString[i];
        if (c > '9' || c < '0')
        {
            return false;
        }
    }
    return true;
}

bool StringUtils::IsIDString(const std::string &aString)
{
    for (std::string::size_type i = 0; i < aString.length() - 1; i++)
    {
        char c = aString[i];
        if (c > '9' || c < '0')
        {
            return false;
        }
    }
    char c = aString[aString.length() - 1];
    if ((c <= '9' && c >= '0') || c == 'X' || c == 'x')
    {
        return true;
    }
    return false;
}

}  // namespace eurora::common
