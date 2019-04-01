#pragma once
#include <string>
#include <functional>
#include <gen/JoystickData_DCPS.hpp>
#include <src/onvif/OnvifPTZ.hpp>
class CameraDDS
{
private:
  dds::domain::DomainParticipant dp;
  dds::topic::Topic<JoystickData::Movement> topic;
  dds::sub::Subscriber sub;
  dds::sub::DataReader<JoystickData::Movement> dr;
  // Create the WaitSet
  dds::core::cond::WaitSet ws;
  // Create a ReadCondition for our DataReader and configure it for new data
  dds::sub::cond::ReadCondition rc;

  std::function<void(float, float, float)> updateCameraCallback;

public:
  CameraDDS() : dp(dds::domain::DomainParticipant(0)), topic(dds::topic::Topic<JoystickData::Movement>(dp, "joystick")),
                sub(dds::sub::Subscriber(dp)), dr(dds::sub::DataReader<JoystickData::Movement>(sub, topic)), rc(dds::sub::cond::ReadCondition(dr, dds::sub::status::DataState::new_data()))
  {
    this->ws += this->rc;
  }

  void setCameraUpdateCallback(std::function<void(float, float, float)> f)
  {
    updateCameraCallback = f;
  }
  
  void update()
  {
    // Wait for new data to be available
    ws.wait();
    // Read the data
    auto samples = dr.read();
    std::for_each(samples.begin(),
                  samples.end(),
                  [this](const dds::sub::Sample<JoystickData::Movement> &m) {
                    this->updateCameraCallback(float(m.data().panSpeed()), float(m.data().tiltSpeed()), float(m.data().zoomSpeed()));
                  });
  }
};