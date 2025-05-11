/**
 * @file crc8.cpp
 * @author Hayden Mai
 * @date May-10-2025
 * @brief A class to verify data integrity via CRC8.
 * @link https://github.com/tbs-fpv/tbs-crsf-spec/blob/main/crsf.md
 */

#include "crsf/crc8.h"

CRC8::CRC8(uint8_t polynomial) { init(polynomial); }

uint8_t CRC8::calculate(const uint8_t *data, size_t len) const noexcept
{
    uint8_t crc_result {};
    for (int i {}; i < len; i++) {
        crc_result = table_[crc_result ^ data[i]];
    }

    return crc_result;
}