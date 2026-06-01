
#include <vector>
#include "climate.h"
#include "esphome/core/log.h"

namespace esphome
{
  namespace daikin200
  {

    static const char *TAG = "daikin200";

    uint8_t base_frame[25] = {
        0x11, 0xDA, 0x17, 0x48, 0x04,
        0x00, 0x4E, 0x11, 0xDA, 0x17,
        0x48, 0x00, 0x73, 0x00, 0x21,
        0x00, 0x00, 0x1C, 0x50, 0x00,
        0x20, 0x00, 0x00, 0x00, 0x6A};

    static constexpr uint8_t MODE_BYTE = 12;
    static constexpr uint8_t TEMP_BYTE = 16;
    static constexpr uint8_t FAN_BYTE = 17;
    static constexpr uint8_t SWING_BYTE_1 = 18;
    static constexpr uint8_t SWING_BYTE_2 = 19;
    static constexpr uint8_t CHECKSUM_BYTE = 24;

    void Daikin200Climate::setup()
    {
      ESP_LOGI(TAG, "Daikin200 climate setup complete");
      this->mode = climate::CLIMATE_MODE_OFF;
      this->target_temperature = 21.0f; // 🔥 MUST NOT BE NULL

      this->publish_state();
    }

    climate::ClimateTraits Daikin200Climate::traits()
    {
      auto t = climate::ClimateTraits();

      t.set_supported_modes({climate::CLIMATE_MODE_OFF,
                             climate::CLIMATE_MODE_COOL,
                             climate::CLIMATE_MODE_HEAT,
                             climate::CLIMATE_MODE_AUTO,
                             climate::CLIMATE_MODE_DRY,
                             climate::CLIMATE_MODE_FAN_ONLY});

      t.set_visual_min_temperature(16);
      t.set_visual_max_temperature(30);
      t.set_visual_temperature_step(0.5);

      return t;
    }

    // ------------------------------
    // AIRFLOW ENCODER
    // ------------------------------

    void Daikin200Climate::control(const climate::ClimateCall &call)
    {

      if (std::isnan(this->target_temperature))
      {
        this->target_temperature = 21.0f;
      }
      // Update ESPHome internal state
      if (call.get_mode().has_value())
        this->mode = *call.get_mode();

      if (call.get_target_temperature().has_value())
        this->target_temperature = *call.get_target_temperature();

      if (call.get_swing_mode().has_value())
      {
        this->swing_mode = *call.get_swing_mode();
      }

      // Rebuild IR frame from new state
      this->build_frame();

      // Send IR signal
      this->send_frame();

      // Publish updated state to Home Assistant
      this->publish_state();
    }

    void Daikin200Climate::build_frame()
    {
      memcpy(frame, base_frame, 25);

      // -------------------------
      // MODE
      // -------------------------
      switch (this->mode)
      {

      case climate::CLIMATE_MODE_COOL:
        frame[MODE_BYTE] = 0x73;
        break;

      case climate::CLIMATE_MODE_HEAT:
        frame[MODE_BYTE] = 0x51;
        break;

      case climate::CLIMATE_MODE_DRY:
        frame[MODE_BYTE] = 0x47;
        break;

      case climate::CLIMATE_MODE_AUTO:
        frame[MODE_BYTE] = 0x43;
        break;

      case climate::CLIMATE_MODE_FAN_ONLY:
        frame[MODE_BYTE] = 0x40;
        break;

      default:
        frame[MODE_BYTE] = 0x00;
        break;
      }

      // -------------------------
      // TEMP
      // -------------------------
      uint8_t temp = (uint8_t)this->target_temperature;
      if (temp < 16)
        temp = 16;
      if (temp > 30)
        temp = 30;

      frame[TEMP_BYTE] = 0x1A + (temp - 22) * 2;

      // -------------------------
      // FAN (ESPHome-native)
      // -------------------------
      switch (this->fan_mode.value_or(climate::CLIMATE_FAN_AUTO))
      {

      case climate::CLIMATE_FAN_LOW:
        frame[FAN_BYTE] = 0x1F;
        break;

      case climate::CLIMATE_FAN_MEDIUM:
        frame[FAN_BYTE] = 0x3F;
        break;

      case climate::CLIMATE_FAN_HIGH:
        frame[FAN_BYTE] = 0x5F;
        break;

      case climate::CLIMATE_FAN_AUTO:
      default:
        frame[FAN_BYTE] = 0x0F;
        break;
      }

      // -------------------------
      // SWING (ESPHome-native)
      // -------------------------
      switch (this->swing_mode)
      {

      case climate::CLIMATE_SWING_VERTICAL:
        frame[SWING_BYTE_1] = 0x52;
        frame[SWING_BYTE_2] = 0x00;
        break;

      case climate::CLIMATE_SWING_BOTH:
        frame[SWING_BYTE_1] = 0x5F;
        frame[SWING_BYTE_2] = 0x00;
        break;

      case climate::CLIMATE_SWING_OFF:
      default:
        frame[SWING_BYTE_1] = 0x20;
        frame[SWING_BYTE_2] = 0x00;
        break;
      }

      // -------------------------
      // CHECKSUM
      // -------------------------
      uint8_t sum = 0;
      for (int i = 0; i < CHECKSUM_BYTE; i++)
      {
        sum += frame[i];
      }

      frame[CHECKSUM_BYTE] = sum;
    }

