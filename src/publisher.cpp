#include <iostream>
#include <gen/HelloWorldData_DCPS.hpp>
#include <thread>         // std::thread, std::this_thread::sleep_for
#include <chrono> 
#include <sstream>
#include <string>


std::ostream& operator<< (std::ostream& os, const HelloWorldData::Msg& ts) {
  os << ts.message();
  return os;
}

int main(int argc, char* argv[])
{
  dds::domain::DomainParticipant dp(0);
  dds::topic::Topic<HelloWorldData::Msg> topic(dp, "HelloWorld");
  dds::pub::Publisher pub(dp);
  dds::pub::DataWriter<HelloWorldData::Msg> dw(pub, topic);
    

  // Initialize random number generation with a seed
  char tab[200];
  HelloWorldData::Msg msg;
  msg.userID(1);
  msg.message(DDS::string_dup(""));
  std::cout << "write things\n";

  while(true) {
    std::cin.getline(tab,200);
    msg.message(tab);
    dw.write(msg);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  return 0;
}