static int button,mousex,mousey;
static int button_track=-1;	//track the current button focus to avoid repeat


//-------------------This function basically handles the mouse interaction on
//--------------------toolbar and directs to correct places based on tab
void toolbar()
{
	
	canvas_flag=0;
	switch(cur_panel)
	{
		case 0:monitor_tools(tool_but_num,0);
			break;
		case 1:monitor_tools(buttons_data[cur_tool-1],1);
			break;
		case 2:monitor_color(16);
			break;
	}
}

void tools_icons()
{
	int poly[12]={323,3,337,3,330,10,337,17,323,17,323,3};
	int x, y, mx, my, mb,i,j,c=0;
	int pix[16][16];
	int startX=19,startY=0,BUTTON_WIDTH=20,GAP=0;
	FILE* fp = NULL;
	if((fp = fopen("Icons.icl","rb")) !=NULL)
	{
		x = startX+4, y = startY+2;
		while( fread(&pix, sizeof(pix), 1, fp) != NULL)
		{
			for(i=0; i<16; i++)
			{
				for(j=0; j<16; j++)
				{
					if(pix[i][j] != 16)
						putpixel(x+j, y+i, pix[i][j]);
				}
			}
				x+=BUTTON_WIDTH+GAP;
				c++;
				if(c==14)
				break;
		}
		fclose(fp);
	}
	else
	{
		closegraph();
		printf("\n Error: Icons.icl file not found.....");
		getch();

	}
	setcolor(BLUE);
	setlinestyle(1,0xffff,1);
	rectangle(303,3,317,17);
	setlinestyle(0,0xffff,1);
	drawpoly(6,poly);
	circle(350,10,7);
	setfillstyle(1,9);
	bar(368,3,373,17);
	setfillstyle(1,4);
	bar(373,3,378,17);
}


void monitor_tools(int n,int t)
{
	getmousepos(&button,&mousex,&mousey);
	for(;mousey<20;getmousepos(&button,&mousex,&mousey))
	{
	       if((mousex/20>0)&&(mousex/20)<=n&&(mousex/20)!=button_track)
	       {
			button_track=mousex/20;
			top_panel_buttons(n,button_track,2);
			if(t==0)
			{
				hidemouseptr();
				settextstyle(0,0,1);
				if(cur_color==0)
					setcolor(WHITE);
				else
					setcolor(BLACK);
				hover_text(mousex/20);
				showmouseptr();
			}
	       }
	       else if(mousex<20&&button==1&&t!=0)
	       {
			cur_panel=0;
			t=0;
			cur_style=1;
			n=tool_but_num;
			button_hover(2,2,18,18);
			top_panel_default();
			top_panel_buttons(tool_but_num,0,1);
			button_click((2+cur_tool*20),2,(18+cur_tool*20),18);
		   }
		   else if(t==0&&mousex>619&&button==1)
			{
				button_hover(620,2,638,18);
				ask_exit();
			}
	       else if(((mousex/20)==0||(mousex/20>(n+1)))&&button_track!=-1)
			{
			button_track=-1;
			top_panel_buttons(n,0,2);
			if(t==0)
			button_click((2+cur_tool*20),2,(18+cur_tool*20),18);
			else if(t==1)
			button_click((2+cur_style*20),2,(18+cur_style*20),18);
			}
		   if((mousex/20>0)&&(mousex/20)<=n&&button==1)
	       {
			if(t==0)
			{
			if(button_track!=18)
			cur_tool=button_track;
			button_hover((2+button_track*20),2,(18+button_track*20),18);
			if(button_track==18)
			{
			cur_panel=2;
		   draw_color();
			}
			draw_style();
			break;
			}
			else
			{
			cur_style=button_track;
			button_hover((2+cur_style*20),2,(18+cur_style*20),18);
			}
	       }

	}
}

void draw_style()
{
	
	switch(cur_tool)
	{
		case 1:
			user_new();
			break;
		case 2:user_save();
				break;
		case 3:user_load();
			break;
			
		case 10:
		case 17:
			cur_panel=1;
			top_panel_background();
			top_panel_buttons(buttons_data[cur_tool-1],0,1);
			style_icon();
			break;
	}
}

