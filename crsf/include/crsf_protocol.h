/**
 * @file crsf_protocol.h
 * @brief Constants associated with frame types
 * @author Hayden Mai
 * @date May-07-2025
 *
 * @link https://github.com/tbs-fpv/tbs-crsf-spec/blob/main/crsf.md
 * @link https://github.com/CapnBry/CRServoF/blob/main/lib/CrsfSerial/crsf_protocol.h
 */

#pragma once

#include <stdint.h>

#define CRSF_BAUDRATE           420000 // for receiver only
#define CRSF_NUM_CHANNELS       16
#define CRSF_CHANNEL_VALUE_MIN  172 // 987us - actual CRSF min is 0 with E.Limits on
#define CRSF_CHANNEL_VALUE_1000 191
#define CRSF_CHANNEL_VALUE_MID  992
#define CRSF_CHANNEL_VALUE_2000 1792
#define CRSF_CHANNEL_VALUE_MAX  1811 // 2012us - actual CRSF max is 1984 with E.Limits on
#define CRSF_CHANNEL_VALUE_SPAN (CRSF_CHANNEL_VALUE_MAX - CRSF_CHANNEL_VALUE_MIN)
#define CRSF_ELIMIT_US_MIN      891  // microseconds for CRSF=0 (E.Limits=ON)
#define CRSF_ELIMIT_US_MAX      2119 // microseconds for CRSF=1984
#define CRSF_MAX_PACKET_SIZE    64 // max declared len is 62+DEST+LEN on top of that = 64
#define CRSF_MAX_PAYLOAD_LEN    (CRSF_MAX_PACKET_SIZE - 4) // [dest|len|type|payload|crc8]
#define CRSF_BITS_PER_CHANNEL   11

#define CRSF_SYNC_BYTE 0XC8

enum class eCRSF_frame_len : uint8_t {
    ADDRESS      = 1,
    FRAMELENGTH  = 1,
    TYPE         = 1, 
    CRC          = 1, 
    TYPE_CRC     = 2, // length of TYPE and CRC fields combined
    EXT_TYPE_CRC = 4, // length of Extended Dest/Origin, TYPE & CRC fields combined
    NON_PAYLOAD  = 4, // combined length of all fields except payload
};

enum class eCRSF_frame_payload_size : size_t {
    GPS             = 15,
    BATTERY_SENSOR  = 8,
    LINK_STATISTICS = 10,
    RC_CHANNELS     = 22, // 11 bits per channel * 16 channels = 22 bytes.
    ATTITUDE        = 6,
};

enum class eCRSF_devAddr : uint8_t {
    BROADCAST         = 0x00,
    CLOUD             = 0x0E,
    USB               = 0x10,
    BLE_WIFI          = 0x12,
    WIFI_RX           = 0x13,
    VIDEO_RX          = 0x14,
    TBS_CORE_PNP_PRO  = 0x80,
    ESC_1             = 0x90,
    ESC_2             = 0x91,
    ESC_3             = 0x92,
    ESC_4             = 0x93,
    ESC_5             = 0x94,
    ESC_6             = 0x95,
    ESC_7             = 0x96,
    ESC_8             = 0x97,
    RESERVED_1        = 0x8A,
    CRSF_RESERVED_1   = 0xB0,
    CRSF_RESERVED_2   = 0xB2,
    CURRENT_SENSOR    = 0xC0,
    GPS               = 0xC2,
    TBS_BLACKBOX      = 0xC4,
    FLIGHT_CONTROLLER = 0xC8,
    RESERVED_2        = 0xCA,
    RACE_TAG          = 0xCC,
    VTX               = 0xCE,
    REMOTE_CTRL       = 0xEA,
    CRSF_RX           = 0xEC,
    CRSF_TX           = 0xEE,
    RESERVED_3        = 0xF0,
    RESERVED_4        = 0xF2,
};

