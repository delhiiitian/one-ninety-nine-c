/*
 * Chapter 8.6 - Fixed-width integers: <stdint.h> and <inttypes.h>
 * When the exact size matters (files, networks, hardware), say it in the type.
 */
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

/* pack a little sensor reading into exactly 8 bytes, byte by byte (portable) */
static void pack_reading(uint8_t out[8], uint16_t sensor_id, int16_t temp_tenths, uint32_t timestamp)
{
    out[0] = (uint8_t)(sensor_id >> 8);          /* big-endian ("network order") */
    out[1] = (uint8_t)(sensor_id);
    out[2] = (uint8_t)((uint16_t)temp_tenths >> 8);
    out[3] = (uint8_t)(temp_tenths);
    out[4] = (uint8_t)(timestamp >> 24);
    out[5] = (uint8_t)(timestamp >> 16);
    out[6] = (uint8_t)(timestamp >> 8);
    out[7] = (uint8_t)(timestamp);
}

static void unpack_reading(const uint8_t in[8], uint16_t *id, int16_t *temp, uint32_t *ts)
{
    *id = (uint16_t)((in[0] << 8) | in[1]);
    *temp = (int16_t)((in[2] << 8) | in[3]);
    *ts = ((uint32_t)in[4] << 24) | ((uint32_t)in[5] << 16) | ((uint32_t)in[6] << 8) | in[7];
}

int main(void)
{
    printf("int8_t  %zu byte : %" PRId8 " to %" PRId8 "\n", sizeof(int8_t), INT8_MIN, INT8_MAX);
    printf("uint16_t %zu bytes: 0 to %" PRIu16 "\n", sizeof(uint16_t), UINT16_MAX);
    printf("int32_t %zu bytes: %" PRId32 " to %" PRId32 "\n", sizeof(int32_t), INT32_MIN, INT32_MAX);
    printf("uint64_t %zu bytes: 0 to %" PRIu64 "\n", sizeof(uint64_t), UINT64_MAX);
    printf("size_t is %zu bytes, uintptr_t is %zu bytes on this machine\n", sizeof(size_t), sizeof(uintptr_t));

    /* unsigned arithmetic wraps around, which is well defined */
    uint8_t odometer = 250;
    for (int i = 0; i < 10; i++) odometer++;
    printf("uint8_t 250 + 10 wraps to %" PRIu8 "\n", odometer);

    uint8_t packet[8];
    pack_reading(packet, 513, -42, 1761300000u);
    printf("packet bytes:");
    for (int i = 0; i < 8; i++) printf(" %02X", packet[i]);
    printf("\n");

    uint16_t id; int16_t temp; uint32_t ts;
    unpack_reading(packet, &id, &temp, &ts);
    printf("unpacked: sensor %" PRIu16 ", %.1f C, time %" PRIu32 "\n", id, temp / 10.0, ts);
    return 0;
}
