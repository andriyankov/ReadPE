
namespace Test
{


template<typename arch_type>
class Fixture
{
public:
    typedef  std::shared_ptr<Fixture>  FixturePtr_t;

public:
    explicit Fixture(const void* data, size_t sz, bool lazyMode)
        : image(stream, lazyMode)
    {
        Fw::Io::StreamUtils::prepare(stream);
        stream.write(reinterpret_cast<const char*>(data),
            static_cast<std::streamsize>(sz));
    }

    static FixturePtr_t create(const std::string& name, bool lazyMode=true)
    {
        const auto& data = TestDataHolder::getInstance().get(name);
        return FixturePtr_t(new Fixture(data.ptr, data.size, lazyMode));
    }

    std::stringstream& getStream()
    {
        return stream;
    }

    pe::Image<arch_type>& getImage()
    {
        return image;
    }

private:
    std::stringstream    stream;
    pe::Image<arch_type> image;
};


} // namespace Test
