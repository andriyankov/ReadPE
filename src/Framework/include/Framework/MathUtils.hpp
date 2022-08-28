
namespace Math {


uint32_t alignDown(uint32_t value_, uint32_t factor);
uint32_t alignUp(uint32_t value_, uint32_t factor);
double   entropy(std::istream& stream, std::streampos offset, std::streamsize size);


inline uint32_t alignDown(uint32_t value_, uint32_t factor)
{
    return value_ & ~(factor-1);
}

inline uint32_t alignUp(uint32_t value_, uint32_t factor)
{
    return alignDown(value_ - 1,factor) + factor;
}


} // namespace Math
