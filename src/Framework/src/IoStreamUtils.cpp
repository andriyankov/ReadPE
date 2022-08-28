#include "std.hpp"


namespace Framework {
namespace Io {
namespace StreamUtils {


void prepare(std::istream& stream)
{
    using namespace std;

    stream.sync_with_stdio(false);
    stream.exceptions(ios::failbit | ios::eofbit | ios::badbit);
}

std::streamsize getSize(std::istream& stream)
{
    using namespace std;

    assert(stream.good());
    streampos curPos = stream.tellg();
    stream.seekg(0, ios::end);
    streamsize result = stream.tellg();
    stream.seekg(curPos);
    return result;
}

std::string readString(std::istream& stream)
{
    assert(stream.good());

    std::string result;

    bool wasError = false;
    try {
        std::getline(stream, result, '\0');
    }
    catch (const std::ios::failure&) {
        wasError = true;
    }
    if (wasError || stream.eof())
        stream.clear();

    assert(stream.good());
    return result;
}

std::string readStringAt(std::istream& stream, std::streampos offset)
{
    assert(stream.good());

    std::string result;

    stream.seekg(offset);
    if (stream.good())
        result = readString(stream);
    else
        stream.clear();

    return result;
}


}}} // namespace Framework::Io::StreamUtils
