#pragma once

#include "esphome/components/climate/climate.h"
#include "esphome/core/component.h"

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

    protected:
      void build_frame();
      void send_frame();
      uint8_t frame[25]{0}; // your Daikin200 payload buffer
    };
  } // namespace daikin200
} // namespace esphome