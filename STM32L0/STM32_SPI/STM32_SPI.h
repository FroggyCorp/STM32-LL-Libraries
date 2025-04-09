#ifndef STM32_SPI_H
#define STM32_SPI_H

#include <stdint.h>
#include <stm32l0xx_ll_spi.h>
#include <stm32l0xx_ll_gpio.h>
#include <stm32l0xx_ll_bus.h>

class STM32_SPI {
    public:
    uint8_t SPI_Pin;
    SPI_TypeDef *SPIx;
    uint32_t NSS_Pin;
    GPIO_TypeDef *NSS_Port;
    STM32_SPI(SPI_TypeDef *_SPIx, uint8_t _SPI_Pin);
    void init();
    void init(SPI_TypeDef *_SPIx, uint8_t SPI_Pin);
    void begin();
    void begin(SPI_TypeDef *_SPIx, uint8_t SPI_Pin);
    void end();
    uint8_t transfert8b(uint8_t data);
    void writeReg(uint8_t Reg, uint8_t data);
    uint8_t readReg(uint8_t Reg);
    void NSS_start();
    void NSS_end();
};

#endif