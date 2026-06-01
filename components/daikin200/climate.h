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

  // Optional but useful if you ever add IR receive support later
  void process_received_frame(const uint8_t *data, size_t len);

 protected:
  void build_frame();
  void send_frame();

  bool validate_frame_(const uint8_t *frame, size_t len);
  bool validate_checksum_(const uint8_t *frame);
  uint8_t calculate_checksum_(const uint8_t *frame);

  void decode_mode_(const uint8_t *frame);
  void decode_temperature_(const uint8_t *frame);
  void decode_fan_(const uint8_t *frame);
  void decode_swing_(const uint8_t *frame);

  remote_base::RemoteTransmitterBase *transmitter_{nullptr};

  uint8_t frame[25]{0};
};

}  // namespace daikin200
}  // namespace esphome