enum class eCRSF_frameType : uint8_t {
    GPS                     = 0x02,
    GPS_TIME                = 0x03,
    GPS_EXTENDED            = 0x06,
    VARIOMETER_SENSOR       = 0x07,
    BATTERY_SENSOR          = 0x08,
    ALTITUDE_VERTICAL_SPEED = 0x09,
    AIRSPEED                = 0x0A,
    HEARTBEAT               = 0x0B,
    RPM                     = 0x0C,
    TEMP                    = 0x0D,
    CELLS_SENSOR            = 0x0E,
    VTX_TELEMETRY           = 0x10,
    LINK_STATISTICS         = 0x14,
    RC_CHANNELS_PAYLOAD     = 0x16,
    SUBSET_RC_CHANNELS      = 0x17, // Discouraged
    RC_CHANNELS_11_BITS     = 0x18, // Unused
    LINK_STATISTIICS_RX     = 0x1C,
    LINK_STATISTIICS_TX     = 0x1D,
    ATTITUDE                = 0x1E, // Angle values within [-180, 180] degrees
    MAVLINK_FC              = 0x1F,
    FLIGHT_MODE             = 0x21,
    ESP_NOW_MSGS            = 0x22,
};

enum class eCRSF_frameType_ext : uint8_t {
    DEVICE_PING               = 0x28,
    DEVICE_INFO               = 0x29,
    PARAM_SETTINGS_ENTRY      = 0x2B,
    PARAM_SETTINGS_READ       = 0x2C,
    PARAM_SETTINGS_WRITE      = 0x2D,
    COMMAND                   = 0x32,
    LOGGING                   = 0x34,
    REMOTE_RELATED_FRAME      = 0x3A,
    GAME                      = 0x3C,
    MSP_REQUEST               = 0x7A,
    MSP_RESPONSE              = 0x7B,
    ARDUPILOT_PASSTHROUGH     = 0x80,
    MAVLINK_EVENLOPE          = 0xAA,
    MAVLINK_SYS_SENSOR_STATUS = 0xAC,
};

#pragma pack(push, 1)
struct crsf_header_s {
    uint8_t sync_byte;  // CRSF_SYNC_BYTE
    uint8_t frame_size; // counts size after this byte, so it must be the payload size + 2
                        // (type and crc)
    uint8_t type;       // from crsf_frame_type_e
    uint8_t data[0];
};

struct crsf_channels_s {
    unsigned ch0 : 11;
    unsigned ch1 : 11;
    unsigned ch2 : 11;
    unsigned ch3 : 11;
    unsigned ch4 : 11;
    unsigned ch5 : 11;
    unsigned ch6 : 11;
    unsigned ch7 : 11;
    unsigned ch8 : 11;
    unsigned ch9 : 11;
    unsigned ch10 : 11;
    unsigned ch11 : 11;
    unsigned ch12 : 11;
    unsigned ch13 : 11;
    unsigned ch14 : 11;
    unsigned ch15 : 11;
};

struct crsf_payloadLinkStatistics_s {
    uint8_t uplink_RSSI_1;
    uint8_t uplink_RSSI_2;
    uint8_t uplink_Link_quality;
    int8_t uplink_SNR;
    uint8_t active_antenna;
    uint8_t rf_Mode;
    uint8_t uplink_TX_Power;
    uint8_t downlink_RSSI;
    uint8_t downlink_Link_quality;
    int8_t downlink_SNR;
};

struct crsf_sensor_battery_s {
    uint16_t voltage;       // V * 10 big endian
    uint16_t current;       // A * 10 big endian
    uint32_t capacity : 24; // mah big endian
    uint8_t remaining;      // %
};

struct crsf_sensor_gps_s {
    int32_t latitude;     // degree / 10,000,000 big endian
    int32_t longitude;    // degree / 10,000,000 big endian
    uint16_t groundspeed; // km/h / 10 big endian
    uint16_t heading;     // GPS heading, degree/100 big endian
    uint16_t altitude;    // meters, +1000m big endian
    uint8_t satellites;   // satellites
};
#pragma pack(pop)

// crsf = (us - 1500) * 8/5 + 992
#define US_to_CRSF(us) ((us) * 8 / 5 + (CRSF_CHANNEL_VALUE_MID - 2400))
// us = (crsf - 992) * 5/8 + 1500
#define CRSF_to_US(crsf) ((crsf) * 5 / 8 + (1500 - 620))

#if !defined(__linux__)
static inline uint16_t htobe16(uint16_t val)
{
#if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    return val;
#else
    return __builtin_bswap16(val);
#endif
}

inline static uint16_t be16toh(uint16_t val)
{
#if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    return val;
#else
    return __builtin_bswap16(val);
#endif
}

inline static uint32_t htobe32(uint32_t val)
{
#if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    return val;
#else
    return __builtin_bswap32(val);
#endif
}

inline static uint32_t be32toh(uint32_t val)
{
#if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    return val;
#else
    return __builtin_bswap32(val);
#endif
}
#endif