#include "std.hpp"


namespace Framework {
namespace Math {

namespace detailed
{

const int numInternalBufferSize = 0x1000;


void calcBytesOccurrence(std::istream& stream, std::streamsize size, uint32_t* occurrences)
{
    assert(stream.good() && !stream.eof());
    
    if (size == 0)
        throw std::invalid_argument("Size for bytes occurrences calculation cannot be zero");
    
    char buffer[numInternalBufferSize];
    memset(&buffer, 0, sizeof(char) * numInternalBufferSize);

    std::streamsize streamSize = size;
    while (streamSize > 0 && !stream.eof()) {
        std::streamsize len = std::min(streamSize, (std::streamsize)sizeof(buffer));

        stream.read(buffer, len);
        assert(stream.good());

        std::streamsize bytesCount = stream.gcount(); 
        for (std::streamsize i=0; i < bytesCount; ++i) {
            unsigned char byteValue = (unsigned char)buffer[i];
            occurrences[byteValue]++;

            assert(occurrences[byteValue] < UINT32_MAX);
        }

        streamSize -= bytesCount;
    }
    assert(stream.good());
}

double log_2()
{
    static double result = log((double)2);
    return result;
}

double calcProbability(uint32_t byteOccurrence, std::streamsize len)
{
    assert( len > 0 );

    double probability = (double)byteOccurrence / len;

    if (probability > 0) {
        double log2p = log(probability) / log_2();
        return -probability * log2p;
    }
    else
        throw std::domain_error("Probability cannot be non-positive");
}

} // namespace detailed


double entropy(std::istream& stream, std::streampos offs, std::streamsize size)
{
    assert(stream.good() && !stream.eof());
    assert(size >= 0);

    double resultValue = 0;

    try {
        stream.seekg(offs);

        std::array<uint32_t, 256> bytesOccurrence;
        bytesOccurrence.fill(0);

        detailed::calcBytesOccurrence(stream, size, bytesOccurrence.data());

        for (const auto& occurrence : bytesOccurrence) {
            if (occurrence > 0)
                resultValue += detailed::calcProbability(occurrence, size);
        }
    }
    catch (const std::logic_error&) {
        stream.clear();
    }
    catch (const std::runtime_error&) {
        stream.clear();
    }

    assert(stream.good());
    return resultValue;
}


}} // namespace Framework::Math
