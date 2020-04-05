extern unsigned char inb(unsigned short int port_from);
extern void outb(unsigned short int port_to, unsigned char value);

//8259A主片的控制端口号1
#define INT_MASTER_CTL (0x20)
//8259A从片的控制端口号1 
#define INT_SLAVE_CTL (0xa0)
//8259A主片的控制端口号2 
#define INT_MASTER_CTLMSK (0x21)
//8259A从片的控制端口号2 
#define INT_SLAVE_CTLMSK (0xa1)
//外中断的个数  
#define IRQ_NUM (16)          

void init8259A(void)
{	
	/* 8259A 主片和从片，OCW1,起屏蔽外中断作用 */
	outb(INT_MASTER_CTLMSK, 0xff);
	outb(INT_SLAVE_CTLMSK, 0xff);
	/* 8259A 主片和从片，ICW1 */
	outb(INT_MASTER_CTL, 0x11);
	outb(INT_SLAVE_CTL, 0x11);
	/* 8259A 主片和从片，ICW2 */
	outb(INT_MASTER_CTLMSK, 0x20);
	outb(INT_SLAVE_CTLMSK, 0x28);
	/* 8259A 主片和从片，ICW3 */
	outb(INT_MASTER_CTLMSK, 4);
	outb(INT_SLAVE_CTLMSK, 2);
	/* 8259A 主片和从片，ICW4 */
	outb(INT_MASTER_CTLMSK, 3);
	outb(INT_SLAVE_CTLMSK, 1);
}