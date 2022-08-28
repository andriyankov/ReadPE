#include "PshPack1.h"


struct ImgDosHeader final
{
    uint16_t   e_magic;        // Magic number
    uint16_t   e_cblp;         // Bytes on last page of file
    uint16_t   e_cp;           // Pages in file
    uint16_t   e_crlc;         // Relocations
    uint16_t   e_cparhdr;      // Size of header in paragraphs
    uint16_t   e_minalloc;     // Minimum extra paragraphs needed
    uint16_t   e_maxalloc;     // Maximum extra paragraphs needed
    uint16_t   e_ss;           // Initial (relative) SS value
    uint16_t   e_sp;           // Initial SP value
    uint16_t   e_csum;         // Checksum
    uint16_t   e_ip;           // Initial IP value
    uint16_t   e_cs;           // Initial (relative) CS value
    uint16_t   e_lfarlc;       // File address of relocation table
    uint16_t   e_ovno;         // Overlay number
    uint16_t   e_res[4];       // Reserved words
    uint16_t   e_oemid;        // OEM identifier (for e_oeminfo)
    uint16_t   e_oeminfo;      // OEM information; e_oemid specific
    uint16_t   e_res2[10];     // Reserved words
    uint32_t   e_lfanew;       // File address of new exe header
};
static_assert( sizeof(ImgDosHeader) ==
              (2*14+ 2*4 + 2*2 + 2*10 + 1*4),
               "sizeof(ImgDosHeader) alignment error" );

//
// File header format.
//
struct ImgFileHeader final
{
    uint16_t    Machine;
    uint16_t    NumberOfSections;
    uint32_t    TimeDateStamp;
    uint32_t    PointerToSymbolTable;
    uint32_t    NumberOfSymbols;
    uint16_t    SizeOfOptionalHeader;
    uint16_t    Characteristics;
};
static_assert( sizeof(ImgFileHeader) ==
              (2*2 + 4*3+ 2*2), 
               "sizeof(ImgFileHeader) alignment error" );

//
// Directory format.
//
struct ImgDataDirectory final
{
    uint32_t    VirtualAddress;
    uint32_t    Size;
};
static_assert( sizeof(ImgDataDirectory) ==
              (4*2), 
               "sizeof(ImgDataDirectory) alignment error" );

//
// Optional header format.
//
struct ImgOptionalHeader32 final
{
    //
    // Standard fields.
    //
    uint16_t   Magic;
    uint8_t    MajorLinkerVersion;
    uint8_t    MinorLinkerVersion;
    uint32_t   SizeOfCode;
    uint32_t   SizeOfInitializedData;
    uint32_t   SizeOfUninitializedData;
    uint32_t   AddressOfEntryPoint;
    uint32_t   BaseOfCode;
    uint32_t   BaseOfData;

    //
    // NT additional fields.
    //
    uint32_t   ImageBase;
    uint32_t   SectionAlignment;
    uint32_t   FileAlignment;
    uint16_t   MajorOperatingSystemVersion;
    uint16_t   MinorOperatingSystemVersion;
    uint16_t   MajorImageVersion;
    uint16_t   MinorImageVersion;
    uint16_t   MajorSubsystemVersion;
    uint16_t   MinorSubsystemVersion;
    uint32_t   Win32VersionValue;
    uint32_t   SizeOfImage;
    uint32_t   SizeOfHeaders;
    uint32_t   CheckSum;
    uint16_t   Subsystem;
    uint16_t   DllCharacteristics;
    uint32_t   SizeOfStackReserve;
    uint32_t   SizeOfStackCommit;
    uint32_t   SizeOfHeapReserve;
    uint32_t   SizeOfHeapCommit;
    uint32_t   LoaderFlags;
    uint32_t   NumberOfRvaAndSizes;
    ImgDataDirectory DataDirectory[NumberOfDirectoryEntries];
};
static_assert( sizeof(ImgOptionalHeader32) ==
              (2 + 1*2 + 4*9 + 2*6 + 4*4 + 2*2 + 4*6 + 4*2*16),
               "sizeof(ImgOptionalHeader32) alignment error" );


