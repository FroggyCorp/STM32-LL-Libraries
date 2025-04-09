/*
********************************
* I2C STM32 LL Library example *
********************************

What to change ?
in STM32_I2C.cpp : APB1, GRP1, GPIOA, LL_GPIO_PIN, LL_GPIO_AF depending on your STM32
in STM32_I2C.h : all #define refering to your STM32 version
in calling function : I2C1 depending on your I2C
*/

#include <STM32_I2C.h>
#include <stdint.h>
#include <stdio.h>

STM32_I2C i2c = STM32_I2C();
int main() {
    uint8_t error;          //get error number depending on timeout
    uint8_t data[8] = {0};  //array to share data read/write

    i2c.init(I2C1);         //init clock, GPIO on I2C1
    data[0] = 0x12;
    error = i2c.write(I2C1, 0x44, data, 1); //write 1 byte = 0x12 to I2C with addr 0x44
    error = i2c.read(I2C1, 0x44, data, 3); //read 3 byte from I2C with addr 0x44 and save it to data
    printf("data[0] %d data[1] %d data[2] %d\n", data[0], data[1], data[2]);
    printf(error);

   i2c.end(I2C1);   //stop I2C clock, desinit GPIO for saving power
}


/*
********************************
* SPI STM32 LL Library example *
********************************

What to change ?
in STM32_SPI.cpp : APB1, GRP1, GPIOA/GPIOB, LL_GPIO_PIN, LL_GPIO_AF depending on your STM32
in STM32_SPI.h : all #define refering to your STM32 version
in calling function : SPI1 depending on your SPI, i have 2 port using SPI, so refer to which port you want to use

NSS is software, documentation say it could be hardware but don't get success on STM32L0.
*/

#include <STM32_SPI.h>
#include <stdint.h>
#include <stdio.h>

STM32_SPI spi = STM32_SPI(SPI1, 0);

int main() {
    spi.begin(SPI1, 1); //it can be define at constructor or at begin
    uint8_t read_back = 0;  //get 1 byte result
    read_back = spi.readReg(0x02); //read value of reg 0x02. Don't forget mask if needed
    spi.writeReg(0x12, 0x01); //write 0x01 to reg 0x12 aka send 2 byte
    read_back = spi.transfert8b(0x12); //write 0x12 and get result of same transmission
    spi.end();  //stop clock, desinit GPIO for saving power
}

/*
************************************
* pintf() STM32 LL Library example *
* delay() STM32 LL Library example *
***********************************

What to change ?
in STM32_utils.cpp : APB1, GRP1, GPIOA/GPIOB, LL_GPIO_PIN, LL_GPIO_AF, USART2 depending on your STM32 (USART2 is actually hard coded, should be change for better use)
in STM_utils.h : all #define inside STM32_I2C refering to your STM32 version
in calling function : USART depending on your USART using

*/

#include <STM32_utils.h>

int maint () {
    mDelay(10);             //10ms delay. Change the 1ms tick depending on your AHB prescaler
    
    SerialInit(USART2, 9600);   //initialize the USART2 with 9600 baudrate
    printf("Hello World\n");    //printf using


}