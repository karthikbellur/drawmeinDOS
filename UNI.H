/*
CONTENTS OF FILE:
*all the #include files
*all the mouse functions
*functions and variables in mycgmain.c
*/


//------------INCLUDE 		FILES		-----------------------------------------------
#include<stdio.h>
#include<dos.h>
#include<conio.h>
#include<graphics.h>
#include<math.h>
#include<stdlib.h>
#include<bios.h>
//-----		USER DEFINITIONS
#define XOR_MODE  0x80
#define COPY_MODE 0x00
#define setdrawmode( x ) draw_mode = x
#define set_color(c) cur_color=c;
#define WORD  unsigned int
#define DWORD unsigned long
//------------INCLUDE 		FILES		ENDS-----------------------------------------------





//--------------		GLOBAL 		VARIABLE		DECLARATIONS---------------------------

union REGS i,o;  // the mouse registers
struct SREGS sregs; //predefined structure for changing mouse pointer
int cur_panel=0,cur_tool=5,cur_style=1,cur_color=0,cur_pat=0;
int tool_but_num=18,style_but_num=0;
int buttons_data[18]={0,0,0,0,0,0,0,0,0,4,0,0,0,0,3,2,5,0};
int xmax,ymax;
int canvas_flag=0;
int bgcolor;
int arrx1[50],arrx2[50],arry1[250],arry2[50];
int arrcnt=-1,arrcnt2=-1;
int arrx12[50],arrx22[50],arry12[50],arry22[50];
int draw_mode = COPY_MODE;
int color=WHITE;   //set user color
int prgx=1;
//------------------------------------POINTER MASKS		=--------------------------------------------------


static int arrow[]={
					// Arrow: SCREEN MASK
					0x3FFF, 0x1FFF, 0x07FF, 0x01FF,
					 0x007F, 0x001F, 0x0007, 0x0001,
					 0x0000, 0x01FF, 0x03FF, 0x07FF,
					 0x0FFF, 0x1FFF, 0x3FFF, 0x7FFF,
					 // BITMAP
					 0x0000, 0x4000, 0x6000, 0x7800,
					 0x7E00, 0x7F80, 0x7FE0, 0x7FF8,
					 0x7E00, 0x7C00, 0x7800, 0x7000,
					 0x6000, 0x4000, 0x0000, 0x0000,
					 };
					 
static int cross[]={
					// Cross: SCREEN MASK
					 0xFFFF, 0xFC7F, 0xFC7F, 0xFC7F,
					 0xFC7F, 0xFC7F, 0xFC7F, 0x0381,
					 0x0381, 0x0381, 0xFC7F, 0xFC7F,
					 0xFC7F, 0xFC7F, 0xFC7F, 0xFC7F,
					 // BITMAP
					 0x0000, 0x0280, 0x0280, 0x0280,
					 0x0280, 0x0280, 0x0000, 0xF83E,
					 0x0100, 0xF83E, 0x0000, 0x0280,
					 0x0280, 0x0280, 0x0280, 0x0280,
					 };
					 
static int text[]={
					// Text Select: SCREEN MASK
					 0xFFFF, 0xF83F, 0xF01F, 0xF83F,
					 0xFC7F, 0xFC7F, 0xFC7F, 0xFC7F,
					 0xFC7F, 0xFC7F, 0xFC7F, 0xFC7F,
					 0xF83F, 0xF01F, 0xF83F, 0xFFFF,
					 // BITMAP
					 0x0000, 0x07C0, 0x0820, 0x06C0,
					 0x0280, 0x0280, 0x0280, 0x0280,
					 0x0280, 0x0280, 0x0280, 0x0280,
					 0x06C0, 0x0820, 0x07C0, 0x0000,
					 };

static int pen[]={// Pen: SCREEN MASK
					 0xFFFF, 0xFE3F, 0xFC1F, 0xF80F,
					 0xF80F, 0xF01F, 0xF01F, 0xE03F,
					 0xE03F, 0xC07F, 0xC07F, 0xC0FF,
					 0xC1FF, 0xC3FF, 0xC7FF, 0xEFFF,
					 // BITMAP
					 0x0000, 0x01C0, 0x0220, 0x05D0,
					 0x05D0, 0x09A0, 0x0A20, 0x1740,
					 0x1740, 0x2E80, 0x2480, 0x2100,
					 0x2200, 0x2400, 0x2800, 0x1000,
					 };
					 
