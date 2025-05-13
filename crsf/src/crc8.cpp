/**
 * @file crc8.cpp
 * @author Hayden Mai
 * @date May-10-2025
 * @brief A class to verify data integrity via CRC8.
 * @link https://github.com/tbs-fpv/tbs-crsf-spec/blob/main/crsf.md
 */

#include "crsf/crc8.h"


CRC8::CRC8(uint8_t polynomial) { init(polynomial); }


uint8_t CRC8::calculate(std::span<const uint8_t> data) const noexcept
{
    uint8_t crc_result {};
    for (uint8_t byte : data) {
        crc_result = table_[crc_result ^ byte];
    }

    return crc_result;
}


void CRC8::init(uint8_t polynomial) noexcept
{
    constexpr int BITS_PER_BYTE {8};
    for (size_t i {}; i < table_.size(); i++) {
        uint8_t crc = static_cast<uint8_t>(i);

        for (int bit {}; bit < BITS_PER_BYTE; bit++) {
            // 1) Shift the currect CRC value left by one bit
            // 2) XOR with the polynomial or zero, dependent on whether the highest bit
            // 	was set to high
            crc = (crc << 1) ^ ((crc & 0x80) ? polynomial : 0);
        }

        table_[i] = crc & 0xFF;
    }
}