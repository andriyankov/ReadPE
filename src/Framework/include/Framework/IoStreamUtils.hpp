
namespace Io {
namespace StreamUtils {


void prepare(std::istream& stream);

std::streamsize getSize(std::istream& stream);

std::string readString(std::istream& stream);

std::string readStringAt(std::istream& stream, std::streampos offset);


}} // namespace Io::StreamUtils