static int bucket[]={
					// Bucket: SCREEN MASK
					 0xFFFF, 0xFE7F, 0xFC3F, 0xF81F,
					 0XF80F, 0xF007, 0xE003, 0xE001,
					 0xE000, 0xF000, 0xF801, 0xFC03,
					 0xFE07, 0xFF0F, 0xFF9F, 0xFFFF,
					 // BITMAP
					 0x0000, 0x0180, 0x0240, 0x0560,
					 0x7B90, 0xF768, 0xEFC4, 0xE582,
					 0xFB05, 0xEA0D, 0xE41A, 0xE234,
					 0xE168, 0x6090, 0x6060, 0x2000,
					 };
					 
static int eraser[]={
		//pointer mask starts here
			0x0000,	0x7ffe,	0x7ffe,	0x7ffe,
			0x7ffe,	0x7ffe,	0x7ffe,	0x7ffe,
			0x7ffe,	0x7ffe,	0x7ffe,	0x7ffe,
			0x7ffe,	0x7ffe,	0x7ffe,	0x0000,
		//pointer mask ends here
		//screen mask starts here
			0x0000,	0x0000,	0x0000,	0x0000,
			0x0000,	0x0000,	0x0000,	0x0000,
			0x0000,	0x0000,	0x0000,	0x0000,
			0x0000,	0x0000,	0x0000,	0x0000,
		//screen mask ends here
		 };
//-------------------------------------POINTER MASKS 		END--------------------------------------------


typedef struct node
{
	int x, y;
	struct node *next;
}NODE;

NODE *front = NULL, *rear = NULL;
struct header
{     	char type[2];   		// BITMAP TYPE USUALLY "BM"
	DWORD size;		// SIZE OF THE BITMAP FILE
	WORD reserved1;	// RESERVED FOR FUTURE USE (ALWAYS SET TO ZERO)
	WORD reserved2;     // ---------- " -------------- " -----------
	DWORD offset;		// STARTING ADDRESS OF THE BITMAP DATA
}t_bfh,bfh;

struct info
{
   DWORD  size;		// SIZE FOR THE BITMAP_INFO_HEADER
   DWORD  width;		// WIDTH OF THE IMAGE IN PIXELS
   DWORD  height;		// HEIGHT OF THE IMAGE IN PIXELS
   WORD   planes;		// NO. OF PLANES FOR THE TARGET DEVICE
   WORD   bitsPerPixel;	// NO. OF BITS PER PIXEL (1=MONO, 4=16, 8=256)
   DWORD  compression;	// COMPRESSION TECHNIQUE USED (0=NO COMPRESSION)
   DWORD  sizeImage;	// IMAGE SIZE IN BYTES
   DWORD  horzRes;		// HORIZONTAL RESOLUTION IN PIXEL
   DWORD  vertRes;		// VERTICAL RESOLUTION IN PIXEL

   DWORD  colorsUsed;	// NO. COLORS USED IN THE IMAGE (0=MAX. COLORS
					// CORRESPONDING TO THE bitsPerPixel FIELD)

   DWORD  impColors;	// NO. OF COLORS THAT ARE IMPORTANT FOR DISPLAYING
					// THE BITMAP (0=ALL COLORS ARE IMPORTANT)
}t_bih,bih;

