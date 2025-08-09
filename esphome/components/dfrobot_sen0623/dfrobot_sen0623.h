#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"

#ifdef USE_SENSOR
#include "esphome/components/sensor/sensor.h"
#endif

#ifdef USE_BINARY_SENSOR
#include "esphome/components/binary_sensor/binary_sensor.h"
#endif

#ifdef USE_SWITCH
#include "esphome/components/switch/switch.h"
#endif

#ifdef USE_BUTTON
#include "esphome/components/button/button.h"
#endif

namespace esphome {
namespace dfrobot_sen0623 {

#define CON_01 0x01
#define CMD_INIT 0x83

class DfrobotSen0623Component : public uart::UARTDevice, public Component {
#ifdef USE_SWITCH
  SUB_SWITCH(request_rate)
  SUB_SWITCH(hp_led)
#endif


  public:
    void forge_packet(uint8_t control, uint8_t command, uint8_t *senData, uint16_t senLen);

    void send_packet(uint8_t *packetData, size_t len);
    uint8_t read_packet(uint8_t *packetData);

    void print_data(std::string tag, const uint8_t *bytes, size_t len);

    // sensor
    void set_heart_rate_sensor(sensor::Sensor *rate_sensor) { heart_rate_sensor_ = rate_sensor; }
    // binary_sensor
    void set_motion_binary_sensor(binary_sensor::BinarySensor *motion_sensor) { motion_sensor_ = motion_sensor; }
    // button
    void set_reset_button(button::Button *reset_button) { reset_button_ = reset_button; }
    // switch
    void set_switch_request_rate(bool val);
    void set_switch_hp_led(bool val);
    // actions
    void cmd_reset();

    void setup() override;
    void loop() override;
    void dump_config() override;
  protected:
    sensor::Sensor *heart_rate_sensor_{nullptr};
    binary_sensor::BinarySensor *motion_sensor_{nullptr};

    button::Button *reset_button_{nullptr};
};


}  // namespace dfrobot_sen0623
}  // namespace esphome