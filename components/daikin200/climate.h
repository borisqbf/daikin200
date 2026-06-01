#pragma once

#include "esphome/components/climate/climate.h"
#include "esphome/core/component.h"
#include "esphome/components/remote_transmitter/remote_transmitter.h"

namespace esphome
{
  namespace daikin200
  {

    class Daikin200Climate : public climate::Climate, public Component
    {
    public:
      void setup() override;
      void control(const climate::ClimateCall &call) override;
      climate::ClimateTraits traits() override;
      void set_transmitter(remote_transmitter::RemoteTransmitterComponent *tx)
      {
        this->transmitter_ = tx;
      };

    protected:
      // sender
      void build_frame();
      void send_frame();
      uint8_t frame[25]{0}; // your Daikin200 payload buffer

      // Receiver helpers
      bool validate_frame_(const uint8_t *frame, size_t len);
      void decode_mode_(const uint8_t *frame);
      void decode_temperature_(const uint8_t *frame);
      void decode_fan_(const uint8_t *frame);
      void decode_swing_(const uint8_t *frame);

      uint8_t calculate_checksum_(const uint8_t *frame);
      bool validate_checksum_(const uint8_t *frame);
      void process_received_frame(const uint8_t *data, size_t len);
      remote_transmitter::RemoteTransmitterComponent *transmitter_{nullptr};
    };
  } // namespace daikin200
} // namespace esphome