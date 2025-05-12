/**
 * @file crc8.h
 * @author Hayden Mai
 * @date May-10-2025
 * @brief A class to verify data integrity via CRC8.
 * @link https://github.com/tbs-fpv/tbs-crsf-spec/blob/main/crsf.md
 */

#ifndef CRC8_H_
#define CRC8_H_

#include <array>
#include <cstdint>
#include <span>


class CRC8 {
  public:
    CRC8(uint8_t polynomial);
    ~CRC8() = default;

    uint8_t calculate(std::span<const uint8_t> data) const noexcept;

  protected:
    std::array<uint8_t, 256> table_ {};
    void init(uint8_t polynomial) noexcept;
};

#endif