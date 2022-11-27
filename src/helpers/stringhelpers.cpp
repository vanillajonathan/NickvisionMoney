#include "stringhelpers.hpp"

using namespace NickvisionMoney::Helpers;

std::vector<std::string> StringHelpers::split(const std::string& s, const std::string& delim)
{
    std::vector<std::string> result;
    size_t last{ 0 };
    size_t next{ s.find(delim) };
    while(next != std::string::npos)
    {
        result.push_back(s.substr(last, next - last));
        last = next + delim.length();
        next = s.find(delim, last);
    }
    result.push_back(s.substr(last));
    return result;
}
