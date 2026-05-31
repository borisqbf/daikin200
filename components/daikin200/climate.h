#include "esphome.h"

namespace esphome {
namespace daikin200 {

class Daikin200Climate : public climate::Climate, public Component {

 public:
  void setup() override {}

  void control(const climate::ClimateCall &call) override;

  climate::ClimateTraits traits() override {
    auto t = climate::ClimateTraits();
    t.set_supported_modes({
      climate::CLIMATE_MODE_COOL,
      climate::CLIMATE_MODE_HEAT,
      climate::CLIMATE_MODE_AUTO,
      climate::CLIMATE_MODE_FAN_ONLY,
      climate::CLIMATE_MODE_DRY
    });
    return t;
  }
};

}  // namespace daikin200
}  // namespace esphome