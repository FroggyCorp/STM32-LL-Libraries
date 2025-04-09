#include "STM32_SPI.h"

STM32_SPI::STM32_SPI(SPI_TypeDef *_SPIx, uint8_t _SPI_Pin) {
    SPI_Pin = _SPI_Pin;
    SPIx = _SPIx;
}

void STM32_SPI::init() {
    init(SPIx, SPI_Pin);
}

void STM32_SPI::init(SPI_TypeDef *_SPIx, uint8_t _SPI_Pin) {
    SPIx = _SPIx;
    SPI_Pin = _SPI_Pin;
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (SPI_Pin == 0) {//PA5=SCK PA6=MISO PA7=MOSI PA4=NSS
        // Enable clocks
        LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

        // Configure Pins
        GPIO_InitStruct.Pin = LL_GPIO_PIN_5 | LL_GPIO_PIN_6 | LL_GPIO_PIN_7;
        GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
        GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
        GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
        GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
        LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        
        // Pin PA4 (NSS) 
        GPIO_InitStruct.Pin = LL_GPIO_PIN_4;
        GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
        GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
        GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
        GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
        LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        NSS_Pin = LL_GPIO_PIN_4;
        NSS_Port = GPIOA;
    } 
    else if (SPI_Pin == 1) {//PB3=SCK PB4=MISO PB5=MOSI PA15=NSS
        // Enable clocks
        LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA | LL_IOP_GRP1_PERIPH_GPIOB);

        // Configure Pins

        GPIO_InitStruct.Pin = LL_GPIO_PIN_3 | LL_GPIO_PIN_4 | LL_GPIO_PIN_5;
        GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
        GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
        GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
        GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
        LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        // Pin PA15 (NSS) 
        GPIO_InitStruct.Pin = LL_GPIO_PIN_15;
        GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
        GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
        GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
        GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
        LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        NSS_Pin = LL_GPIO_PIN_15;
        NSS_Port = GPIOA;

    }
    
    // Configure device
    if (SPIx == SPI1)
        LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);
    #if defined(SPI2)
    if (SPIx == SPI2)
        LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI2);
    #endif

    LL_SPI_InitTypeDef SPI_InitStruct = {0};
    SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
    SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
    SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
    SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW; 
    SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE; 
    SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
//    SPI_InitStruct.NSS = LL_SPI_NSS_HARD_OUTPUT;
    SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV2;
    SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
    SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
    LL_SPI_Init(SPIx, &SPI_InitStruct);
    LL_SPI_SetStandard(SPIx, LL_SPI_PROTOCOL_MOTOROLA);    
}; 

void STM32_SPI::writeReg(uint8_t Reg, uint8_t data) {
    NSS_start();
    transfert8b(Reg);
    transfert8b(data);
    NSS_end();
}

uint8_t STM32_SPI::readReg(uint8_t Reg) {
    uint8_t data = 0;
    NSS_start();
    transfert8b(Reg);
    data = transfert8b(0);
    NSS_end();
    return data;
}

uint8_t STM32_SPI::transfert8b(uint8_t data) {
    
    //on envoit
    LL_SPI_TransmitData8(SPIx, data);
    while(!LL_SPI_IsActiveFlag_TXE(SPIx));
    while(!LL_SPI_IsActiveFlag_RXNE(SPIx));
    return LL_SPI_ReceiveData8(SPIx);
};

void STM32_SPI::NSS_start() {
    LL_GPIO_ResetOutputPin(NSS_Port, NSS_Pin);
}
void STM32_SPI::NSS_end() {
    LL_GPIO_SetOutputPin(NSS_Port, NSS_Pin);    
}

void STM32_SPI::begin() {
    begin(SPIx, SPI_Pin);
}

void STM32_SPI::begin(SPI_TypeDef *_SPIx, uint8_t _SPI_Pin) {
    SPIx = _SPIx;
    init(SPIx, _SPI_Pin);
    LL_SPI_Enable(SPIx);
    NSS_end();
};

void STM32_SPI::end() {
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    //on desinit les sorties
    if (SPI_Pin == 0) {//PA5=SCK PA6=MISO PA7=MOSI PA4=NSS
        // Configure Pins
        GPIO_InitStruct.Pin = LL_GPIO_PIN_5 | LL_GPIO_PIN_6 | LL_GPIO_PIN_7 | LL_GPIO_PIN_4;
        GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
        GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
        GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
        GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
        LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        LL_GPIO_ResetOutputPin(GPIOA, GPIO_InitStruct.Pin);
    } 
    else if (SPI_Pin == 1) {//PB3=SCK PB4=MISO PB5=MOSI PA15=NSS
        // Configure Pins
        GPIO_InitStruct.Pin = LL_GPIO_PIN_3 | LL_GPIO_PIN_4 | LL_GPIO_PIN_5;
        GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
        GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
        GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
        GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
        LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        LL_GPIO_ResetOutputPin(GPIOB, GPIO_InitStruct.Pin);

        // Pin PA15 (NSS) 
        GPIO_InitStruct.Pin = LL_GPIO_PIN_15;
        GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
        GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
        GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
        GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
        LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        LL_GPIO_ResetOutputPin(GPIOA, GPIO_InitStruct.Pin);
    }

    LL_SPI_Disable(SPIx);
    
    if (SPIx == SPI1)
        LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_SPI1);
    #if defined(SPI2)
    if (SPIx == SPI2)
        LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_SPI2);
    #endif

};



