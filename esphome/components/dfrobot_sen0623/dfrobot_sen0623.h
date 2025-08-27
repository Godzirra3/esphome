#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"

#ifdef USE_SENSOR
#include "esphome/components/sensor/sensor.h"
#endif

#ifdef USE_TEXT_SENSOR
#include "esphome/components/text_sensor/text_sensor.h"
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

#ifdef USE_SELECT
#include "esphome/components/select/select.h"
#endif

namespace esphome {
namespace dfrobot_sen0623 {

enum Sen0623Mode : uint8_t {
  SEN0623_MODE_FALL = 0x01,
  SEN0623_MODE_SLEEP = 0x02,
};

class DfrobotSen0623Component : public uart::UARTDevice, public PollingComponent {
#ifdef USE_SWITCH
  SUB_SWITCH(request_rate)
  SUB_SWITCH(hp_led)
#endif
#ifdef USE_SELECT
  SUB_SELECT(mode)
#endif

 public:
  void request(std::pair<uint8_t, uint8_t> operation);

  void forge_packet(uint8_t control, uint8_t command, uint8_t *senData, uint16_t senLen);

  void send_packet(uint8_t *packetData, size_t len);

  bool process_packet(uint8_t *packetData, size_t len);

  uint8_t wait_for_packet(std::pair<uint8_t, uint8_t> operation);
  uint8_t read_packet(uint8_t *packetData);

  void print_data(std::string tag, const uint8_t *bytes, size_t len);

  // sensor
  void set_heart_rate_sensor(sensor::Sensor *rate_sensor) { heart_rate_sensor_ = rate_sensor; }
  void set_breath_rate_sensor(sensor::Sensor *rate_sensor) { breath_rate_sensor_ = rate_sensor; }
  void set_human_distance_sensor(sensor::Sensor *human_distance_sensor) {
    human_distance_sensor_ = human_distance_sensor;
  }
  void set_human_move_range_sensor(sensor::Sensor *human_move_range_sensor) {
    human_move_range_sensor_ = human_move_range_sensor;
  }
  // text sensor
  void set_status_text_sensor(text_sensor::TextSensor *status_text_sensor) { status_text_sensor_ = status_text_sensor; }
  void set_movement_text_sensor(text_sensor::TextSensor *movement_text_sensor) {
    movement_text_sensor_ = movement_text_sensor;
  }
  // binary_sensor
  void set_presence_binary_sensor(binary_sensor::BinarySensor *presence_sensor) { presence_sensor_ = presence_sensor; }
  void set_fall_detected_binary_sensor(binary_sensor::BinarySensor *binary_sensor) {
    fall_detected_binary_sensor_ = binary_sensor;
  }
  // button
  void set_reset_button(button::Button *reset_button) { reset_button_ = reset_button; }
  // switch
  void set_switch_request_rate(bool val);
  void set_switch_hp_led(bool val);
  void set_mode(uint8_t mode);
  // actions
  void cmd_reset();

  void setup() override;
  void loop() override;
  void update() override;
  void dump_config() override;

 protected:
  sensor::Sensor *heart_rate_sensor_{nullptr};
  sensor::Sensor *breath_rate_sensor_{nullptr};
  sensor::Sensor *human_distance_sensor_{nullptr};
  sensor::Sensor *human_move_range_sensor_{nullptr};
  binary_sensor::BinarySensor *presence_sensor_{nullptr};
  binary_sensor::BinarySensor *fall_detected_binary_sensor_{nullptr};

  text_sensor::TextSensor *status_text_sensor_{nullptr};
  text_sensor::TextSensor *movement_text_sensor_{nullptr};

  button::Button *reset_button_{nullptr};
};

}  // namespace dfrobot_sen0623
}  // namespace esphome