void style_icon()
{
int poly[12]={43,3,57,3,50,10,57,17,43,17,43,3};
switch(cur_tool)
{
case 10:
	setlinestyle(1,0xffff,1);
	setcolor(BLUE);
	line(63,17,77,3);
	setlinestyle(0,0xffff,1);
	setlinestyle(1,0xffff,3);
	setcolor(BLUE);
	line(83,17,97,3);
	setlinestyle(0,0xffff,1);
	setlinestyle(0,0xffff,1);
	setcolor(BLUE);
	line(23,17,37,3);
	setlinestyle(0,0xffff,3);
	setcolor(BLUE);
	line(43,17,57,3);
	setlinestyle(0,0xffff,1);
	break;
case 17:
	setcolor(BLUE);
	circle(30,10,7);
	arc(50,10,0,180,7);
	arc(70,10,90,270,7);
	arc(90,10,180,0,7);
	arc(110,10,270,90,7);
	break;
case 15:
	setcolor(BLUE);
	line(23,17,37,3);
	drawpoly(6,poly);
	setlinestyle(1,0xffff,1);
	rectangle(63,3,77,17);
	setlinestyle(0,0xffff,1);
	break;
	}
}
void hover_text(int n)
{	
if(cur_color==BLACK)
setcolor(WHITE);
else
setcolor(BLACK);
	switch(n)
	{
	case 1:
			setfillstyle(1,cur_color);
			bar(500,3,600,17);
			outtextxy(500,5,"New File");
			break;
	case 2:
			setfillstyle(1,cur_color);
			bar(500,3,600,17);
			outtextxy(500,5,"Save File");
			break;
	case 3:
			setfillstyle(1,cur_color);
			bar(500,3,600,17);
			outtextxy(500,5,"Load File");
			break;
	case 4:
			setfillstyle(1,cur_color);
			bar(500,3,600,17);
			outtextxy(500,5,"Text");
			break;
	case 5:
			setfillstyle(1,cur_color);
			bar(500,3,600,17);
			outtextxy(500,5,"Pencil");
			break;
	case 6:
			setfillstyle(1,cur_color);
			bar(500,3,600,17);
			outtextxy(500,5,"Eraser");
			break;
	case 7:	setfillstyle(1,cur_color);
			bar(500,3,600,17);
			outtextxy(500,5,"Fill Color");
			break;
	case 8:	setfillstyle(1,cur_color);
			bar(500,3,600,17);
			outtextxy(500,5,"Brush");
			break;
	case 9:	setfillstyle(1,cur_color);
			bar(500,3,600,17);
			outtextxy(500,5,"Pick Color");
			break;
	case 10:setfillstyle(1,cur_color);
			bar(500,3,600,17);
			outtextxy(500,5,"Draw Line");
			break;
	case 11:setfillstyle(1,cur_color);
			bar(500,3,600,17);
			outtextxy(500,5,"Rectangle");
			break;
	case 12:setfillstyle(1,cur_color);
			bar(500,3,600,17);
			outtextxy(500,5,"Ellipse");
			break;
	case 13:setfillstyle(1,cur_color);
			bar(500,3,600,17);
			outtextxy(500,5,"Spray");
			break;
	case 14:setfillstyle(1,cur_color);
			bar(500,3,600,17);
			outtextxy(500,5,"Spiral");
			break;
	case 15:setfillstyle(1,cur_color);
			bar(500,3,600,17);
			outtextxy(500,5,"Clipping");
			break;
	case 16:setfillstyle(1,cur_color);
			bar(500,3,600,17);
			outtextxy(500,5,"Polygon");
			break;
	case 17:setfillstyle(1,cur_color);
			bar(500,3,600,17);
			outtextxy(500,5,"Circle");
			break;
	case 18:setfillstyle(1,cur_color);
			bar(500,3,600,17);
			outtextxy(500,5,"Colors");
			break;
	}

}


//----------------------COLOR FUNCTIONS START-----------------
void draw_color()
{
	 int n;
	 top_panel_background();
	 hidemouseptr();
	 for(n=1;n<=16;n++)
	 {
		setfillstyle(1,15);
		bar((3+n*20),3,(17+n*20),17);
		setfillstyle(1,n);
		bar((4+n*20),4,(16+n*20),16);
	 }
	 user_pattern();
	 showmouseptr();
	 monitor_color(16);

}


void user_pattern()
{
	int n,prec;
	n=17;
	prec=cur_color;
	if(cur_color==BLACK)
	cur_color=WHITE;
	setfillstyle(12,cur_color);
	setfillpattern(pat_fil[0],cur_color);
	bar((4+n*20),4,(16+n*20),16);
	n++;
	setfillpattern(pat_fil[1],cur_color);
	bar((4+n*20),4,(16+n*20),16);
	n++;
	setfillpattern(pat_fil[2],cur_color);
	bar((4+n*20),4,(16+n*20),16);
	n++;
	cur_color=prec;
}

