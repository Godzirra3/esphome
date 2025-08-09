#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace dfrobot_sen0623 {

class DfrobotSen0623Component : public uart::UARTDevice, public Component {
  public:
    void forge_packet(uint8_t control, uint8_t command, uint8_t *senData, uint16_t senLen);

    void send_packet(uint8_t *packetData, size_t len);
    uint8_t read_packet(uint8_t *packetData);

    void print_data(std::string tag, const uint8_t *bytes, size_t len);

    void setup() override;
    void loop() override;
    void dump_config() override;
};


}  // namespace dfrobot_sen0623
}  // namespace esphome