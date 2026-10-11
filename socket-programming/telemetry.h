#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <stdint.h>

#pragma pack(push, 1)

typedef struct {
    uint16_t paket_id;
    float rakim;
    float hiz;
    int16_t pitch;
    int16_t roll;
    uint8_t batarya;
    uint8_t sistem_durumu;
} TelemetriPaketi;

#pragma pack(pop)

#endif
