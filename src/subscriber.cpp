#include <iostream>
#include <gen/HelloWorldData_DCPS.hpp>
#include <thread> // std::thread, std::this_thread::sleep_for
#include <chrono>

template<typename T>
class ModelListener : public dds::sub::NoOpDataReaderListener<T>
{
  public:
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

    dds::domain::DomainParticipant dp(0);
    dds::topic::Topic<HelloWorldData::Msg> topic(dp, "HelloWorld");
    dds::sub::Subscriber sub(dp);
    dds::sub::DataReader<HelloWorldData::Msg> dr(sub, topic);

    ModelListener<HelloWorldData::Msg> listener;
    dr.listener(&listener, dds::core::status::StatusMask::data_available());
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return 0;
}
