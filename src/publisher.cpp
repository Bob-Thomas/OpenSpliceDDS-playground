#include <iostream>
#include <gen/JoystickData_DCPS.hpp>
#include <thread> // std::thread, std::this_thread::sleep_for
#include <chrono>
#include <sstream>
#include <string>
#include "joystick.hh"
#include <src/inputLister.hpp>
#include <src/joystickDDS.hpp>

int main(int argc, char *argv[])
{

  srand(time(NULL));
  InputLister lister("/sys/class/input/", "js+");
  std::cout << "Select your input device\n";
  lister.showList(std::cout);

  int answer = 0;
  std::cin >> answer;
  Joystick joystick(answer);

  if (!joystick.isFound())
  {
    printf("open failed.\n");
    exit(1);
  }

  DDSJoystick joystickController(joystick);

  while (true)
  {
    joystickController.update();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  return 0;
}