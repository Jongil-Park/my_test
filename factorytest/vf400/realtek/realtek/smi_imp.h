int _rtl865x_initGpioPin(int pin, int direction);
int _rtl865x_setGpioDataBit(uint32_t pin, uint32_t value);
uint32_t _rtl865x_getGpioDataBit(uint32_t pin);
void _smi_start(void);
void _smi_writeBit(uint16_t signal, uint32_t bitLen);
void _smi_readBit(uint32_t bitLen, uint32_t *rData);
void _smi_stop(void);