void monitor_color(int n)
{
	getmousepos(&button,&mousex,&mousey);
	for(;mousey<20;getmousepos(&button,&mousex,&mousey))
	{
		if(mousex>20&&(mousex/20)<=n&&button==1)
		{
			cur_color=(mousex/20)%16;
			cur_pat=0;
			setfillstyle(1,cur_color);
			bar(500,3,600,17);
			setcolor(BLACK);
			rectangle(500,3,600,17);
			button_hover((2+(mousex/20)*20),2,(18+(mousex/20)*20),18);
		}
		else if(mousex>20&&(mousex/20)<=19&&button==1)
		{
			cur_pat=(mousex/20)-16;
			button_hover((2+(mousex/20)*20),2,(18+(mousex/20)*20),18);
			
		}
		else if(mousex<20&&button==1)
		{
			cur_panel=0;
			button_hover(2,2,18,18);
			top_panel_default();
			top_panel_buttons(tool_but_num,0,1);
			start();
		}
	}
}


//---------------------COLOR	ENDS----------------------


void top_panel_buttons(int n,int a,int c)
{
	hidemouseptr();
	for(;n>0;n--)
	{
		setfillstyle(1,3);				//2,3
		if(n==a)
		setfillstyle(1,c);
		bar((2+n*20),2,(18+n*20),18);
	}
	if(cur_panel==0)
	tools_icons();
	else if(cur_panel==1)
	style_icon();
	showmouseptr();
}


void top_panel_background()
{
	int back_arrow[16]={2,10,10,18,10,15,17,15,17,5,10,5,10,2,2,10};
	hidemouseptr();
	setfillstyle(1,BLUE);
	bar(0,0,xmax,20);
	setfillstyle(1,GREEN);
	fillpoly(8,back_arrow);
	setfillstyle(1,cur_color);
	bar(500,3,600,17);
	setcolor(BLACK);
	rectangle(500,3,600,17);
	showmouseptr();
}



void button_hover(int x1,int y1,int x2,int y2)
{
	hidemouseptr();
	setcolor(WHITE);
	line(x1,y2,x1,y1);
	line(x1,y1,x2,y1);
	setcolor(BLACK);
	line(x2,y1,x2,y2);
	line(x2,y2,x1,y2);
	showmouseptr();
	delay(100);
	button_click(x1,y1,x2,y2);
}

void button_click(int x1,int y1,int x2,int y2)
{
	hidemouseptr();
	setcolor(BLACK);
	line(x1,y2,x1,y1);
	line(x1,y1,x2,y1);
	setcolor(WHITE);
	line(x2,y1,x2,y2);
	line(x2,y2,x1,y2);
	showmouseptr();
	delay(200);
}

void canvas()
{	
	int button,mousex,mousey;	//local mouse variables
	if(cur_panel==0&&canvas_flag!=1)
	{
	top_panel_buttons(tool_but_num,0,5);
	hover_text(cur_tool);
	button_click((2+cur_tool*20),2,(18+cur_tool*20),18);
	canvas_flag=1;
	}
	else if(cur_panel==1&&canvas_flag!=2)
	{
	hover_text(cur_tool);
	top_panel_buttons(buttons_data[cur_tool-1],0,5);
	button_click((2+cur_style*20),2,(18+cur_style*20),18);
	canvas_flag=2;
	}
	hover_text(cur_tool);
	getmousepos(&button,&mousex,&mousey);
	for(;mousey>20;getmousepos(&button,&mousex,&mousey))
	{
		if(button==1)
		{
			restrictmouseptr(0,27,xmax,ymax);
			switch(cur_tool)
			{
				case 4: user_text(mousex,mousey);
						break;
				case 5: pencil();
						break;
				case 6: brush(2);
						break;
				case 7: bfill(mousex,mousey);
						break;
				case 8: brush(1);
						break;
				case 9: user_pickcolor();
						break;
				case 10: user_line();
						break;
				case 11: drawRectangle();
						break;
				case 12: drawEllipse();
						break;
				case 13: drawSpray();
						break;
				case 14: drawSpiral();
						break;
				case 15:user_clip();
						break;
				case 16:user_poly();
						break;
				case 17:drawCircle(cur_style);
						break;

			}

		}
		else
		{
			restrictmouseptr(0,0,xmax,ymax);
			
		}
	}
}

