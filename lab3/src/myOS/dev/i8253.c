extern void outb(unsigned short int port_to, unsigned char value);

//253Counter0端口号
#define COUNTER0 (0x40)
//PC的频率
#define PC_FREQ (1193180L)
//8253模式控制寄存器
#define MODE_CTL (0x43)
//1秒钟发生100次时钟中断，即每隔0.01秒发生1次      
#define MY_HZ (100)

void init8253(void)
{
	outb(MODE_CTL, 0x34);
	//写入分频参数
	outb(COUNTER0, (unsigned char)(PC_FREQ / MY_HZ));
	outb(COUNTER0, (unsigned char)((PC_FREQ / MY_HZ) >> 8));
}