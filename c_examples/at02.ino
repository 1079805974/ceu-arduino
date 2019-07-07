#define AT24C_Add_W  0xA0   //器件读地址，A2,A1=0， 默认选择0~255字节地址
#define AT24C_Add_R  0xA1   //器件写地址，A2,A1=0， 默认选择0~255字节地址
#define SLAW         0x18   //模块正确地址应答常量写 page200
#define DataOKW      0x28   //模块正确数据写应答常量
#define SLAR         0x40   //地址收到ok,page204
#define DataOKR      0x50   //数据收到ok

//数据类型宏定义
typedef unsigned char uint08;
typedef signed   char sint08;
typedef unsigned int  uint16;
typedef signed   int  sint16;
//变量定义


//发送数据缓存区，存入FLASH ,ASCII码形式，不支持中文,注意在字符串尾加0xFF作为结束标记
const PROGMEM  uint08 Send_Buff[512]="As food is to the body, so is learning to the mind. Our bodies grow and muscles develop with the intake of adequate nutritious food. Likewise, we should keep learning day by day to maintain our keen mental power and expand our intellectual capacity. Constant learning supplies us with inexhaustible fuel for driving us to sharpen our power of reasoning, analysis, and judgment. Learning incessantly is the surest way to keep pace with the times in the information age\xFF";   
uint08 Page=0x00;                                 //页选择位，选择0或1,选择0页写0，选择1页写2
uint08 Read_Buff[512];                            //数据接收缓存区，AT24C04最大空间256字节

//------------------------------------------------------------------------
//AT24时钟频率1Mhz,设置单片机比特率
//SCL frequency=cpu clock/(16+2(TWBR)*(prescalerValue))
void Init_TWI(void)
{ TWCR = 0x00;                       //中止IIC
  //PRR =  0x00;                     //复位功耗抑制寄存器，TWI唤醒
  TWBR =   24;                       //比特率寄存器为24，产生9615波特率
  TWSR|= 0x02;                       //比特率预分频因子16，page194
  //TWAR=0xFF;                       //工作于主机模式不需要此地址
  TWCR = (1<<TWEN);                  //TWI使能,TWINF写1清零

}
//------------------------------------------------------------------------
//启动I2C
void TWIStart(void)                             //page198
{  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);     //TWI start，主机模式，page149
    while(!(TWCR&(1<<TWINT))) ;                 //等待TWINF置位以及收到应答信号
}
//-------------------------------------------------------------------------------
//主机发送一个字节
void TWI_Write(uint08 str)
{   TWDR = str;                               //将字符写入数据寄存器
     TWCR = (1<<TWINT)|(1<<TWEN);              //启动发送地址及数据，page198
      while(!(TWCR&(1<<TWINT)));               //等待TWINF置位，SLA+W或data已发出
}
//-------------------------------------------------------------------------------
//总线读出一个字符返回读出的字符
uint08 TWI_READ(void)
{  TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN);
   while(!(TWCR&(1<<TWINT)));
   return(TWDR);                            //返回读出的数据
}
//I2C应答函数-------------------------------------------------------------------
uint08 TWI_ACK(void)
{ _NOP();                                  //延时1个指令周期
  return (TWSR&0xF8);                      //返回TWI状态，高5位
}
//--------------------------------------------------------------------------------
//I2C 停止
void TWI_Stop(void)
{  
  TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);   //page198
}
//---------------------------------------------------------------------------------
//向I2C slave写入数据,第一个参数是页选择为0和1。第二个是字节地址0xFF，第三个参数为字节数据
void AT24_TWI_Write(uint08 Page,uint08 ByteAdd,uint08 Str)
{  TWIStart();                                 //开启发送     
  TWI_Write(Page|AT24C_Add_W);               //发送从机地址
  if(TWI_ACK()==SLAW)                        //应答
   TWI_Write(ByteAdd);                       //发送字节地址
  if(TWI_ACK()==DataOKW)                    //应答
   TWI_Write(Str);                          //发送数据
   TWI_ACK();                               //写入数据应答
   TWI_Stop();                             //发送停止信号
   delay(1);                               //延时1ms,完成写入
}

//--------------------------------------------------------------------------------
//从I2C读取数据参数1为变量地址，参数2为页选择，参数3为字节地址，参数4为读取数量
//读取模式为随机读
uint08 AT24_TWI_Read(uint08 Page,uint08 ByteAdd)
{ uint08 i,u08temp;
  TWIStart();                                  //发送起始信号                      
  TWI_Write(Page|AT24C_Add_W);                 //写入地址及页选择位
  if(TWI_ACK()==SLAW)                         //地址发送应答
    TWI_Write(ByteAdd);                        //写入字节地址
  if(TWI_ACK()==DataOKW)                      //发送页码和字节地址
   TWIStart();                                //再一次开始
  TWI_Write(Page|AT24C_Add_R);                  //写入读地址及页选择位
  if(TWI_ACK()==SLAR)                        //读应答,返回值有误
  TWI_ACK();
   u08temp=TWI_READ();                         //从总线读一个字节存入中间量
  TWI_ACK();                                   //应答
  TWI_Stop();                                 //发送停止信号   
  return u08temp;                             //返回读取的数据
}


//写入数据函数------------------------------------------------------------------
void WriteAT24(const uint08 ch[])
{ uint16 u16add=0;
  uint08 ByteAdd;                                         //字节起始地址
  uint08 u08temp;
  Page=0;
  for(ByteAdd=0;;u16add++,ByteAdd++)
  {
   u08temp=pgm_read_byte_near(ch+u16add);
  if(u08temp==0xff) return;
  if(ByteAdd<0xff)                              //如果长度小于255则写入第一页
    AT24_TWI_Write(Page,ByteAdd,u08temp);      //向AT24C04写入数据
  else 
    {
     AT24_TWI_Write(Page,ByteAdd,u08temp);      //向AT24C04写入数据
     if(!Page)Page=0x02;                           //换页
     else return;
     ByteAdd=0;
    }

  }
} 
//读入数据函数------------------------------------------------------------------
void ReadAt24( uint08 ch[])
{uint08 u08tempt;
uint16 u16add;
uint08 ByteAdd=0;                                         //字节起始地址
Page=0;
for(u16add=0;;u16add++,ByteAdd++)
{
u08tempt=AT24_TWI_Read(Page,ByteAdd);                   //读一个字节
if(u08tempt==0xff) break;                              //返回字符为0xff提前结束
if(ByteAdd<0xff) ch[u16add]=u08tempt;
else 
  { ch[u16add]=u08tempt; 
  if(!Page) Page=2;
  else return;
  ByteAdd=0;
  }
}
}
//======================================================================
void WDT_OFF(void)
{WDTCSR|=0x80;
MCUSR&=~(1<<WDRF);
WDTCSR |= (1<<WDCE) | (1<<WDE);
WDTCSR = 0x00;
}
//======================================================================
void setup(void )
{
  cli();                      //清中断
  Init_TWI();                 //初始化设备
  WDT_OFF();                  //关闭看门狗
  Serial.begin(9600);
  sei();                      //中断使能 

  WriteAT24(Send_Buff);      //写数据
}
//主函数------------------------------------------------------------------------
void loop(void) 
{
// WDTCSR|=0x80;
  ReadAt24(Read_Buff);
//Serial.println("have read data:");
  Serial.println((char*)Read_Buff);                       //串口发送读到的数据             
  delay(1000);                                               //延时1s
}
