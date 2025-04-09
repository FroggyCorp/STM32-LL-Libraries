#include "STM32_I2C.h"


STM32_I2C::STM32_I2C() {;}

void STM32_I2C::init(I2C_TypeDef *I2Cx) {


    LL_GPIO_InitTypeDef GPIO_InitStruct;
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

    /* Configure SCL Pin as : Alternate function, High Speed, Open drain, Pull up */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_9 | LL_GPIO_PIN_10;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Enable the peripheral clock for I2C1 */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);
    
    /* Set I2C1 clock source as SYSCLK */
    LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_SYSCLK);

    LL_I2C_Disable(I2Cx);
    LL_I2C_InitTypeDef I2C_InitStruct = {0};
    I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;

    I2C_InitStruct.Timing = __LL_I2C_CONVERT_TIMINGS(0x0, 0x6, 0xFF, 0xFF, 0xFF);
    I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
    I2C_InitStruct.DigitalFilter = 0;
    I2C_InitStruct.OwnAddress1 = 0;
    I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
    I2C_InitStruct.OwnAddrSize = 0;
    LL_I2C_Init(I2Cx, &I2C_InitStruct);

    LL_I2C_Enable(I2Cx);
}

void STM32_I2C::end(I2C_TypeDef *I2Cx) {
    LL_I2C_Disable(I2Cx);
    LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_I2C1);

    LL_GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = LL_GPIO_PIN_9 | LL_GPIO_PIN_10;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    

    LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_9 | LL_GPIO_PIN_10);
}

uint8_t STM32_I2C::readReg(I2C_TypeDef *I2Cx, uint8_t addr, uint8_t reg, uint8_t *data, uint8_t len) {
    init(I2Cx);

    uint32_t timeout;

    LL_I2C_DisableAutoEndMode(I2Cx);
    LL_I2C_SetTransferSize(I2Cx, 1);
    LL_I2C_SetTransferRequest(I2Cx, LL_I2C_REQUEST_WRITE);
    LL_I2C_GenerateStartCondition(I2Cx);
    timeout = 100000;
    while (!LL_I2C_IsActiveFlag_TXE(I2Cx)) {
		timeout--;
		if (timeout == 0)
            return 1;
	}

    //on écrit le registre
    LL_I2C_TransmitData8(I2Cx, reg);
    timeout = 100000;
    while (!LL_I2C_IsActiveFlag_TXE(I2Cx)) {
		timeout--;
		if (timeout == 0)
            return 2;
	}

    LL_I2C_SetSlaveAddr(I2Cx, addr << 1);
    LL_I2C_DisableAutoEndMode(I2Cx);
    LL_I2C_SetTransferSize(I2Cx, len);
    LL_I2C_SetTransferRequest(I2Cx, LL_I2C_REQUEST_READ);
    LL_I2C_GenerateStartCondition(I2Cx);

    timeout = 100000;
    while (!LL_I2C_IsActiveFlag_TXE(I2Cx)) {
		timeout--;
		if (timeout == 0)
            return 1;
	}

    while (len != 0) {
        timeout = 100000;
        while (!LL_I2C_IsActiveFlag_RXNE(I2Cx)) {
            timeout--;
            if (timeout == 0)
                return 3;
        }

        *data = LL_I2C_ReceiveData8(I2Cx);
        data ++;
        len --;

    }
    
    timeout = 100000;
    while (!LL_I2C_IsActiveFlag_TC(I2Cx)) {
        timeout--;
        if (timeout == 0)
            return 4;
    }

    LL_I2C_GenerateStopCondition(I2Cx);

    end(I2Cx);
    return 0;
}

uint8_t STM32_I2C::read(I2C_TypeDef *I2Cx, uint8_t addr, uint8_t *data, uint8_t len) {
    init(I2Cx);

    uint32_t timeout;


    LL_I2C_SetSlaveAddr(I2Cx, addr << 1);
    LL_I2C_DisableAutoEndMode(I2Cx);
    LL_I2C_SetTransferSize(I2Cx, len);
    LL_I2C_SetTransferRequest(I2Cx, LL_I2C_REQUEST_READ);
    LL_I2C_GenerateStartCondition(I2Cx);

    timeout = 100000;
    while (!LL_I2C_IsActiveFlag_TXE(I2Cx)) {
		timeout--;
		if (timeout == 0)
            return 1;
	}

    while (len != 0) {
        timeout = 100000;
        while (!LL_I2C_IsActiveFlag_RXNE(I2Cx)) {
            timeout--;
            if (timeout == 0)
                return 3;
        }

        *data = LL_I2C_ReceiveData8(I2Cx);
        data ++;
        len --;

    }
    
    timeout = 100000;
    while (!LL_I2C_IsActiveFlag_TC(I2Cx)) {
        timeout--;
        if (timeout == 0)
            return 4;
    }

    LL_I2C_GenerateStopCondition(I2Cx);

    end(I2Cx);
    return 0;
}


uint8_t STM32_I2C::write(I2C_TypeDef *I2Cx, uint8_t addr, uint8_t *data, uint8_t len) {
    
    init(I2Cx);

    uint32_t timeout = 0;
    LL_I2C_SetSlaveAddr(I2Cx, addr << 1);
    LL_I2C_EnableAutoEndMode(I2Cx);
    LL_I2C_SetTransferSize(I2Cx, len);
    LL_I2C_SetTransferRequest(I2Cx, LL_I2C_REQUEST_WRITE);
    LL_I2C_ClearFlag_STOP(I2Cx);    //util ?
    LL_I2C_GenerateStartCondition(I2Cx);
    timeout = 100000;

    while (!LL_I2C_IsActiveFlag_TXE(I2Cx)) {
		timeout--;
		if (timeout == 0)
            return 1;
	}


    while (len != 0) {
        LL_I2C_TransmitData8(I2Cx, *data);
        timeout = 100000;
        while (!LL_I2C_IsActiveFlag_TXE(I2Cx)) {
            timeout--;
            if (timeout == 0)
                return 2;
        }
        data ++;
        len--;
    }

    timeout = 100000;
    while (!LL_I2C_IsActiveFlag_STOP(I2Cx)) {
        timeout--;
        if (timeout == 0)
            return 3;
    }
  
    LL_I2C_GenerateStopCondition(I2Cx);
    end(I2Cx);
    return 0;
}