#include<graph.h>
#include<stdlib.h>
#include<malloc.h>
#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include<memory.h>
#include<math.h>
#include<errno.h>
#include<bios.h>
#include<string.h>
			
unsigned char far *videoBuffer=(unsigned char far *)0xA0000000L;
unsigned char far *doubleBuffer=NULL;
unsigned char far *dataBuffer=NULL;
unsigned int far *clock=(unsigned int far *)0x0000046CL;

char att1,dec1,sus1,rel1,mul1,vol1,am1,vib1,con1,ksr1,ksl1,ws1;
char att2,dec2,sus2,rel2,mul2,vol2,am2,vib2,con2,ksr2,ksl2,ws2;
char oct,key,fb,fm,vib,csm,sel,rhy,bas,sna,tom,cym,hat;
int freq;
char *filename;
char xLoc,yLoc,done;
char oct1=0;

void Initialize(void);
void Restore(void);
void DrawLetter(char letter,int x,int y,char color);
long TranslateBits(char *bits);
void VertLine(int x,int y1,int y2,char color);
void HorizLine(int y,int x1,int x2,char color);
void CreateScreen(void);
void DrawBar(int x,int y, char len);
void DrawBox(int x1,int y1,int x2,int y2,char color);
void DrawCheckbox(int x1,int y1);
void Mirror(int y1,int y2);
void ResetChip(void);
void DrawScreen(void);
void DrawSlider(int x,int y);
void DrawCheck(int x,int y);
void DrawLine(int x1,int y1,int x2,int y2,char color);
void UserInput(void);
void ChangeColor(int x1,int y1,char dx,char dy);
int ChangeFreq(void);
char* ChangeFilename(void);
void SaveFile(void);
void LoadFile(void);
void WriteAllRegs(void);
void WriteReg(char index,char data);
void Randomize(void);
void PlayKey(int freq2);

void Initialize(void)
	{
	_setvideomode(0x13);
	doubleBuffer=(unsigned char far *)_fmalloc(64000);
	_FP_OFF(doubleBuffer)=0;
	dataBuffer=(unsigned char far *)_fmalloc(64000);
	_FP_OFF(dataBuffer)=0;
	_asm                
		{
		mov cx,32000
		mov ax,0
		les di,videoBuffer
		cld
		rep stosw
		mov cx,32000
		mov ax,0
		les di,doubleBuffer
		rep stosw
		mov cx,32000
		mov ax,0
		les di,dataBuffer
		rep stosw
		}
	ResetChip();
	xLoc=0;
	yLoc=0;
	done=0;
	srand(*clock);
	}        

void Restore(void)
	{
	ResetChip();
	_ffree(doubleBuffer);
	_ffree(dataBuffer);
	_setvideomode(0x03);
	}

void DrawLetter(char letter,int x,int y,char color)
	{
	unsigned int writePos=x+(y*320);
	long data,value=1;
	char count1,count2;
	switch(letter)
		{
		case('a'):
			data=TranslateBits("0111010001111111000110001");
			break;
		case('b'):
			data=TranslateBits("1111010001111101000111110");
			break;
		case('c'):
			data=TranslateBits("0111110000100001000001111");
			break;
		case('d'):
			data=TranslateBits("1111010001100011000111110");
			break;
		case('e'):
			data=TranslateBits("1111110000111101000011111");
			break;
		case('f'):
			data=TranslateBits("1111110000111101000010000");
			break;
		case('g'):
			data=TranslateBits("0111110000100111000101110");
			break;
		case('h'):
			data=TranslateBits("1000110001111111000110001");
			break;
		case('i'):
			data=TranslateBits("0111000100001000010001110");
			break;
		case('j'):
			data=TranslateBits("0000100001000011000101110");
			break;
		case('k'):
			data=TranslateBits("1000110010111001001010001");
			break;
		case('l'):
			data=TranslateBits("1000010000100001000011111");
			break;
		case('m'):
			data=TranslateBits("1000111011101011000110001");
			break;
		case('n'):
			data=TranslateBits("1000111001101011001110001");
			break;
		case('o'):
			data=TranslateBits("0111010001100011000101110");
			break;
		case('p'):
			data=TranslateBits("1111010001111101000010000");
			break;
		case('q'):
			data=TranslateBits("0111010001100011001001101");
			break;
		case('r'):
			data=TranslateBits("1111010001111101000110001");
			break;
		case('s'):
			data=TranslateBits("0111110000011100000111110");
			break;
		case('t'):
			data=TranslateBits("1111100100001000010000100");
			break;
		case('u'):
			data=TranslateBits("1000110001100011000101110");
			break;
		case('v'):
			data=TranslateBits("1000110001100010101000100");
			break;
		case('w'):
			data=TranslateBits("1000110001101011101110001");
			break;
		case('x'):
			data=TranslateBits("1000101010001000101010001");
			break;
		case('y'):
			data=TranslateBits("1000110001010100010000100");
			break;
		case('z'):
			data=TranslateBits("1111100010001000100011111");
			break;
		case('0'):
			data=TranslateBits("0111010011101011100101110");
			break;
		case('1'):
			data=TranslateBits("0010001100001000010001110");
			break;
		case('2'):
			data=TranslateBits("1111000001001100100011111");
			break;
		case('3'):
			data=TranslateBits("1111000001011100000111110");
			break;
		case('4'):
			data=TranslateBits("0011001010100101111100010");
			break;
		case('5'):
			data=TranslateBits("1111110000111100000111110");
			break;
		case('6'):
			data=TranslateBits("0111010000111101000101110");
			break;
		case('7'):
			data=TranslateBits("1111100001000100010000100");
			break;
		case('8'):
			data=TranslateBits("0111010001011101000101110");
			break;
		case('9'):
			data=TranslateBits("0111010001011110000101110");
			break;
		case(':'):
			data=TranslateBits("0000000100000000010000000");
			break;
		}
	for(count1=4;count1>-1;count1--)
		for(count2=4;count2>-1;count2--)
			{
			if(data&value)
				doubleBuffer[x+count2+((y+count1)*320)]=color;
			value*=2;
			}
	}                    