struct ImgOptionalHeader64 final
{
    uint16_t    Magic;
    uint8_t     MajorLinkerVersion;
    uint8_t     MinorLinkerVersion;
    uint32_t    SizeOfCode;
    uint32_t    SizeOfInitializedData;
    uint32_t    SizeOfUninitializedData;
    uint32_t    AddressOfEntryPoint;
    uint32_t    BaseOfCode;
    uint64_t    ImageBase;
    uint32_t    SectionAlignment;
    uint32_t    FileAlignment;
    uint16_t    MajorOperatingSystemVersion;
    uint16_t    MinorOperatingSystemVersion;
    uint16_t    MajorImageVersion;
    uint16_t    MinorImageVersion;
    uint16_t    MajorSubsystemVersion;
    uint16_t    MinorSubsystemVersion;
    uint32_t    Win32VersionValue;
    uint32_t    SizeOfImage;
    uint32_t    SizeOfHeaders;
    uint32_t    CheckSum;
    uint16_t    Subsystem;
    uint16_t    DllCharacteristics;
    uint64_t    SizeOfStackReserve;
    uint64_t    SizeOfStackCommit;
    uint64_t    SizeOfHeapReserve;
    uint64_t    SizeOfHeapCommit;
    uint32_t    LoaderFlags;
    uint32_t    NumberOfRvaAndSizes;
    ImgDataDirectory DataDirectory[NumberOfDirectoryEntries];
};
static_assert( sizeof(ImgOptionalHeader64) ==
              (2 + 1*2 + 4*5 + 8 + 4*2 + 2*6 + 4*4 + 2*2 + 8*4 + 4*2 + 4*2*16),
               "sizeof(ImgOptionalHeader64) alignment error" );


struct ImgNtHeaders32 final
{
    uint32_t            Signature;
    ImgFileHeader       FileHeader;
    ImgOptionalHeader32 OptionalHeader;
};

struct ImgNtHeaders64 final
{
    uint32_t            Signature;
    ImgFileHeader       FileHeader;
    ImgOptionalHeader64 OptionalHeader;
};


//
// Section header format.
//
struct ImgSectionHeader final
{
    uint8_t    Name[SizeOfShortName];
    union {
        uint32_t   PhysicalAddress;
        uint32_t   VirtualSize;
    } Misc;
    uint32_t   VirtualAddress;
    uint32_t   SizeOfRawData;
    uint32_t   PointerToRawData;
    uint32_t   PointerToRelocations;
    uint32_t   PointerToLinenumbers;
    uint16_t   NumberOfRelocations;
    uint16_t   NumberOfLinenumbers;
    uint32_t   Characteristics;
};
static_assert( sizeof(ImgSectionHeader) ==
              (1*8 + 4 + 4*5 + 2*2 + 4), 
               "sizeof(ImgSectionHeader) alignment error" );

//
// Export Format
//
struct ImgExportDirectory final
{
    uint32_t   Characteristics;
    uint32_t   TimeDateStamp;
    uint16_t   MajorVersion;
    uint16_t   MinorVersion;
    uint32_t   Name;
    uint32_t   Base;
    uint32_t   NumberOfFunctions;
    uint32_t   NumberOfNames;
    uint32_t   AddressOfFunctions;     // RVA from base of image
    uint32_t   AddressOfNames;         // RVA from base of image
    uint32_t   AddressOfNameOrdinals;  // RVA from base of image
};
static_assert( sizeof(ImgExportDirectory) ==
              (4*2 + 2*2 + 4*7), 
               "sizeof(ImgExportDirectory) alignment error" );

//
// Import Format
//
struct ImgImportByName final
{
    uint16_t   Hint;
    uint8_t    Name[1];
};
static_assert( sizeof(ImgImportByName) ==
              (2 + 1*1), 
               "sizeof(ImgImportByName) alignment error" );


struct ImgThunkData32 final
{
    union {
        uint32_t ForwarderString;    // uint8_t* 
        uint32_t Function;           // PDWORD
        uint32_t Ordinal;
        uint32_t AddressOfData;      // ImgImportByName*
    } u1;
};
static_assert( sizeof(ImgThunkData32) ==
              (4), 
               "sizeof(ImgThunkData32) alignment error" );


struct ImgThunkData64 final
{
    union {
        uint64_t ForwarderString;    // uint8_t* 
        uint64_t Function;           // PDWORD
        uint64_t Ordinal;
        uint64_t AddressOfData;      // ImgImportByName*
    } u1;
};
static_assert( sizeof(ImgThunkData64) ==
              (8), 
               "sizeof(ImgThunkData64) alignment error" );


