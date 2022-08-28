#include "std.hpp"
#include "pe.hpp"


namespace pe {
namespace parse {


using namespace Fw;


template<typename Arch>
std::list<BoundDescriptor> BoundImport(const Image<Arch>& image)
{
    std::list<BoundDescriptor> result;

    assert(image.getStream().good());

    const auto& DataDir = image.getNtHeaders().OptionalHeader.DataDirectory;
    uint32_t boundRva = DataDir[Directory::BoundImportEntry].VirtualAddress;

    for (const auto& boundDir : image.getBoundDirHeaders()) {
        auto nameRva = boundRva + boundDir.OffsetModuleName; 
        auto namePos = utils::RvaToRaw(image, nameRva);

        const auto& timestamp = Time::unixtimeToString(boundDir.TimeDateStamp);
        if (!timestamp.empty()) {
            BoundDescriptor descriptor;
            descriptor.timestamp = timestamp;
            descriptor.moduleNamePos = namePos;
            descriptor.moduleNameRva = nameRva;
            if (namePos != utils::INVALID_RVA)
                descriptor.moduleName = Io::StreamUtils::readStringAt(image.getStream(), namePos);
            result.emplace_back(descriptor);
        }
    }
    assert(image.getStream().good());

    return result;
}


template std::list<BoundDescriptor> BoundImport(const Image<arch32>& image);
template std::list<BoundDescriptor> BoundImport(const Image<arch64>& image);


}} // namespace pe::parse
