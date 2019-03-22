#include <iostream>
#include <gen/HelloWorldData_DCPS.hpp>
#include <thread> // std::thread, std::this_thread::sleep_for
#include <chrono>
#include <sstream>
#include <string>

/**
 * @brief Simple overload to print out pretty HelloWorld structure
 *
 * @param os
 * @param ts
 * @return std::ostream&
 */
std::ostream &operator<<(std::ostream &os, const HelloWorldData::Msg &ts)
{
  os << ts.message();
  return os;
}

int main(int argc, char *argv[])
{

  // Setup the domain participant and create the helloworld topic
  dds::domain::DomainParticipant dp(0);
  dds::topic::Topic<HelloWorldData::Msg> topic(dp, "HelloWorld");

  // Create a subscriber for the helloWorld topic and attach a DataWriter
  dds::pub::Publisher pub(dp);
  dds::pub::DataWriter<HelloWorldData::Msg> dw(pub, topic);

  // Enable random number generator
  srand(time(NULL));

  char tab[200];

  // Create base message with random ID
  HelloWorldData::Msg msg;
  msg.userID(rand());
  msg.message(DDS::string_dup(""));
  std::cout << "write things\n";

  while (true)
  {
    // Wait for console input and then transmit it over the DDS
    std::cin.getline(tab, 200);
    msg.message(tab);
    dw.write(msg);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  return 0;
}