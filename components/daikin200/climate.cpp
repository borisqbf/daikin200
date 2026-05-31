#include "esphome/components/climate/climate.h"
#include "esphome/core/component.h"
#include "esphome/core/log.h"



namespace esphome {
namespace daikin200 {

static const char *TAG = "daikin200";

void Daikin200Climate::setup() {
  ac_ = new IRDaikin200(4);   // GPIO pin
  ac_->begin();
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
uint8_t Daikin200Climate::encode_airflow(int fan, int swing) {

  if (swing == 1) return 0x52;
  if (swing == 2) return 0x5F;

  switch (fan) {
    case 0: return 0x0F;
    case 1: return 0x1F;
    case 2: return 0x2F;
    case 3: return 0x3F;
    case 4: return 0x5F;
    default: return 0x3F;
  }
}

void Daikin200Climate::control(const climate::ClimateCall &call) {

  if (call.get_mode().has_value())
    this->mode = *call.get_mode();

  if (call.get_target_temperature().has_value())
    this->target_temperature = *call.get_target_temperature();

  switch (this->mode) {

    case climate::CLIMATE_MODE_OFF:
      ac_->off();
      break;

    case climate::CLIMATE_MODE_COOL:
      ac_->on();
      ac_->setMode(kDaikinCool);
      break;

    case climate::CLIMATE_MODE_HEAT:
      ac_->on();
      ac_->setMode(kDaikinHeat);
      break;

    case climate::CLIMATE_MODE_DRY:
      ac_->on();
      ac_->setMode(kDaikinDry);
      break;

    case climate::CLIMATE_MODE_FAN_ONLY:
      ac_->on();
      ac_->setMode(kDaikinFan);
      break;

    case climate::CLIMATE_MODE_AUTO:
      ac_->on();
      ac_->setMode(kDaikinAuto);
      break;

    default:
      break;
  }

  ac_->setTemp((uint8_t)this->target_temperature);

  ac_->send();

  this->publish_state();
}

}  // namespace daikin200
}  // namespace esphome