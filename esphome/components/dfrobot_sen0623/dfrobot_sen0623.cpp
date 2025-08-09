#include "dfrobot_sen0623.h"
#include "esphome/core/log.h"

namespace esphome
{
    namespace dfrobot_sen0623
    {

        static const char *TAG = "dfrobot_sen0623.component";

        void DfrobotSen0623Component::forge_packet(uint8_t control, uint8_t command, uint8_t *senData, uint16_t senLen)
        {
            std::vector<uint8_t> buffer;
            buffer.push_back(0x53);
            buffer.push_back(0x59);
            buffer.push_back(control);
            buffer.push_back(command);
            buffer.push_back((senLen >> 8) & 0xff);
            buffer.push_back(senLen & 0xff);
            for (uint8_t i = 0; i < senLen; i++)
            {
                buffer.push_back(senData[i]);
            }
            // Calculate check
            uint8_t crSum = 0;
            for (uint8_t i = 0; i < buffer.size(); i++)
            {
                crSum += buffer.data()[i];
            }
            buffer.push_back(crSum & 0xff);
            buffer.push_back(0x54);
            buffer.push_back(0x43);

            // this->print_data("XX", buffer.data(), buffer.size());
            this->send_packet(buffer.data(), buffer.size());
        }

        void DfrobotSen0623Component::send_packet(uint8_t *packetData, size_t len)
        {
            this->print_data(">>", packetData, len);
            for (uint8_t i = 0; i < len; i++)
            {
                while (!this->available())
                {
                }
                this->write_byte(packetData[i]);
            }
        }

        uint8_t DfrobotSen0623Component::read_packet(uint8_t *packetData)
        {
            std::vector<uint8_t> buffer;
            uint8_t byte;

            // Read bytes until '\n' delimiter or no more bytes available
            while (this->available() && this->read_byte(&byte))
            {
                buffer.push_back(byte);
                if (byte == '\n')
                {
                    break;
                }
            }

            // Copy data to packetData and return the length
            size_t len = buffer.size();
            if (len > 0)
            {
                // Make sure to not overflow packetData buffer — adjust max length accordingly
                // For example, if packetData is fixed size 100 bytes:
                size_t max_len = 100; // Change as needed
                if (len > max_len)
                {
                    len = max_len;
                }
                memcpy(packetData, buffer.data(), len);
            }

            if (len > 0)
            {
                this->print_data("<<", packetData, len);
            }

            return (uint8_t)len;
        }

        void DfrobotSen0623Component::print_data(std::string tag, const uint8_t *bytes, size_t len)
        {
            std::string out;
            char buf[5];
            for (size_t i = 0; i < len; i++)
            {
                if (i > 0)
                {
                    out += " ";
                }
                sprintf(buf, "%02X", bytes[i]);
                out += buf;
            }
            ESP_LOGI(TAG, "%s %s", tag.c_str(), out.c_str());
        }

        void DfrobotSen0623Component::setup()
        {
            ESP_LOGI(TAG, "WAITING FOR INIT");
            delay(1000);

            uint8_t data[1];
            data[0] = 0x0f;
            this->forge_packet(0x01, 0x83, data, sizeof(data));

            uint8_t packetData[100];
            uint8_t len = this->read_packet(packetData);
            if (len < 5)
            {
                this->mark_failed();
                return;
            }
        }

        // getData(uint8_t con, uint8_t cmd, uint16_t len, uint8_t *senData, uint8_t *retData)

        void DfrobotSen0623Component::loop()
        {
            // Request Hearth Rate
            uint8_t data[1];
            data[0] = 0x0f;
            this->forge_packet(0x85, 0x82, data, sizeof(data));

            uint8_t packetData[100]; // adjust size as needed
            uint8_t len = this->read_packet(packetData);

            if (len > 5)
            {
                //this->print_data("**", packetData, len);
                // Check packet validity
                uint8_t dataLen = ((uint16_t)packetData[4] << 8) | packetData[5];
                uint8_t csum = 0;
                for (uint8_t i = 0; i < 6 + dataLen; i++)
                {
                    csum += packetData[i];
                }
                csum = csum & 0xff;
                if (packetData[0] == 0x53 && packetData[1] == 0x59 && packetData[len - 2] == 0x54 && packetData[len - 1] == 0x43 && csum == packetData[len - 3])
                {
                    ESP_LOGI(TAG, "-----");
                    ESP_LOGI(TAG, "%02X %02X (%i)", packetData[2], packetData[3], dataLen);
                    uint8_t data[dataLen];
                    for (uint8_t i = 0; i < dataLen; i++)
                    {
                        data[i] = packetData[6 + i];
                    }
                    this->print_data("**", data, dataLen);
                    // ESP_LOGI(TAG, "CHECK_I: %02X", packetData[len-3]);
                    // ESP_LOGI(TAG, "CHECK_C: %02X", csum);
                    if (packetData[2] == 0x85 && packetData[3] == 0x82 && data[0] > 0)
                    {
                        ESP_LOGI(TAG, "HRRRRR: %i", data[0]);
                        delay(500);
                    }
                    ESP_LOGI(TAG, "-----");
                }
            }
        }

        void DfrobotSen0623Component::dump_config()
        {
            ESP_LOGCONFIG(TAG, "DfrobotSen0623Component");
        }

    } // namespace dfrobot_sen0623
} // namespace esphome
