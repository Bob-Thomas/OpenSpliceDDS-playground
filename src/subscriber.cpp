#include <iostream>
#include <thread> // std::thread, std::this_thread::sleep_for
#include <string>
#include <chrono>
#include <src/cameraDDS.hpp>

template <typename T>
class ModelListener : public dds::sub::NoOpDataReaderListener<T>
{
  public:
    virtual void on_data_available(
        dds::sub::DataReader<T> &dr)
    {
        auto samples = dr.read();
        std::for_each(samples.begin(), samples.end(),
                      [](const dds::sub::Sample<T> &s) {
                          std::cout << s.data() << std::endl;
                      });
    }
};

int main(int argc, char *argv[])
{
    CameraDDS cameraDDS(argv[1], argv[2], argv[3]);

    while (true)
    {
        cameraDDS.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return 0;
}
