#include "std.hpp"
#include "pe.hpp"


namespace pe {
namespace detect {


Platform detectArch32(uint16_t machine)
{
    uint16_t machines[] = { Machine::I386, Machine::SH4 };

    for (const auto& machine_ : machines) {
        if (machine_ == machine)
            return Platform::Arch32;
    }
    return Platform::Unsupported;
}

Platform detectArch64(uint16_t machine)
{
    uint16_t machines[] = { Machine::IA64, Machine::AMD64 };

    for (const auto& machine_ : machines) {
        if (machine_ == machine)
            return Platform::Arch64;
    }
    return Platform::Unsupported;
}

Platform detectByMachine(uint16_t machine)
{
    Platform result = detectArch32(machine);
    return result == Platform::Unsupported ?
        detectArch64(machine) : result;
}

bool isMachineSupported(uint16_t machine) 
{
    return detectByMachine(machine) != Platform::Unsupported;
}

Platform detectPlatform(std::istream& stream)
{
    assert(stream.good());

    Platform type = Platform::Unknown;

    try {
        stream.seekg(0);

        ImgDosHeader dosHeader;
        stream.read((char*)&dosHeader, sizeof(dosHeader));
        if (dosHeader.e_magic == Signature::DOS) {
            stream.seekg(dosHeader.e_lfanew);
            ImgNtHeaders64 ntHeaders;
            stream.read((char*)&ntHeaders, sizeof(ntHeaders));

            bool isPE = (ntHeaders.Signature == Signature::PE);
            bool isExecutable = (ntHeaders.FileHeader.Characteristics & File::ExecutableImage) > 0;
            if (isPE && isExecutable)
                type = detectByMachine(ntHeaders.FileHeader.Machine);
        }
    }
    catch (const std::ios::failure&) {
        stream.clear();
    }
    assert(stream.good());
    return type;
}


}} // namespace pe::detect
