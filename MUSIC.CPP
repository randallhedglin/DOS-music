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
unsigned char far *dataBuffer1=NULL;
unsigned char far *dataBuffer2=NULL;
unsigned char far *dataBuffer3=NULL;

char done;
struct
	{
	char op1,op2;
	} chn[9];
int key[14]={327,346,365,389,410,435,461,490,519,548,585,612,648,690};
int ofs[7]={1,3,5,6,8,10,12};
const cf=0,cn=1,cs=2,df=2,dn=3,ds=4,ef=4,en=5,es=6,ff=5,fn=6,fs=7,gf=7,gn=8,gs=9,af=9,an=10,as=11,bf=11,bn=12,bs=13;
const whole=6,half=5,quarter=4,eighth=3,sixteenth=2,thirtysecond=1,sixtyfourth=0;
const notdotted=0,dotted=1,over=1,thru=2,under=3;
const natural=0,sharp=1,flat=2;
const up=0,down=1;
const noline=0;
const treb=0,bass=1,treb8va=2,bass8vb=3;
const rest=23,blank=24;
const yes=0,rec=1;
unsigned int viewPos=0;
unsigned int ch1=0;
int xCen[20];
char numX;
unsigned int userY=0;
char *filename="default1";
char *instr0="empty111";
char *instr1="empty222";
char *instr2="empty333";
char *instr3="empty444";
char *instr4="empty555";
char *instr5="empty666";
char *instr6="empty777";
char *instr7="empty888";
char *instr8="empty999";
int tempo;
char res,num;

void Initialize(void);
void Restore(void);
void DrawLetter(char letter,int x,int y,char color);
long TranslateBits(char *bits);
void DrawBox(int x1,int y1,int x2,int y2,char color);
void CreateScreen(void);
void ResetChip(void);
void ChangeColor(int x1,int y1,char dx,char dy);
void DrawScreen(void);
void UserInput(void);
void SaveFile(void);
void LoadFile(void);
void WriteReg(char index,char data);
void VertLine(int x,int y1,int y2,char color);
void HorizLine(int y,int x1,int x2,char color);
void LoadInsFile(char *filename2,unsigned int channel);
void PlayNote(unsigned int freq,unsigned char octave,unsigned int channel);
void DrawNote(int x,int y,char type,char dot,char sfn,char updn,char line);
void DrawRest(int x,int y,char type);
void DrawClef(int x,int y,char type);
void DrawMusic(unsigned int channel1,unsigned int channel2,unsigned int channel3,int musicPos);
void DrawNotes(char yPos,int xPos,char note1,char sfn1,char length1,char dot1);
void WriteNote(char clef,char note,char sfn,char length,char dot,unsigned int pos,unsigned int channel);
void NewSong(void);
char GetClef(unsigned int pos,unsigned int channel);
char GetNote(unsigned int pos,unsigned int channel);
char GetSFN(unsigned int pos,unsigned int channel);
char GetLength(unsigned int pos,unsigned int channel);
char GetDot(unsigned int pos,unsigned int channel);
int FindLast(unsigned int pos,unsigned int channel);
void DrawString(char *str1,int x,int y,char color);
void DrawNum(int num,int x,int y,char color);
void HighlightName(char pos);
char *ChangeFilename(void);
void ChangeInsname(unsigned int channel);
void PlaySong(char play);
void EncodeMusic(void);
void PlayMusic(void);
void SetClock(long hz);
void WaitClock(void);
void WriteClef(unsigned int pos,unsigned int channel,char clef);
void SaveMusic(void);

void Initialize(void)
	{
	_setvideomode(0x13);
	doubleBuffer=(unsigned char far *)_fmalloc(64000);
	_FP_OFF(doubleBuffer)=0;
	dataBuffer1=(unsigned char far *)_fmalloc(64000);
	_FP_OFF(dataBuffer1)=0;
	dataBuffer2=(unsigned char far *)_fmalloc(64000);
	_FP_OFF(dataBuffer2)=0;
	dataBuffer3=(unsigned char far *)_fmalloc(64000);
	_FP_OFF(dataBuffer3)=0;
	_asm                
		{
		mov cx,32000
		mov ax,0
		les di,videoBuffer
		cld
		rep stosw
		mov cx,32000
		les di,doubleBuffer
		rep stosw
		mov cx,32000
		les di,dataBuffer1
		rep stosw
		mov cx,32000
		les di,dataBuffer2
		rep stosw
		mov cx,32000
		les di,dataBuffer3
		rep stosw
		}
	done=0;
	chn[0].op1=0x00; chn[0].op2=0x03;
	chn[1].op1=0x01; chn[1].op2=0x04;
	chn[2].op1=0x02; chn[2].op2=0x05;
	chn[3].op1=0x08; chn[3].op2=0x0B;
	chn[4].op1=0x09; chn[4].op2=0x0C;
	chn[5].op1=0x0A; chn[5].op2=0x0D;
	chn[6].op1=0x10; chn[6].op2=0x13;
	chn[7].op1=0x11; chn[7].op2=0x14;
	chn[8].op1=0x12; chn[8].op2=0x15;
	NewSong();
	}        