long TranslateBits(char *bits)
	{
	long value=0;
	long value2=1;
	char count;
	for(count=24;count>-1;count--)
		{
		if(bits[count]=='1')
			value+=value2;
		value2*=2;
		}
	return value;
	}

void VertLine(int x,int y1,int y2,char color)
	{
	for(;y1<=y2;y1++)
		doubleBuffer[x+(y1*320)]=color;
	}

void HorizLine(int y,int x1,int x2,char color)
	{
	for(;x1<=x2;x1++)
		doubleBuffer[x1+(y*320)]=color;
	}

void DrawBar(int x,int y, char len)
	{
	char count;
	VertLine(x,y,y+2,7);
	VertLine(x+len+1,y,y+2,7);
	HorizLine(y,x+1,x+len,7);
	HorizLine(y+1,x+1,x+len,8);
	HorizLine(y+2,x+1,x+len,7);
	}

void DrawBox(int x1,int y1,int x2,int y2,char color)
	{
	for(;y1<=y2;y1++)
		HorizLine(y1,x1,x2,color);
	}

void DrawCheckbox(int x1,int y1)
	{
	HorizLine(y1,x1,x1+4,7);
	HorizLine(y1+4,x1,x1+4,7);
	VertLine(x1,y1,y1+4,7);
	VertLine(x1+4,y1,y1+4,7);
	DrawBox(x1+1,y1+1,x1+3,y1+3,8);
	}

void Mirror(int y1,int y2)
	{
	y2++;
	_asm
		{
		mov ax,y1
		mov bx,y2
		push ds
		les di,doubleBuffer
		lds si,doubleBuffer
		mov cx,ax
		shl cx,8
		mov dx,ax
		shl dx,6
		add cx,dx
		mov si,cx
		mov dx,160
		add cx,dx
		mov di,cx
		cld
		loop1:
			mov cx,80
			rep movsw
			add si,dx
			add di,dx
			inc ax
			cmp ax,bx
			jnz loop1
		pop ds
		}
	}

