#include "climate.h"

namespace esphome {
namespace daikin200 {

void Daikin200Climate::setup() {}

void Daikin200Climate::control(const climate::ClimateCall &call) {
  // your IR logic here
}

climate::ClimateTraits Daikin200Climate::traits() {
  auto traits = climate::ClimateTraits();

  traits.set_supported_modes({
    climate::CLIMATE_MODE_OFF,
    climate::CLIMATE_MODE_COOL,
    climate::CLIMATE_MODE_HEAT,
    climate::CLIMATE_MODE_AUTO,
    climate::CLIMATE_MODE_FAN_ONLY,
    climate::CLIMATE_MODE_DRY
  });

  return traits;
}

}  // namespace daikin200
}  // namespace esphome