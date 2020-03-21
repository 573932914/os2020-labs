extern int myPrintk(int color,const char *format, ...);
extern int myPrintf(int color, const char* format, ...);
extern void clear_screen(void);
extern void myMain(void);

void osStart(void){
	clear_screen();
	myPrintk(0x2,"START RUNNING......\n");
	myPrintf(0x2, "START RUNNING......\n");
	myMain();
	myPrintk(0x2, "STOP RUNNING......ShutDown\n");
	myPrintf(0x2, "STOP RUNNING......ShutDown\n");
	//while(1);
}
