#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace aj_sr04m {

class AJ_SR04M_Sensor : public sensor::Sensor, public PollingComponent {
 public:
  AJ_SR04M_Sensor(uart::UARTComponent *uart) : uart_(uart) {}

  void update() override {
    uint8_t frame[4];
    int pos = 0;

    uart_->write(0x01);

    while (available()) {
      frame[pos++] = read();
      if(pos == 4) {
        uint8_t checksum = frame[0] + frame[1] + frame[2];
        ESP_LOGV(TAG, "Reply %x:%x:%x:%x", frame[0], frame[1], frame[2], frame[3]);
        if (checksum == frame[3]) {
          uint16_t distance = encode_uint16(frame[1], frame[2]);
          publish_state(distance);
        }
        break;
      }
    }
  }

 protected:
   uart::UARTComponent *uart_;
};

}  // namespace aj_sr04m
}  // namespace esphome