typedef struct
{
	char blue;
	char green;
	char red;
	char reserved;
}BITMAP_PALETTE;


 char pat[38][8] =  {{ 0x18, 0x24, 0x42, 0x81, 0xFF, 0x81, 0x81, 0x81 },						
				    { 0x00, 0xFF, 0x81, 0x81, 0xFF, 0x81, 0x81, 0xFF },						
					{ 0xFE, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFE },
					{ 0x00, 0xFC, 0x82, 0x81, 0x81, 0x81, 0x82, 0xFC },
					{ 0x00, 0xFE, 0x80, 0x80, 0xFE, 0x80, 0x80, 0xFE },
					{ 0xFE, 0x80, 0x80, 0xFE, 0x80, 0x80, 0x80, 0x80 },
					{ 0xFF, 0x80, 0x80, 0x80, 0x9F, 0x91, 0x91, 0xF1 },
					{ 0x00, 0x81, 0x81, 0x81, 0xFF, 0x81, 0x81, 0x81 },
					{ 0xFF, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xFF },
					{ 0xFE, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0xF0 },
					{ 0x83, 0x8C, 0x90, 0xE0, 0xE0, 0x90, 0x8C, 0x83 },
			 		{ 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFE },
					{ 0xC3, 0xA5, 0x99, 0x81, 0x81, 0x81, 0x81, 0x81 },
					{ 0xC1, 0xC1, 0xA1, 0x91, 0x89, 0x85, 0x83, 0x83 },
					{ 0x00, 0x3C, 0x42, 0x81, 0x81, 0x81, 0x42, 0x3C },
					{ 0xFF, 0x81, 0x81, 0xFF, 0x80, 0x80, 0x80, 0x80 },
					{ 0xFE, 0x82, 0x82, 0x82, 0x92, 0x8A, 0x86, 0xFF },
					{ 0xFC, 0x82, 0x82, 0xFE, 0xE0, 0x90, 0x8C, 0x82 },
					{ 0xFF, 0x80, 0x80, 0xFF, 0x01, 0x01, 0x01, 0xFF },
					{ 0xFF, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 },
					{ 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xFF, 0xFF },
					{ 0x81, 0x81, 0x42, 0x42, 0x24, 0x24, 0x18, 0x18 },
					{ 0x81, 0x81, 0x81, 0x81, 0x99, 0x99, 0xA5, 0xE7 },
					{ 0x81, 0xC3, 0x66, 0x3C, 0x3C, 0x66, 0xC3, 0x81 },
					{ 0x81, 0xC3, 0x66, 0x3C, 0x18, 0x18, 0x18, 0x18 },
					{ 0xFF, 0x01, 0x06, 0x08, 0x10, 0x60, 0x80, 0xFF },
					{ 0x3E, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x3E },
					{ 0x00, 0x38, 0x38, 0x18, 0x18, 0x18, 0x18, 0x3C },
					{ 0x3C, 0x42, 0x41, 0x01, 0x1F, 0x20, 0x40, 0x7F  },
					{ 0x00, 0x3F, 0x01, 0x06, 0x08, 0x06, 0x01, 0x3F },
				 	{ 0x0C, 0x14, 0x34, 0x24, 0x44, 0x7E, 0x04, 0x00 },
				 	{ 0xFC, 0x80, 0x80, 0xFC, 0x04, 0x04, 0x04, 0xFC },
					{ 0xF8, 0x80, 0x80, 0x80, 0xF8, 0x88, 0x88, 0xF8 },
					{ 0xFC, 0x04, 0x08, 0x18, 0x30, 0x60, 0x40, 0x80 },
					{ 0x00, 0xFC, 0x84, 0x84, 0xFC, 0x84, 0x84, 0xFC },
					{ 0xFC, 0x84, 0x84, 0xFC, 0x04, 0x04, 0x04, 0x04 },
					{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff },
					{ 0x33, 0x33, 0xCC, 0xCC, 0x33, 0x33, 0xCC, 0xCC }};
int fil_pat[4][8][8]={
					{
					{1,1,1,1,1,1,1,1},
					{1,1,1,1,1,1,1,1},
					{1,1,1,1,1,1,1,1},
					{1,1,1,1,1,1,1,1},
					{1,1,1,1,1,1,1,1},
					{1,1,1,1,1,1,1,1},
					{1,1,1,1,1,1,1,1},
					{1,1,1,1,1,1,1,1}
					},
					{
					{1,1,0,0,0,0,1,1},
					{1,1,0,0,0,0,1,1},
					{1,1,0,0,0,0,1,1},
					{1,1,0,0,0,0,1,1},
					{1,1,0,0,0,0,1,1},
					{1,1,0,0,0,0,1,1},
					{1,1,0,0,0,0,1,1},
					{1,1,0,0,0,0,1,1}
					},
					{
					{1,1,0,0,1,1,0,0},
					{1,1,0,0,1,1,0,0},
					{0,0,1,1,0,0,1,1},
					{0,0,1,1,0,0,1,1},
					{1,1,0,0,1,1,0,0},
					{1,1,0,0,1,1,0,0},
					{0,0,1,1,0,0,1,1},
					{0,0,1,1,0,0,1,1}
					},
					{
					{0,0,0,0,0,0,0,0},
					{0,1,1,1,0,1,1,1},
					{0,1,0,1,0,1,0,1},
					{0,1,1,1,0,1,1,1},
					{0,0,0,0,0,0,0,0},
					{0,1,1,1,0,1,1,1},
					{0,1,0,1,0,1,0,1},
					{0,1,1,1,0,1,1,1}
					}
					};
char pat_fil[3][8]={
					{ 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00 },
					{ 0xCC, 0xcc, 0x33, 0x33, 0xcc, 0xcc, 0x33, 0x33 },
					{ 0x00, 0x77, 0x55, 0x77, 0x00, 0x77, 0x55, 0x77 }
					};
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX			END OF GLOBAL VARIABLES			XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX



