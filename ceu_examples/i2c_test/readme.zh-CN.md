# I2C 库测试

这个文件夹包含I2C驱动的样例, 你能学到该驱动的基本使用方式. 

## 硬件需求

需要2个Arduino, 以如下方式相连.  

- A4 -> A4
- A5 -> A5
- GND -> GND

## 文件说明

1. master_send.ceu  
每隔一秒发送`buf`中的数据到I2C从地址为8的Arduino, 与`slave_recieve.ceu`对应.

2. master_recieve.ceu  
每隔一秒向I2C从地址为8的Arduino请求1 字节(byte)数据, 并输出到串口.

3. slave_recieve.ceu  
声明I2C从地址为8, 收到任何数据时, 将数据输出到串口.

4. slave_send.ceu  
声明I2C从地址为8, 当主机请求数据时, 发送`str`中的数据.

5. master_recieve_send.ceu  
同时具有`master_recieve`和`master_send`的功能, 并使用`usart`模块.

6. master_recieve_send.ceu  
同时具有`slave_recieve`和`slave_send`的功能, 并使用`usart`模块.
