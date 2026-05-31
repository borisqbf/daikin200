#include "daikin200.h"

namespace esphome {
namespace daikin200 {

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

// ------------------------------
// FRAME BUILDER
// ------------------------------
void Daikin200Climate::build_frame() {

  memset(frame, 0, sizeof(frame));

  frame[0] = 0x11; frame[1] = 0xDA; frame[2] = 0x17;
  frame[3] = 0x48; frame[4] = 0x04; frame[5] = 0x00;
  frame[6] = 0x4E;

  frame[7] = 0x11; frame[8] = 0xDA; frame[9] = 0x17;
  frame[10] = 0x48; frame[11] = 0x00; frame[12] = 0x73;

  // MODE
  switch (this->mode) {
    case climate::CLIMATE_MODE_COOL:
      frame[13] = 0x42;
      frame[14] = 0x10;
      break;

    case climate::CLIMATE_MODE_HEAT:
      frame[13] = 0x04;
      frame[14] = 0x11;
      break;

    case climate::CLIMATE_MODE_FAN_ONLY:
      frame[13] = 0x04;
      frame[14] = 0x01;
      break;

    case climate::CLIMATE_MODE_DRY:
      frame[13] = 0x04;
      frame[14] = 0x71;
      break;

    case climate::CLIMATE_MODE_AUTO:
      frame[13] = 0x04;
      frame[14] = 0x31;
      break;
  }

  // TEMP
  frame[15] = encode_temp(this->target_temp);

  // AIRFLOW
  frame[16] = encode_airflow(this->fan, this->swing);

  // padding (fixed pattern observed)
  frame[17] = 0x00;
  frame[18] = 0x20;
  frame[19] = 0x00;
  frame[20] = 0x00;
  frame[21] = 0x00;
  frame[22] = 0x00;
  frame[23] = 0x00;

  // CHECKSUM
  uint8_t sum = 0;
  for (int i = 13; i < 24; i++) sum += frame[i];
  frame[24] = sum;
}

// ------------------------------
// SEND (hook ESPHome IR)
// ------------------------------
void Daikin200Climate::send_frame() {
  this->build_frame();
  auto call = id(remote_transmitter);
  call->transmit_raw((std::vector<int>)frame, 38);
}

// ------------------------------
// CONTROL ENTRY POINT
// ------------------------------
void Daikin200Climate::control(const climate::ClimateCall &call) {

  if (call.get_mode().has_value())
    this->mode = *call.get_mode();

  if (call.get_target_temperature().has_value())
    this->target_temp = *call.get_target_temperature();

  if (call.get_fan_mode().has_value())
    this->fan = *call.get_fan_mode();

  this->send_frame();
}

// ------------------------------
// CAPABILITIES
// ------------------------------
climate::ClimateTraits Daikin200Climate::traits() {
  auto t = climate::ClimateTraits();

  t.set_supported_modes({
    climate::CLIMATE_MODE_COOL,
    climate::CLIMATE_MODE_HEAT,
    climate::CLIMATE_MODE_AUTO,
    climate::CLIMATE_MODE_FAN_ONLY,
    climate::CLIMATE_MODE_DRY
  });

  t.set_visual_min_temperature(16);
  t.set_visual_max_temperature(30);
  t.set_visual_temperature_step(1);

  return t;
}

} // namespace daikin200
} // namespace esphome\

