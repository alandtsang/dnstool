#ifndef INCLUDE_UTILS_H_
#define INCLUDE_UTILS_H_

#define RTE_CPU_TO_BE_16(cpu_16_v) \
    (uint16_t) ((((cpu_16_v) & 0xFF) << 8) | ((cpu_16_v) >> 8))

#endif