void CreateScreen(void)
	{
	HorizLine(0,0,159,9);
	HorizLine(67,0,159,9);
	VertLine(0,1,66,9);
	VertLine(159,1,66,9);
	DrawLetter('o',50,2,15);
	DrawLetter('p',56,2,15);
	DrawLetter('e',62,2,15);
	DrawLetter('r',68,2,15);
	DrawLetter('a',74,2,15);
	DrawLetter('t',80,2,15);
	DrawLetter('o',86,2,15);
	DrawLetter('r',92,2,15);
	DrawLetter('1',104,2,15);
	VertLine(47,0,8,9);
	VertLine(48,0,8,9);
	VertLine(110,0,8,9);
	VertLine(111,0,8,9);
	HorizLine(8,0,159,9);
	HorizLine(10,53,117,7);
	HorizLine(58,53,117,7);
	VertLine(52,10,58,7);
	VertLine(53,11,57,7);
	VertLine(117,11,57,7);
	VertLine(118,10,58,7);
	DrawBox(54,11,116,57,2);
	HorizLine(16,54,117,10);
	HorizLine(22,54,117,10);
	HorizLine(28,54,117,10);
	HorizLine(34,54,117,10);
	HorizLine(40,54,117,10);
	HorizLine(46,54,117,10);
	HorizLine(52,54,117,10);
	VertLine(61,11,57,10);
	VertLine(69,11,57,10);
	VertLine(77,11,57,10);
	VertLine(85,11,57,10);
	VertLine(93,11,57,10);
	VertLine(101,11,57,10);
	VertLine(109,11,57,10);
	DrawLetter('a',5,30,7);
	DrawLetter('t',11,30,7);
	DrawLetter('t',17,30,7);
	DrawLetter(':',23,30,7);
	DrawBar(29,31,16);
	DrawLetter('d',5,36,7);
	DrawLetter('e',11,36,7);
	DrawLetter('c',17,36,7);
	DrawLetter(':',23,36,7);
	DrawBar(29,37,16);
	DrawLetter('s',5,42,7);
	DrawLetter('u',11,42,7);
	DrawLetter('s',17,42,7);
	DrawLetter(':',23,42,7);
	DrawBar(29,43,16);
	DrawLetter('r',5,48,7);
	DrawLetter('e',11,48,7);
	DrawLetter('l',17,48,7);
	DrawLetter(':',23,48,7);
	DrawBar(29,49,16);
	DrawLetter('a',124,30,7);
	DrawLetter('m',130,30,7);
	DrawLetter(':',136,30,7);
	DrawCheckbox(149,30);
	DrawLetter('v',124,36,7);
	DrawLetter('i',130,36,7);
	DrawLetter('b',136,36,7);
	DrawLetter(':',142,36,7);
	DrawCheckbox(149,36);
	DrawLetter('c',124,42,7);
	DrawLetter('o',130,42,7);
	DrawLetter('n',136,42,7);
	DrawLetter(':',142,42,7);
	DrawCheckbox(149,42);
	DrawLetter('k',124,48,7);
	DrawLetter('s',130,48,7);
	DrawLetter('r',136,48,7);
	DrawLetter(':',142,48,7);
	DrawCheckbox(149,48);
	DrawLetter('m',5,54,7);
	DrawLetter('u',11,54,7);
	DrawLetter('l',17,54,7);
	DrawLetter(':',23,54,7);
	DrawBar(29,55,16);
	DrawLetter('v',5,60,7);
	DrawLetter('o',11,60,7);
	DrawLetter('l',17,60,7);
	DrawLetter(':',23,60,7);
	DrawBar(29,61,64);
	DrawLetter('k',124,54,7);
	DrawLetter('s',130,54,7);
	DrawLetter('l',136,54,7);
	DrawLetter(':',142,54,7);
	DrawBar(148,55,4);
	DrawLetter('w',124,60,7);
	DrawLetter('s',130,60,7);
	DrawLetter(':',142,60,7);
	DrawBar(148,61,4);
	Mirror(0,67);
	DrawBox(264,2,268,6,0);
	DrawLetter('2',264,2,15);
	DrawLetter('f',133,69,7);
	DrawLetter('r',139,69,7);
	DrawLetter('e',145,69,7);
	DrawLetter('q',151,69,7);
	DrawLetter(':',157,69,7);
	DrawLetter('o',127,75,7);
	DrawLetter('c',133,75,7);
	DrawLetter('t',139,75,7);
	DrawLetter(':',145,75,7);
	DrawBar(151,76,8);
	DrawLetter('k',164,75,7);
	DrawLetter('e',170,75,7);
	DrawLetter('y',176,75,7);
	DrawLetter(':',182,75,7);
	DrawCheckbox(188,75);
	DrawLetter('f',127,81,7);
	DrawLetter('b',133,81,7);
	DrawLetter(':',139,81,7);
	DrawBar(151,82,8);
	DrawLetter('f',164,81,7);
	DrawLetter('m',170,81,7);
	DrawLetter(':',176,81,7);
	DrawCheckbox(188,81);
	DrawLetter('v',127,87,7);
	DrawLetter('i',133,87,7);
	DrawLetter('b',139,87,7);
	DrawLetter(':',145,87,7);
	DrawBar(151,88,4);
	DrawLetter('r',164,87,7);
	DrawLetter('e',170,87,7);
	DrawLetter('s',176,87,7);
	DrawLetter('e',182,87,7);
	DrawLetter('t',188,87,7);
	DrawLetter('c',127,93,7);
	DrawLetter('s',133,93,7);
	DrawLetter('m',139,93,7);
	DrawLetter(':',145,93,7);
	DrawCheckbox(151,93);
	DrawLetter('s',164,93,7);
	DrawLetter('e',170,93,7);
	DrawLetter('l',176,93,7);
	DrawLetter(':',182,93,7);
	DrawCheckbox(188,93);
	DrawLetter('r',127,99,7);
	DrawLetter('h',133,99,7);
	DrawLetter('y',139,99,7);
	DrawLetter(':',145,99,7);
	DrawCheckbox(151,99);
	DrawLetter('b',164,99,7);
	DrawLetter('a',170,99,7);
	DrawLetter('s',176,99,7);
	DrawLetter(':',182,99,7);
	DrawCheckbox(188,99);
	DrawLetter('s',127,105,7);
	DrawLetter('n',133,105,7);
	DrawLetter('a',139,105,7);
	DrawLetter(':',145,105,7);
	DrawCheckbox(151,105);
	DrawLetter('t',164,105,7);
	DrawLetter('o',170,105,7);
	DrawLetter('m',176,105,7);
	DrawLetter(':',182,105,7);
	DrawCheckbox(188,105);
	DrawLetter('c',127,111,7);
	DrawLetter('y',133,111,7);
	DrawLetter('m',139,111,7);
	DrawLetter(':',145,111,7);
	DrawCheckbox(151,111);
	DrawLetter('h',164,111,7);
	DrawLetter('a',170,111,7);
	DrawLetter('t',176,111,7);
	DrawLetter(':',182,111,7);
	DrawCheckbox(188,111);
	DrawLetter('s',127,117,7);
	DrawLetter('a',133,117,7);
	DrawLetter('v',139,117,7);
	DrawLetter('e',145,117,7);
	DrawLetter('l',170,117,7);
	DrawLetter('o',176,117,7);
	DrawLetter('a',182,117,7);
	DrawLetter('d',188,117,7);
	DrawLetter('f',133,123,7);
	DrawLetter('i',139,123,7);
	DrawLetter('l',145,123,7);
	DrawLetter('e',151,123,7);
	DrawLetter('n',157,123,7);
	DrawLetter('a',163,123,7);
	DrawLetter('m',169,123,7);
	DrawLetter('e',175,123,7);
	DrawLetter(':',181,123,7);
	VertLine(123,68,135,9);
	VertLine(124,68,135,9);
	VertLine(195,68,135,9);
	VertLine(196,68,135,9);
	HorizLine(135,125,194,9);
	DrawLetter('e',0,183,15);
	DrawLetter('d',6,183,15);
	DrawLetter('i',12,183,15);
	DrawLetter('t',18,183,15);
	DrawLetter(':',24,183,15);
	DrawLetter('1',6,195,15);
	DrawLetter('2',18,195,15);
	DrawLetter('m',291,183,15);
	DrawLetter('o',297,183,15);
	DrawLetter('v',303,183,15);
	DrawLetter('e',309,183,15);
	DrawLetter(':',315,183,15);
	DrawLetter('8',303,189,15);
	DrawLetter('4',297,195,15);
	DrawLetter('5',303,195,15);
	DrawLetter('6',309,195,15);
	_asm
		{
		push ds
		les di,dataBuffer
		lds si,doubleBuffer
		mov cx,32000
		cld
		rep movsw
		pop ds
		}
	}

void ResetChip(void)
	{
	att1=0;
	dec1=0;
	sus1=0;
	rel1=0;
	mul1=0;
	vol1=0;
	am1=0;
	vib1=0;
	con1=0;
	ksr1=0;
	ksl1=0;
	ws1=0;
	att2=0;
	dec2=0;
	sus2=0;
	rel2=0;
	mul2=0;
	vol2=0;
	am2=0;
	vib2=0;
	con2=0;
	ksr2=0;
	ksl2=0;
	ws2=0;
	oct=3;
	key=0;
	fb=0;
	fm=0;
	vib=0;
	csm=0;
	sel=0;
	rhy=0;
	bas=0;
	sna=0;
	tom=0;
	cym=0;
	hat=0;
	freq=512;
	filename="default";
	WriteAllRegs();
	}

