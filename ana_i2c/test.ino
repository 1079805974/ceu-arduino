
#define I2C_SDA_1() digitalWrite(5, 1); // SDA线置高电平
#define I2C_SDA_0() digitalWrite(5, 0); // SDA线置低电平

#define I2C_SCL_1() digitalWrite(4, 1); // SCL线置高电平
#define I2C_SCL_0() digitalWrite(4, 0); // SCL线置低电平

#define SDA 5
#define SCL 4

int I2C_SDA_READ()
{
    int res = digitalRead(SDA);
    return res;
}

int I2C_SCL_READ()
{
    int res = digitalRead(SCL);
    return res;
}

void Delay()
{
    //delayMicroseconds(400000);
    //_delay_us(400 * 1000);
    delay(400);
}

void i2c_Start(void)
{
    I2C_SDA_1();
    I2C_SCL_1();
    Delay();
    I2C_SDA_0();
    Delay();
    I2C_SCL_0();
}

void i2c_Stop(void)
{
//    I2C_SDA_0();
//    Delay();
    I2C_SCL_1();
    Delay();
    I2C_SDA_1();
    Delay();
}

void i2c_send_bit(uint8_t c){
  if(c == 0){
    I2C_SDA_0();
  }else{
    I2C_SDA_1();
  }
  I2C_SCL_1();
  Delay();
  I2C_SCL_0();
  Delay();
  if(c != 0){
    I2C_SDA_0();
    Delay();
  }
}

void i2c_Ack(void)
{
    I2C_SDA_0();
    I2C_SCL_1();
    Delay();
    I2C_SCL_0();
    Delay();
    I2C_SDA_1(); // CPU释放SDA
}

void i2c_NAck(void)
{
    I2C_SDA_1();
    Delay();
    I2C_SCL_1();
    Delay();
    I2C_SCL_0();
    Delay();
}

uint8_t i2c_WaitAck(void)
{
    uint8_t re;

    I2C_SDA_1();
    pinMode(SDA, INPUT); // CPU释放SDA总线
    I2C_SCL_1(); // CPU拉高SCL电平，发送一个时钟, 此时会返回ACK应答
    Delay();
    if (I2C_SDA_READ() != 0) // 判断读取的SDA电平状态
    {
        re = 1;
    }
    else
    {
        re = 0;
    }
    I2C_SCL_0();
    pinMode(SDA, OUTPUT);
    Delay();

    return re;
}

void i2c_SendByte(uint8_t byte)
{
    I2C_SCL_0();
    uint8_t i;

    /* 先发送字节的最高位 */
    for (i = 0; i < 8; i++)
    {
        if (byte & 0x80) // 判断最高位为1或0
        {
            I2C_SDA_1();
        }
        else
        {
            I2C_SDA_0();
        }
        Delay();
        I2C_SCL_1();
        Delay();
        I2C_SCL_0();
        if (i == 7)
        {
            I2C_SDA_1(); // 释放总线
        }
        byte <<= 1; // 左移1位，准备发送下一位
        Delay();
    }
}

void i2c_WaitAck_Delay(int delay)
{
    uint8_t ack_status;

    while (delay--)
    {
        ack_status = i2c_WaitAck();
        if (ack_status) // 如果是非应答，主机需要产生停止信号终止传输
        {
            i2c_Stop();
            return 0;
        }
        else
        {
            break;
        }
    }
}

uint32_t I2C_ByteWrite(u8 WriteAddr, u8 pBuffer)
{
    i2c_Start();

    i2c_SendByte(0xA0); // 发送设备地址
    i2c_WaitAck_Delay(1000);

    i2c_SendByte(WriteAddr); // 发送要写入的内存地址
    i2c_WaitAck_Delay(1000);

    i2c_SendByte(pBuffer); // 开始写入数据
    i2c_WaitAck_Delay(1000);

    i2c_Stop();
    return 1;
}

uint8_t i2c_ReadByte(void)
{
    uint8_t i;
    uint8_t value = 0;

    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        I2C_SCL_1();
        Delay();
        if (I2C_SDA_READ())
        {
            value++;
        }
        I2C_SCL_0();
        Delay();
    }

    return value;
}

