#include "std.hpp"
#include "cajun/cajun.inl"
#include "cppformat/format.cc"
#include "readpe.hpp"


namespace readpe
{


using namespace pe;


template<typename T>
json::String to_String(const T& v)
{
    using namespace std;

    ostringstream stream;
    stream << "0x" << hex << uppercase << v;
    return json::String(stream.str());
}

template<>
json::String to_String<int>(const int& v)
{
    return json::String(std::to_string(v));
}

template<typename Arch>
OutputFormatter<Arch>::OutputFormatter(const Image<Arch>& image, const FormatOptions& options)
    : image_(image)
    , jsonObj()
    , resultStr()
    , formatOptions(options)
{
}

template<typename Arch>
void OutputFormatter<Arch>::computeAndSetRealFormatOptions()
{
    auto& rawOptions = formatOptions.rawOptions;
    auto& detailedOptions = formatOptions.detailedOptions;

    if (!utils::isDirectoryExist(image_, Directory::ExportEntry)) {
        rawOptions.exportHeader = false;
        detailedOptions.exportHeader = false;
    }
    if (!utils::isDirectoryExist(image_, Directory::ImportEntry)) {
        rawOptions.importHeaders = false;
        detailedOptions.importHeaders = false;
    }
    if (!utils::isDirectoryExist(image_, Directory::TlsEntry)) {
        rawOptions.tlsHeader = false;
        detailedOptions.tlsHeader = false;
    }
    if (!utils::isDirectoryExist(image_, Directory::BoundImportEntry)) {
        rawOptions.boundHeaders = false;
        detailedOptions.boundHeaders = false;
    }
    if (!utils::isDirectoryExist(image_, Directory::BaserelocEntry)) {
        rawOptions.fixupHeaders = false;
        detailedOptions.fixupHeaders = false;
    }
    if (!utils::isDirectoryExist(image_, Directory::LoadConfigEntry)) {
        rawOptions.loadConfigHeader = false;
        detailedOptions.loadConfigHeader = false;
    }
}


template<typename Arch>
const std::string& OutputFormatter<Arch>::format()
{
    computeAndSetRealFormatOptions();
    formatRawOptions();
    formatDetailedOptions();
    return str();
}

template<typename Arch>
void OutputFormatter<Arch>::formatRawOptions()
{
    const RawOptions& options = formatOptions.rawOptions;
    if (options.dosHeader) formatDosHeader();
    if (options.ntHeaders) formatNtHeaders();
    if (options.ntSections) formatSectionHeaders();
    if (options.exportHeader) formatExportHeader();
    if (options.importHeaders) formatImportHeaders(); 
    if (options.boundHeaders) formatBoundHeaders();
    if (options.tlsHeader) formatTlsHeader(); 
    if (options.fixupHeaders) formatRelocationHeaders();
    if (options.loadConfigHeader) formatLoadConfigDirHeader();
}

template<typename Arch>
void OutputFormatter<Arch>::formatDetailedOptions()
{
    const DetailedOptions& options = formatOptions.detailedOptions;
    if (options.ntSections) formatSectionsDetails();
    if (options.exportHeader) formatExportDetails();
    if (options.importHeaders) formatImportsDetails();
    if (options.boundHeaders) formatBoundsDetails();
    if (options.tlsHeader) formatTlsDetails();
    if (options.fixupHeaders) formatRelocationsDetails();
}

template<typename Arch>
void OutputFormatter<Arch>::formatDosHeader()
{
    const auto& dosHeader = image_.getDosHeader();
    json::UnknownElement & jsonDos = jsonObj["Dos-Header"];

    jsonDos["e_magic"] = to_String(dosHeader.e_magic);
    jsonDos["e_cblp"] = to_String(dosHeader.e_cblp);
    jsonDos["e_cp"] = to_String(dosHeader.e_cp);
    jsonDos["e_crlc"] = to_String(dosHeader.e_crlc);
    jsonDos["e_cparhdr"] = to_String(dosHeader.e_cparhdr);
    jsonDos["e_minalloc"] = to_String(dosHeader.e_minalloc);
    jsonDos["e_maxalloc"] = to_String(dosHeader.e_maxalloc);
    jsonDos["e_ss"] = to_String(dosHeader.e_ss);
    jsonDos["e_sp"] = to_String(dosHeader.e_sp);
    jsonDos["e_csum"] = to_String(dosHeader.e_csum);
    jsonDos["e_ip"] = to_String(dosHeader.e_ip);
    jsonDos["e_cs"] = to_String(dosHeader.e_cs);
    jsonDos["e_lfarlc"] = to_String(dosHeader.e_lfarlc);
    jsonDos["e_ovno"] = to_String(dosHeader.e_ovno);

    for (int i=0; i<4; i++)
        jsonDos["e_res"][i] = to_String(dosHeader.e_res[i]);

    jsonDos["e_oemid"] = to_String(dosHeader.e_oemid);
    jsonDos["e_oeminfo"] = to_String(dosHeader.e_oeminfo);

    for(int i=0; i<10; i++)
        jsonDos["e_res2"][i] = to_String(dosHeader.e_res2[i]);

    jsonDos["e_lfanew"] = to_String(dosHeader.e_lfanew);
}

template<typename Arch>
void OutputFormatter<Arch>::formatFileHeader()
{
    json::UnknownElement & jsonFileHeader = jsonObj["NT-Headers"]["FileHeader"];
    const ImgFileHeader& fileHeader = image_.getNtHeaders().FileHeader;

    jsonFileHeader["Machine"] = to_String(fileHeader.Machine);
    jsonFileHeader["NumberOfSections"] = to_String<int>(fileHeader.NumberOfSections);
    jsonFileHeader["TimeDateStamp"] = to_String(fileHeader.TimeDateStamp);
    jsonFileHeader["PointerToSymbolTable"] = to_String(fileHeader.PointerToSymbolTable);
    jsonFileHeader["NumberOfSymbols"] = to_String<int>(fileHeader.NumberOfSymbols);
    jsonFileHeader["SizeOfOptionalHeader"] = to_String(fileHeader.SizeOfOptionalHeader);
    jsonFileHeader["Characteristics"] = to_String(fileHeader.Characteristics);
}

template<typename Arch>
void OutputFormatter<Arch>::formatOptionalHeader()
{
    json::UnknownElement & jsonOptionalHeader = jsonObj["NT-Headers"]["OptionalHeader"];
    const headers_traits<Arch>::ImgOptionalHeader& optHeader = image_.getNtHeaders().OptionalHeader;

    jsonOptionalHeader["Magic"] = to_String(optHeader.Magic);
    jsonOptionalHeader["MajorLinkerVersion"] = to_String<int>(optHeader.MajorLinkerVersion);
    jsonOptionalHeader["MinorLinkerVersion"] = to_String<int>(optHeader.MinorLinkerVersion);
    jsonOptionalHeader["SizeOfCode"] = to_String(optHeader.SizeOfCode);
    jsonOptionalHeader["SizeOfInitializedData"] = to_String(optHeader.SizeOfInitializedData);
    jsonOptionalHeader["SizeOfUninitializedData"] = to_String(optHeader.SizeOfUninitializedData);
    jsonOptionalHeader["AddressOfEntryPoint"] = to_String(optHeader.AddressOfEntryPoint);
    jsonOptionalHeader["BaseOfCode"] = to_String(optHeader.BaseOfCode);
    jsonOptionalHeader["ImageBase"] = to_String(optHeader.ImageBase);
    jsonOptionalHeader["SectionAlignment"] = to_String(optHeader.SectionAlignment);
    jsonOptionalHeader["FileAlignment"] = to_String(optHeader.FileAlignment);
    jsonOptionalHeader["MajorOperatingSystemVersion"] = to_String<int>(optHeader.MajorOperatingSystemVersion);
    jsonOptionalHeader["MinorOperatingSystemVersion"] = to_String<int>(optHeader.MinorOperatingSystemVersion);
    jsonOptionalHeader["MajorImageVersion"] = to_String<int>(optHeader.MajorImageVersion);
    jsonOptionalHeader["MinorImageVersion"] = to_String<int>(optHeader.MinorImageVersion);
    jsonOptionalHeader["MajorSubsystemVersion"] = to_String<int>(optHeader.MajorSubsystemVersion);
    jsonOptionalHeader["MinorSubsystemVersion"] = to_String<int>(optHeader.MinorSubsystemVersion);
    jsonOptionalHeader["Win32VersionValue"] = to_String<int>(optHeader.Win32VersionValue);
    jsonOptionalHeader["SizeOfImage"] = to_String(optHeader.SizeOfImage);
    jsonOptionalHeader["SizeOfHeaders"] = to_String(optHeader.SizeOfHeaders);
    jsonOptionalHeader["CheckSum"] = to_String(optHeader.CheckSum);
    jsonOptionalHeader["Subsystem"] = to_String(optHeader.Subsystem);
    jsonOptionalHeader["DllCharacteristics"] = to_String(optHeader.DllCharacteristics);
    jsonOptionalHeader["SizeOfStackReserve"] = to_String(optHeader.SizeOfStackReserve);
    jsonOptionalHeader["SizeOfStackCommit"] = to_String(optHeader.SizeOfStackCommit);
    jsonOptionalHeader["SizeOfHeapReserve"] = to_String(optHeader.SizeOfHeapReserve);
    jsonOptionalHeader["SizeOfHeapCommit"] = to_String(optHeader.SizeOfHeapCommit);
    jsonOptionalHeader["LoaderFlags"] = to_String(optHeader.LoaderFlags);
    jsonOptionalHeader["NumberOfRvaAndSizes"] = to_String<int>(optHeader.NumberOfRvaAndSizes);

    const ImgDataDirectory* dataDirs = optHeader.DataDirectory;
    int rvaAndSizesNum = (int)optHeader.NumberOfRvaAndSizes;
    for (int i = 0; i < rvaAndSizesNum; i++) {
        json::UnknownElement & jsonDataDir = jsonOptionalHeader["DataDirectory"][i];

        jsonDataDir["VirtualAddress"] = to_String(dataDirs[i].VirtualAddress);
        jsonDataDir["Size"] = to_String(dataDirs[i].Size);
    }
}

template<typename Arch>
void OutputFormatter<Arch>::formatNtHeaders()
{
    jsonObj["NT-Headers"]["Signature"] = to_String(image_.getNtHeaders().Signature);

    formatFileHeader();
    formatOptionalHeader();
}

template<typename Arch>
void OutputFormatter<Arch>::formatSectionHeaders()
{
    const SectionHeaders_t& sections = image_.getSectionHeaders();
    uint16_t sectionsCount = image_.getNtHeaders().FileHeader.NumberOfSections;
    for (uint16_t i=0; i < sectionsCount; i++) {
        const ImgSectionHeader& section = sections[i];

        json::UnknownElement & jsonSection = jsonObj["nt-sections"][i];

        const auto& name = utils::getSectionName(&section.Name[0]);
        jsonSection["Name"] = json::String(name);
        jsonSection["VirtualSize"] = to_String(section.Misc.VirtualSize);
        jsonSection["VirtualAddress"] = to_String(section.VirtualAddress);
        jsonSection["SizeOfRawData"] = to_String(section.SizeOfRawData);
        jsonSection["PointerToRawData"] = to_String(section.PointerToRawData);
        jsonSection["PointerToRelocations"] = to_String(section.PointerToRelocations);
        jsonSection["PointerToLinenumbers"] = to_String(section.PointerToLinenumbers);
        jsonSection["NumberOfRelocations"] = to_String<int>(section.NumberOfRelocations);
        jsonSection["NumberOfLinenumbers"] = to_String<int>(section.NumberOfLinenumbers);
        jsonSection["Characteristics"] = to_String(section.Characteristics);
    }
}

template<typename Arch>
void OutputFormatter<Arch>::formatExportHeader()
{
    const ImgExportDirectory& export_ = image_.getExportDirHeader();
    json::UnknownElement & jsonExport = jsonObj["export-directory-header"];

    jsonExport["Characteristics"] = to_String(export_.Characteristics);
    jsonExport["TimeDateStamp"] = to_String(export_.TimeDateStamp);
    jsonExport["MajorVersion"] = to_String<int>(export_.MajorVersion);
    jsonExport["MinorVersion"] = to_String<int>(export_.MinorVersion);
    jsonExport["Name"] = to_String(export_.Name);
    jsonExport["Base"] = to_String(export_.Base);
    jsonExport["NumberOfFunctions"] = to_String<int>(export_.NumberOfFunctions);
    jsonExport["NumberOfNames"] = to_String<int>(export_.NumberOfNames);
    jsonExport["AddressOfFunctions"] = to_String(export_.AddressOfFunctions);
    jsonExport["AddressOfNames"] = to_String(export_.AddressOfNames);
    jsonExport["AddressOfNameOrdinals"] = to_String(export_.AddressOfNameOrdinals);
}

template<typename Arch>
void OutputFormatter<Arch>::formatImportDescriptor(const ImgImportDescriptor& import,
    json::UnknownElement& jsonImportDir)
{
    jsonImportDir["OriginalFirstThunk"] = to_String(import.OriginalFirstThunk);
    jsonImportDir["TimeDateStamp"] = to_String(import.TimeDateStamp);
    jsonImportDir["ForwarderChain"] = to_String(import.ForwarderChain);
    jsonImportDir["Name"] = to_String(import.Name);
    jsonImportDir["FirstThunk"] = to_String(import.FirstThunk);
}

template<typename Arch>
void OutputFormatter<Arch>::formatImportHeaders()
{
    const ImportDescriptors_t& imports = image_.getImportDirHeaders();
    for (ImportDescriptors_t::size_type i = 0; i < imports.size(); i++) {
        const ImgImportDescriptor& import = imports[i];
        json::UnknownElement & jsonImportDir = jsonObj["import-directory-headers"][i];

        formatImportDescriptor(import, jsonImportDir);
    }
}

template<typename Arch>
void OutputFormatter<Arch>::formatBoundHeaders()
{
    const BoundImportDescriptors_t& bounds = image_.getBoundDirHeaders();

    size_t boundsCount = bounds.size();
    size_t boundIndex = 0;
    int    jsonItem = 0;
    bool   isBoundsFinished = (boundIndex >= boundsCount);

    while (!isBoundsFinished) {
        json::UnknownElement & jsonBound = jsonObj["bound-directories-headers"][jsonItem];

        jsonBound["TimeDateStamp"] = to_String(bounds[boundIndex].TimeDateStamp);
        jsonBound["OffsetModuleName"] = to_String(bounds[boundIndex].OffsetModuleName);
        jsonBound["NumberOfModuleForwarderRefs"] = to_String<int>(bounds[boundIndex].NumberOfModuleForwarderRefs);

        uint16_t NumberOfModuleForwarderRefs = bounds[boundIndex].NumberOfModuleForwarderRefs;
        ++boundIndex;

        json::UnknownElement & jsonForwarder = jsonBound["forwarders"];
        for (size_t j = 0; !isBoundsFinished && j < NumberOfModuleForwarderRefs; ++j) {
            jsonForwarder[j]["TimeDateStamp"] = to_String(bounds[boundIndex].TimeDateStamp);
            jsonForwarder[j]["OffsetModuleName"] = to_String(bounds[boundIndex].OffsetModuleName);
            jsonForwarder[j]["Reserved"] = to_String(bounds[boundIndex].NumberOfModuleForwarderRefs);

            ++boundIndex;
            isBoundsFinished = (boundIndex >= boundsCount);
        }

        ++jsonItem;
        isBoundsFinished = (boundIndex >= boundsCount);
    }
}

template<typename Arch>
void OutputFormatter<Arch>::formatTlsHeader()
{
    auto& jsonTlsDirHeader = jsonObj["tls-directory-header"];
    const auto& tlsDirHeader = image_.getTlsDirHeader();    
    jsonTlsDirHeader["StartAddressOfRawData"] = to_String(tlsDirHeader.StartAddressOfRawData);
    jsonTlsDirHeader["EndAddressOfRawData"] = to_String(tlsDirHeader.EndAddressOfRawData);
    jsonTlsDirHeader["AddressOfIndex"] = to_String(tlsDirHeader.AddressOfIndex);
    jsonTlsDirHeader["AddressOfCallBacks"] = to_String(tlsDirHeader.AddressOfCallBacks);
    jsonTlsDirHeader["SizeOfZeroFill"] = to_String(tlsDirHeader.SizeOfZeroFill);
    jsonTlsDirHeader["Characteristics"] = to_String(tlsDirHeader.Characteristics);
}

template<typename Arch>
void OutputFormatter<Arch>::formatRelocationHeaders()
{
    const BaseRelocationList_t& relocDirHeaders = image_.getRelocationDirHeaders();

    if (!relocDirHeaders.empty()) {
        size_t dirIndex = 0;
        for (auto relocDir : relocDirHeaders) {
            size_t relocsCount = (relocDir.SizeOfBlock - sizeof(ImgBaseRelocation)) / sizeof(uint16_t);

            json::UnknownElement& jsonRelocDir = jsonObj["relocation-headers"][dirIndex];

            jsonRelocDir["VirtualAddress"] = to_String(relocDir.VirtualAddress);
            jsonRelocDir["SizeOfBlock"] = to_String(relocDir.SizeOfBlock);
            jsonRelocDir["relocations-count"] = to_String(relocsCount);

            ++dirIndex;
        }
    }
}

template<typename Arch>
void OutputFormatter<Arch>::formatLoadConfigDirHeader()
{
    auto& jsonLoadConfigDirHeader = jsonObj["load-config-directory-header"];
    const auto& loadConfigDirHeader = image_.getImgLoadConfigDirectory();

    jsonLoadConfigDirHeader["Size"] = to_String(loadConfigDirHeader.Size);
    jsonLoadConfigDirHeader["TimeDateStamp"] = to_String(loadConfigDirHeader.TimeDateStamp);
    jsonLoadConfigDirHeader["MajorVersion"] = to_String(loadConfigDirHeader.MajorVersion);
    jsonLoadConfigDirHeader["MinorVersion"] = to_String(loadConfigDirHeader.MinorVersion);
    jsonLoadConfigDirHeader["GlobalFlagsClear"] = to_String(loadConfigDirHeader.GlobalFlagsClear);
    jsonLoadConfigDirHeader["GlobalFlagsSet"] = to_String(loadConfigDirHeader.GlobalFlagsSet);
    jsonLoadConfigDirHeader["CriticalSectionDefaultTimeout"] = to_String(loadConfigDirHeader.CriticalSectionDefaultTimeout);
    jsonLoadConfigDirHeader["DeCommitFreeBlockThreshold"] = to_String(loadConfigDirHeader.DeCommitFreeBlockThreshold);
    jsonLoadConfigDirHeader["DeCommitTotalFreeThreshold"] = to_String(loadConfigDirHeader.DeCommitTotalFreeThreshold);
    jsonLoadConfigDirHeader["LockPrefixTable"] = to_String(loadConfigDirHeader.LockPrefixTable);
    jsonLoadConfigDirHeader["MaximumAllocationSize"] = to_String(loadConfigDirHeader.MaximumAllocationSize);
    jsonLoadConfigDirHeader["VirtualMemoryThreshold"] = to_String(loadConfigDirHeader.VirtualMemoryThreshold);
    jsonLoadConfigDirHeader["ProcessHeapFlags"] = to_String(loadConfigDirHeader.ProcessHeapFlags);
    jsonLoadConfigDirHeader["ProcessAffinityMask"] = to_String(loadConfigDirHeader.ProcessAffinityMask);
    jsonLoadConfigDirHeader["CSDVersion"] = to_String(loadConfigDirHeader.CSDVersion);
    jsonLoadConfigDirHeader["DependentLoadFlags"] = to_String(loadConfigDirHeader.DependentLoadFlags);
    jsonLoadConfigDirHeader["EditList"] = to_String(loadConfigDirHeader.EditList);
    jsonLoadConfigDirHeader["SecurityCookie"] = to_String(loadConfigDirHeader.SecurityCookie);
    jsonLoadConfigDirHeader["SEHandlerTable"] = to_String(loadConfigDirHeader.SEHandlerTable);
    jsonLoadConfigDirHeader["SEHandlerCount"] = to_String(loadConfigDirHeader.SEHandlerCount);
}

template<typename Arch>
void OutputFormatter<Arch>::formatSectionsDetails()
{
    const auto& sections = parse::Sections(image_);
    int i = 0;
    for (const auto& section: sections) {
        auto& jsonSection = jsonObj["nt-sections-details"][ i++ ];
        jsonSection["Name"] = json::String(section.name);
        jsonSection["real-size"] = to_String(section.realSize);
        jsonSection["real-file-offset"] = to_String(section.realOffset);
        jsonSection["entropy"] = json::String(fmt::format("{:.3f}", section.entropy));
        jsonSection["Characteristics"] = json::String(Fw::StringUtils::join(section.flags));
    }
}

template<typename Arch>
void OutputFormatter<Arch>::formatExportElement( 
    const parse::Export::Element& element,
    json::UnknownElement & jsonExpDir)
{
    jsonExpDir["name"] = json::String(element.name);
    jsonExpDir["ordinal"] = to_String(element.ordinal);

    bool isForward = (element.rva == utils::INVALID_RVA) && (element.offset == Fw::Io::INVALID_OFFSET);
    if (isForward) {
        jsonExpDir["forward-to"] = json::String(element.forwardName);
    }
    else {
        jsonExpDir["rva"] = to_String(element.rva);
        jsonExpDir["offset"] = to_String(element.offset);
    }
}

template<typename Arch>
void OutputFormatter<Arch>::formatExportDetails()
{
    parse::Export export_(image_);
    if (export_.elements().empty())
        return;

    auto& jsonExport = jsonObj["export-directory-details"];
    jsonExport["timeDateStamp"] = to_String(export_.timeDateStamp());

    size_t jsonItemIndex = 0;
    for (auto element : export_.elements()) {
        formatExportElement(element, jsonExport[jsonItemIndex]);
        ++jsonItemIndex;
    }
}

void formatImportModule(const parse::import::Module& module, 
    json::UnknownElement& jsonModule)
{
    json::UnknownElement& jsonElements = jsonModule["elements"];

    size_t elementIndex = 0;
    for (auto element : module.elements) {
        json::UnknownElement& jsonElement = jsonElements[elementIndex];
        ++elementIndex;

        if (element.isForwarded) {
            jsonElement["isForwarded"] = json::String( "true" );
        }

        jsonElement["rva"] = to_String(element.think.rva);
        jsonElement["offset"] = to_String(element.think.offset);

        bool byOrdinal = (element.ordinal > 0);
        if (byOrdinal) {
            jsonElement["ordinal"] = to_String(element.ordinal);
        }
        else {
            jsonElement["name"] = json::String(element.name);
            jsonElement["hint"] = to_String(element.hint);
        }

        if (element.hook.rva) {
            jsonElement["hookRva"] = to_String(element.hook.rva);
            jsonElement["hookOffset"] = to_String(element.hook.offset);
        }
    }
}

template<typename Arch>
void OutputFormatter<Arch>::formatImportsDetails()
{
    const auto& modules = parse::ImportedModules(image_);
    if (modules.empty())
        return;

    json::UnknownElement& jsonModules = jsonObj["import-directories-details"];

    size_t moduleIndex = 0;
    for (const auto& module : modules) {
        json::UnknownElement& jsonModule = jsonModules[moduleIndex];

        jsonModule["name"] = json::String(module.name);

        const auto& tmStr = module.timestampUTC0;
        if (!tmStr.empty()) {
            jsonModule["timestampUTC0"] = json::String(tmStr);
        }

        if (!module.elements.empty()) {
            formatImportModule(module, jsonModule);
        }

        ++moduleIndex;
    }
}

template<typename Arch>
void OutputFormatter<Arch>::formatBoundDescriptor( 
    const parse::BoundDescriptor& descriptor,
    json::UnknownElement& jsonBound)
{
    jsonBound["moduleName"] = json::String(descriptor.moduleName);
    jsonBound["timestampUTC0"] = json::String(descriptor.timestamp);
    jsonBound["moduleNamePos"] = to_String(descriptor.moduleNamePos);
    jsonBound["moduleNameRva"] = to_String(descriptor.moduleNameRva);
}

template<typename Arch>
void OutputFormatter<Arch>::formatBoundsDetails()
{
    const auto& boundsDescriptors = parse::BoundImport(image_);
    if (boundsDescriptors.empty())
        return;

    const BoundImportDescriptors_t& boundsDirs = image_.getBoundDirHeaders();

    auto boundIter = boundsDescriptors.begin();
    auto boundEnd = boundsDescriptors.end();

    size_t boundIndex = 0;
    bool isBoundsFinished = (boundIter == boundEnd);
    while (!isBoundsFinished) {
        json::UnknownElement& jsonBound = jsonObj["bound-directories-details"][boundIndex];

        formatBoundDescriptor(*boundIter, jsonBound);

        ++boundIter;

        uint16_t forwardsCount = boundsDirs[boundIndex].NumberOfModuleForwarderRefs;
        if (forwardsCount > 0) {
            json::UnknownElement & jsonForwarders = jsonBound["forwarders"];

            isBoundsFinished = (boundIter == boundEnd);
            for (uint16_t forwardIndex = 0;
                !isBoundsFinished && forwardIndex < forwardsCount; ++forwardIndex)
            {
                formatBoundDescriptor(*boundIter, jsonForwarders[forwardIndex]);

                ++boundIter;
                isBoundsFinished = (boundIter == boundEnd);
            }
        }
        ++boundIndex;
        isBoundsFinished = (boundIter == boundEnd);
    }
}

template<typename Arch>
void OutputFormatter<Arch>::formatTlsDetails()
{
    const auto& callbacks = parse::TlsCallbacks(image_);
    if (callbacks.empty())
        return;

    size_t callbackIndex = 0;
    json::UnknownElement& jsonTls = jsonObj["tls-directory-details"]["callbacks"];
    for (const auto& callback : callbacks) {
        jsonTls[callbackIndex]["rva"] = to_String(callback.rva);
        jsonTls[callbackIndex]["offset"] = to_String(callback.offset);
        ++callbackIndex;
    }
}

template<typename Arch>
void OutputFormatter<Arch>::formatRelocationsDetails()
{
    const auto& relocations = parse::Relocations(image_);
    if (relocations.empty())
        return;

    json::UnknownElement& jsonRelocs = jsonObj["relocations-details"];
    size_t relocIndex = 0;
    for (auto reloc : relocations) {
        jsonRelocs[relocIndex]["type"] = to_String(reloc.type);
        jsonRelocs[relocIndex]["rva"] = to_String(reloc.rva);
        jsonRelocs[relocIndex]["offset"] = to_String(reloc.offset);
        ++relocIndex;
    }
}

template<typename Arch>
const std::string& OutputFormatter<Arch>::str() const
{
    if (!jsonObj.Empty())
    {
        std::stringstream sstrem;
        json::Writer::Write(jsonObj, sstrem);
        resultStr = sstrem.str();
    }
    return resultStr;
}


template  class  OutputFormatter<arch32>;
template  class  OutputFormatter<arch64>;


} // namespace readpe