void DrawSlider(int x,int y)
	{
	unsigned int writePos=x+(y*320);
	doubleBuffer[writePos]=15;
	doubleBuffer[writePos+1]=15;
	doubleBuffer[writePos+2]=15;
	doubleBuffer[writePos+321]=15;
	doubleBuffer[writePos+641]=15;
	doubleBuffer[writePos+961]=15;
	doubleBuffer[writePos+1280]=15;
	doubleBuffer[writePos+1281]=15;
	doubleBuffer[writePos+1282]=15;
	}
	
void DrawCheck(int x,int y)
	{
	unsigned int writePos=x+(y*320);
	doubleBuffer[writePos]=15;
	doubleBuffer[writePos+2]=15;
	doubleBuffer[writePos+321]=15;
	doubleBuffer[writePos+640]=15;
	doubleBuffer[writePos+642]=15;
	}

void DrawLine(int x1,int y1,int x2,int y2,char color)
	{
	unsigned int errorTerm,yUnit,yDiff,xUnit,xDiff;
	_asm
		{
		les di,doubleBuffer
		mov ax,y1
		mov dx,320
		mul dx
		add ax,x1
		mov bx,ax
		initLine:
			mov dl,color
			mov dh,0
			mov errorTerm,0
			mov ax,y2
			sub ax,y1
			jns yPos
			mov yUnit,-320
			neg ax
			mov yDiff,ax
			jmp next
		yPos:
			mov yUnit,320
			mov yDiff,ax
		next:
			mov ax,x2
			sub ax,x1
			jns xPos
			mov xUnit,-1
			neg ax
			mov xDiff,ax
			jmp next2
		xPos:
			mov xUnit,1
			mov xDiff,ax
		next2:
			cmp ax,yDiff
			jc yLine
		xLine:
			mov cx,xDiff
			inc cx
		xLine1:
			mov es:[bx],dl
			add bx,xUnit
			mov ax,errorTerm
			add ax,yDiff
			mov errorTerm,ax
			sub ax,xDiff
			jc xLine2
			mov errorTerm,ax
			add bx,yUnit
		xLine2:
			loop xLine1
			jmp lineDone
		yLine:
			mov cx,yDiff
			inc cx
		yLine1:
			mov es:[bx],dl
			add bx,yUnit
			mov ax,errorTerm
			add ax,xDiff
			mov errorTerm,ax
			sub ax,yDiff
			jc yLine2
			mov errorTerm,ax
			add bx,xUnit
		yLine2:
			loop yLine1
		lineDone:
		}
	return;
	}

void ChangeColor(int x1,int y1,char dx,char dy)
	{
	char c1,c2;
	for(c1=0;c1<dy;c1++)
		for(c2=0;c2<dx;c2++)        
			if(doubleBuffer[x1+c2+((y1+c1)*320)]==7)
				doubleBuffer[x1+c2+((y1+c1)*320)]=12;
	}

void DrawScreen(void)
	{
	_asm
		{
		push ds
		les di,doubleBuffer
		lds si,dataBuffer
		mov cx,32000
		cld
		rep movsw
		pop ds
		}
	
	DrawSlider(29+att1,30);
	DrawSlider(29+dec1,36);
	DrawSlider(29+sus1,42);
	DrawSlider(29+rel1,48);
	DrawSlider(29+mul1,54);
	DrawSlider(29+vol1,60);
	if(am1)
		DrawCheck(150,31);
	if(vib1)
		DrawCheck(150,37);
	if(con1)
		DrawCheck(150,43);
	if(ksr1)
		DrawCheck(150,49);
	DrawSlider(148+ksl1,54);
	DrawSlider(148+ws1,60);
	DrawSlider(189+att2,30);
	DrawSlider(189+dec2,36);
	DrawSlider(189+sus2,42);
	DrawSlider(189+rel2,48);
	DrawSlider(189+mul2,54);
	DrawSlider(189+vol2,60);
	if(am2)
		DrawCheck(310,31);
	if(vib2)
		DrawCheck(310,37);
	if(con2)
		DrawCheck(310,43);
	if(ksr2)
		DrawCheck(310,49);
	DrawSlider(308+ksl2,54);
	DrawSlider(308+ws2,60);
	unsigned int freq2=freq;
	unsigned int value=freq2/1000;
	DrawLetter('0'+(unsigned char)value,163,69,15);
	freq2-=(value*1000);
	value=freq2/100;
	DrawLetter('0'+(unsigned char)value,169,69,15);
	freq2-=(value*100);
	value=freq2/10;
	DrawLetter('0'+(unsigned char)value,175,69,15);
	freq2-=(value*10);
	DrawLetter('0'+(unsigned char)freq2,181,69,15);
	DrawSlider(151+oct,75);
	DrawSlider(151+fb,81);
	DrawSlider(151+vib,87);
	if(csm)
		DrawCheck(152,94);
	if(rhy)
		DrawCheck(152,100);
	if(bas)
		DrawCheck(152,106);
	if(sna)
		DrawCheck(152,112);
	if(key)
		DrawCheck(189,76);
	if(fm)
		DrawCheck(189,82);
	if(sel)
		DrawCheck(189,94);
	if(tom)
		DrawCheck(189,100);
	if(cym)
		DrawCheck(189,106);
	if(hat)
		DrawCheck(189,112);
	if(filename)
		for(value=0;value<strlen(filename);value++)
			DrawLetter(filename[value],135+(value*6),129,15);
	int x1,y1,x2,y2;
	x2=70-att1;
	y2=57-((vol1*3)/4);
	if(y2<11)
		y2=11;
	DrawLine(54,57,x2,y2,15);
	x1=x2+(16-dec1);
	y1=57-(int)((float)(sus1*3)*((float)vol1/64));
	if(y2<11)
		y2=11;
	DrawLine(x1,y1,x2,y2,15);
	x2=99+rel1;
	y2=y1;
	DrawLine(x1,y1,x2,y2,15);
	DrawLine(116,57,x2,y2,15);
	x2=230-att2;
	y2=57-((vol2*3)/4);
	if(y2<11)
		y2=11;
	DrawLine(214,57,x2,y2,15);
	x1=x2+(16-dec2);
	y1=57-(int)((float)(sus2*3)*((float)vol2/64));
	if(y2<11)
		y2=11;
	DrawLine(x1,y1,x2,y2,15);
	x2=259+rel2;
	y2=y1;
	DrawLine(x1,y1,x2,y2,15);
	DrawLine(276,57,x2,y2,15);
	
	if(yLoc<6)
		switch(xLoc)
			{
			case(0):
				ChangeColor(5,30+(yLoc*6),23,5);
				break;
			case(1):
				ChangeColor(124,30+(yLoc*6),23,5);
				break;
			case(2):
				ChangeColor(165,30+(yLoc*6),23,5);
				break;
			case(3):
				ChangeColor(284,30+(yLoc*6),23,5);
				break;
			}
	if(yLoc==6)
		ChangeColor(133,69,29,5);
	if(yLoc>6&&yLoc<15)
		switch(xLoc)
			{
			case(0):
			case(1):
				ChangeColor(127,75+((yLoc-7)*6),23,5);
				break;
			case(2):
			case(3):
				ChangeColor(164,75+((yLoc-7)*6),23,5);
				if(yLoc==9)
					ChangeColor(188,87,23,5);
				if(yLoc==14)
					ChangeColor(188,117,23,5);
				break;
			}
	if(yLoc==15)
		ChangeColor(133,123,53,5);

	_asm
		{
		push ds
		les di,videoBuffer
		lds si,doubleBuffer
		mov cx,32000
		cld
		rep movsw
		pop ds
		}
	}