struct ImgImportDescriptor final
{
    union {
        uint32_t   Characteristics;            // 0 for terminating null import descriptor
        uint32_t   OriginalFirstThunk;         // RVA to original unbound IAT (ImgThunkData*)
    };
    uint32_t   TimeDateStamp;                  // 0 if not bound,
    // -1 if bound, and real date\time stamp
    //     in ImgDirectoryEntryBoundImport (new BIND)
    // O.W. date/time stamp of DLL bound to (Old BIND)

    uint32_t   ForwarderChain;                 // -1 if no forwarders
    uint32_t   Name;
    uint32_t   FirstThunk;                     // RVA to IAT (if bound this IAT has actual addresses)
};
static_assert( sizeof(ImgImportDescriptor) ==
              (4 + 4*4), 
               "sizeof(ImgImportDescriptor) alignment error" );

//
// Thread Local Storage
//
typedef void
(__stdcall *PImgTlsCallback) (
          void*     DllHandle,
          uint32_t  Reason,
          void*     Reserved
          );

struct ImgTlsDirectory32 final
{
    uint32_t   StartAddressOfRawData;
    uint32_t   EndAddressOfRawData;
    uint32_t   AddressOfIndex;             // uint32_t*
    uint32_t   AddressOfCallBacks;         // PImgTlsCallback *
    uint32_t   SizeOfZeroFill;
    uint32_t   Characteristics;
};
static_assert( sizeof(ImgTlsDirectory32) ==
              (4*6), 
               "sizeof(ImgTlsDirectory32) alignment error" );


struct ImgTlsDirectory64 final
{
    uint64_t   StartAddressOfRawData;
    uint64_t   EndAddressOfRawData;
    uint64_t   AddressOfIndex;         // PDWORD
    uint64_t   AddressOfCallBacks;     // PImgTlsCallback *;
    uint32_t   SizeOfZeroFill;
    uint32_t   Characteristics;
};
static_assert( sizeof(ImgTlsDirectory64) ==
              (8*4 + 4*2), 
               "sizeof(ImgTlsDirectory64) alignment error" );


//
// New format import descriptors pointed to by DataDirectory[ ImageDirectoryEntryBoundImport ]
//

struct ImgBoundImportDescriptor final
{
    uint32_t   TimeDateStamp;
    uint16_t   OffsetModuleName;
    uint16_t   NumberOfModuleForwarderRefs;
    // Array of zero or more ImgBoundForwarderRef follows
};
static_assert( sizeof(ImgBoundImportDescriptor) ==
              (1*4 + 2*2), 
               "sizeof(ImgBoundImportDescriptor) alignment error" );


struct ImgBoundForwarderRef final
{
    uint32_t   TimeDateStamp;
    uint16_t   OffsetModuleName;
    uint16_t   Reserved;
};
static_assert( sizeof(ImgBoundForwarderRef) ==
              (1*4 + 2*2), 
               "sizeof(ImgBoundForwarderRef) alignment error" );


struct ImgBaseRelocation final
{
    uint32_t   VirtualAddress;
    uint32_t   SizeOfBlock;
    //  WORD    TypeOffset[1];
};
static_assert( sizeof(ImgBaseRelocation) ==
              (2*4), 
               "sizeof(ImgBaseRelocation) alignment error" );


//
// Code Integrity in loadconfig (CI)
//

struct ImgLoadConfigCodeIntegrity final
{
    uint16_t   Flags;          // Flags to indicate if CI information is available, etc.
    uint16_t   Catalog;        // 0xFFFF means not available
    uint32_t   CatalogOffset;
    uint32_t   Reserved;       // Additional bitmask to be defined later
};
static_assert( sizeof(ImgLoadConfigCodeIntegrity) ==
              (2*2+2*4), 
               "sizeof(ImgLoadConfigCodeIntegrity) alignment error" );


//
// Load Configuration Directory Entry
//

