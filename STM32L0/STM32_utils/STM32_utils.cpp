#include "STM32_utils.h"

void SerialInit(USART_TypeDef *USARTx, uint32_t baudrate) {
    setvbuf(stdout, NULL, _IONBF, 0);   //to send despite no \n
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
    LL_USART_InitTypeDef LL_USART_InitType;
    LL_USART_InitType.BaudRate = 115200;
    LL_USART_InitType.DataWidth = LL_USART_DATAWIDTH_8B;
    LL_USART_InitType.StopBits = LL_USART_STOPBITS_1;
    LL_USART_InitType.Parity = LL_USART_PARITY_NONE;
    LL_USART_InitType.TransferDirection = LL_USART_DIRECTION_TX_RX;
    LL_USART_InitType.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    LL_USART_InitType.OverSampling = LL_USART_OVERSAMPLING_16;
    LL_USART_Init(USARTx, &LL_USART_InitType);
    LL_USART_Enable(USARTx);
}

int __io_putchar(int ch) {

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

    // Configure Pins PA2 PA3
    GPIO_InitStruct.Pin = LL_GPIO_PIN_2 | LL_GPIO_PIN_3;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    LL_USART_TransmitData8(USART2, ch);
  return ch;
}

extern "C" int _write(int fd, char *ptr, int len) {
      (void)fd;
      int i;
      for(i=0;i<len;i++){
        __io_putchar(*ptr++);
      }
      return len;
    }
    
/*    extern "C" size_t _read(int fd, char *ptr, size_t len){
      (void)fd;
      size_t i;
      for(i=0;i<len;i++){
        *ptr++ = uart_read();
        uart_write(*ptr++);  //For Terminal Echo
      }
      return i;
    }*/



void mDelay(uint32_t delay) {
    ReInit1msTick();
    LL_mDelay(delay);  
}

void ReInit1msTick() {
    LL_RCC_ClocksTypeDef RCC_CLOCKS;
    LL_RCC_GetSystemClocksFreq(&RCC_CLOCKS);
    LL_Init1msTick(RCC_CLOCKS.HCLK_Frequency);
}
