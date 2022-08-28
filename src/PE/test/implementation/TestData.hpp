
namespace Test
{


struct TestData
{
    const uint8_t* ptr;
    size_t size;

    TestData() = default;
    TestData(const uint8_t* ptr_, size_t size_);
};

class TestDataHolder
{
public:
    static TestDataHolder& getInstance();
    const TestData& get(const std::string& name);

private:
    using TestDataStorage_t = std::map<std::string, TestData>;
    TestDataStorage_t dataStorage;

    TestDataHolder();
};


} // namespace Test
