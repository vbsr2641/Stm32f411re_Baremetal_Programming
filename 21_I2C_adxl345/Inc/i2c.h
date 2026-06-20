#ifndef I2C_H_
#define I2C_H_

void I2C1_init(void);

void I2C1_byteRead(char slaveaddr, char memoryaddr, char* data);

void I2C1_burstRead(char slaveaddr, char memoryaddr, int n, char* data);

void I2C1_burstWrite(char slaveaddr, char memoryaddr, int n, char* data);

#endif /* I2C_H_ */