    void Daikin200Climate::send_frame()
    {
      auto call = this->transmitter_->transmit();

      auto *data = call.get_data();

      // Header
      data->item(3500, 1700);

      for (int i = 0; i < 25; i++)
      {
        uint8_t b = frame[i];

        for (int bit = 7; bit >= 0; bit--)
        {
          if (b & (1 << bit))
          {
            data->item(430, 1300);
          }
          else
          {
            data->item(430, 420);
          }
        }
      }

      data->item(430, 8000);

      call.perform();
    }

    void Daikin200Climate::process_received_frame(const uint8_t *data, size_t len)
    {

      if (!this->validate_frame_(frame, len))
      {
        ESP_LOGW("daikin200", "Invalid frame received");
        return;
      }

      this->decode_mode_(frame);
      this->decode_temperature_(frame);
      this->decode_fan_(frame);
      this->decode_swing_(frame);

      this->publish_state();
    }

    bool Daikin200Climate::validate_frame_(
        const uint8_t *frame,
        size_t len)
    {

      if (len != 25)
      {
        ESP_LOGW("daikin200", "Unexpected frame length %u", (unsigned)len);
        return false;
      }

      return this->validate_checksum_(frame);
    }

    uint8_t Daikin200Climate::calculate_checksum_(
        const uint8_t *frame)
    {

      uint8_t sum = 0;

      for (int i = 0; i < 24; i++)
      {
        sum += frame[i];
      }

      return sum;
    }

    bool Daikin200Climate::validate_checksum_(
        const uint8_t *frame)
    {

      return frame[24] == this->calculate_checksum_(frame);
    }

    void Daikin200Climate::decode_mode_(
        const uint8_t *frame)
    {

      switch (frame[12])
      {

      case 0x73:
        this->mode = climate::CLIMATE_MODE_COOL;
        break;

      case 0x51:
        this->mode = climate::CLIMATE_MODE_HEAT;
        break;

      case 0x43:
        this->mode = climate::CLIMATE_MODE_AUTO;
        break;

      case 0x47:
        this->mode = climate::CLIMATE_MODE_DRY;
        break;

      case 0x40:
        this->mode = climate::CLIMATE_MODE_FAN_ONLY;
        break;

      default:
        ESP_LOGW("daikin200", "Unknown mode byte 0x%02X", frame[12]);
        break;
      }
    }

    void Daikin200Climate::decode_temperature_(
        const uint8_t *frame)
    {

      this->target_temperature =
          22.0f + ((int)frame[16] - 0x1A) / 2.0f;
    }

    void Daikin200Climate::decode_fan_(
        const uint8_t *frame)
    {

      switch (frame[17])
      {

      case 0x0F:
        this->fan_mode = climate::CLIMATE_FAN_AUTO;
        break;

      case 0x1F:
        this->fan_mode = climate::CLIMATE_FAN_LOW;
        break;

      case 0x3F:
        this->fan_mode = climate::CLIMATE_FAN_MEDIUM;
        break;

      case 0x5F:
        this->fan_mode = climate::CLIMATE_FAN_HIGH;
        break;

      default:
        ESP_LOGW("daikin200", "Unknown fan byte 0x%02X", frame[17]);
        break;
      }
    }

    void Daikin200Climate::decode_swing_(
        const uint8_t *frame)
    {

      switch (frame[18])
      {

      case 0x20:
        this->swing_mode = climate::CLIMATE_SWING_OFF;
        break;

      case 0x52:
        this->swing_mode = climate::CLIMATE_SWING_VERTICAL;
        break;

      case 0x5F:
        this->swing_mode = climate::CLIMATE_SWING_BOTH;
        break;

      default:
        ESP_LOGW("daikin200", "Unknown swing byte 0x%02X", frame[18]);
        break;
      }
    }
  } // namespace daikin200
} // namespace esphome