void UserInput(void)
	{
	char inc;
	switch(getch())
		{
		case('8'):
			yLoc--;
			if(yLoc<0)
				yLoc=15;
			break;
		case('5'):
			yLoc++;
			if(yLoc>15)
				yLoc=0;
			break;
		case('4'):
			if(yLoc<6)
				{
				xLoc--;
				if(xLoc<0)
					xLoc=3;
				}
			if(yLoc>6&&yLoc<15)
				{
				xLoc-=2;
				if(xLoc<0)
					xLoc+=4;
				}
			break;
		case('6'):
			if(yLoc<6)
				{
				xLoc++;
				if(xLoc>3)
					xLoc=0;
				}
			if(yLoc>6&&yLoc<15)
				{
				xLoc+=2;
				if(xLoc>3)
					xLoc-=4;
				}
			break;
		case(27):
			done=1;
			SaveFile();
			break;
		case('1'):
			inc=-1;
			goto skip1;
		case('2'):
			inc=1;
			skip1:;
			if(yLoc==0)
				{
				if(xLoc==0)
					{
					att1+=inc;
					if(att1<0)
						att1=15;
					if(att1>15)
						att1=0;
					WriteReg(0x60,dec1+(att1<<4));
					}
				else if(xLoc==1)
					{
					am1+=inc;
					if(am1<0)
						am1=1;
					if(am1>1)
						am1=0;
					WriteReg(0x20,mul1+(ksr1<<4)+(con1<<5)+(vib1<<6)+(am1<<7));
					}
				else if(xLoc==2)
					{
					att2+=inc;
					if(att2<0)
						att2=15;
					if(att2>15)
						att2=0;
					WriteReg(0x63,dec2+(att2<<4));
					}
				else if(xLoc==3)
					{
					am2+=inc;
					if(am2<0)
						am2=1;
					if(am2>1)
						am2=0;
					WriteReg(0x23,mul2+(ksr2<<4)+(con2<<5)+(vib2<<6)+(am2<<7));
					}
				}
			else if(yLoc==1)
				{
				if(xLoc==0)
					{
					dec1+=inc;
					if(dec1<0)
						dec1=15;
					if(dec1>15)
						dec1=0;
					WriteReg(0x60,dec1+(att1<<4));
					}
				else if(xLoc==1)
					{
					vib1+=inc;
					if(vib1<0)
						vib1=1;
					if(vib1>1)
						vib1=0;
					WriteReg(0x20,mul1+(ksr1<<4)+(con1<<5)+(vib1<<6)+(am1<<7));
					}
				else if(xLoc==2)
					{
					dec2+=inc;
					if(dec2<0)
						dec2=15;
					if(dec2>15)
						dec2=0;
					WriteReg(0x63,dec2+(att2<<4));
					}
				else if(xLoc==3)
					{
					vib2+=inc;
					if(vib2<0)
						vib2=1;
					if(vib2>1)
						vib2=0;
					WriteReg(0x23,mul2+(ksr2<<4)+(con2<<5)+(vib2<<6)+(am2<<7));
					}
				}
			else if(yLoc==2)
				{
				if(xLoc==0)
					{
					sus1+=inc;
					if(sus1<0)
						sus1=15;
					if(sus1>15)
						sus1=0;
					WriteReg(0x80,rel1+(sus1<<4));
					}
				else if(xLoc==1)
					{
					con1+=inc;
					if(con1<0)
						con1=1;
					if(con1>1)
						con1=0;
					WriteReg(0x20,mul1+(ksr1<<4)+(con1<<5)+(vib1<<6)+(am1<<7));
					}
				else if(xLoc==2)
					{
					sus2+=inc;
					if(sus2<0)
						sus2=15;
					if(sus2>15)
						sus2=0;
					WriteReg(0x83,rel2+(sus2<<4));
					}
				else if(xLoc==3)
					{
					con2+=inc;
					if(con2<0)
						con2=1;
					if(con2>1)
						con2=0;
					WriteReg(0x23,mul2+(ksr2<<4)+(con2<<5)+(vib2<<6)+(am2<<7));
					}
				}
			else if(yLoc==3)
				{
				if(xLoc==0)
					{
					rel1+=inc;
					if(rel1<0)
						rel1=15;
					if(rel1>15)
						rel1=0;
					WriteReg(0x80,rel1+(sus1<<4));
					}
				else if(xLoc==1)
					{
					ksr1+=inc;
					if(ksr1<0)
						ksr1=1;
					if(ksr1>1)
						ksr1=0;
					WriteReg(0x20,mul1+(ksr1<<4)+(con1<<5)+(vib1<<6)+(am1<<7));
					}
				else if(xLoc==2)
					{
					rel2+=inc;
					if(rel2<0)
						rel2=15;
					if(rel2>15)
						rel2=0;
					WriteReg(0x83,rel2+(sus2<<4));
					}
				else if(xLoc==3)
					{
					ksr2+=inc;
					if(ksr2<0)
						ksr2=1;
					if(ksr2>1)
						ksr2=0;
					WriteReg(0x23,mul2+(ksr2<<4)+(con2<<5)+(vib2<<6)+(am2<<7));
					}
				}
			else if(yLoc==4)
				{
				if(xLoc==0)
					{
					mul1+=inc;
					if(mul1<0)
						mul1=15;
					if(mul1>15)
						mul1=0;
					WriteReg(0x20,mul1+(ksr1<<4)+(con1<<5)+(vib1<<6)+(am1<<7));
					}
				else if(xLoc==1)
					{
					ksl1+=inc;
					if(ksl1<0)
						ksl1=3;
					if(ksl1>3)
						ksl1=0;
					WriteReg(0x40,(63-vol1)+(ksl1<<6));
					}
				else if(xLoc==2)
					{
					mul2+=inc;
					if(mul2<0)
						mul2=15;
					if(mul2>15)
						mul2=0;
					WriteReg(0x23,mul2+(ksr2<<4)+(con2<<5)+(vib2<<6)+(am2<<7));
					}
				else if(xLoc==3)
					{
					ksl2+=inc;
					if(ksl2<0)
						ksl2=3;
					if(ksl2>3)
						ksl2=0;
					WriteReg(0x43,(63-vol2)+(ksl2<<6));
					}
				}
			else if(yLoc==5)
				{
				if(xLoc==0)
					{
					vol1+=inc;
					if(vol1<0)
						vol1=63;
					if(vol1>63)
						vol1=0;
					WriteReg(0x40,(63-vol1)+(ksl1<<6));
					}
				else if(xLoc==1)
					{
					ws1+=inc;
					if(ws1<0)
						ws1=3;
					if(ws1>3)
						ws1=0;
					WriteReg(0xE0,ws1);
					}
				else if(xLoc==2)
					{
					vol2+=inc;
					if(vol2<0)
						vol2=63;
					if(vol2>63)
						vol2=0;
					WriteReg(0x43,(63-vol2)+(ksl2<<6));
					}
				else if(xLoc==3)
					{
					ws2+=inc;
					if(ws2<0)
						ws2=3;
					if(ws2>3)
						ws2=0;
					WriteReg(0xE3,ws2);
					}
				}
			else if(yLoc==6)
				{
				freq=ChangeFreq();
				WriteReg(0xA0,(char)(freq&0xFF));
				WriteReg(0xB0,((char)(freq>>8))+(oct<<2)+(key<<5));
				}
			else if(yLoc==7)
				{
				if(xLoc==0||xLoc==1)
					{
					oct+=inc;
					if(oct<0)
						oct=7;
					if(oct>7)
						oct=0;
					WriteReg(0xB0,((char)(freq>>8))+(oct<<2)+(key<<5));
					}
				else if(xLoc==2||xLoc==3)
					{
					key+=inc;
					if(key<0)
						key=1;
					if(key>1)
						key=0;
					WriteReg(0xB0,((char)(freq>>8))+(oct<<2)+(key<<5));
					}
				}
			else if(yLoc==8)
				{
				if(xLoc==0||xLoc==1)
					{
					fb+=inc;
					if(fb<0)
						fb=7;
					if(fb>7)
						fb=0;
					WriteReg(0xC0,fm+(fb<<1));
					}
				else if(xLoc==2||xLoc==3)
					{
					fm+=inc;
					if(fm<0)
						fm=1;
					if(fm>1)
						fm=0;
					WriteReg(0xC0,fm+(fb<<1));
					}
				}
			else if(yLoc==9)
				{
				if(xLoc==0||xLoc==1)
					{
					vib+=inc;
					if(vib<0)
						vib=3;
					if(vib>3)
						vib=0;
					WriteReg(0xBD,hat+(cym<<1)+(tom<<2)+(sna<<3)+(bas<<4)+(rhy<<5)+(vib<<6));
					}
				else if(xLoc==2||xLoc==3)
					ResetChip();
				}
			else if(yLoc==10)
				{
				if(xLoc==0||xLoc==1)
					{
					csm+=inc;
					if(csm<0)
						csm=1;
					if(csm>1)
						csm=0;
					WriteReg(0x08,(csm<<7)+(sel<<6));
					}
				else if(xLoc==2||xLoc==3)
					{
					sel+=inc;
					if(sel<0)
						sel=1;
					if(sel>1)
						sel=0;
					WriteReg(0x08,(csm<<7)+(sel<<6));
					}
				}
			else if(yLoc==11)
				{
				if(xLoc==0||xLoc==1)
					{
					rhy+=inc;
					if(rhy<0)
						rhy=1;
					if(rhy>1)
						rhy=0;
					WriteReg(0xBD,hat+(cym<<1)+(tom<<2)+(sna<<3)+(bas<<4)+(rhy<<5)+(vib<<6));
					}
				else if(xLoc==2||xLoc==3)
					{
					tom+=inc;
					if(tom<0)
						tom=1;
					if(tom>1)
						tom=0;
					WriteReg(0xBD,hat+(cym<<1)+(tom<<2)+(sna<<3)+(bas<<4)+(rhy<<5)+(vib<<6));
					}
				}
			else if(yLoc==12)
				{
				if(xLoc==0||xLoc==1)
					{
					bas+=inc;
					if(bas<0)
						bas=1;
					if(bas>1)
						bas=0;
					WriteReg(0xBD,hat+(cym<<1)+(tom<<2)+(sna<<3)+(bas<<4)+(rhy<<5)+(vib<<6));
					}
				else if(xLoc==2||xLoc==3)
					{
					cym+=inc;
					if(cym<0)
						cym=1;
					if(cym>1)
						cym=0;
					WriteReg(0xBD,hat+(cym<<1)+(tom<<2)+(sna<<3)+(bas<<4)+(rhy<<5)+(vib<<6));
					}
				}
			else if(yLoc==13)
				{
				if(xLoc==0||xLoc==1)
					{
					sna+=inc;
					if(sna<0)
						sna=1;
					if(sna>1)
						sna=0;
					WriteReg(0xBD,hat+(cym<<1)+(tom<<2)+(sna<<3)+(bas<<4)+(rhy<<5)+(vib<<6));
					}
				else if(xLoc==2||xLoc==3)
					{
					hat+=inc;
					if(hat<0)
						hat=1;
					if(hat>1)
						hat=0;
					WriteReg(0xBD,hat+(cym<<1)+(tom<<2)+(sna<<3)+(bas<<4)+(rhy<<5)+(vib<<6));
					}
				}
			else if(yLoc==14)
				{
				if(xLoc==0||xLoc==1)
					SaveFile();
				else if(xLoc==2||xLoc==3)
					LoadFile();
				}
			else if(yLoc==15)
				filename=ChangeFilename();
			break;
		case('3'):
			key++;
			if(key>1)
				key=0;
			WriteReg(0xB0,((char)(freq>>8))+(oct<<2)+(key<<5));
			break;
		case('0'):
			Randomize();
			break;
		case('s'):
			PlayKey(346);
			break;
		case('e'):
			PlayKey(365);
			break;
		case('d'):
			PlayKey(389);
			break;
		case('r'):
			PlayKey(410);
			break;
		case('f'):
			PlayKey(435);
			break;
		case('g'):
			PlayKey(461);
			break;
		case('y'):
			PlayKey(490);
			break;
		case('h'):
			PlayKey(519);
			break;
		case('u'):
			PlayKey(548);
			break;
		case('j'):
			PlayKey(585);
			break;
		case('i'):
			PlayKey(612);
			break;
		case('k'):
			PlayKey(648);
			break;
		case('l'):
			if(oct<7)
				{
				oct1=1;
				PlayKey(346);
				oct1=0;
				}
			break;
		}
	}

