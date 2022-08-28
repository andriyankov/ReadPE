#include "std.hpp"


#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable:4996)
#endif


namespace Framework {
namespace StringUtils {


std::string escape_sequence(const unsigned char* bytes, size_t len)
{
    std::string result;

    for (size_t i=0; i<len; i++) {
        unsigned char ch = bytes[i];
        if (isascii (ch) && isprint (ch))
            result += ch;
        else {
            char buf [16];
            sprintf (buf, "\\x%02x", ch);
            result += buf;
        }
    }
    return result;
}

std::string join(const std::list<std::string>& strings, const std::string& delim)
{
    if (strings.empty())
        return std::string();

    std::ostringstream stream;
    for (const auto& s : strings)
        stream << s << delim;
    auto result = stream.str();
    return std::string(result, 0, result.size() - delim.size());
}


}} // namespace Framework::StringUtils


#if _MSC_VER == 1800
#   pragma warning(pop)
#endif
