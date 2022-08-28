struct arch32 {};
struct arch64 {};


template<typename Arch>
struct headers_traits;


template<>
struct headers_traits<arch32> final
{
    typedef  arch32                   arch_type;
    typedef  uint32_t                 VirtualAddress_type;
    typedef  ImgOptionalHeader32      ImgOptionalHeader;
    typedef  ImgNtHeaders32           ImgNtHeaders;
    typedef  ImgThunkData32           ImgThunkData;  
    typedef  ImgTlsDirectory32        ImgTlsDirectory;
    typedef  ImgLoadConfigDirectory32 ImgLoadConfigDirectory;
};


template<>
struct headers_traits<arch64> final
{
    typedef  arch64                   arch_type;
    typedef  uint64_t                 VirtualAddress_type;
    typedef  ImgOptionalHeader64      ImgOptionalHeader;
    typedef  ImgNtHeaders64           ImgNtHeaders;
    typedef  ImgThunkData64           ImgThunkData;  
    typedef  ImgTlsDirectory64        ImgTlsDirectory;
    typedef  ImgLoadConfigDirectory64 ImgLoadConfigDirectory;
};
