#include "climate.h"
#include "esphome/core/log.h"

namespace esphome {
namespace daikin200 {

static const char *TAG = "daikin200";

uint8_t base_frame[25] = {
  0x11, 0xDA, 0x17, 0x48, 0x04,
  0x00, 0x4E, 0x11, 0xDA, 0x17,
  0x48, 0x00, 0x73, 0x00, 0x21,
  0x00, 0x00, 0x1C, 0x50, 0x00,
  0x20, 0x00, 0x00, 0x00, 0x6A
};

static constexpr uint8_t DAIKIN_MODE_BYTE = 12;
static constexpr uint8_t DAIKIN_TEMP_BYTE = 16;
static constexpr uint8_t DAIKIN_FAN_BYTE = 17;
static constexpr uint8_t DAIKIN_SWING_BYTE_1 = 18;
static constexpr uint8_t DAIKIN_SWING_BYTE_2 = 19;
static constexpr uint8_t DAIKIN_CHECKSUM_BYTE = 24;



void Daikin200Climate::setup() {
  ESP_LOGI(TAG, "Daikin200 climate setup complete");
}

climate::ClimateTraits Daikin200Climate::traits() {
  auto t = climate::ClimateTraits();

  t.set_supported_modes({
    climate::CLIMATE_MODE_OFF,
    climate::CLIMATE_MODE_COOL,
    climate::CLIMATE_MODE_HEAT,
    climate::CLIMATE_MODE_AUTO,
    climate::CLIMATE_MODE_DRY,
    climate::CLIMATE_MODE_FAN_ONLY
  });


  t.set_visual_min_temperature(16);
  t.set_visual_max_temperature(30);
  t.set_visual_temperature_step(0.5);

  return t;
}

// ------------------------------
// AIRFLOW ENCODER
// ------------------------------


void Daikin200Climate::control(const climate::ClimateCall &call) {

  // Update ESPHome internal state
  if (call.get_mode().has_value())
    this->mode = *call.get_mode();

  if (call.get_target_temperature().has_value())
    this->target_temperature = *call.get_target_temperature();

 if (call.get_swing_mode().has_value()) {
    this->swing_mode = *call.get_swing_mode();
}

  // Rebuild IR frame from new state
  this->build_frame();

  // Send IR signal
  this->send_frame();

  // Publish updated state to Home Assistant
  this->publish_state();
}

void Daikin200Climate::build_frame() {
  memcpy(frame, base_frame, 25);

  // -------------------------
  // MODE
  // -------------------------
  switch (this->mode) {

    case climate::CLIMATE_MODE_COOL:
      frame[MODE_BYTE] = 0x73;
      break;

    case climate::CLIMATE_MODE_HEAT:
      frame[MODE_BYTE] = 0x51;
      break;

    case climate::CLIMATE_MODE_DRY:
      frame[MODE_BYTE] = 0x47;
      break;

    case climate::CLIMATE_MODE_AUTO:
      frame[MODE_BYTE] = 0x43;
      break;

    case climate::CLIMATE_MODE_FAN_ONLY:
      frame[MODE_BYTE] = 0x40;
      break;

    default:
      frame[MODE_BYTE] = 0x00;
      break;
  }

  // -------------------------
  // TEMP
  // -------------------------
  uint8_t temp = (uint8_t) this->target_temperature;
  if (temp < 16) temp = 16;
  if (temp > 30) temp = 30;

  frame[TEMP_BYTE] = 0x1A + (temp - 22) * 2;

  // -------------------------
  // FAN (ESPHome-native)
  // -------------------------
  switch (this->fan_mode.value_or(climate::CLIMATE_FAN_AUTO)) {

    case climate::CLIMATE_FAN_LOW:
      frame[FAN_BYTE] = 0x1F;
      break;

    case climate::CLIMATE_FAN_MEDIUM:
      frame[FAN_BYTE] = 0x3F;
      break;

    case climate::CLIMATE_FAN_HIGH:
      frame[FAN_BYTE] = 0x5F;
      break;

    case climate::CLIMATE_FAN_AUTO:
    default:
      frame[FAN_BYTE] = 0x0F;
      break;
  }

  // -------------------------
  // SWING (ESPHome-native)
  // -------------------------
  switch (this->swing_mode) {

    case climate::CLIMATE_SWING_VERTICAL:
      frame[SWING_BYTE] = 0x52;
      frame[SWING_BYTE2] = 0x00;
      break;

    case climate::CLIMATE_SWING_BOTH:
      frame[SWING_BYTE] = 0x5F;
      frame[SWING_BYTE2] = 0x00;
      break;

    case climate::CLIMATE_SWING_OFF:
    default:
      frame[SWING_BYTE] = 0x20;
      frame[SWING_BYTE2] = 0x00;
      break;
  }

  // -------------------------
  // CHECKSUM
  // -------------------------
  uint8_t sum = 0;
  for (int i = 0; i < CHECKSUM_BYTE; i++) {
    sum += frame[i];
  }

  frame[CHECKSUM_BYTE] = sum;
}

void Daikin200Climate::send_frame() {
  // Placeholder for ESPHome IR transmission

  ESP_LOGD(TAG, "Sending IR frame (not implemented yet)");
}

}  // namespace daikin200
}  // namespace esphome