int ChangeFreq(void)
	{
	unsigned int freq2=0,freq3;
	char keyPress;
	notYet1:;
		freq3=freq2;
		DrawBox(163,69,185,73,0);
		unsigned int value=freq3/1000;
		DrawLetter('0'+(unsigned char)value,163,69,12);
		freq3-=(value*1000);
		value=freq3/100;
		DrawLetter('0'+(unsigned char)value,169,69,12);
		freq3-=(value*100);
		value=freq3/10;
		DrawLetter('0'+(unsigned char)value,175,69,12);
		freq3-=(value*10);
		DrawLetter('0'+(unsigned char)freq3,181,69,12);
		_asm
			{
			push ds
			les di,videoBuffer
			lds si,doubleBuffer
			mov cx,32000
			cld
			rep movsw
			pop ds
			}
		keyPress=getch();
		if(keyPress==27)
			return freq;
		if(keyPress==8)
			freq2/=10;
		if(keyPress>='0'&&keyPress<='9')
			freq2=(freq2*10)+(keyPress-'0');
		if(keyPress==13||freq2>=1000)
			{
			if(freq2<1024)
				return freq2;
			else 
				freq2=0;
			}
	goto notYet1;
	}
	
char* ChangeFilename(void)
	{
	char *filename2;
	strcpy(filename2,filename);
	char keyPress,value,len;
	notYet2:;
		DrawBox(135,129,182,134,0);
		if(strlen(filename2)>0)
			for(value=0;value<strlen(filename2);value++)
				DrawLetter(filename2[value],135+(value*6),129,12);
		_asm
			{
			push ds
			les di,videoBuffer
			lds si,doubleBuffer
			mov cx,32000
			cld
			rep movsw
			pop ds
			}
		keyPress=getch();
		if(keyPress==27)
			return filename;
		if(keyPress==8)
			if(strlen(filename2)>0)
				{
				len=strlen(filename2);
				filename2[len-1]=filename2[len];
				filename2[len]=filename2[len+1];
				}
		if(keyPress>='a'&&keyPress<='z'&&strlen(filename2)<8)
			{
			len=strlen(filename2);
			filename2[len+2]=filename2[len+1];
			filename2[len+1]=filename2[len];
			filename2[len]=keyPress;
			}
		if(keyPress>='0'&&keyPress<='9'&&strlen(filename2)<8)
			{
			len=strlen(filename2);
			filename2[len+2]=filename2[len+1];
			filename2[len+1]=filename2[len];
			filename2[len]=keyPress;
			}
		if(keyPress==13||strlen(filename2)==8)
			if(strlen(filename2)>0)
				return filename2;
			else 
				return filename;
	goto notYet2;
	}

