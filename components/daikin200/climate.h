#pragma once

#include "esphome/components/climate/climate.h"
#include "esphome/core/component.h"

namespace esphome {
namespace daikin200 {

class Daikin200Climate : public climate::Climate, public Component {
 public:
  void setup() override;
  void control(const climate::ClimateCall &call) override;
  climate::ClimateTraits traits() override;
  private:
  
    float target_temp;
    int fan = 2;
    int swing = 0;

    uint8_t frame[25];

    uint8_t encode_temp(float temp) {
      if (mode == climate::CLIMATE_MODE_HEAT) {
        return 0x20 + 2 * (int)(temp - 22);
      }
      return 0x1A + 2 * (int)(temp - 22);
    }

    uint8_t encode_airflow(int fan, int swing);

    void build_frame();
    void send_frame();
};

}  // namespace daikin200
}  // namespace esphome