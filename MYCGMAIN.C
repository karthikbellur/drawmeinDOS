/*
*this is the main file from which the code starts executing
*/



#include"uni.h"
#include"gui.c"
#include"mycgfunc.c"

static int crex,crey;	//provides storing the middle co-ordinates for credits animation window

void main()
{
	int gd=DETECT,gm; //variables for initialization of graph
	initgraph(&gd,&gm,"c:\\tc\\bgi"); // the actual initialzation function
	initmouse();
	showmouseptr();
	xmax=getmaxx();
	ymax=getmaxy();
	top_panel_default();
	top_panel_buttons(tool_but_num,0,1);
	draw_canvas_default();
	credits_window_anime();
	start();
}

//this is central function to draw the top_panel/toolbar in its initial
//configuration
void top_panel_default()
{
	hidemouseptr();
	//----------------------background drawing starts
	setfillstyle(1,BLUE);
	bar(0,0,xmax,20);
	setcolor(WHITE);
	line(0,21,xmax,21);
	//----------------------background drawing ends

	//------------------------start drawing the close button
	setfillstyle(1,RED);
	bar((xmax-13),4,(xmax-4),11);
	setcolor(BLACK);
	outtextxy((xmax-12),5,"X");
	setfillstyle(1,cur_color);
	bar(500,3,600,17);
	setcolor(BLACK);
	rectangle(500,3,600,17);
	//-----------------------close button is drawn
	showmouseptr();


}

//restores the canvas back to inital state... useful in NEW layout tools
void draw_canvas_default()
{   hidemouseptr();
	setfillstyle(1,WHITE);
	bar(0,21,xmax,ymax);
	showmouseptr();
}


//this is the function after the initialization and credits will
//start the monitoring of mouse activity and take necessary action
void start()
{
	int button,mousex,mousey;	//local variables which hold the present position and condition of mouse
	for(;;)
	{
		getmousepos(&button,&mousex,&mousey);
		if(mousey<=20)
		{
			changeptr(1);
			toolbar();
		}
	     else
		 {	change_canvas_ptr();
			canvas();
		}
	 }
}



//This function provides the animation for opening of CREDITS window
void credits_window_anime()
{
	int i;
	hidemouseptr();
	crey=ymax/2+75;
	crex=xmax/2;
	for(i=0;i<=10;i++)
	{
		setcolor(BROWN);
		line(crex,crey,(crex+i*10),crey);
		line(crex,crey,(crex-i*10),crey);
		delay(40);
	}
	for(i=1;i<=10;i++)
	{
		setfillstyle(SOLID_FILL,BROWN);
		bar((crex-100),(crey-i*15),(crex+100),crey);
		delay(50);
	}
	window_text();
}




//below is the function to put text into the credits page
void window_text()
{
	int button,mousex,mousey;
	settextstyle(4,0,2);
	setcolor(BLACK);
	outtextxy((crex-65),(crey-150+12),"C r e d i t S");
	settextstyle(0,0,1);
	outtextxy((crex-90),(crey-150+50),"Veeresh B");
	outtextxy((crex-90),(crey-150+80),"Karthik Bellur");
	showmouseptr();
	 for(;;)
	 {
		 getmousepos(&button,&mousex,&mousey);
		 if(button!=0)
		{
			draw_canvas_default();
			break;
		}
	 }
}
//----------------------POINTER CHANGING FUNCTIONS-----------------------------------------------------------------------
void change_canvas_ptr()
{
	switch(cur_tool)
	{
		case 1:
		case 2:
		case 3:
		changeptr(1);
		break;
		case 4:
		changeptr(3);
		break;
		case 5:
		changeptr(4);
		break;
		case 6:
		changeptr(6);
		break;
		case 7:
		changeptr(5);
		break;
		default:
		changeptr(2);
	}
}



void changeptr(int type)
{
	int left,top,addr;

	switch(type)
	{
	case 1:
		//arrow pointer
		left=0;
		top=0;
		addr=(int) arrow;
		break;

	case 2:
		//cross pointer
		left=7;
		top=7;
		addr=(int) cross;
		break;

	case 3:
		//text pointer
		left=9;
		top=1;
		addr=(int)text;
		break;

	case 4:
		//pen pointer
		left=3;
		top=15;
		addr=(int)pen;
		break;

	case 5:
		//bucket pointer
		left=2;
		top=15;
		addr=(int)bucket;
		break;
	case 6:
		//eraser pointer
		left=8;
		top=8;
		addr=(int)eraser;
		break;
	}//end of switch
	i.x.ax=9;
	i.x.bx=left;//bit from left
	i.x.cx=top;//bit from top
	i.x.dx=addr;//address of pointer mask
	segread(&sregs);
	sregs.es=sregs.ds;

	int86x(0x33,&i,&o,&sregs);

	i.x.ax=0x01;
	int86(0x33,&i,&o);

}



//this is the function which pops up the menu to confirm/deny the user's
//intention to quit
void ask_exit()
{

	int a,b,button,mousex,mousey;	//a and b are local variables defined to
					//reduce the complexity in code
	user_save();
	restrictmouseptr(0,0,xmax,20);
	hidemouseptr();
	a=textwidth("Really Exit:")+10;
	setfillstyle(1,BLUE);
	bar(0,0,xmax,20);
	settextstyle(0,0,1);
	setcolor(BLACK);
	outtextxy(10,5,"Really Exit:");
	setfillstyle(1,CYAN);
	bar((a+18),1,(a+18+textwidth("Ya")+2),18);
	outtextxy((a+20),5,"Ya");
	b=a+textwidth("Ya")+10;
	bar((b+18),1,(b+18+textwidth("Na")+2),18);
	outtextxy((b+20),5,"Na");
	showmouseptr();
	for(;;)
	{
		getmousepos(&button,&mousex,&mousey);
		if(mousex>(a+20)&&mousex<b+10&&button==1)      //exit confirm
		{
			 initmouse();
			 button_hover((a+18),1,(a+18+textwidth("Ya")+2),18);
			 showmouseptr();
			 exit(1);
		}
	else if(mousex>b+20&&mousex<(b+36)&&button==1)        //deny exit
		{
			hidemouseptr();
			initmouse();
			button_hover((b+18),1,(b+18+textwidth("Na")+2),18);
			top_panel_default();
			top_panel_buttons(tool_but_num,0,1);
			showmouseptr();
			break;
		}
	}
}

