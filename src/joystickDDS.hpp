#pragma once

#include <gen/JoystickData_DCPS.hpp>
#include "joystick.hh"

/**
 * DDSJoystick combines the joystick with the DDS writer
 */
class DDSJoystick
{
  private:
    static constexpr int16_t MIN_AXIS = -32767;
    static constexpr uint16_t MAX_AXIS = 32767;

  private:
    Joystick &joystick;
    dds::domain::DomainParticipant dp;
    dds::topic::Topic<JoystickData::Movement> topic;
    dds::pub::Publisher pub;
    dds::pub::DataWriter<JoystickData::Movement> dw;

    JoystickData::Movement movement;

    /**
     * @brief Returns a linear mapped value based on a incomming max and min an a output max and min
     * 
     * @param x Actual input value
     * @param in_min minimal value of the input
     * @param in_max maximal value of the input
     * @param out_min minimal output value
     * @param out_max maximal output value
     * @return long 
     */
    long mapSpeed(long x, long in_min, long in_max, long out_min, long out_max)
    {

        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

  public:
    DDSJoystick(Joystick &joystick) : joystick(joystick), dp(dds::domain::DomainParticipant(0)),
                                      topic(dds::topic::Topic<JoystickData::Movement>(dp, "joystick")),
                                      pub(dds::pub::Publisher(dp)), dw(dds::pub::DataWriter<JoystickData::Movement>(pub, topic)){};
    void update()
    {
        JoystickEvent event;
        if (joystick.sample(&event))
        {
            std::cout << event.value << "\n";
            if (event.isAxis())
            {
                switch (int(event.number))
                {
                case 0:
                {
                    movement.panSpeed(this->mapSpeed(event.value, MIN_AXIS, MAX_AXIS, -100, 100));
                    break;
                }
                case 1:
                {
                    movement.tiltSpeed(this->mapSpeed(event.value, MIN_AXIS, MAX_AXIS, -100, 100));
                    break;
                }
                case 3:
                {
                    movement.zoomSpeed(this->mapSpeed(event.value, MIN_AXIS, MAX_AXIS, 9999, 1));
                    break;
                }
                }
                dw.write(movement);
            }
        }
    };
};