void Restore(void)
	{
	ResetChip();
	_ffree(doubleBuffer);
	_ffree(dataBuffer1);
	_ffree(dataBuffer2);
	_ffree(dataBuffer3);
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
		case('+'):
			data=TranslateBits("0101011111010101111101010");
			break;
		case('-'):
			data=TranslateBits("0100001000011100100101110");
			break;
		case('^'):
			data=0;
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

void DrawBox(int x1,int y1,int x2,int y2,char color)
	{
	for(;y1<=y2;y1++)
		HorizLine(y1,x1,x2,color);
	}

void ResetChip(void)
	{
	int count;        
	for(count=0;count<256;count++)
		WriteReg(count,0);
	}

void ChangeColor(int x1,int y1,char dx,char dy)
	{
	char c1,c2;
	for(c1=0;c1<dy;c1++)
		for(c2=0;c2<dx;c2++)        
			if(doubleBuffer[x1+c2+((y1+c1)*320)]==0)
				doubleBuffer[x1+c2+((y1+c1)*320)]=4;
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

void LoadInsFile(char *filename2,unsigned int channel)
	{
	char *filename3="filename.333";
	strcpy(filename3,filename2);
	strcat(filename3,".ins");
	FILE *fp;
	if(!(fp=fopen(filename3,"rb")))
		return;
	int writePos=3+(channel*12);
	dataBuffer3[writePos+2]=(getc(fp)<<4)+getc(fp);
	dataBuffer3[writePos+3]=(getc(fp)<<4)+getc(fp);
	char mul=getc(fp);
	char vol=getc(fp);
	dataBuffer3[writePos]=(getc(fp)<<7)+(getc(fp)<<6)+(getc(fp)<<5)+(getc(fp)<<4)+mul;
	dataBuffer3[writePos+1]=(getc(fp)<<6)+(63-vol);
	dataBuffer3[writePos+4]=getc(fp);
	dataBuffer3[writePos+7]=(getc(fp)<<4)+getc(fp);
	dataBuffer3[writePos+8]=(getc(fp)<<4)+getc(fp);
	mul=getc(fp);
	vol=getc(fp);
	dataBuffer3[writePos+5]=(getc(fp)<<7)+(getc(fp)<<6)+(getc(fp)<<5)+(getc(fp)<<4)+mul;
	dataBuffer3[writePos+6]=(getc(fp)<<6)+(63-vol);
	dataBuffer3[writePos+9]=getc(fp);
	char dummy=getc(fp);
	dummy=getc(fp);
	dataBuffer3[writePos+10]=(getc(fp)<<1)+getc(fp);
	dataBuffer3[writePos+11]=getc(fp)<<6;
	fclose(fp);
	}

void PlayNote(unsigned int freq,unsigned char octave,unsigned int channel)
	{
	WriteReg(0xB0+channel,0);
	WriteReg(0xA0+channel,(char)(freq&0xFF));
	WriteReg(0xB0+channel,(freq>>8)+(octave<<2)+32);
	}

void DrawNote(int x,int y,char type,char dot,char sfn,char updn,char line)
	{
	int y1;
	char inc;
	if(line)
		{
		switch(line)
			{
			case(over):
				y1=y-2;
				inc=-4;
				break;
			case(thru):
				y1=y;
				if(updn==up)
					inc=-4;
				else
					inc=4;
				break;
			case(under):
				y1=y+2;
				inc=4;
				break;
			}
		HorizLine(y1,x-3,x+3,7);
		HorizLine(y1+inc,x-3,x+3,7);
		HorizLine(y1+(inc*2),x-3,x+3,7);
		HorizLine(y1+(inc*3),x-3,x+3,7);
		}
	switch(type)
		{
		case(whole): 
		case(half):
			HorizLine(y-2,x-1,x+1,15);
			VertLine(x-2,y-1,y+1,15);
			VertLine(x+2,y-1,y+1,15);
			HorizLine(y+2,x-1,x+1,15);
			break;
		case(quarter):
		case(eighth):
		case(sixteenth):
		case(thirtysecond):
		case(sixtyfourth):
			HorizLine(y-2,x-1,x+1,15);
			HorizLine(y-1,x-2,x+2,15);
			HorizLine(y,x-2,x+2,15);
			HorizLine(y+1,x-2,x+2,15);
			HorizLine(y+2,x-1,x+1,15);
			break;
		}
	if(dot&&type!=sixtyfourth)
		switch(dot)
			{
			case(over):
				doubleBuffer[(x+5)+((y-2)*320)]=15;
				break;
			case(thru):
				doubleBuffer[(x+5)+(y*320)]=15;
				break;
			case(under):
				doubleBuffer[(x+5)+((y+2)*320)]=15;
				break;
			}
	switch(sfn)
		{
		case(natural):
			break;
		case(sharp):
			HorizLine(y-1,x-8,x-4,15);
			HorizLine(y+1,x-8,x-4,15);
			VertLine(x-7,y-2,y+2,15);
			VertLine(x-5,y-2,y+2,15);
			break;
		case(flat):
			HorizLine(y+1,x-7,x-5,15);
			HorizLine(y-1,x-7,x-5,15);
			doubleBuffer[(x-4)+(y*320)]=15;
			VertLine(x-7,y-4,y+1,15);
			break;
		}
	if(type!=whole)
		if(updn==up)
			{
			VertLine(x+2,y-14,y-2,15);
			switch(type)
				{
				case(half):
				case(quarter):
					break;
				case(eighth):
					doubleBuffer[(x+3)+((y-13)*320)]=15;
					doubleBuffer[(x+4)+((y-12)*320)]=15;
					VertLine(x+5,y-11,y-10,15);
					doubleBuffer[(x+4)+((y-9)*320)]=15;
					break;
				case(sixteenth):
					doubleBuffer[(x+3)+((y-13)*320)]=15;
					doubleBuffer[(x+4)+((y-12)*320)]=15;
					doubleBuffer[(x+3)+((y-11)*320)]=15;
					doubleBuffer[(x+4)+((y-10)*320)]=15;
					VertLine(x+5,y-11,y-9,15);
					doubleBuffer[(x+4)+((y-8)*320)]=15;
					break;
				case(thirtysecond):
					doubleBuffer[(x+3)+((y-13)*320)]=15;
					doubleBuffer[(x+4)+((y-12)*320)]=15;
					doubleBuffer[(x+3)+((y-11)*320)]=15;
					doubleBuffer[(x+4)+((y-10)*320)]=15;
					doubleBuffer[(x+3)+((y-9)*320)]=15;
					doubleBuffer[(x+4)+((y-8)*320)]=15;
					VertLine(x+5,y-11,y-7,15);
					doubleBuffer[(x+4)+((y-6)*320)]=15;
					break;
				case(sixtyfourth):
					doubleBuffer[(x+3)+((y-13)*320)]=15;
					doubleBuffer[(x+4)+((y-12)*320)]=15;
					doubleBuffer[(x+3)+((y-11)*320)]=15;
					doubleBuffer[(x+4)+((y-10)*320)]=15;
					doubleBuffer[(x+3)+((y-9)*320)]=15;
					doubleBuffer[(x+4)+((y-8)*320)]=15;
					doubleBuffer[(x+3)+((y-7)*320)]=15;
					doubleBuffer[(x+4)+((y-6)*320)]=15;
					VertLine(x+5,y-11,y-5,15);
					doubleBuffer[(x+4)+((y-4)*320)]=15;
					break;
				}
			}
		else
			{
			VertLine(x-2,y+2,y+14,15);
			switch(type)
				{
				case(half):
				case(quarter):
					break;
				case(eighth):
					doubleBuffer[(x-3)+((y+13)*320)]=15;
					doubleBuffer[(x-4)+((y+12)*320)]=15;
					VertLine(x-5,y+10,y+11,15);
					doubleBuffer[(x-4)+((y+9)*320)]=15;
					break;
				case(sixteenth):
					doubleBuffer[(x-3)+((y+13)*320)]=15;
					doubleBuffer[(x-4)+((y+12)*320)]=15;
					doubleBuffer[(x-3)+((y+11)*320)]=15;
					doubleBuffer[(x-4)+((y+10)*320)]=15;
					VertLine(x-5,y+9,y+11,15);
					doubleBuffer[(x-4)+((y+8)*320)]=15;
					break;
				case(thirtysecond):
					doubleBuffer[(x-3)+((y+13)*320)]=15;
					doubleBuffer[(x-4)+((y+12)*320)]=15;
					doubleBuffer[(x-3)+((y+11)*320)]=15;
					doubleBuffer[(x-4)+((y+10)*320)]=15;
					doubleBuffer[(x-3)+((y+9)*320)]=15;
					doubleBuffer[(x-4)+((y+8)*320)]=15;
					VertLine(x-5,y+7,y+11,15);
					doubleBuffer[(x-4)+((y+6)*320)]=15;
					break;
				case(sixtyfourth):
					doubleBuffer[(x-3)+((y+13)*320)]=15;
					doubleBuffer[(x-4)+((y+12)*320)]=15;
					doubleBuffer[(x-3)+((y+11)*320)]=15;
					doubleBuffer[(x-4)+((y+10)*320)]=15;
					doubleBuffer[(x-3)+((y+9)*320)]=15;
					doubleBuffer[(x-4)+((y+8)*320)]=15;
					doubleBuffer[(x-3)+((y+7)*320)]=15;
					doubleBuffer[(x-4)+((y+6)*320)]=15;
					VertLine(x-5,y+5,y+11,15);
					doubleBuffer[(x-4)+((y+4)*320)]=15;
					break;
				}
			}
	}

void DrawRest(int x,int y,char type)
	{
	switch(res)
		{
		case(whole):
			HorizLine(y-3,x-1,x+1,15);
			break;
		case(half):
			HorizLine(y-1,x-1,x+1,15);
			break;
		case(quarter):
			HorizLine(y+1,x-1,x+1,15);
			doubleBuffer[(x+2)+((y+2)*320)]=15;
			VertLine(x-2,y+2,y+4,15);
			HorizLine(y+5,x-1,x+1,15);
			doubleBuffer[x+((y-7)*320)]=15;
			VertLine(x+1,y-6,y-5,15);
			HorizLine(y-4,x-1,x,15);
			VertLine(x-2,y-3,y-2,15);
			doubleBuffer[(x-1)+((y-1)*320)]=15;
			doubleBuffer[x+(y*320)]=15;
			break;
		case(eighth):
			HorizLine(y-3,x-2,x-1,15);
			HorizLine(y-2,x-3,x,15);
			HorizLine(y-1,x-2,x-1,15);
			VertLine(x+1,y,y+1,15);
			VertLine(x+2,y-2,y-1,15);
			doubleBuffer[x+(y*320)]=15;
			VertLine(x,y+2,y+3,15);
			break;
		case(sixteenth):
			HorizLine(y-3,x-2,x-1,15);
			HorizLine(y-2,x-3,x,15);
			HorizLine(y-1,x-2,x-1,15);
			VertLine(x+1,y,y+1,15);
			VertLine(x+2,y-2,y-1,15);
			doubleBuffer[x+(y*320)]=15;
			HorizLine(y+1,x-4,x-3,15);
			HorizLine(y+2,x-5,x-2,15);
			HorizLine(y+3,x-4,x-3,15);
			VertLine(x-1,y+4,y+5,15);
			VertLine(x,y+2,y+3,15);
			doubleBuffer[(x-2)+((y+4)*320)]=15;
			VertLine(x-2,y+6,y+7,15);
			break;
		case(thirtysecond):
			HorizLine(y-7,x,x+1,15);
			HorizLine(y-6,x-1,x+2,15);
			HorizLine(y-5,x,x+1,15);
			VertLine(x+3,y-4,y-3,15);
			VertLine(x+4,y-6,y-5,15);
			doubleBuffer[(x+2)+((y-4)*320)]=15;
			HorizLine(y-3,x-2,x-1,15);
			HorizLine(y-2,x-3,x,15);
			HorizLine(y-1,x-2,x-1,15);
			VertLine(x+1,y,y+1,15);
			VertLine(x+2,y-2,y-1,15);
			doubleBuffer[x+(y*320)]=15;
			HorizLine(y+1,x-4,x-3,15);
			HorizLine(y+2,x-5,x-2,15);
			HorizLine(y+3,x-4,x-3,15);
			VertLine(x-1,y+4,y+5,15);
			VertLine(x,y+2,y+3,15);
			doubleBuffer[(x-2)+((y+4)*320)]=15;
			VertLine(x-2,y+6,y+7,15);
			break;
		case(sixtyfourth):
			HorizLine(y-7,x,x+1,15);
			HorizLine(y-6,x-1,x+2,15);
			HorizLine(y-5,x,x+1,15);
			VertLine(x+3,y-4,y-3,15);
			VertLine(x+4,y-6,y-5,15);
			doubleBuffer[(x+2)+((y-4)*320)]=15;
			HorizLine(y-3,x-2,x-1,15);
			HorizLine(y-2,x-3,x,15);
			HorizLine(y-1,x-2,x-1,15);
			VertLine(x+1,y,y+1,15);
			VertLine(x+2,y-2,y-1,15);
			doubleBuffer[x+(y*320)]=15;
			HorizLine(y+1,x-4,x-3,15);
			HorizLine(y+2,x-5,x-2,15);
			HorizLine(y+3,x-4,x-3,15);
			VertLine(x-1,y+4,y+5,15);
			VertLine(x,y+2,y+3,15);
			doubleBuffer[(x-2)+((y+4)*320)]=15;
			HorizLine(y+5,x-6,x-5,15);
			HorizLine(y+6,x-7,x-4,15);
			HorizLine(y+7,x-6,x-5,15);
			VertLine(x-3,y+8,y+9,15);
			VertLine(x-2,y+6,y+7,15);
			doubleBuffer[(x-4)+((y+8)*320)]=15;
			VertLine(x-4,y+10,y+11,15);
			break;
		}
	}

void DrawClef(int x,int y,char type)
	{
	switch(type)
		{
		case(treb8va):
			HorizLine(y-9,x+3,x+4,15);
			HorizLine(y-11,x+3,x+4,15);
			HorizLine(y-13,x+3,x+4,15);
			doubleBuffer[(x+2)+((y-12)*320)]=15;
			doubleBuffer[(x+5)+((y-12)*320)]=15;
			doubleBuffer[(x+2)+((y-10)*320)]=15;
			doubleBuffer[(x+5)+((y-10)*320)]=15;
			doubleBuffer[(x+7)+((y-11)*320)]=15;
			doubleBuffer[(x+7)+((y-10)*320)]=15;
			doubleBuffer[(x+8)+((y-9)*320)]=15;
			doubleBuffer[(x+9)+((y-10)*320)]=15;
			doubleBuffer[(x+9)+((y-11)*320)]=15;
			doubleBuffer[(x+11)+((y-10)*320)]=15;
			doubleBuffer[(x+12)+((y-9)*320)]=15;
			doubleBuffer[(x+12)+((y-11)*320)]=15;
			VertLine(x+13,y-11,y-9,15);
		case(treb):
			VertLine(x,y-1,y,15);
			VertLine(x-1,y-3,y-2,15);
			VertLine(x-2,y-5,y-4,15);
			VertLine(x-3,y-7,y-6,15);
			VertLine(x-4,y-9,y-8,15);
			VertLine(x-4,y-9,y-8,15);
			VertLine(x-5,y-12,y-10,15);
			HorizLine(y-13,x-4,x-2,15);
			VertLine(x-1,y-12,y-10,15);
			VertLine(x-2,y-9,y-8,15);
			VertLine(x-4,y-5,y-4,15);
			VertLine(x-5,y-3,y-2,15);
			VertLine(x-6,y-1,y,15);
			VertLine(x-7,y+1,y+4,15);
			VertLine(x-6,y+5,y+6,15);
			doubleBuffer[(x-5)+((y+7)*320)]=15;
			HorizLine(y+8,x-4,x+3,15);
			doubleBuffer[(x+4)+((y+7)*320)]=15;
			VertLine(x+5,y+2,y+6,15);
			doubleBuffer[(x+4)+((y+1)*320)]=15;
			HorizLine(y,x-1,x+3,15);
			doubleBuffer[(x-2)+((y+1)*320)]=15;
			VertLine(x-3,y+2,y+4,15);
			doubleBuffer[(x-2)+((y+5)*320)]=15;
			VertLine(x+1,y+1,y+2,15);
			VertLine(x+2,y+3,y+6,15);
			VertLine(x+1,y+7,y+8,15);
			VertLine(x,y+9,y+10,15);
			doubleBuffer[(x-1)+((y+11)*320)]=15;
			HorizLine(y+12,x-5,x-2,15);
			HorizLine(y+11,x-6,x-3,15);
			HorizLine(y+10,x-5,x-4,15);
			break;
		case(bass8vb):
			HorizLine(y+9,x-5,x-4,15);
			HorizLine(y+11,x-5,x-4,15);
			HorizLine(y+13,x-5,x-4,15);
			doubleBuffer[(x-6)+((y+12)*320)]=15;
			doubleBuffer[(x-3)+((y+12)*320)]=15;
			doubleBuffer[(x-6)+((y+10)*320)]=15;
			doubleBuffer[(x-3)+((y+10)*320)]=15;
			doubleBuffer[(x-1)+((y+11)*320)]=15;
			doubleBuffer[(x-1)+((y+12)*320)]=15;
			doubleBuffer[(x)+((y+13)*320)]=15;
			doubleBuffer[(x+1)+((y+11)*320)]=15;
			doubleBuffer[(x+1)+((y+12)*320)]=15;
			doubleBuffer[(x+4)+((y+13)*320)]=15;
			doubleBuffer[(x+5)+((y+12)*320)]=15;
			doubleBuffer[(x+4)+((y+11)*320)]=15;
			VertLine(x+3,y+9,y+13,15);
		case(bass):
			HorizLine(y-2,x-6,x-5,15);
			HorizLine(y-3,x-7,x-4,15);
			HorizLine(y-4,x-7,x-5,15);
			VertLine(x-7,y-6,y-5,15);
			HorizLine(y-7,x-6,x-5,15);
			HorizLine(y-8,x-4,x+1,15);
			HorizLine(y-7,x+2,x+3,15);
			doubleBuffer[(x+4)+((y-6)*320)]=15;
			VertLine(x+5,y-5,y,15);
			VertLine(x+4,y+1,y+2,15);
			VertLine(x+3,y+3,y+4,15);
			doubleBuffer[(x+2)+((y+5)*320)]=15;
			HorizLine(y+6,x,x+1,15);
			doubleBuffer[(x+8)+((y-2)*320)]=15;
			doubleBuffer[(x+8)+((y-6)*320)]=15;
			break;
		}
	}

void DrawMusic(unsigned int channel1,unsigned int channel2,unsigned int channel3,int musicPos)
	{
	char skip;
	if(musicPos&1)
		{
		skip=1;
		musicPos--;
		}
	else
		skip=0;
	if(res!=sixtyfourth)
		skip=0;
	channel1=(channel1*7110)+((musicPos*3)>>1);
	channel2=(channel2*7110)+((musicPos*3)>>1);
	channel3=(channel3*7110)+((musicPos*3)>>1);
	char clef1,clef2,clef3,clef1a,clef2a,clef3a;
	char note1,sfn1,length1,dot1;
	char note2,sfn2,length2,dot2;
	char note3,sfn3,length3,dot3;
	int curr1,curr2,curr3;
	clef1=255;
	clef2=255;
	clef3=255;
	int xPos=7;
	numX=0;
	startLoop:;
		clef1a=dataBuffer2[channel1]>>6;
		clef2a=dataBuffer2[channel2]>>6;
		clef3a=dataBuffer2[channel3]>>6;
		if(clef1!=clef1a||clef2!=clef2a||clef3!=clef3a)
			{
			clef1=clef1a;
			clef2=clef2a;
			clef3=clef3a;
			DrawClef(xPos,74,clef1);
			DrawClef(xPos,124,clef2);
			DrawClef(xPos,174,clef3);
			xPos+=21;
			if(skip)
				{
				skip=0;
				goto skipFirst;
				}
			if(xPos>305)
				{
				numX--;
				return;
				}
			}
		note1=(dataBuffer2[channel1]>>1)&31;
		note2=(dataBuffer2[channel2]>>1)&31;
		note3=(dataBuffer2[channel3]>>1)&31;
		sfn1=((dataBuffer2[channel1]&1)<<1)+(dataBuffer2[channel1+1]>>7);
		sfn2=((dataBuffer2[channel2]&1)<<1)+(dataBuffer2[channel2+1]>>7);
		sfn3=((dataBuffer2[channel3]&1)<<1)+(dataBuffer2[channel3+1]>>7);
		length1=(dataBuffer2[channel1+1]>>4)&7;
		length2=(dataBuffer2[channel2+1]>>4)&7;
		length3=(dataBuffer2[channel3+1]>>4)&7;
		dot1=(dataBuffer2[channel1+1]>>3)&1;
		dot2=(dataBuffer2[channel2+1]>>3)&1;
		dot3=(dataBuffer2[channel3+1]>>3)&1;
		DrawNotes(1,xPos,note1,sfn1,length1,dot1);
		DrawNotes(2,xPos,note2,sfn2,length2,dot2);
		DrawNotes(3,xPos,note3,sfn3,length3,dot3);
		xCen[numX++]=xPos;
		xPos+=15;
		if(xPos>305)
				{
				numX--;
				return;
				}
		if(res!=sixtyfourth)
			{
			channel1+=3<<res;
			channel2+=3<<res;
			channel3+=3<<res;
			goto startLoop;
			}
		skipFirst:;
		note1=((dataBuffer2[channel1+1]&7)<<2)+(dataBuffer2[channel1+2]>>6);
		note2=((dataBuffer2[channel2+1]&7)<<2)+(dataBuffer2[channel2+2]>>6);
		note3=((dataBuffer2[channel3+1]&7)<<2)+(dataBuffer2[channel3+2]>>6);
		sfn1=(dataBuffer2[channel1+2]>>4)&3;
		sfn2=(dataBuffer2[channel2+2]>>4)&3;
		sfn3=(dataBuffer2[channel3+2]>>4)&3;
		length1=(dataBuffer2[channel1+2]>>1)&7;
		length2=(dataBuffer2[channel2+2]>>1)&7;
		length3=(dataBuffer2[channel3+2]>>1)&7;
		dot1=dataBuffer2[channel1+2]&1;
		dot2=dataBuffer2[channel2+2]&1;
		dot3=dataBuffer2[channel3+2]&1;
		DrawNotes(1,xPos,note1,sfn1,length1,dot1);
		DrawNotes(2,xPos,note2,sfn2,length2,dot2);
		DrawNotes(3,xPos,note3,sfn3,length3,dot3);
		xCen[numX++]=xPos;
		xPos+=15;
		if(xPos>305)
			{
			numX--;
			return;
			}
		channel1+=3;
		channel2+=3;
		channel3+=3;
	goto startLoop;
	}

void DrawNotes(char yPos,int xPos,char note1,char sfn1,char length1,char dot1)
	{
	if(note1==blank||note1==blank)
		return;
	if(note1==rest)
		{
		DrawRest(xPos,(yPos*50)+24,length1);
		return;
		}
	int x,y;
	char type,dot,sfn,updn,line;
	x=xPos;
	y=2+(yPos*50)+(note1*2);
	type=length1;
	if(dot1)
		{
		if(note1<11)
			{
			if(note1&1)
				dot=under;
			else
				dot=thru;
			}
		else
			{
			if(note1&1)
				dot=over;
			else
				dot=thru;
			}
		}
	else
		dot=notdotted;
	sfn=sfn1;
	if(note1<11)
		updn=down;
	else
		updn=up;
	if(note1<6&&note1&1)
		line=thru;
	else if(note1<6&&!(note1&1))
		line=under;
	else if(note1>16&&note1&1)
		line=thru;
	else if(note1>16&&!(note1&1))
		line=over;
	else line=noline;
	DrawNote(x,y,type,dot,sfn,updn,line);
	}

void WriteNote(char clef,char note,char sfn,char length,char dot,unsigned int pos,unsigned int channel)
	{
	char char1,char2,char3;
	if(pos&1)
		goto odd1;
	pos=(channel*7110)+((pos*3)>>1);
	char1=(clef<<6)+(note<<1)+(sfn>>1);
	char2=((sfn&1)<<7)+(length<<4)+(dot<<3)+(dataBuffer2[pos+1]&7);
	char3=dataBuffer2[pos+2];
	goto even;
	odd1:;
	pos=(channel*7110)+(((pos-1)*3)>>1);
	char1=dataBuffer2[pos];
	char2=(dataBuffer2[pos+1]&0xF8)+(note>>2);
	char3=((note&3)<<6)+(sfn<<4)+(length<<1)+dot;
	even:;
	dataBuffer2[pos]=char1;
	dataBuffer2[pos+1]=char2;
	dataBuffer2[pos+2]=char3;
	}

char GetClef(unsigned int pos,unsigned int channel)
	{
	if(pos&1)
		pos--;
	pos=(channel*7110)+((pos*3)>>1);
	return dataBuffer2[pos]>>6;
	}

void WriteClef(unsigned int pos,unsigned int channel,char clef)
	{
	if(pos&1)
		pos--;
	pos=(channel*7110)+((pos*3)>>1);
	dataBuffer2[pos]=(dataBuffer2[pos]&0x3F)+(clef<<6);
	}

char GetNote(unsigned int pos,unsigned int channel)
	{
	if(pos&1)
		{
		pos=(channel*7110)+(((pos-1)*3)>>1);
		return ((dataBuffer2[pos+1]&7)<<2)+(dataBuffer2[pos+2]>>6);
		}
	else
		{
		pos=(channel*7110)+((pos*3)>>1);
		return (dataBuffer2[pos]>>1)&31;
		}
	}

char GetSFN(unsigned int pos,unsigned int channel)
	{
	if(pos&1)
		{
		pos=(channel*7110)+(((pos-1)*3)>>1);
		return (dataBuffer2[pos+2]>>4)&3;
		}
	else
		{
		pos=(channel*7110)+((pos*3)>>1);
		return ((dataBuffer2[pos]&1)<<1)+((dataBuffer2[pos+1]>>7)&1);
		}
	}

char GetLength(unsigned int pos,unsigned int channel)
	{
	if(pos&1)
		{
		pos=(channel*7110)+(((pos-1)*3)>>1);
		return (dataBuffer2[pos+2]>>1)&7;
		}
	else
		{
		pos=(channel*7110)+((pos*3)>>1);
		return (dataBuffer2[pos+1]>>4)&7;
		}
	}

char GetDot(unsigned int pos,unsigned int channel)
	{
	if(pos&1)
		{
		pos=(channel*7110)+(((pos-1)*3)>>1);
		return dataBuffer2[pos+2]&1;
		}
	else
		{
		pos=(channel*7110)+((pos*3)>>1);
		return (dataBuffer2[pos+1]>>3)&1;
		}
	}

int FindLast(unsigned int pos,unsigned int channel)
	{
	char note;
	pos--;
	while(pos<4720)
		{
		note=GetNote(pos,channel);
		if(note!=rest&&note!=blank)
			break;        
		pos--;
		}
	if(pos>4720)
		pos=0;
	return pos;
	}

void UserInput(void)
	{
	char note,clef,sfn,length,dot;
	char altered=0;
	int count,count2,num;
	unsigned int last;
	note=GetNote(viewPos,ch1+userY);
	clef=GetClef(viewPos,ch1+userY);
	sfn=GetSFN(viewPos,ch1+userY);
	length=GetLength(viewPos,ch1+userY);
	dot=GetDot(viewPos,ch1+userY);
	if(kbhit())
		{
		switch(getch())
			{
			case('8'):
				userY--;
				if(userY>2)
					{
					userY=0;
					ch1--;
					if(ch1>10)
						ch1=0;
					}
				break;
			case('5'):
				userY++;
				if(userY>2)
					{
					userY=2;
					ch1++;
					if(ch1>6)
						ch1=6;
					}
			       break;
			case('4'):
				viewPos--;
				while(GetNote(viewPos,ch1+userY)==blank)
					{
					viewPos--;
					if(viewPos>4720)
						{
						viewPos=0;
						break;
						}
					}
				if(viewPos>4720)
					viewPos=0;
				break;
			case('6'):
				viewPos++;
				while(GetNote(viewPos,ch1+userY)==blank)
					{
					viewPos++;
					if(viewPos>4720)
						{
						viewPos=0;
						break;
						}
					}
				if(viewPos>4720)
					viewPos=0;
				break;
			case('`'):
				if(note!=blank)
					{
					last=FindLast(viewPos,ch1+userY);
					if(last!=-1)
						{
						note=GetNote(last,ch1+userY);
						sfn=GetSFN(last,ch1+userY);
						length=GetLength(last,ch1+userY);
						dot=GetDot(last,ch1+userY);
						altered=1;
						}
					}
				break;
			case('@'):
				if(note!=blank)
					{
					note=rest;
					sfn=natural;
					dot=notdotted;
					length=sixtyfourth;
					altered=1;
					}
				break;
			case('q'):
				if(note!=blank)
					{
					note--;
					if(note<0)
						{
						note=rest;
						length=sixtyfourth;
						dot=notdotted;
						sfn=natural;
						altered=1;
						}
					altered=1;
					}
				break;
			case('a'):
				if(note!=blank)
					{
					note++;
					if(note>rest)
						note=0;
					altered=1;
					}
				break;
			case('X'):
				clef++;
				if(clef>3)
					clef=0;
				count=viewPos;
				while(count<4720)
					{
					WriteClef(count,ch1+userY,clef);
					count+=2;
					}
				break;
			case('w'):
				if(note!=blank)
					{
					sfn++;
					if(sfn>2)
						sfn=0;
					altered=1;
					}
				break;
			case('z'):
				if(note!=blank)
					{
					length--;
					altered=1;
					if(length<0)
						{
						length=0;
						altered=0;
						}
					}
				break;
			case('Z'):
				if(note!=blank)
					{
					length++;
					altered=1;
					if(length>6)
						{
						length=6;
						altered=0;
						}
					}
				break;
			case('S'):
				if(note!=blank)
					{
					if(dot==0)
						{
						dot=1;
						altered=1;
						}
					else
						{
						dot=0;
						altered=1;
						}
					}
				break;
			case('f'):
				filename=ChangeFilename();
				break;
			case('i'):
				ChangeInsname(ch1+userY);
				break;
			case(' '):
				PlaySong(yes);
				break;
			case('L'):
				LoadFile();
				break;
			case('s'):
				SaveFile();
				PlaySong(rec);
				break;
			case('N'):
				SaveFile();
				PlaySong(rec);
				NewSong();
				break;
			case('t'):
				tempo-=2;
				if(tempo<10)
					tempo=10;
				break;
			case('T'):
				tempo+=2;
				if(tempo>240)
					tempo=240;
				break;
			case('r'):
				res--;
				if(res<sixtyfourth)
					res=sixtyfourth;
				break;
			case('R'):
				res++;
				if(res>whole)
					res=whole;
				break;
			case(27):
				done=1;
				break;
			}
		}
	if(altered)
		{
		count=viewPos;
		WriteNote(clef,note,sfn,length,dot,count,ch1+userY);
		num=1<<length;
		if(dot)
			num+=num>>1;
		if(note==rest)
			{
			if(num>1)
				for(count2=1;count2<num;count2++)
					WriteNote(GetClef(count,ch1+userY),rest,natural,sixtyfourth,notdotted,count+count2,ch1+userY);
			}
		else
			{
			if(num>1)
				for(count2=1;count2<num;count2++)
					WriteNote(GetClef(count,ch1+userY),blank,natural,sixtyfourth,notdotted,count+count2,ch1+userY);
			}
		count+=num;
		loop1:;
		if(GetNote(count,ch1+userY)==blank)
			{
			WriteNote(GetClef(count,ch1+userY),rest,natural,sixtyfourth,notdotted,count,ch1+userY);
			count++;
			goto loop1;
			}
		}
	}

void DrawString(char *str1,int x,int y,char color)
	{
	char count;
	for(count=0;count<strlen(str1);count++)
		DrawLetter(str1[count],x+(count*6),y,color);
	}

void DrawNum(int num,int x,int y,char color)
	{
	int num1=num/1000;
	DrawLetter('0'+num1,x,y,color);
	num-=num1*1000;
	num1=num/100;
	DrawLetter('0'+num1,x+6,y,color);
	num-=num1*100;
	num1=num/10;
	DrawLetter('0'+num1,x+12,y,color);
	num-=num1*10;
	DrawLetter('0'+num,x+18,y,color);
	}

char *ChangeFilename(void)
	{
	char *filename2="filename.xxx";
	strcpy(filename2,filename);
	char keyPress,value,len;
	notYet2:;
		DrawBox(14,2,61,6,0);
		if(strlen(filename2)>0)
			for(value=0;value<strlen(filename2);value++)
				DrawLetter(filename2[value],14+(value*6),2,12);
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
			filename2[len+1]=filename2[len];
			filename2[len]=keyPress;
			}
		if(keyPress>='0'&&keyPress<='9'&&strlen(filename2)<8)
			{
			len=strlen(filename2);
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

void ChangeInsname(unsigned int channel)
	{
	int x,y;
	if(channel<4)
		{
		x=14;
		y=(channel*8)+10;
		}
	else
		{
		x=76;
		y=((channel-4)*8)+2;
		}
	char *filename2="instrxxx.xxx";
	switch(channel)
		{
		case(0):
			strcpy(filename2,instr0);
			break;
		case(1):
			strcpy(filename2,instr1);
			break;
		case(2):
			strcpy(filename2,instr2);
			break;
		case(3):
			strcpy(filename2,instr3);
			break;
		case(4):
			strcpy(filename2,instr4);
			break;
		case(5):
			strcpy(filename2,instr5);
			break;
		case(6):
			strcpy(filename2,instr6);
			break;
		case(7):
			strcpy(filename2,instr7);
			break;
		case(8):
			strcpy(filename2,instr8);
			break;
		}
	char keyPress,value,len;
	notYet3:;
		DrawBox(x,y,x+47,y+4,4);
		if(strlen(filename2)>0)
			for(value=0;value<strlen(filename2);value++)
				DrawLetter(filename2[value],x+(value*6),y,12);
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
			return;
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
			filename2[len+1]=filename2[len];
			filename2[len]=keyPress;
			}
		if(keyPress>='0'&&keyPress<='9'&&strlen(filename2)<8)
			{
			len=strlen(filename2);
			filename2[len+1]=filename2[len];
			filename2[len]=keyPress;
			}
		if(keyPress==13||strlen(filename2)==8)
			if(strlen(filename2)>0)
				goto yet;
			else 
				return;
	goto notYet3;
	yet:;
	switch(channel)
		{
		case(0):
			strcpy(instr0,filename2);
			break;
		case(1):
			strcpy(instr1,filename2);
			break;
		case(2):
			strcpy(instr2,filename2);
			break;
		case(3):
			strcpy(instr3,filename2);
			break;
		case(4):
			strcpy(instr4,filename2);
			break;
		case(5):
			strcpy(instr5,filename2);
			break;
		case(6):
			strcpy(instr6,filename2);
			break;
		case(7):
			strcpy(instr7,filename2);
			break;
		case(8):
			strcpy(instr8,filename2);
			break;
		}
	}

void SaveFile(void)
	{
	char *filename2="filename.222";
	strcpy(filename2,filename);
	strcat(filename2,".sng");
	FILE *fp;
	fp=fopen(filename2,"wb");
	unsigned int count;
	for(count=0;count<9;count++)
		{
		putc(instr0[count],fp);
		putc(instr1[count],fp);
		putc(instr2[count],fp);
		putc(instr3[count],fp);
		putc(instr4[count],fp);
		putc(instr5[count],fp);
		putc(instr6[count],fp);
		putc(instr7[count],fp);
		putc(instr8[count],fp);
		}
	for(count=0;count<64000;count++)
		putc(dataBuffer2[count],fp);
	putc(res,fp);
	putc(char(tempo>>8),fp);
	putc(char(tempo&255),fp);
	fclose(fp);
	}

void LoadFile(void)
	{
	char *filename2="filename.22a";
	strcpy(filename2,filename);
	strcat(filename2,".sng");
	char freq1;
	FILE *fp;
	if(!(fp=fopen(filename2,"rb")))
		return;
	unsigned int count;
	for(count=0;count<9;count++)
		{
		instr0[count]=getc(fp);
		instr1[count]=getc(fp);
		instr2[count]=getc(fp);
		instr3[count]=getc(fp);
		instr4[count]=getc(fp);
		instr5[count]=getc(fp);
		instr6[count]=getc(fp);
		instr7[count]=getc(fp);
		instr8[count]=getc(fp);
		}
	for(count=0;count<64000;count++)
		dataBuffer2[count]=getc(fp);
	res=getc(fp);
	tempo=int(getc(fp));
	tempo=(tempo<<8)+getc(fp);
	fclose(fp);
	viewPos=0;
	ch1=0;
	userY=0;
	}

void NewSong(void)
	{
	int count1,count2;
	for(count2=0;count2<9;count2++)
		for(count1=0;count1<4740;count1++)
			WriteNote(treb,rest,natural,sixtyfourth,notdotted,count1,count2);
	strcpy(filename,"default");
	strcpy(instr0,"empty");
	strcpy(instr1,"empty");
	strcpy(instr2,"empty");
	strcpy(instr3,"empty");
	strcpy(instr4,"empty");
	strcpy(instr5,"empty");
	strcpy(instr6,"empty");
	strcpy(instr7,"empty");
	strcpy(instr8,"empty");
	ResetChip();
	viewPos=0;
	ch1=0;
	userY=0;
	res=sixtyfourth;
	tempo=120;
	}

void CreateScreen(void)
	{
	HorizLine(66,0,319,7);
	HorizLine(70,0,319,7);
	HorizLine(74,0,319,7);
	HorizLine(78,0,319,7);
	HorizLine(82,0,319,7);
	HorizLine(116,0,319,7);
	HorizLine(120,0,319,7);
	HorizLine(124,0,319,7);
	HorizLine(128,0,319,7);
	HorizLine(132,0,319,7);
	HorizLine(166,0,319,7);
	HorizLine(170,0,319,7);
	HorizLine(174,0,319,7);
	HorizLine(178,0,319,7);
	HorizLine(182,0,319,7);
	HorizLine(0,0,124,7);
	HorizLine(8,0,124,7);
	HorizLine(16,0,124,7);
	HorizLine(24,0,124,7);
	HorizLine(32,0,124,7);
	HorizLine(40,0,124,7);
	VertLine(0,0,40,7);
	VertLine(62,0,40,7);
	VertLine(124,0,40,7);
	DrawString("f:",2,2,7);
	DrawString("1:",2,10,7);
	DrawString("2:",2,18,7);
	DrawString("3:",2,26,7);
	DrawString("4:",2,34,7);
	DrawString("5:",64,2,7);
	DrawString("6:",64,10,7);
	DrawString("7:",64,18,7);
	DrawString("8:",64,26,7);
	DrawString("9:",64,34,7);
	DrawString("pos:",2,42,7);
	DrawString("1:copy",126,2,7);
	DrawString("q:noteup",126,10,7);
	DrawString("a:notedn",126,18,7);
	DrawString("z:length",126,26,7);
	DrawString("f:flname",126,34,7);
	DrawString("i:insnam",126,42,7);
	DrawString("2:rest",180,2,7);
	DrawString("w:srpflt",180,10,7);
	DrawString("s:dot",180,18,7);
	DrawString("x:clef",180,26,7);
	DrawString("^:play",180,42,7);
	DrawString("l:load",234,2,7);
	DrawString("s:save",234,10,7);
	DrawString("n:new",234,18,7);
	DrawString("r:res",234,26,7);
	DrawString("t:",234,42,7);
	_asm
		{
		push ds
		les di,dataBuffer1
		lds si,doubleBuffer
		mov cx,32000
		cld
		rep movsw
		pop ds
		}
	}

void HighlightName(char pos)
	{
	int x,y;
	if(pos<5)
		{
		x=1;
		y=(pos*8)+1;
		}
	else
		{
		x=63;
		y=((pos-5)*8)+1;
		}
	ChangeColor(x,y,62,8);
	}

void DrawScreen(void)
	{
	_asm
		{
		push ds
		les di,doubleBuffer
		lds si,dataBuffer1
		mov cx,32000
		cld
		rep movsw
		pop ds
		}
	DrawMusic(ch1,ch1+1,ch1+2,viewPos);
	ChangeColor(xCen[0]-8,(userY+1)*50,15,50);
	DrawString(filename,14,2,7);
	DrawString(instr0,14,10,7);
	DrawString(instr1,14,18,7);
	DrawString(instr2,14,26,7);
	DrawString(instr3,14,34,7);
	DrawString(instr4,76,2,7);
	DrawString(instr5,76,10,7);
	DrawString(instr6,76,18,7);
	DrawString(instr7,76,26,7);
	DrawString(instr8,76,34,7);
	HighlightName(ch1+userY+1);
	DrawNum(viewPos,26,42,7);
	DrawNum(tempo,247,42,7);
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

void PlaySong(char play)
	{
	int len[10],pos,songLen;
	char count;
	for(count=0;count<9;count++)
		{
		len[count]=0;
		pos=4719;
		while(pos>0)
			{
			if(GetNote(pos,count)==rest)
				pos--;
			else
				break;
			}
		len[count]=pos;
		}
	len[9]=0;
	num=0;
	count=0;
	while(len[count++]!=0)
		num++;
	switch(num)
		{
		case(9):
			LoadInsFile(instr8,8);
		case(8):
			LoadInsFile(instr7,7);
		case(7):
			LoadInsFile(instr6,6);
		case(6):                 
			LoadInsFile(instr5,5);
		case(5):
			LoadInsFile(instr4,4);
		case(4):
			LoadInsFile(instr3,3);
		case(3):
			LoadInsFile(instr2,2);
		case(2):
			LoadInsFile(instr1,1);
		case(1):
			LoadInsFile(instr0,0);
			break;
		case(0):
			return;
			break;
		}
	dataBuffer3[0]=num;
	songLen=0;
	for(count=0;count<9;count++)
		if(songLen<len[count])
			songLen=len[count];
	dataBuffer3[1]=char(songLen>>8);
	dataBuffer3[2]=char(songLen&0xFF);
	EncodeMusic();
	if(play==yes)
		PlayMusic();
	else
		SaveMusic();
	}

void SetClock(long hz)
	{
	long val1=1193180/hz;
	int val=int(val1&0xFFFF);
	char lsb=char(val&0xFF);
	char msb=char(val>>8);
	if(hz==18)
		{
		lsb=0xFF;
		msb=0xFF;
		}
	outp(0x43,0x3C);
	outp(0x40,lsb);
	outp(0x40,msb);
	}

void WaitClock(void)
	{
	unsigned int far *clock=(unsigned int far *)0x0000046CL;
	unsigned int now=*clock;
	while(now==*clock);
	}

void EncodeMusic(void)
	{
	char num=dataBuffer3[0];
	int songLen=int(dataBuffer3[1]);
	songLen=(songLen<<8)+int(dataBuffer3[2]);
	int writePos=3+(num*12);
	int count=0;
	char count2;
	unsigned char note,oct,val;
	while(count<=songLen)
		{
		for(count2=0;count2<num;count2++)
			{
			note=GetNote(count,count2);
			if(note==blank||note==rest)
				val=128;
			else
				{
				switch(GetClef(count,count2))
					{
					case(treb8va):
						if(note<=3)
							{
							oct=7;
							note=3-note;
							}
						else if(note<=10)
							{
							oct=6;
							note=10-note;
							}
						else if(note<=17)
							{
							oct=5;
							note=17-note;
							}
						else
							{
							oct=4;
							note=24-note;
							}
						break;
					case(treb):
						if(note<=3)
							{
							oct=6;
							note=3-note;
							}
						else if(note<=10)
							{
							oct=5;
							note=10-note;
							}
						else if(note<=17)
							{
							oct=4;
							note=17-note;
							}
						else
							{
							oct=3;
							note=24-note;
							}
						break;
					case(bass):
						if(note<=5)
							{
							oct=4;
							note=5-note;
							}
						else if(note<=12)
							{
							oct=3;
							note=12-note;
							}
						else if(note<=19)
							{
							oct=2;
							note=19-note;
							}
						else
							{
							oct=1;
							note=26-note;
							}
						break;
					case(bass8vb):
						if(note<=5)
							{
							oct=3;
							note=5-note;
							}
						else if(note<=12)
							{
							oct=2;
							note=12-note;
							}
						else if(note<=19)
							{
							oct=1;
							note=19-note;
							}
						else
							{
							oct=0;
							note=26-note;
							}
						break;
					}
				val=(oct<<4)+ofs[note];
				switch(GetSFN(count,count2))
					{
					case(sharp):
						val++;
						break;
					case(flat):
						val--;
						break;
					}
				}
			dataBuffer3[writePos++]=val;
			}
		count++;
		}
	}

void PlayMusic(void)    
	{
	char count;
	int count2;
	int writePos=3;
	int songLen=int(dataBuffer3[1]);
	songLen=(songLen<<8)+int(dataBuffer3[2]);
	char num=dataBuffer3[0];
	unsigned char note;
	for(count=0;count<num;count++)
		{
		WriteReg(0x20+chn[count].op1,dataBuffer3[writePos++]);
		WriteReg(0x40+chn[count].op1,dataBuffer3[writePos++]);
		WriteReg(0x60+chn[count].op1,dataBuffer3[writePos++]);
		WriteReg(0x80+chn[count].op1,dataBuffer3[writePos++]);
		WriteReg(0xE0+chn[count].op1,dataBuffer3[writePos++]);
		WriteReg(0x20+chn[count].op2,dataBuffer3[writePos++]);
		WriteReg(0x40+chn[count].op2,dataBuffer3[writePos++]);
		WriteReg(0x60+chn[count].op2,dataBuffer3[writePos++]);
		WriteReg(0x80+chn[count].op2,dataBuffer3[writePos++]);
		WriteReg(0xE0+chn[count].op2,dataBuffer3[writePos++]);
		WriteReg(0xC0+count,dataBuffer3[writePos++]);
		WriteReg(0xBD,dataBuffer3[writePos++]);
		}
	SetClock(tempo);
	for(count2=0;count2<songLen;count2++)
		{
		for(count=0;count<num;count++)
			{
			note=dataBuffer3[writePos++];
			if(note!=128)
				PlayNote(key[note&0x0F],note>>4,count);
			}
		if(kbhit())
			{
			getch();
			goto userEnd;
			}
		WaitClock();
		WaitClock();
		WaitClock();
		}
	userEnd:;
	SetClock(18);
	ResetChip();
	}

void SaveMusic(void)
	{
	}

void main(void)
	{
	Initialize();
	CreateScreen();
	while(!done)                                  
		{
		DrawScreen();
		UserInput();
		}
	Restore();
	return;
	}

