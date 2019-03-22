#include <iostream>
#include <gen/HelloWorldData_DCPS.hpp>
#include <thread> // std::thread, std::this_thread::sleep_for
#include <chrono>

/**
 * @brief Generic listener for DDS data readers
 *
 * @tparam T
 */
template <typename T>
class ModelListener : public dds::sub::NoOpDataReaderListener<T>
{
  public:
    /**
     * @brief Callback that the dataReader calls when data is available
     *
     * @param dr data reader
     */
    virtual void on_data_available(
        dds::sub::DataReader<T> &dr)
    {
        auto samples = dr.read();
        std::for_each(samples.begin(), samples.end(),
                      [](const dds::sub::Sample<T> &s) {
                          std::cout << s.data().message() << std::endl;
                      });
    }
};

int main(int argc, char *argv[])
{

    // Setup the domain participant and create the helloworld topic
    dds::domain::DomainParticipant dp(0);
    dds::topic::Topic<HelloWorldData::Msg> topic(dp, "HelloWorld");

    // Create a subscriber for the helloWorld topic and attach a DataReader
    dds::sub::Subscriber sub(dp);
    dds::sub::DataReader<HelloWorldData::Msg> dr(sub, topic);

    // Use the listener pattern to listen for messsages
    ModelListener<HelloWorldData::Msg> listener;
    dr.listener(&listener, dds::core::status::StatusMask::data_available());

    // keep the program alive
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return 0;
}