//----------------------		FUNCTIONS 		DECLARATIONS		--------------------------------------

//------------------------MOUSE FUNCTIONS-----------------------
int initmouse();
void showmouseptr();
void hidemouseptr();
void restrictmouseptr(int x1,int y1,int x2,int y2);
void getmousepos(int *button,int *x,int *y);
//------------------------MYCGMAIN.C	------------------------------------------------------------------

void top_panel_default();
void draw_canvas_default();
void start();
void credits_window_anime(); //handles the animation for layout of window
void window_text();
void change_canvas_ptr();
void changeptr(int);
void ask_exit();
//------------------------MYCGMAIN.C 		ENDS--------------------------------------------------------




//-------------------------INTERFACE.C		STARTS-------------------------------------------------------------
void toolbar();
void tools_icons();
void monitor_tools(int,int);
void draw_style();
void style_icon();
void hover_text(int);
void draw_color();
void user_pattern();
void monitor_color(int);
void top_panel_buttons(int,int,int);
void top_panel_background();
void button_hover(int,int,int,int);
void button_click(int,int,int,int);
void canvas();
//----------------------INTERFACE.C			ENDS----------------------------------------


//---------------------MYCGFUNC.C 			STARTS---------------------------------------


void put_pixel(int,int,int);
int user_new();
void user_save();
	void save_bitmap(const char *filename);
	int writeBitmapFile(int xpos, int ypos, const char *filename);
void user_load();
	void load_bitmap(const char *filename);
	int displayBitmapFile(int xpos,int ypos,const char* filename);
	void progress();
	char get_filename(int x,int y);
	void top_message(char *message);
void user_text(int, int );
void pencil();
void plotbrush(int x,int y,int color);
	void brush(int op);
void user_pickcolor();
void Queue(int op, int *x, int *y);
	void bfill(int x,int y);
void user_line();
	void bresline(int,int,int,int,int);
	void drawLine(int);
	void thickline(int,int,int,int,int);
	void drawThickLine(int);
void drawRect(int,int,int,int);
	void drawRectangle();
void plotpoints(int xc,int yc,int x,int y);
	void bellipse(int xc, int yc, float r1,float r2);
	void drawEllipse();
void spray(int x,int y,int color);
	void drawSpray();
void spiral(int sx,int sy,float rad);
	void drawSpiral();
void adjust(int x1,int y1,int x2,int y2,int bg);
	void array_line_entry(int x1,int y1,int x2,int y2);
	void array_line_entry2(int x1,int y1,int x2,int y2);
	void user_clip();
	int cliptest(float p,float q,float *u1,float *u2);
	void clip(int x1,int y1,int x2,int y2,int xmin,int ymin,int xmax,int ymax);
	int round(int);
void user_poly();
void circleallpoints(int xc,int yc,int x,int y);
	void circle34points(int xc,int yc,int x,int y);
	void circle23points(int xc,int yc,int x,int y);
	void circle12points(int xc,int yc,int x,int y);
	void circle14points(int xc,int yc,int x,int y);
	void bcircle(int xc,int yc,int r,int op);
	void drawCircle(int op);
//---------------------------MYCGFUNC.C			ENDS--------------------------------

//----------------------		FUNCTIONS 		DECLARATIONS 		ENDS	------------------------------



//--------------------FUNCTION			DEFINITOINS			----------------------------------------

int initmouse() //intialze the mouse pointer
{
 i.x.ax=0;
 int86(0x33,&i,&o);
 return(o.x.ax);
}
void showmouseptr()  // show the mouse pointer
{
 i.x.ax=1;
 int86(0x33,&i,&o);
}
void hidemouseptr() // hide the mouse pointer
{
 i.x.ax=2;
 int86(0x33,&i,&o);
}
void restrictmouseptr(int x1,int y1,int x2,int y2)  // restrict mouse to given area specified by x1,x2,y1,y2
{
 i.x.ax=7;
 i.x.cx=x1;
 i.x.dx=x2;
 int86(0x33,&i,&o);

 i.x.ax=8;
 i.x.cx=y1;
 i.x.dx=y2;
 int86(0x33,&i,&o);
}
void getmousepos(int *button,int *x,int *y) // get current postion and status of mouse pointer
{
 i.x.ax=3;
 int86(0x33,&i,&o);
 *button=(o.x.bx);
 *x=(o.x.cx);
 *y=(o.x.dx);
}

//--------------------------	FUNCTIONS 		DEFINITIONS 		ENDS----------------------