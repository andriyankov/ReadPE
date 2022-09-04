#include "std.hpp"
#include "readpe.hpp"


namespace readpe {
namespace PeDumper {


template<typename Arch>
void dump(std::iostream& stream, const FormatOptions& options)
{
    using namespace pe;

    try {
        Image<Arch> image(stream);
        image.read();

        OutputFormatter<Arch> formatter(image, options);
        std::cout << formatter.format();
    }
    catch (const pe::FormatException& e) {
        throw FormatException(e.what());
    }
    catch (const pe::UnsupportedException& e) {
        throw UnsupportedException(e.what());
    }
    catch (const pe::CorruptedException& e) {
        throw CorruptedException(e.what());
    }
    catch (const pe::Exception& e) {
        throw ReadpeException(e.what(), ErrorCode::Unknown);
    }
}

void dump(const ProgramOptions& programOptions)
{
    using namespace std;
    using namespace pe::detect;

    fstream fileStream;
    try {
        fileStream.open(programOptions.inputFilename.c_str(), ios::binary | ios::in);
        Fw::Io::StreamUtils::prepare(fileStream);
    }
    catch (const runtime_error&) {
        throw ConfigureException("Cannot open file");
    }

    Platform platform = detectPlatform(fileStream);

    if (platform == Platform::Arch32) {
        dump<pe::arch32>(fileStream, programOptions.formatOptions);
    }
    else if (platform == Platform::Arch64) {
        dump<pe::arch64>(fileStream, programOptions.formatOptions);
    }
    else if (platform == Platform::Unsupported) {
        throw UnsupportedException("This executable image is not supported");
    }
    else {
        throw FormatException("This file is not PE32/PE32+");
    }
}


}} // namespace readpe::PeDumper
