//本文件实现vga的相关功能，清屏和屏幕输出，clear_screen和append2screen必须按照如下实现，可以增加其他函数供clear_screen和append2screen调用

//extern void disable_interrupt(void);
//extern void enable_interrupt(void);

extern void outb(unsigned short int port_to, unsigned char value);
extern unsigned char inb(unsigned short int port_from);

#define VGA_BASE (0xB8000)
#define LineLenth (80)
#define ColumnLenth (25)
static unsigned char* VGA_Base;

static int m_CursorX;
static int m_CursorY;

void SetCursor(int x, int y)
{
	unsigned short pos = y * 80 + x;
	outb(0x3d4, 14);
	outb(0x3d5, (pos >> 8) & 0xff);
	outb(0x3d4, 15);
	outb(0x3d5, pos & 0xff);
	m_CursorX = x;
	m_CursorY = y;
}

void ScrollUp()
{
	for (int line = 0; line < ColumnLenth; line++)
		for (int i = 0; i < 160; i++)
			* (VGA_Base + line * 160 + i) = *(VGA_Base + (line + 1) * 160 + i);
	m_CursorY = 24;
}

void VGAputchar(unsigned char c, int color)
{
	switch (c)
	{
	case '\r':
	case '\n':
		m_CursorX = 0;
		m_CursorY++;
		break;
	case '\t':
		m_CursorX = (m_CursorX / 8 + 1) * 8;
		if (m_CursorX >= LineLenth)
		{
			m_CursorX -= LineLenth;
			m_CursorY++;
		}
		break;
	default: 
	{
		unsigned int pos = m_CursorY * LineLenth + m_CursorX;
		*(VGA_Base + pos * 2) = c;
		*(VGA_Base + pos * 2 + 1) = color;
		pos++;
		m_CursorX = pos % LineLenth;
		m_CursorY = pos / LineLenth;
	}
	break;
	}
	if (m_CursorY == ColumnLenth)
		ScrollUp();
	SetCursor(m_CursorX,m_CursorY);
}

void clear_screen(void)
{
	VGA_Base = (char*)VGA_BASE;
	unsigned char* p;
	p = (char*)VGA_BASE;
	for (int i = 0; i < 1000; i++)
		VGAputchar(' ', 0);
	p = (char*)(VGA_Base + 1);
	*p = 0;
	VGA_Base = (char*)VGA_BASE;
}

void append2screen(char* str, int color)
{
	while (*str != '\0')
	{
		VGAputchar(*str, color);
		str++;
	}
}