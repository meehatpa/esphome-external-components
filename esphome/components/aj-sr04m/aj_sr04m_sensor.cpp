#include "aj_sr04m_sensor.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"


namespace esphome {
namespace aj_sr04m {

static const char *const TAG = "aj_sr04m.sensor";

void Aj_sr04mComponent::update() {
  this->write_byte(0x1);
  ESP_LOGV(TAG, "Request read out from sensor");
}

void Aj_sr04mComponent::loop() {
  while (this->available() > 0) {
    uint8_t data;
    this->read_byte(&data);

    ESP_LOGV(TAG, "Read byte from sensor: %x", data);

    if (this->buffer_.empty() && data != 0xFF)
      continue;

    this->buffer_.push_back(data);
    if (this->buffer_.size() == 4)
      this->check_buffer_();
  }
}

void Aj_sr04mComponent::check_buffer_() {
  uint8_t checksum = this->buffer_[0] + this->buffer_[1] + this->buffer_[2];

  if (this->buffer_[3] == checksum) {
    uint16_t distance = encode_uint16(this->buffer_[1], this->buffer_[2]);
    if (distance > 250) {
      float meters = distance / 1000.0f;
      ESP_LOGV(TAG, "Distance from sensor: %umm, %.3fm", distance, meters);
      this->publish_state(meters);
    } else {
      ESP_LOGW(TAG, "Invalid data read from sensor: %s", format_hex_pretty(this->buffer_).c_str());
    }
  } else {
    ESP_LOGW(TAG, "checksum failed: %02x != %02x", checksum, this->buffer_[3]);
  }
  this->buffer_.clear();
}

void Aj_sr04mComponent::dump_config() {
  LOG_SENSOR("", "AJ_SR04M Sensor", this);
  ESP_LOGCONFIG(TAG, "  sensor model: aj_sr04m");

  LOG_UPDATE_INTERVAL(this);
}

}  // namespace aj_sr04m
}  // namespace esphome
