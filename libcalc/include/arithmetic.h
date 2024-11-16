#include "decimal.h"

// functions should create copies/ not change dst until after operation is finished in case dst points to source
// functions assume bcd buffers are fully used e.g. trailing byte does not only have one digit valid
int bcd_shl(const uint8_t* src, uint8_t* dst, uint32_t length, uint32_t shift);
int bcd_shr(const uint8_t* src, uint8_t* dst, uint32_t length, uint32_t shift);

int bcd_add(uint8_t* a, uint8_t* b, uint8_t* dst, uint32_t length);
int bcd_sub(uint8_t* a, uint8_t* b, uint8_t* dst, uint32_t length);
