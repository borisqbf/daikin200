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
};

}  // namespace daikin200
}  // namespace esphome