void SaveFile(void)
	{
	char *filename2;
	strcpy(filename2,filename);
	char *filename3;
	filename3=".ins";
	strcat(filename2,filename3);
	FILE *fp;
	fp=fopen(filename2,"wb");
	putc(att1,fp);
	putc(dec1,fp);
	putc(sus1,fp);
	putc(rel1,fp);
	putc(mul1,fp);
	putc(vol1,fp);
	putc(am1,fp);
	putc(vib1,fp);
	putc(con1,fp);
	putc(ksr1,fp);
	putc(ksl1,fp);
	putc(ws1,fp);
	putc(att2,fp);
	putc(dec2,fp);
	putc(sus2,fp);
	putc(rel2,fp);
	putc(mul2,fp);
	putc(vol2,fp);
	putc(am2,fp);
	putc(vib2,fp);
	putc(con2,fp);
	putc(ksr2,fp);
	putc(ksl2,fp);
	putc(ws2,fp);
	putc(oct,fp);
	putc(key,fp);
	putc(fb,fp);
	putc(fm,fp);
	putc(vib,fp);
	putc(csm,fp);
	putc(sel,fp);
	putc(rhy,fp);
	putc(bas,fp);
	putc(sna,fp);
	putc(tom,fp);
	putc(cym,fp);
	putc(hat,fp);
	putc((char)(freq&0xFF),fp);
	putc((char)(freq>>8),fp);
	fclose(fp);
	}

