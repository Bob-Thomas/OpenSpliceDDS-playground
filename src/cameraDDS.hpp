#pragma once
#include <string>
#include <gen/JoystickData_DCPS.hpp>
#include <cpr/cpr.h>

class CameraDDS
{
  private:
    std::string cameraIP;
    std::string username;
    std::string password;
    dds::domain::DomainParticipant dp;
    dds::topic::Topic<JoystickData::Movement> topic;
    dds::sub::Subscriber sub;
    dds::sub::DataReader<JoystickData::Movement> dr;
    // Create the WaitSet
    dds::core::cond::WaitSet ws;
    // Create a ReadCondition for our DataReader and configure it for new data
    dds::sub::cond::ReadCondition rc;
    void moveCamera(const JoystickData::Movement &movement)
    {
        std::cout << "TILT : " << movement.tiltSpeed() << "\n";
        auto r = cpr::Get(cpr::Url{this->cameraIP + "/axis-cgi/com/ptz.cgi"},
                          cpr::Digest{this->username, this->password},
                          cpr::Parameters{
                              {"continuouspantiltmove", std::to_string(movement.panSpeed()) + "," + std::to_string(movement.tiltSpeed())},
                              {"zoom", std::to_string(movement.zoomSpeed())}});
    }

  public:
    CameraDDS(std::string cameraIP, std::string username, std::string password) : cameraIP(cameraIP), username(username), password(password), dp(dds::domain::DomainParticipant(0)), topic(dds::topic::Topic<JoystickData::Movement>(dp, "joystick")),
                                                                                  sub(dds::sub::Subscriber(dp)), dr(dds::sub::DataReader<JoystickData::Movement>(sub, topic)), rc(dds::sub::cond::ReadCondition(dr, dds::sub::status::DataState::new_data()))
    {
        this->ws += this->rc;
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
                          this->moveCamera(m.data());
                      });
    }
};