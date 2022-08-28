enum Signature
{
    DOS = 0x5A4D,      // MZ
    PE  = 0x00004550,  // PE00
};

enum Machine : uint16_t
{
     I386  = 0x014C,  // Intel 386.
     SH4   = 0x01A6,  // SH4 little-endian
     IA64  = 0x0200,  // Intel 64
     AMD64 = 0x8664,  // AMD64 (K8)
};

enum OptionalHeader : uint16_t
{
    Magic32 = 0x10B,
    Magic64 = 0x20B,
};

enum File
{
     RelocsStripped       = 0x0001,  // Relocation info stripped from file
     ExecutableImage      = 0x0002,  // File is executable  (i.e. no unresolved externel references)
     LineNumsStripped     = 0x0004,  // Line nunbers stripped from file
     LocalSymsStripped    = 0x0008,  // Local symbols stripped from file
     AggresiveWsTrim      = 0x0010,  // Agressively trim working set
     LargeAddressAware    = 0x0020,  // App can handle >2gb addresses
     BytesReversedLo      = 0x0080,  // Bytes of machine word are reversed
     Machine32bit         = 0x0100,  // 32 bit word machine
     DebugStripped        = 0x0200,  // Debugging info stripped from file in .DBG file
     RemovableRunFromSwap = 0x0400,  // If Image is on removable media, copy and run from the swap file
     NetRunFromSwap       = 0x0800,  // If Image is on Net, copy and run from the swap file
     System               = 0x1000,  // System File
     Dll                  = 0x2000,  // File is a DLL
     UpSystemOnly         = 0x4000,  // File should only be run on a UP machine
     BytesReversedHi      = 0x8000,  // Bytes of machine word are reversed
};

enum Directory : uint32_t
{
     ExportEntry        =  0,   // Export Directory
     ImportEntry        =  1,   // Import Directory
     ResourceEntry      =  2,   // Resource Directory
     ExceptionEntry     =  3,   // Exception Directory
     SecurityEntry      =  4,   // Security Directory
     BaserelocEntry     =  5,   // Base Relocation Table
     DebugEntry         =  6,   // Debug Directory
     ArchitectureEntry  =  7,   // Architecture Specific Data
     GlobalptrEntry     =  8,   // RVA of GP
     TlsEntry           =  9,   // TLS Directory
     LoadConfigEntry    = 10,   // Load Configuration Directory
     BoundImportEntry   = 11,   // Bound Import Directory in headers
     IatEntry           = 12,   // Import Address Table
     DelayImportEntry   = 13,   // Delay Load Import Descriptors
     ComDescriptorEntry = 14,   // COM Runtime descriptor
};

enum Subsystem
{
     Unknown                =  0,  // Unknown subsystem
     Native                 =  1,  // Image doesn't require a subsystem
     WindowsGui             =  2,  // Image runs in the Windows GUI subsystem
     WindowsCui             =  3,  // Image runs in the Windows character subsystem
     Os2Cui                 =  5,  // image runs in the OS/2 character subsystem
     PosixCui               =  7,  // image runs in the Posix character subsystem
     NativeWindows          =  8,  // image is a native Win9x driver
     WindowsCeGui           =  9,  // Image runs in the Windows CE subsystem
     EfiApplication         = 10,
     EfiBootServiceDriver   = 11,
     EfiRuntimeDriver       = 12,
     EfiRom                 = 13,
     Xbox                   = 14,
     WindowsBootApplication = 16,
};

enum SectionCharacteristics
{
    CntCode               = 0x00000020,  // Section contains code
    CntInitializedData    = 0x00000040,  // Section contains initialized data
    CntUninitializedData  = 0x00000080,  // Section contains uninitialized data
    LnkInfo               = 0x00000200,  // Section contains comments or some other type of information
    LnkRemove             = 0x00000800,  // Section contents will not become part of image
    LnkComdat             = 0x00001000,  // Section contents comdat
    NoDeferSpecExc        = 0x00004000,  // Reset speculative exceptions handling bits in the TLB entries for this section
    Gprel                 = 0x00008000,  // Section content can be accessed relative to GP
    MemFardata            = 0x00008000,
    MemPurgeable          = 0x00020000,
    Mem16bit              = 0x00020000,
    MemLocked             = 0x00040000,
    MemPreload            = 0x00080000,
    Align_1bytes          = 0x00100000,
    Align_2bytes          = 0x00200000,
    Align_4bytes          = 0x00300000,
    Align_8bytes          = 0x00400000,
    Align_16bytes         = 0x00500000,  // Default alignment if no others are specified
    Align_32bytes         = 0x00600000,
    Align_64bytes         = 0x00700000,
    Align_128bytes        = 0x00800000,
    Align_256bytes        = 0x00900000,
    Align_512bytes        = 0x00A00000,
    Align_1024bytes       = 0x00B00000,
    Align_2048bytes       = 0x00C00000,
    Align_4096bytes       = 0x00D00000,
    Align_8192bytes       = 0x00E00000,
    AlignMask             = 0x00F00000,
    LnkNrelocOvfl         = 0x01000000,  // Section contains extended relocations
    MemDiscardable        = 0x02000000,  // Section can be discarded
    MemNotCached          = 0x04000000,  // Section is not cachable
    MemNotPaged           = 0x08000000,  // Section is not pageable
    MemShared             = 0x10000000,  // Section is shareable
    MemExecute            = 0x20000000,  // Section is executable
    MemRead               = 0x40000000,  // Section is readable
    MemWrite              = 0x80000000,  // Section is writeable
    TlsScaleIndex         = 0x00000001,  // Tls index is scaled
};

enum
{
     SizeofFileHeader          = 20,
     NumberOfDirectoryEntries  = 16,
     SizeOfShortName           =  8,
     SizeOfSectionHeader       = 40,
};

//
// Library specific constants
//

enum
{
     MaxSectionHeadersCount    = 104857, // See PeLib Thesises 2.1
     MaxImportDescriptorsCount = 209715, // See PeLib Thesises 2.2
     MaxBoundHeadersCount      = 524288, // See PeLib Thesises 2.3
};
