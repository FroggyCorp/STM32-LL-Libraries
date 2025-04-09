# STM32 LL driver Libraries
I recently start with STM32, coming from arduino (Attiny, Nano, ...) and ESP. It's really easy to find libraries for everything for Arduino & ESP but it seems impossible to find easy to use libraries for common function under STM32 environnement. Starting with platformio, framework arduino, getting SPI, I2C, Serial.print functionnal took me around 30Ko.
Most of documentation refer to HAL drivers, which like arduino environnement are easy to use but took huge flash and RAM memory. So i wanted to make everything under Low Level drivers but documentation seems to be hard to find.
As usual, forum give some part of answer but not a .c/.cpp which only need to be include.

So, this repository regroup easy to use basic function. They are all for STM32L03K6, but you only need to change PIN configuration and #include refering to your version. All PIN information are inside datasheet at "Pin descriptions", easy finding.

Some change will be made, like saving I2Cx to not ask for it each time.

Refer to the main.c which regroups example and how to use

## STM32 SPI LL driver
begin() - init the SPI & PIN
transfert8b() - basic function send 8b on MOSI, read 8b on MISO on 1 clock
writeReg() - 2 clock, first send 8b, second send 8b (2 times transfert8b())
readReg() - 2 clock, first send 8b, second return 8b read
end() - stop SPI clock, redefine pin for power saving

## STM32 I2C LL driver
init() - init the I2C & PIN
write() - write to the I2C addr, len bytes
read() - read from the I2C addr, len bytes
end() - stop I2C clock, redefine pin for power saving

## STM32 utils LL driver
mDelay() - same as delay() in arduino. Change the 1ms tick depending on AHB prescaler
SerialInit() - configure serial and printf to use serial

# Next coming
v_bat() to calculate the Vcc (Vrefint)
Some change on librairy to make them more easy to use. Like saving I2Cx at constructor or init