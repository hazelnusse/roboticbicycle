/*! \file utils.h
 *  \brief Utility functions specific to the Cortex-M3
 */
#ifndef UTILS_H
#define UTILS_H

/*! 
 * */
inline
uint32_t bitband(const uint32_t addr, const uint32_t base, const uint32_t bitnum)
{
  return (addr & 0xF0000000) + base + ((addr & 0xFFFFF) << 5) + (bitnum << 2);
}

inline
void periph_set_bit(const uint32_t addr, const uint32_t bitnum)
{
  *((volatile uint32_t *) bitband(addr, 0x22000000, bitnum)) = 1;
}

inline
void periph_reset_bit(const uint32_t addr, const uint32_t bitnum)
{
  *((volatile uint32_t *) bitband(addr, 0x22000000, bitnum)) = 0;
}

inline
uint32_t periph_get_bit(const uint32_t addr, const uint32_t bitnum)
{
  return *((volatile uint32_t *) bitband(addr, 0x22000000, bitnum));
}


#endif