struct ImgLoadConfigDirectory32 final
{
    uint32_t   Size;
    uint32_t   TimeDateStamp;
    uint16_t   MajorVersion;
    uint16_t   MinorVersion;
    uint32_t   GlobalFlagsClear;
    uint32_t   GlobalFlagsSet;
    uint32_t   CriticalSectionDefaultTimeout;
    uint32_t   DeCommitFreeBlockThreshold;
    uint32_t   DeCommitTotalFreeThreshold;
    uint32_t   LockPrefixTable;                // VA
    uint32_t   MaximumAllocationSize;
    uint32_t   VirtualMemoryThreshold;
    uint32_t   ProcessHeapFlags;
    uint32_t   ProcessAffinityMask;
    uint16_t   CSDVersion;
    uint16_t   DependentLoadFlags;
    uint32_t   EditList;                       // VA
    uint32_t   SecurityCookie;                 // VA
    uint32_t   SEHandlerTable;                 // VA
    uint32_t   SEHandlerCount;
    uint32_t   GuardCFCheckFunctionPointer;    // VA
    uint32_t   GuardCFDispatchFunctionPointer; // VA
    uint32_t   GuardCFFunctionTable;           // VA
    uint32_t   GuardCFFunctionCount;
    uint32_t   GuardFlags;
    ImgLoadConfigCodeIntegrity CodeIntegrity;
    uint32_t   GuardAddressTakenIatEntryTable; // VA
    uint32_t   GuardAddressTakenIatEntryCount;
    uint32_t   GuardLongJumpTargetTable;       // VA
    uint32_t   GuardLongJumpTargetCount;
    uint32_t   DynamicValueRelocTable;         // VA
    uint32_t   CHPEMetadataPointer;
    uint32_t   GuardRFFailureRoutine;          // VA
    uint32_t   GuardRFFailureRoutineFunctionPointer; // VA
    uint32_t   DynamicValueRelocTableOffset;
    uint16_t   DynamicValueRelocTableSection;
    uint16_t   Reserved2;
    uint32_t   GuardRFVerifyStackPointerFunctionPointer; // VA
    uint32_t   HotPatchTableOffset;
};
static_assert( sizeof(ImgLoadConfigDirectory32) ==
              (2*4+2*2+10*4+2*2+9*4+sizeof(ImgLoadConfigCodeIntegrity)+9*4+2*2+2*4), 
               "sizeof(ImgLoadConfigDirectory32) alignment error" );


struct ImgLoadConfigDirectory64 final
{
    uint32_t   Size;
    uint32_t   TimeDateStamp;
    uint16_t   MajorVersion;
    uint16_t   MinorVersion;
    uint32_t   GlobalFlagsClear;
    uint32_t   GlobalFlagsSet;
    uint32_t   CriticalSectionDefaultTimeout;
    uint64_t   DeCommitFreeBlockThreshold;
    uint64_t   DeCommitTotalFreeThreshold;
    uint64_t   LockPrefixTable;                // VA
    uint64_t   MaximumAllocationSize;
    uint64_t   VirtualMemoryThreshold;
    uint64_t   ProcessAffinityMask;
    uint32_t   ProcessHeapFlags;
    uint16_t   CSDVersion;
    uint16_t   DependentLoadFlags;
    uint64_t   EditList;                       // VA
    uint64_t   SecurityCookie;                 // VA
    uint64_t   SEHandlerTable;                 // VA
    uint64_t   SEHandlerCount;
    uint64_t   GuardCFCheckFunctionPointer;    // VA
    uint64_t   GuardCFDispatchFunctionPointer; // VA
    uint64_t   GuardCFFunctionTable;           // VA
    uint64_t   GuardCFFunctionCount;
    uint32_t   GuardFlags;
    ImgLoadConfigCodeIntegrity CodeIntegrity;
    uint64_t   GuardAddressTakenIatEntryTable; // VA
    uint64_t   GuardAddressTakenIatEntryCount;
    uint64_t   GuardLongJumpTargetTable;       // VA
    uint64_t   GuardLongJumpTargetCount;
    uint64_t   DynamicValueRelocTable;         // VA
    uint64_t   CHPEMetadataPointer;            // VA
    uint64_t   GuardRFFailureRoutine;          // VA
    uint64_t   GuardRFFailureRoutineFunctionPointer; // VA
    uint32_t   DynamicValueRelocTableOffset;
    uint16_t   DynamicValueRelocTableSection;
    uint16_t   Reserved2;
    uint64_t   GuardRFVerifyStackPointerFunctionPointer; // VA
    uint32_t   HotPatchTableOffset;
};
static_assert( sizeof(ImgLoadConfigDirectory64) ==
              (2*4 + 2*2 + 3*4 + 6*8 + 1*4 + 2*2 + 8*8 + 1*4 + sizeof(ImgLoadConfigCodeIntegrity) + 8*8 + 1*4 + 2*2 +1*8 + 1*4),
               "sizeof(ImgLoadConfigDirectory64) alignment error" );


#include "PopPack.h"