uint32_t I2C_Read(u8 WriteAddr, u8 *pBuffer, u8 numByteToRead)
{
    /* 第一次起始条件 */
    i2c_Start();

    i2c_SendByte(0xA0); // 此处是写指令
    i2c_WaitAck_Delay(1000);

    i2c_SendByte(WriteAddr);
    i2c_WaitAck_Delay(1000);

    /* 第二次起始条件 */
    i2c_Start();

    i2c_SendByte(0xA1); // 此处是读指令
    i2c_WaitAck_Delay(1000);

    while (numByteToRead)
    {
        *pBuffer = i2c_ReadByte();

        /* 每读完1个字节后，需要发送Ack，最后一个字节发Nack */
        if (numByteToRead != 1)
        {
            i2c_Ack();
        }
        else
        {
            i2c_NAck();
        }

        pBuffer++;
        numByteToRead--;
    }

    i2c_Stop();
    return 0;
}

void setup()
{
    // put your setup code here, to run once:
    pinMode(SDA, OUTPUT);
    pinMode(SCL, OUTPUT);
    I2C_SDA_1();
    I2C_SCL_1();
    Serial.begin(9600);
}

void loop()
{
    I2C_SDA_1();
    I2C_SCL_1();
    Delay();
    I2C_SDA_0();
    Delay();
    I2C_SCL_0();
    Delay();
    
    I2C_SDA_0();
    Delay();
    I2C_SCL_1();
    Delay();
    I2C_SCL_0();
    Delay();

    I2C_SDA_0();
    Delay();
    I2C_SCL_1();
    Delay();
    I2C_SCL_0();
    Delay();    

    I2C_SDA_0();
    Delay();
    I2C_SCL_1();
    Delay();
    I2C_SCL_0();
    Delay();

    I2C_SDA_1();
    Delay();
    I2C_SCL_1();
    Delay();
    I2C_SCL_0();
    Delay();
    I2C_SDA_0();
    Delay();


    I2C_SDA_0();
    Delay();
    I2C_SCL_1();
    Delay();
    I2C_SCL_0();
    Delay();
    
    I2C_SDA_0();
    Delay();
    I2C_SCL_1();
    Delay();
    I2C_SCL_0();
    Delay();
    
    I2C_SDA_0();
    Delay();
    I2C_SCL_1();
    Delay();
    I2C_SCL_0();
    Delay(); 
        
    I2C_SDA_0();
    Delay();
    I2C_SCL_1();
    Delay();
    I2C_SCL_0();
    Delay();
    I2C_SDA_0();
    Delay();


    
    uint8_t res;

    pinMode(SDA, INPUT); // CPU释放SDA总线
    I2C_SCL_1(); // CPU拉高SCL电平，发送一个时钟, 此时会返回ACK应答
    Delay();
    if (I2C_SDA_READ() != 0) // 判断读取的SDA电平状态
    {
        res = 1;
    }
    else
    {
        res = 0; 
    }
    I2C_SCL_0();
    Delay();
    pinMode(SDA, OUTPUT);
    Serial.println(res);
    
    I2C_SDA_0();
    Delay();
    I2C_SCL_1();
    Delay();
    I2C_SCL_0();
    Delay(); 
    
    I2C_SDA_0();
    Delay();
    I2C_SCL_1();
    Delay();
    I2C_SCL_0();
    Delay(); 
    
    I2C_SDA_0();
    Delay();
    I2C_SCL_1();
    Delay();
    I2C_SCL_0();
    Delay(); 
    
    I2C_SDA_0();
    Delay();
    I2C_SCL_1();
    Delay();
    I2C_SCL_0();
    Delay(); 
    
    I2C_SDA_0();
    Delay();
    I2C_SCL_1();
    Delay();
    I2C_SCL_0();
    Delay(); 
    
    I2C_SDA_0();
    Delay();
    I2C_SCL_1();
    Delay();
    I2C_SCL_0();
    Delay(); 
    
    I2C_SDA_0();
    Delay();
    I2C_SCL_1();
    Delay();
    I2C_SCL_0();
    Delay(); 
    
    I2C_SDA_0();
    Delay();
    I2C_SCL_1();
    Delay();
    I2C_SCL_0();
    Delay(); 
    
    pinMode(SDA, INPUT); // CPU释放SDA总线
    I2C_SCL_1(); // CPU拉高SCL电平，发送一个时钟, 此时会返回ACK应答
    Delay();
    if (I2C_SDA_READ() != 0) // 判断读取的SDA电平状态
    {
        res = 1;
    }
    else
    {
        res = 0; 
    }
    I2C_SCL_0();
    Delay();
    pinMode(SDA, OUTPUT);
    Serial.println(res);

    I2C_SDA_0();
    Delay();
    I2C_SCL_1();
    Delay();
    I2C_SDA_1();
    Delay();

    delay(1000);
}