void LoadFile(void)
	{
	char *filename2;
	strcpy(filename2,filename);
	char *filename3;
	filename3=".ins";
	strcat(filename2,filename3);
	char freq1;
	FILE *fp;
	if(!(fp=fopen(filename2,"rb")))
		return;
	att1=getc(fp);
	dec1=getc(fp);
	sus1=getc(fp);
	rel1=getc(fp);
	mul1=getc(fp);
	vol1=getc(fp);
	am1=getc(fp);
	vib1=getc(fp);
	con1=getc(fp);
	ksr1=getc(fp);
	ksl1=getc(fp);
	ws1=getc(fp);
	att2=getc(fp);
	dec2=getc(fp);
	sus2=getc(fp);
	rel2=getc(fp);
	mul2=getc(fp);
	vol2=getc(fp);
	am2=getc(fp);
	vib2=getc(fp);
	con2=getc(fp);
	ksr2=getc(fp);
	ksl2=getc(fp);
	ws2=getc(fp);
	oct=getc(fp);
	key=getc(fp);
	fb=getc(fp);
	fm=getc(fp);
	vib=getc(fp);
	csm=getc(fp);
	sel=getc(fp);
	rhy=getc(fp);
	bas=getc(fp);
	sna=getc(fp);
	tom=getc(fp);
	cym=getc(fp);
	hat=getc(fp);
	freq1=getc(fp);
	freq=freq1;
	freq1=getc(fp);
	freq+=((int)freq1)<<8;
	fclose(fp);
	WriteAllRegs();
	}

void WriteAllRegs(void)
	{
	WriteReg(0x08,(csm<<7)+(sel<<6));
	WriteReg(0x20,mul1+(ksr1<<4)+(con1<<5)+(vib1<<6)+(am1<<7));
	WriteReg(0x23,mul2+(ksr2<<4)+(con2<<5)+(vib2<<6)+(am2<<7));
	WriteReg(0x40,(63-vol1)+(ksl1<<6));
	WriteReg(0x43,(63-vol2)+(ksl2<<6));
	WriteReg(0x60,dec1+(att1<<4));
	WriteReg(0x63,dec2+(att2<<4));
	WriteReg(0x80,rel1+(sus1<<4));
	WriteReg(0x83,rel2+(sus2<<4));
	WriteReg(0xA0,(char)(freq&0xFF));
	WriteReg(0xB0,((char)(freq>>8))+(oct<<2)+(key<<5));
	WriteReg(0xBD,hat+(cym<<1)+(tom<<2)+(sna<<3)+(bas<<4)+(rhy<<5)+(vib<<6));
	WriteReg(0xC0,fm+(fb<<1));
	WriteReg(0xE0,ws1);
	WriteReg(0xE3,ws2);
	}

void WriteReg(char index,char data)
	{
	char count;
	outp(0x388,index);
	count=7;
	while(count--)
		inp(0x388);
	outp(0x389,data);
	count=36;
	while(count--)
		inp(0x388);
	}

void Randomize(void)
	{
	att1=rand()%16;
	dec1=rand()%16;
	sus1=rand()%16;
	rel1=rand()%16;
	mul1=rand()%16;
	vol1=63;
	am1=rand()%2;
	vib1=rand()%2;
	con1=rand()%2;
	ksr1=rand()%2;
	ksl1=rand()%4;
	ws1=rand()%4;
	att2=rand()%16;
	dec2=rand()%16;
	sus2=rand()%16;
	rel2=rand()%16;
	mul2=rand()%16;
	vol2=63;
	am2=rand()%2;
	vib2=rand()%2;
	con2=rand()%2;
	ksr2=rand()%2;
	ksl2=rand()%4;
	ws2=rand()%4;
	oct=rand()%8;
	key=0;
	fb=rand()%8;
	fm=rand()%2;
	vib=rand()%4;
	csm=rand()%2;
	sel=rand()%2;
	rhy=0;
	bas=0;
	sna=0;
	tom=0;
	cym=0;
	hat=0;
	WriteAllRegs();
	}

void PlayKey(int freq2)
	{
	WriteReg(0xB0,0);
	WriteReg(0xA0,(char)(freq2&0xFF));
	if(oct1)
		WriteReg(0xB0,((char)(freq2>>8))+48);
	else
		WriteReg(0xB0,((char)(freq2>>8))+44);
	}

void main(void)
	{
	Initialize();
	CreateScreen();
	DrawScreen();
	while(!done)
		{
		UserInput();
		DrawScreen();
		}
	Restore();
	return;
	}
