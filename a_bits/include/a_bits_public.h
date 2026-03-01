/*
#define STA_BITS_IMPLEMENTATION
#include "sta-main/sta_bits.h"
*/

a_bool a_u8_get_bit(uint8_t b,int bit_num);
void a_u8_set_bit(uint8_t * b,int bit_num,a_bool v);

int a_u8_get_2_bits(uint8_t b,int bit_num);

a_bool a_get_bit(const uint8_t * bits,int loc);
void a_set_bit(uint8_t * bits,int loc,int v);

void a_set_2_bits(uint8_t * bits,int loc,a_bool v,a_bool v2);

//++  left and right(r) side
a_bool a_inc_bits(uint8_t * bits,int max_bits);
a_bool a_inc_bits_r(uint8_t * bits,int max_bits);


// A Project By Arshad Latti