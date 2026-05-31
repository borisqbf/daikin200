#include "climate.h"
#include "esphome/core/log.h"

namespace esphome {
namespace daikin200 {

static const char *TAG = "daikin200";

void Daikin200Climate::setup() {}

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

  t.set_supports_current_temperature(false);
  t.set_visual_min_temperature(16);
  t.set_visual_max_temperature(30);
  t.set_visual_temperature_step(0.5);

  return t;
}

void Daikin200Climate::control(const climate::ClimateCall &call) {
  if (call.get_mode().has_value()) {
    this->mode = *call.get_mode();
  }

  if (call.get_target_temperature().has_value()) {
    this->target_temperature = *call.get_target_temperature();
  }

  this->publish_state();

  ESP_LOGD(TAG, "Climate updated");
}

}  // namespace daikin200
}  // namespace esphome