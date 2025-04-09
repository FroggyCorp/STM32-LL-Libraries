#ifndef STM32_I2C_H
#define STM32_I2C_H

#include <stdint.h>
#include <stm32l0xx_ll_i2c.h>
#include <stm32l0xx_ll_rcc.h>
#include <stm32l0xx_ll_gpio.h>
#include <stm32l0xx_ll_bus.h>
#include <stm32l0xx_ll_cortex.h>



class STM32_I2C {
    public:
    STM32_I2C();
    void init(I2C_TypeDef *I2Cx);
    void end(I2C_TypeDef *I2Cx);
    uint8_t write(I2C_TypeDef *I2Cx, uint8_t addr, uint8_t *data, uint8_t len);
    uint8_t readReg(I2C_TypeDef *I2Cx, uint8_t addr, uint8_t reg, uint8_t *data, uint8_t len);
    uint8_t read(I2C_TypeDef *I2Cx, uint8_t addr, uint8_t *data, uint8_t len);
};

#endif