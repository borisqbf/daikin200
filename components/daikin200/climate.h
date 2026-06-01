#pragma once

#include "esphome/core/component.h"
#include "esphome/components/climate/climate.h"
#include "esphome/components/remote_base/remote_base.h"

namespace esphome {
namespace daikin200 {

class Daikin200Climate : public climate::Climate, public Component {
 public:
  void setup() override;
  void control(const climate::ClimateCall &call) override;
  climate::ClimateTraits traits() override;

  void set_transmitter(remote_base::RemoteTransmitterBase *tx) {
    this->transmitter_ = tx;
  }

 protected:
  remote_base::RemoteTransmitterBase *transmitter_{nullptr};

  // state only (NOT IR encoding)
  climate::ClimateMode mode_{climate::CLIMATE_MODE_OFF};
  optional<float> target_temperature_;
  optional<climate::ClimateFanMode> fan_mode_;
  optional<climate::ClimateSwingMode> swing_mode_;

  void send_state_();
};

}  // namespace daikin200
}  // namespace esphome