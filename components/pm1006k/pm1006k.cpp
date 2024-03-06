#include "pm1006k.h"
#include "esphome/core/log.h"

namespace esphome {
namespace pm1006k {

static const char *const TAG = "pm1006k";

static const uint8_t PM1006k_RESPONSE_HEADER[] = {0x16, 0x11, 0x0B};
static const uint8_t PM1006k_REQUEST[] = {0x11, 0x02, 0x0B, 0x01, 0xE1};

void PM1006kComponent::setup() {
  // because this implementation is currently rx-only, there is nothing to setup
}

void PM1006kComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "PM1006k:");
  LOG_SENSOR("  ", "PM1.0", this->pm_1_0_sensor_);
  LOG_SENSOR("  ", "PM2.5", this->pm_2_5_sensor_);
  LOG_SENSOR("  ", "PM10", this->pm_10_0_sensor_);
  LOG_UPDATE_INTERVAL(this);
  this->check_uart_settings(9600);
}

void PM1006kComponent::update() {
  ESP_LOGV(TAG, "sending measurement request");
  this->write_array(PM1006k_REQUEST, sizeof(PM1006k_REQUEST));
}

void PM1006kComponent::loop() {
  while (this->available() != 0) {
    this->read_byte(&this->data_[this->data_index_]);
    auto check = this->check_byte_();
    if (!check.has_value()) {
      // finished
      this->parse_data_();
      this->data_index_ = 0;
    } else if (!*check) {
      // wrong data
      ESP_LOGV(TAG, "Byte %i of received data frame is invalid.", this->data_index_);
      this->data_index_ = 0;
    } else {
      // next byte
      this->data_index_++;
    }
  }
}

float PM1006kComponent::get_setup_priority() const { return setup_priority::DATA; }

uint8_t PM1006kComponent::pm1006k_checksum_(const uint8_t *command_data, uint8_t length) const {
  uint8_t sum = 0;
  for (uint8_t i = 0; i < length; i++) {
    sum += command_data[i];
  }
  return sum;
}

optional<bool> PM1006kComponent::check_byte_() const {
  uint8_t index = this->data_index_;
  uint8_t byte = this->data_[index];

  // index 0..2 are the fixed header
  if (index < sizeof(PM1006k_RESPONSE_HEADER)) {
    return byte == PM1006k_RESPONSE_HEADER[index];
  }

  if (index < (sizeof(PM1006k_RESPONSE_HEADER) + 16))
    return true;

  // checksum
  if (index == (sizeof(PM1006k_RESPONSE_HEADER) + 16)) {
    uint8_t checksum = pm1006k_checksum_(this->data_, sizeof(PM1006k_RESPONSE_HEADER) + 17);
    if (checksum != 0) {
      ESP_LOGW(TAG, "PM1006k checksum is wrong: %02x, expected zero", checksum);
      return false;
    }
    return {};
  }

  return false;
}

void PM1006kComponent::parse_data_() {
  const int pm_1_0_concentration = this->get_16_bit_uint_(9);
  const int pm_2_5_concentration = this->get_16_bit_uint_(5);
  const int pm_10_0_concentration = this->get_16_bit_uint_(13);
  ESP_LOGD(TAG, "Got PM1.0 Concentration: %d µg/m³", pm_1_0_concentration);
  ESP_LOGD(TAG, "Got PM2.5 Concentration: %d µg/m³", pm_2_5_concentration);
  ESP_LOGD(TAG, "Got PM10 Concentration: %d µg/m³", pm_10_0_concentration);
  
  if (this->pm_1_0_sensor_ != nullptr) {
    this->pm_1_0_sensor_->publish_state(pm_1_0_concentration);
  }
  if (this->pm_2_5_sensor_ != nullptr) {
    this->pm_2_5_sensor_->publish_state(pm_2_5_concentration);
  }
  if (this->pm_10_0_sensor_ != nullptr) {
    this->pm_10_0_sensor_->publish_state(pm_10_0_concentration);
  }

}

uint16_t PM1006kComponent::get_16_bit_uint_(uint8_t start_index) const {
  return encode_uint16(this->data_[start_index], this->data_[start_index + 1]);
}

}  // namespace pm1006k
}  // namespace esphome
