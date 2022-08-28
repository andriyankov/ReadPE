#include "std.hpp"
#include "pe.hpp"
#include "ImageImpl.hpp"
#include "ImageImpl.impl"


namespace pe 
{

template<typename Arch>
Image<Arch>::Image(std::iostream& stream, bool lazyMode)
{
    impl = std::make_unique<Image<Arch>::Impl>(stream, *this);
    if (!lazyMode)
        read();
}

template<typename Arch>
Image<Arch>::~Image()
{
    impl = nullptr;
}

template<typename Arch>
void Image<Arch>::clear()
{
    impl->clear();
}

template<typename Arch>
std::iostream& Image<Arch>::getStream() const
{
    assert(impl->imgStream.good());
    return impl->imgStream;
}

template<typename Arch>
std::iostream& Image<Arch>::getStream()
{
    return
        const_cast<std::iostream&>(
        static_cast<const Image<Arch>&>(*this).getStream()
        );
}

template<typename Arch>
std::streamsize Image<Arch>::sizeOfFile() const
{
    return impl->imgSize;
}

template<typename Arch>
const ImgDosHeader& Image<Arch>::getDosHeader() const
{
    const_cast<Image<Arch>&>(*this).impl->readDosHeader();
    return impl->dosHeader;
}

template<typename Arch>
ImgDosHeader& Image<Arch>::getDosHeader()
{
    return
        const_cast<ImgDosHeader&>(
        static_cast<const Image<Arch>&>(*this).getDosHeader()
    );
}

template<typename Arch>
const typename NtHeaders_t<Arch>& Image<Arch>::getNtHeaders() const
{
    const_cast<Image<Arch>&>(*this).impl->readNtHeaders();
    return impl->ntHeaders;
}

template<typename Arch>
typename NtHeaders_t<Arch>& Image<Arch>::getNtHeaders()
{
    return
        const_cast<typename NtHeaders_t<Arch>&>(
        static_cast<const Image<Arch>&>(*this).getNtHeaders()
    );
}

template<typename Arch>
const SectionHeaders_t& Image<Arch>::getSectionHeaders() const
{
    const_cast<Image<Arch>&>(*this).impl->readNtSections();
    return impl->sectionHeaders;
}

template<typename Arch>
SectionHeaders_t& Image<Arch>::getSectionHeaders()
{
    return const_cast<SectionHeaders_t&>(
        static_cast<const Image<Arch>&>(*this).getSectionHeaders()
    );
}

template<typename Arch>
const ImgExportDirectory& Image<Arch>::getExportDirHeader() const
{
    const_cast<Image<Arch>&>(*this).impl->readExportDirectoryHeader();
    return impl->exportDirectory;
}

template<typename Arch>
ImgExportDirectory& Image<Arch>::getExportDirHeader()
{
    return
        const_cast<ImgExportDirectory&>(
        static_cast<const Image<Arch>&>(*this).getExportDirHeader()
    );
}

template<typename Arch>
const ImportDescriptors_t& Image<Arch>::getImportDirHeaders() const
{
    const_cast<Image<Arch>&>(*this).impl->readImportDirectoryHeaders();
    return impl->importsDirectories;
}

template<typename Arch>
ImportDescriptors_t& Image<Arch>::getImportDirHeaders()
{
    return
        const_cast<ImportDescriptors_t&>(
        static_cast<const Image<Arch>&>(*this).getImportDirHeaders()
    );
}

template<typename Arch>
const typename TlsDirectory_t<Arch>& Image<Arch>::getTlsDirHeader() const
{
    const_cast<Image<Arch>&>(*this).impl->readTlsDirectoryHeader();
    return impl->tlsDirectory;
}

template<typename Arch>
typename TlsDirectory_t<Arch>& Image<Arch>::getTlsDirHeader()
{
    return const_cast<typename TlsDirectory_t<Arch>&>(
        static_cast<const Image<Arch>&>(*this).getTlsDirHeader()
    );
}

template<typename Arch>
const BoundImportDescriptors_t& Image<Arch>::getBoundDirHeaders() const
{
    const_cast< Image<Arch>& >(*this).impl->readBoundDirectoryHeaders();
    return impl->boundDirectories;
}

template<typename Arch>
BoundImportDescriptors_t& Image<Arch>::getBoundDirHeaders()
{
    return const_cast<BoundImportDescriptors_t&>(
        static_cast<const Image<Arch>&>(*this).getBoundDirHeaders()
        );
}

template<typename Arch>
const BaseRelocationList_t& Image<Arch>::getRelocationDirHeaders() const
{
    const_cast<Image<Arch>&>(*this).impl->readRelocationDirectories();
    return impl->relocationDirectories;
}

template<typename Arch>
BaseRelocationList_t& Image<Arch>::getRelocationDirHeaders()
{
    return const_cast<BaseRelocationList_t&>(
        static_cast<const Image<Arch>&>(*this).getRelocationDirHeaders()
        );
}

template<typename Arch>
const LoadConfigDirectory_t<Arch>& Image<Arch>::getImgLoadConfigDirectory() const
{
    const_cast<Image<Arch>&>(*this).impl->readLoadConfigDirectory();
    return impl->loadConfigDirectory;
}

template<typename Arch>
LoadConfigDirectory_t<Arch>& Image<Arch>::ImgLoadConfigDirectory()
{
    return const_cast<typename LoadConfigDirectory_t<Arch>&>(
        static_cast<const Image<Arch>&>(*this).getImgLoadConfigDirectory()
        );
}

template<typename Arch>
void Image<Arch>::read()
{
    assert(impl->imgStream.good());

    impl->imgSize = FW::Io::StreamUtils::getSize(impl->imgStream);
    impl->readDosHeader();
    impl->readNtHeaders();
    impl->readNtSections();
    impl->readExportDirectoryHeader();
    impl->readBoundDirectoryHeaders();
    impl->readImportDirectoryHeaders();
    impl->readTlsDirectoryHeader();
    impl->readRelocationDirectories();
    impl->readLoadConfigDirectory();

    assert(impl->imgStream.good());
}

template<typename Arch>
void Image<Arch>::write()
{
    assert(impl->imgStream.good());

    impl->writeDosHeader();
    impl->writeNtHeaders();
    impl->writeNtSections();    
    impl->writeExportDirectoryHeader();
    impl->writeImportDirectoryHeaders();
    impl->writeTlsDirectoryHeader();
    impl->writeBoundDirectoryHeaders();
    impl->writeLoadConfigDirectory();
    impl->imgStream.flush();

    assert(impl->imgStream.good());
}


template  class  Image<arch32>;
template  class  Image<arch64>;


} // namespace pe
