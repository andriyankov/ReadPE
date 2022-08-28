
namespace StringUtils {


// It is similar to Python 3.x conversation bytes to string
std::string escape_sequence(const unsigned char* bytes, size_t len);

std::string join(const std::list<std::string>& strings, const std::string& delim=std::string(","));


} // namespace StringUtils
