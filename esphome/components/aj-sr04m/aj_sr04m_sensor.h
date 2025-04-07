#pragma once

#include <vector>

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace aj_sr04m {


class Aj_sr04mComponent : public sensor::Sensor, public PollingComponent, public uart::UARTDevice {
 public:

  // ========== INTERNAL METHODS ==========
  void update() override;
  void loop() override;
  void dump_config() override;

 protected:
  void check_buffer_();

  std::vector<uint8_t> buffer_;
};

}  // namespace aj_sr04m
}  // namespace esphome