void put_pixel(int x, int y, int color)
{
	union REGS i;
	if(y<21||x>getmaxx()||x<0)
	return;
	i.h.ah=0x0C;
	i.h.al=( color | draw_mode );
	i.h.bh=0;
	i.x.cx=x;
	i.x.dx=y;
	int86(0x10,&i,&i);
}






int user_new()
{
	int a,b;
	restrictmouseptr(0,0,xmax,20);
	hidemouseptr();
	a=textwidth("This Action will erase current drawing, Continue?")+10;
	setfillstyle(1,BLUE);
	bar(0,0,xmax,20);
	settextstyle(0,0,1);
	setcolor(BLACK);
	outtextxy(10,5,"This Action will erase current drawing, Continue?");
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
		if(mousex>(a+20)&&mousex<b+10&&button==1)      //new confirm
		{
			hidemouseptr();
			initmouse();
			draw_canvas_default();
			top_panel_default();
			top_panel_buttons(tool_but_num,0,1);
			showmouseptr();
			arrcnt=-1;
			return 1;
		}
	else if(mousex>b+20&&mousex<(b+36)&&button==1)        //deny new
		{
			hidemouseptr();
			initmouse();
			top_panel_default();
			top_panel_buttons(tool_but_num,0,1);
			showmouseptr();
			return 0;
		}
	}
}


void user_save()
{
	int a,b,w,px,i=-1;
	char filename[60],ch;
	restrictmouseptr(0,0,xmax,20);
	hidemouseptr();
	a=textwidth("Enter File Name");
	setfillstyle(1,BLUE);
	bar(0,0,xmax,20);
	setcolor(BLACK);
	outtextxy(10,5,"Enter File Name:");
	setfillstyle(1,WHITE);
	bar((a+10),2,(a+410),18);
	setfillstyle(1,CYAN);
	a=textwidth("No");
	bar((xmax-10-a),2,(xmax-10),18);
	b=a+10+textwidth("Save");
	bar((xmax-10-b),2,(xmax-20-a),18);
	setcolor(BLACK);
	outtextxy((xmax-9-b),3,"Save");
	outtextxy((xmax-9-a),3,"No");
	px=textwidth("Enter File Name")+10;
	while(1)
	{
	ch=get_filename(px,4);
	if(ch==13||ch==27)
	break;
	w=textwidth("A");
	px+=w;
	filename[++i]=ch;
	}
	filename[++i]='\0';
	showmouseptr();
	for(;;)
	{
		getmousepos(&button,&mousex,&mousey);
		if(mousex>(xmax-10-a)&&mousex<(xmax-10)&&button==1)       //deny loading/saving
		{
			button_hover((xmax-10-a),2,(xmax-10),18);
			hidemouseptr();
			initmouse();
			top_panel_default();
			top_panel_buttons(tool_but_num,0,1);
			showmouseptr();
			break;
		}
	else if(mousex>(xmax-10-b)&&mousex<(xmax-20-a)&&button==1)        //confirm loading/saving
		{
			button_hover((xmax-10-b),2,(xmax-20-a),18);
			save_bitmap(filename);
			hidemouseptr();
			initmouse();
			top_panel_default();
			top_panel_buttons(tool_but_num,0,1);
			showmouseptr();
			break;
		}
	}
}



void save_bitmap(const char *filename)
{
	int r,be;
	r = access(filename,0);
	if( r == 0)
	{
		top_message("file already exists,Overwriting");
	}
		hidemouseptr();
		top_panel_default();
		setcolor(RED);
		outtextxy(200,3,"Please Wait");
		prgx=1;
		setcolor(BLACK);
		rectangle(0,2,ymax,18);
		be = writeBitmapFile(0,0,filename);
		showmouseptr();
		if( be != 0)
		{
		   top_message("Error While Creating File");
		   return;
		}
		else
		      {
		      top_message("File saved successfully");
		      }
	return;
}




int writeBitmapFile(int xpos, int ypos, const char *filename)
{
	FILE *bitmap_file;
	int x,y;
	BITMAP_PALETTE pal_16[16] = { {0,0,0,0},{170,0,0,0},{0,170,0,0},{170,170,0,0},
	{0,0,170,0},{170,0,170,0},{0,85,170,0},{170,170,170,0},{85,85,85,0},
	{255,85,85,0},{85,255,85,0},{255,255,85,0},{85,85,255,0},{255,85,255,0},
	{85,255,255,0},{255,255,255,0}};
	if((bitmap_file = fopen(filename,"wb")) == NULL)
		return 1;
		// BITMAP FILE HEADER INFORMATION
	t_bfh.type[0]='B', t_bfh.type[1]='M';
	t_bfh.size=152600, t_bfh.reserved1=0, t_bfh.reserved2=0;
	t_bfh.offset=118;
		// BITMAP INFORMATION HEADER DATA
	t_bih.size=40, t_bih.width=xmax, t_bih.height=ymax;
	t_bih.planes=1, t_bih.bitsPerPixel=4, t_bih.compression=0;
	t_bih.sizeImage=0, t_bih.horzRes=2592, t_bih.vertRes=2592;
	t_bih.colorsUsed=0, t_bih.impColors=0;
	fwrite(&t_bfh, sizeof(t_bfh), 1, bitmap_file);
	fwrite(&t_bih, sizeof(t_bih), 1, bitmap_file);
	// WRITE COLOR PALETTE (AS THE BITMAP IMAGE IS INDEXED)
	fwrite(&pal_16, sizeof(BITMAP_PALETTE)*16, 1, bitmap_file);
	fseek(bitmap_file, t_bfh.offset, SEEK_SET);
	for(y=t_bih.height; y>=0; y--)
	{
		for(x=0; x < t_bih.width; x+=2)
		{
			unsigned char px = getpixel(xpos+x, ypos+y)<<4;
			px |= ((getpixel(xpos+x+1, ypos+y))<<4>>4);
			fputc(px, bitmap_file);
		}
		progress();
	}
	fclose(bitmap_file);
	return 0;
}

void user_load()
{
	int a,b,w,px,i=-1;
	char filename[60],ch;
	restrictmouseptr(0,0,xmax,20);
	hidemouseptr();
	a=textwidth("Enter File Name");
	setfillstyle(1,BLUE);
	bar(0,0,xmax,20);
	settextstyle(0,0,1);
	setcolor(BLACK);
	outtextxy(10,5,"Enter File Name:");
	setfillstyle(1,WHITE);
	bar((a+10),2,(a+410),18);
	setfillstyle(1,CYAN);
	a=textwidth("No");
	bar((xmax-10-a),2,(xmax-10),18);
	b=a+10+textwidth("Load");
	bar((xmax-10-b),2,(xmax-20-a),18);
	setcolor(BLACK);
	outtextxy((xmax-9-b),3,"Load");
	outtextxy((xmax-9-a),3,"No");
	px=textwidth("Enter File Name")+10;
	while(1)
	{
	ch=get_filename(px,4);
	if(ch==13||ch==27)
	break;
	w=textwidth("A");
	px+=w;
	filename[++i]=ch;
	}
	filename[++i]='\0';
	showmouseptr();
	for(;;)
	{	
		getmousepos(&button,&mousex,&mousey);
		if(mousex>(xmax-10-a)&&mousex<(xmax-10)&&button==1)      //deny loading/saving
		{
			button_hover((xmax-10-a),2,(xmax-10),18);
			hidemouseptr();
			initmouse();
			top_panel_default();
			top_panel_buttons(tool_but_num,0,1);
			showmouseptr();
			break;
		}
	else if(mousex>(xmax-10-b)&&mousex<(xmax-20-a)&&button==1)        //confirm loading/saving
		{
			button_hover((xmax-10-b),2,(xmax-20-a),18);
			if(user_new())
			load_bitmap(filename);
			hidemouseptr();
			initmouse();
			top_panel_default();
			top_panel_buttons(tool_but_num,0,1);
			showmouseptr();
			break;
		}
	}
}	

void load_bitmap(const char *filename)
{
	int be;
	hidemouseptr();
	top_panel_default();
	setcolor(RED);
	outtextxy(200,3,"Please Wait");
	prgx=1;
	setcolor(BLACK);
	rectangle(0,2,ymax,18);
    be = displayBitmapFile(0, 0, filename);
	showmouseptr();
	if(be != 0)
	{
		top_message("File Read Error");
		return;
	}
		
	else
		top_message("File Opened Successfully");
	return;
}



int displayBitmapFile(int xpos,int ypos,const char* filename)
{
	FILE *bitmap_file;
	int x, y, i,correct;
	unsigned char pixel;
	if((bitmap_file = fopen(filename,"rb")) == NULL)
		return 1;
	if( fread(&bfh,sizeof(bfh),1,bitmap_file) != 1)
	{
		fclose(bitmap_file);
		return 2;
	}
	if( fread(&bih, sizeof(bih),1,bitmap_file) != 1)
	{
		fclose(bitmap_file);
		return 3;
	}
	if( bfh.type[0] != 'B' || bfh.type[1] != 'M')
	{
		fclose(bitmap_file);
		return 4;
	}
	if( bih.compression != 0)
	{
		fclose(bitmap_file);
		return 5;
	}
	if( bih.bitsPerPixel != 4)	// Color depth should be 4 (i.e. 16 color)
	{
		fclose(bitmap_file);
		return 6;
	}
	fseek(bitmap_file, bfh.offset, SEEK_SET);
	correct = bih.width % 8;
	if( correct != 0)
		correct = 8 - correct;

	for(y=bih.height; y>=0; y--)
	{
		i=0;
		for(x=0; x < bih.width+correct; x+=2)
		{
			pixel = fgetc(bitmap_file);
			if( ++i <= bih.width)
			putpixel(xpos+x, ypos+y, (pixel>>4) );
			if( ++i <= bih.width)
			putpixel(xpos+x+1, ypos+y, ((pixel<<4)>>4) );
		}
		progress();
	}
	fclose(bitmap_file);
	return 0;
}

void progress()
{
setcolor(GREEN);
line(prgx,2,prgx,18);
prgx++;
}

char get_filename(int x,int y)
{
	char text[2],ch,sp,i=0;
	setcolor(BLACK);
	settextstyle(0,0,1);
	if(!(_bios_keybrd(_KEYBRD_READY)==0))
	delay(100);
	setcolor(BLACK);
	ch=_bios_keybrd(_KEYBRD_READ);
	text[0]=ch;
	text[1]='\0';
	settextstyle(0,0,1);
	if(!(ch==13||ch==27))
	outtextxy(x,y,text);
	return ch;
}

void top_message(char *message)
{
	top_panel_default();
	restrictmouseptr(0,0,xmax,20);
	hidemouseptr();
	setcolor(BLACK);
	outtextxy(10,3,message);
	showmouseptr();
	for(;;)
	{	
		getmousepos(&button,&mousex,&mousey);
		if(mousex>619&&button==1)      
		{
			hidemouseptr();
			button_hover((xmax-13),4,(xmax-4),11);
			initmouse();
			showmouseptr();
			return;
		}
	}
}


void user_text(int x,int y)
{
	char text[2],ch,sp,bg;
	int i=0,ind;
	int flag=1;
	hidemouseptr();
	x=x/16;
	x=x*16;
	y=y/16;
	y=y*16;
	while(flag)
	{
		while((_bios_keybrd(_KEYBRD_READY)==0))
		{
			setcolor(BLACK);
			line(x-1,y-1,x-1,y+10);
			i=1;
		}
		setcolor(WHITE);
		line(x-1,y-1,x-1,y+10);
		setfillstyle(12,WHITE);
		ch=_bios_keybrd(_KEYBRD_READ);
		if(i==0)
			continue;
		if(x>xmax-16)
			{
			x=0;
			y+=16;
			}
		if(y>ymax-16)
			break;
		else
		{
			i=0;
			switch(ch)
			{
				case 27:
					flag=0;
					break;
				case 13:
					y+=16;
					x=0;
					break;
				case 32:
					x+=16;
					break;
				case 8:
					x-=16;
					setfillstyle(1,WHITE);
					bar(x,y,x+7,y+7);
					break;
				default:
					if((ch>=65&&ch<=90)||(ch>=97&&ch<=122)||(ch>=48&&ch<=57))
					{
						if(ch<58)
						ind=(ch-'0')+26;
						else if(ch<91)
						ind=(ch-'A');
						else if(ch<123)
						ind=(ch-'a');
						bg=getpixel(x,y);
						setfillpattern(pat[ind],RED);
						bar(x,y,x+7,y+7);
						adjust(x,y,x+7,y+7,bg);
						x+=16;
					}
					else
					{
						text[0]=ch;
						text[1]='\0';
						setcolor(cur_color);
						settextstyle(0,0,1);
						outtextxy(x,y,text);
						x+=16;
					}
			}
		}	
	}
	showmouseptr();
 }

 
 void pencil()
{
int x,y,button,prevx,prevy;
getmousepos(&button,&x,&y);
if(button==1)
{
 hidemouseptr();
 prevx=x;
 prevy=y;
 while(button==1)
 {
	if(!((abs(prevx-x))<=5&&(abs(prevy-y))<=5))
  {bresline(prevx,prevy,x,y,1);
  prevx=x;
  prevy=y;}
  getmousepos(&button,&x,&y);
 }
 showmouseptr();
}
}


void plotbrush(int x,int y,int color)
{
int i,j;	//i & j -> thickness of brush
for(i=-5;i<=5;i++)
for(j=-5;j<=5;j++)
putpixel(x+i,y+j,color);

}

void brush(int op)
{
	int b,sx,sy,prec;
	prec=cur_color;
	if(op==2)
	set_color(WHITE);

	getmousepos(&b,&sx,&sy);
	if(b==1)
	hidemouseptr();
	while(b==1)
	{
	getmousepos(&b,&sx,&sy);
	plotbrush(sx,sy,cur_color);
	}
	set_color(prec);	
showmouseptr();
}

void user_pickcolor()
{
	int button,mousex,mousey;
	getmousepos(&button,&mousex,&mousey);
	cur_color=getpixel(mousex,mousey);
	setfillstyle(1,cur_color);
	bar(500,3,600,17);
	setcolor(BLACK);
	rectangle(500,3,600,17);
	hover_text(cur_tool);
}

void Queue(int op, int *x, int *y)
{
NODE *t=NULL;
	switch(op)
	{
		case 1: 	if(rear != NULL)	//add
					{
						rear->next = (NODE*) malloc(sizeof(NODE));
						if(rear->next != NULL)
						{
							rear = rear->next;
							rear->x = *x, rear->y = *y;
							rear->next = NULL;
						}
						else
						{
							*x=-1;
							return;
						}
					}
					else
					{
						rear = (NODE*) malloc(sizeof(NODE));
						if(rear!=NULL)
						{
						rear->x = *x, rear->y = *y;
						rear->next = NULL;
						if( front == NULL )
						front = rear;
						}
						else
						{
							*x=-1;
							return;
						}
					}
					break;

		case 2	:	if(front != NULL)	//delete
					{
						*x = front->x, *y = front->y;
						t = front;
						front = front->next;
						if(front == NULL)
						rear = front = NULL;
						free(t);
					}
					else
						*x = -1;

					break;
	}
}


void bfill(int x,int y)
{
	int b,x1, f_up, f_down,xmax,ymax,tmp;
	int i,j;
	hidemouseptr();
	xmax=getmaxx();
	ymax=getmaxy();
	front = rear = NULL;
	bgcolor=getpixel(x,y);
	if(bgcolor==cur_color)
	{
	showmouseptr();
	return;
	}
	while(x != -1)
	{
		x1 = x, f_up = f_down = 0;
		while(getpixel(x1, y) == bgcolor&&(x1>0)&&(x1<xmax)&&y>21&&y<ymax)
		{
			if( getpixel(x1, y-1) == bgcolor && f_up == 0)
				{
					int t = y-1;
					Queue(1, &x1, &t);
					f_up = 1;
				}
			else if( getpixel(x1, y-1) != bgcolor)
					f_up = 0;
			if( getpixel(x1, y+1) == bgcolor && f_down == 0)
				{
					int t = y+1;
					Queue(1, &x1, &t);
					f_down = 1;
				}
			else if( getpixel(x1, y+1) != bgcolor)
					f_down = 0;
			i=x1%8;
			j=y%8;
		    if(fil_pat[cur_pat][i][j]==1)
			putpixel(x1, y, cur_color);
			else
			{
			tmp=BLACK;
			if(bgcolor==BLACK)
			tmp=WHITE;
			putpixel(x1, y,tmp);
			}
			x1--;
		}
		x1 = x + 1;
		while(getpixel(x1, y) == bgcolor&&(x1<xmax)&&(x1>0)&&y>21&&y<ymax)
		{
			if( getpixel(x1, y-1)==bgcolor && f_up == 0)
				{
					int t = y-1;
					Queue(1, &x1, &t);
					f_up = 1;
				}
			else if( getpixel(x1, y-1) != bgcolor)
					f_up = 0;
			if( getpixel(x1, y+1)== bgcolor && f_down == 0)
				{
					int t = y+1;
					Queue(1, &x1, &t);
					f_down = 1;
				}
			else if( getpixel(x1, y+1) == bgcolor )
					f_down = 0;
			i=x1%8;
			j=y%8;
		    if(fil_pat[cur_pat][i][j]==1)
			putpixel(x1, y, cur_color);
			else
			{
			tmp=BLACK;
			if(bgcolor==BLACK)
			tmp=WHITE;
			putpixel(x1, y,tmp);
			}
			x1++;
		}
		Queue(2, &x, &y);
	}
	//printf("akfhjkds");
	showmouseptr();
}


void user_line()
{
	switch(cur_style)
	{
		case 1:drawLine(1);
				break;
		case 2:drawThickLine(1);
				break;
		case 3:drawLine(2);
				break;
		case 4:drawThickLine(2);
				break;
	}
}


void bresline(int x1,int y1,int x2,int y2,int op)  //op=1->normal,op=2->dashed
{
	int dx,dy,x,y,xend,yend,dirx=-1,diry=-1,p;
	int a[20],i;
	for(i=0;i<8;i++)
		a[i]=1;
	if(op==1)  //normal-line
		for(i=8;i<16;i++)
			a[i]=1;
	else if(op==2)
	for(i=8;i<16;i++)
		a[i]=0;	//dashed-line
	dx=abs(x2-x1);
	dy=abs(y2-y1);
	if(x2>x1)
		dirx=1;
	if(y2>y1)
	diry=1;
	x=x1;y=y1;
	put_pixel(x1,y1,cur_color);
	put_pixel(x2,y2,cur_color);
	if(x1==x2&&y1==y2)
		return;
	if(dx>dy)
	{
	p=dirx*(2*dy-dx);
    for(x=x1;x!=x2;x+=dirx)
    {
	if(p<0)
	{
	 if(a[(i++)%16]==1)
	 put_pixel(x,y,cur_color);
	 p=p+2*dy;
	}
	else
	{
	 if(a[(i++)%16]==1)
	 put_pixel(x,y,cur_color);
	 y+=diry;
	 p=p+2*(dy-dx);
	}
    }
}
else if(dx<dy)
{
  p=diry*(2*dx-dy);
  for(y=y1;y!=y2;y+=diry)
  {
   if(p<0)
   {
   if(a[(i++)%16]==1)
   put_pixel(x,y,cur_color);
   p=p+2*dx;
   }
   else
   {
    if(a[(i++)%16]==1)
    put_pixel(x,y,cur_color);
    x+=dirx;
    p=p+2*(dx-dy);
   }
  }
}
else
{
 for(x=x1;x!=x2;x+=dirx)
 {
  if(a[(i++)%16]==1)
  put_pixel(x,y,cur_color);
  y+=diry;
 }
}
}

void drawLine(int op)
{
	int b, sx, sy, ex, ey, px, py;
	int c=cur_color;
	int x1=-1,y1=-1,x2=-1,y2=-1;
	setdrawmode(XOR_MODE);
	getmousepos(&b,&sx,&sy);
	set_color(15-c);
	ex = px = sx; ey = py = sy;
	if(b==1)
    hidemouseptr();
	while(b==1)
	{
	getmousepos(&b, &ex, &ey);
	if(px != ex || py != ey)
	{
		bresline(sx, sy, px, py,op);
		bresline(sx, sy, ex, ey,op);
		px = ex; py = ey;
		x1=sx;y1=sy;
		x2=ex;y2=ey;
	}
	}
	set_color(c);
	setdrawmode(COPY_MODE);
	array_line_entry(x1,y1,x2,y2);
	bresline(x1, y1, x2, y2,op);
	showmouseptr();
}

void thickline(int x1,int y1,int x2,int y2,int op)
{
int i;	//i->thickness of line
float m;
if(x1==x2)
{
for(i=-2;i<=2;i++)
bresline(x1+i,y1,x2+i,y2,op);
return;
}
m=abs(y2-y1)/abs(x2-x1);
if(m<1)
{
for(i=-2;i<=2;i++)
bresline(x1,y1+i,x2,y2+i,op);
}
else if(m>=1)
{
for(i=-2;i<=2;i++)
bresline(x1+i,y1,x2+i,y2,op);
}
}

void drawThickLine(int op)
{
	int b, sx, sy, ex, ey, px, py;
	int c=cur_color;
	int x1=-1,y1=-1,x2=-1,y2=-1;
	setdrawmode(XOR_MODE);
	getmousepos(&b,&sx,&sy);
	set_color(15-c);
	ex = px = sx; ey = py = sy;
    if(b==1)
    hidemouseptr();
	while(b==1)
	{
	getmousepos(&b, &ex, &ey);
	if(px != ex || py != ey)
	{
		thickline(sx, sy, px, py,op);
		thickline(sx, sy, ex, ey,op);
		px = ex; py = ey;
		x1=sx;y1=sy;
		x2=ex;y2=ey;
	}
	}
	set_color(c);
	setdrawmode(COPY_MODE);
	thickline(x1, y1, x2, y2,op);
	showmouseptr();
}


void drawRect(int x1,int y1,int x2,int y2)
{

bresline(x1,y1,x2,y1,1);
bresline(x1,y1,x1,y2,1);
bresline(x2,y1,x2,y2,1);
bresline(x1,y2,x2,y2,1);
}

void drawRectangle()
{
  int b, sx, sy, ex, ey, px, py;
  int c=cur_color;
  int x1=0,y1=0,x2=0,y2=0;
  setdrawmode(XOR_MODE);
  getmousepos(&b, &sx, &sy);
  set_color(15-c);
  ex = px = sx, ey = py = sy;
  if(b==1)
  hidemouseptr();
  while(b == 1)
  {
	getmousepos(&b, &ex, &ey);
	if(px != ex || py != ey)
	{
		drawRect(sx, sy, px, py);
		drawRect(sx, sy, ex, ey);
		px = ex, py = ey;
		x1=sx;y1=sy;
		x2=ex;y2=ey;
	}
	}
	set_color(c);
	setdrawmode(COPY_MODE);
	array_line_entry(x1,y1,x2,y1);
	array_line_entry(x1,y1,x1,y2);
	array_line_entry(x2,y1,x2,y2);
	array_line_entry(x1,y2,x2,y2);
	drawRect(x1, y1, x2, y2);
	showmouseptr();
}


void plotpoints(int xc,int yc,int x,int y)
{
put_pixel(xc+x,yc+y,cur_color);
put_pixel(xc+x,yc-y,cur_color);
put_pixel(xc-x,yc+y,cur_color);
put_pixel(xc-x,yc-y,cur_color);
}
void bellipse(int xc, int yc, float r1,float r2)
{
	float p, slope=0, sqr;
	int x, y, lasty;

	if (r1!=0)
		sqr=(r2*r2)/(r1*r1);
	else
		return;

	p = 2*sqr-2*r2+1;
	x = 0;
	y = r2;

	while(y && slope>-1)
	{
		plotpoints(xc,yc,x,y);

		if (p<0)
			p+=4*sqr*x+6*sqr;
		else
		{ 	p+=4*sqr*x-4*y+6*sqr+4;
			y--;
		}
		x++;

		if(y==0 || sqr==0) return;

		slope=(float) x/y*sqr*(-1);
	}

	lasty=y, y=0, x=r1;

	if (r2!=0)
		sqr=(r1*r1)/(r2*r2);
	else
		return;

	slope=-2;
	p=2*sqr-2*r1+1;

	while(x && y<=lasty)
	{
		plotpoints(xc,yc,x,y);

		if (p<0)
			p+=4*sqr*y+6*sqr;
		else
		{ 	p+=4*sqr*y-4*x+6*sqr+4;
			x--;
		}
		y++;
	}
}

void drawEllipse()
{
	int b, sx, sy, ex, ey, px, py;
	int c=cur_color;
	float r1=0,r2=0;
	setdrawmode(XOR_MODE);
	getmousepos(&b,&sx,&sy);
	set_color(15-c);
	ex = px = sx;ey = py = sy;
	if(b==1)
    hidemouseptr();
	while(b == 1)
	{
		getmousepos(&b,&ex,&ey);
		if(px != ex || py != ey)
		{
			bellipse((px+sx)/2, (py+sy)/2, r1, r2);
			px = ex;py = ey;
			r1 = (sx<px)?(px-sx)/2:(sx-px)/2;
			r2 = (sy<py)?(py-sy)/2:(sy-py)/2;
			bellipse((px+sx)/2, (py+sy)/2, r1, r2);
		}
	}
		
		r1 = (sx<px)?(px-sx)/2:(sx-px)/2;
		r2 = (sy<py)?(py-sy)/2:(sy-py)/2;
		set_color(c);
		setdrawmode(COPY_MODE);
		bellipse((px+sx)/2, (py+sy)/2,r1, r2);
		showmouseptr();
}



void spray(int x,int y,int color)
{
putpixel(x,y,color);
putpixel(x-random(10),y+random(20),color);
delay(random(50));
}
void drawSpray()
{
	int b,sx,sy;
	getmousepos(&b,&sx,&sy);
	if(b==1)
	hidemouseptr();
	while(b==1)
	{
	getmousepos(&b,&sx,&sy);
	spray(sx,sy,cur_color);
	}
	showmouseptr();
	}
	
	
	
void spiral(int sx,int sy,float rad)
{
	float t,s,i=10,x,y;
	for(t=0;t<=i*M_PI;t+=0.01)
	{
		s = rad * (t/(i*M_PI));
		x = sx + s * cos(t);
		y = sy + s * sin(t);
		put_pixel(x,y,cur_color);
	}
}
void drawSpiral()
{
	int b, sx, sy, ex, ey, px, py;
	int c=cur_color;
	float r;
	setdrawmode(XOR_MODE);
	getmousepos(&b,&sx,&sy);
	set_color(15-c);
	ex = px = sx;ey = py = sy;
	if(b==1)
    hidemouseptr();
	while(b == 1)
	{
		if(px != ex || py != ey)
		{
			set_color(cur_color);
			spiral((px+sx)/2, (py+sy)/2, r);
			px = ex; py = ey;
			r = (sx<px)?(px-sx)/2:(sx-px)/2;
			spiral((px+sx)/2, (py+sy)/2, r);
		}
		getmousepos(&b,&ex,&ey);
	}
		set_color(c);
		setdrawmode(COPY_MODE);
		r = (sx<px)?(px-sx)/2:(sx-px)/2;
		spiral((px+sx)/2, (py+sy)/2, r);
		showmouseptr();
}




void adjust(int x1,int y1,int x2,int y2,int bg)
{
int x,y,c;
for(y=y1;y<=y2;y++)
for(x=x1;x<=x2;x++)
{
c = getpixel(x,y);
if(c==BLACK)
putpixel(x,y,bg);
if(c==RED)
putpixel(x,y,cur_color);
}
}
void array_line_entry(int x1,int y1,int x2,int y2)
{
	if(arrcnt==49)
	return;
	arrcnt++;
	arrx1[arrcnt]=x1;
	arrx2[arrcnt]=x2;
	arry1[arrcnt]=y1;
	arry2[arrcnt]=y2;
}
void array_line_entry2(int x1,int y1,int x2,int y2)
{
	if(arrcnt2==49)
	return;
	arrcnt2++;
	arrx12[arrcnt2]=x1;
	arrx22[arrcnt2]=x2;
	arry12[arrcnt2]=y1;
	arry22[arrcnt2]=y2;
}


void user_clip()
{
	
	int j,temp,p;
	int b, sx, sy, ex, ey, px, py;
	int c=cur_color;
	int x1=0,y1=0,x2=0,y2=0;
	setdrawmode(XOR_MODE);
	getmousepos(&b, &sx, &sy);
	set_color(15-c);
	ex = px = sx, ey = py = sy;
	if(b==1)
	hidemouseptr();
	while(b == 1)
	{
		getmousepos(&b, &ex, &ey);
		if(px != ex || py != ey)
		{
 		drawRect(sx, sy, px, py);
 		drawRect(sx, sy, ex, ey);
 		px = ex, py = ey;
  		x1=sx;y1=sy;
  		x2=ex;y2=ey;
		}
  	}
  	set_color(c);
  	setdrawmode(COPY_MODE);
	
	drawRect(x1,y1,x2,y2);
	draw_canvas_default();
	if(x1>x2)
	{
	temp=x1;
	x1=x2;
	x2=temp;
	}
	if(y1>y2)
	{
	temp=y1;
	y1=y2;
	y2=temp;
	}
	
	p=arrcnt;
	while(arrcnt!=-1)
	{
	arrcnt--;
	}
	arrcnt=p;
	while(arrcnt!=-1)
	{
	clip(arrx1[arrcnt],arry1[arrcnt],arrx2[arrcnt],arry2[arrcnt],x1,y1,x2,y2);
	arrcnt--;
	}
	while(arrcnt2!=-1)
	{
	array_line_entry(arrx12[arrcnt2],arry12[arrcnt2],arrx22[arrcnt2],arry22[arrcnt2]);
	arrcnt2--;
	}
	showmouseptr();
}


int cliptest(float p,float q,float *u1,float *u2)
{
float r;
int val=1;
if(p<0.0)
{
r=q/p;
if(r>*u2)
val=0;
else if(r>*u1)
*u1=r;
}
else if(p>0.0)
{
r=q/p;
if(r<*u1)
val=0;
else if(r<*u2)
*u2=r;
}
else if(q<0.0)
val=0;
return val;
}

void clip(int x1,int y1,int x2,int y2,int xmin,int ymin,int xmax1,int ymax1)
{
float u1=0.0,u2=1.0,dx=x2-x1,dy=y2-y1;
if(x1==x2&&y1==y2)
return;
if(cliptest(-dx,x1-xmin,&u1,&u2))
  if(cliptest(dx,xmax1-x1,&u1,&u2))
	if(cliptest(-dy,y1-ymin,&u1,&u2))
	  if(cliptest(dy,ymax1-y1,&u1,&u2))
	   {
	    if(u2<1.0)
	   {
	   	 x2=x1+u2*dx;
	   	 y2=y1+u2*dy;
	   }
	   if(u1>0.0)
	   {
	     x1+=u1*dx;
	     y1+=u1*dy;
	   }
	   array_line_entry2(round(x1),round(y1),round(x2),round(y2));
	   set_color(cur_color);
	   setdrawmode(COPY_MODE);
	   bresline(round(x1),round(y1),round(x2),round(y2),1);
	   }
}
int round(int x)
{
return ((int)(x+0.5));
}



void user_poly()
{
	int inix,iniy,curx1,cury1,curx2,cury2;
	int mousex,mousey,button;
	getmousepos(&button,&mousex,&mousey);
	inix=mousex;
	iniy=mousey;
	curx1=inix;
	cury1=iniy;
	for(;button!=2;getmousepos(&button,&mousex,&mousey))
	{
	if(button==1)
	{
		curx2=mousex;
		cury2=mousey;
		//printf("%d %d\n",mousex,mousey);
		hidemouseptr();
		array_line_entry(curx1,cury1,curx2,cury2);
		bresline(curx1,cury1,curx2,cury2,1);
		showmouseptr();
		curx1=curx2;
		cury1=cury2;
		delay(200);
	}
	}
	hidemouseptr();
	array_line_entry(curx1,cury1,inix,iniy);
	bresline(curx1,cury1,inix,iniy,1);
	showmouseptr();
}



void circleallpoints(int xc,int yc,int x,int y)
{
put_pixel(xc+x,yc+y,cur_color);
put_pixel(xc+y,yc+x,cur_color);
put_pixel(xc+y,yc-x,cur_color);
put_pixel(xc+x,yc-y,cur_color);
put_pixel(xc-x,yc-y,cur_color);
put_pixel(xc-y,yc-x,cur_color);
put_pixel(xc-y,yc+x,cur_color);
put_pixel(xc-x,yc+y,cur_color);
}

void circle34points(int xc,int yc,int x,int y)
{
put_pixel(xc+x,yc+y,cur_color);
put_pixel(xc+y,yc+x,cur_color);
put_pixel(xc-y,yc+x,cur_color);
put_pixel(xc-x,yc+y,cur_color);
}

void circle23points(int xc,int yc,int x,int y)
{
put_pixel(xc-y,yc-x,cur_color);
put_pixel(xc-x,yc-y,cur_color);
put_pixel(xc-y,yc+x,cur_color);
put_pixel(xc-x,yc+y,cur_color);
}

void circle12points(int xc,int yc,int x,int y)
{
put_pixel(xc+y,yc-x,cur_color);
put_pixel(xc-x,yc-y,cur_color);
put_pixel(xc-y,yc-x,cur_color);
put_pixel(xc+x,yc-y,cur_color);
}

void circle14points(int xc,int yc,int x,int y)
{
put_pixel(xc+x,yc+y,cur_color);
put_pixel(xc+y,yc+x,cur_color);
put_pixel(xc+y,yc-x,cur_color);
put_pixel(xc+x,yc-y,cur_color);
}

void bcircle(int xc,int yc,int r,int op)
{
 int x,y,de,dse;
 float p;
 x=0;y=r;
 p=5/4-r;
 de = 3;
 dse = 5 - 2*r;
	while(y>=x)
	{
		switch(op)
		{
		case 1: circleallpoints(xc,yc,x,y);
				break;
		case 2: circle12points(xc,yc,x,y);
				break;
		case 3: circle23points(xc,yc,x,y);
				break;
		case 4: circle34points(xc,yc,x,y);
				break;
		case 5: circle14points(xc,yc,x,y);
				break;
		}
		if(p<0)
		   {
		     p+=de;
		     de+=2;
		     dse+=2;
		   }
		  else
		  {
		   p+=dse;
		   de+=2;
		   dse+=4;
		   y--;
		  }
		  x++;
	}
}

void drawCircle(int op)
{
	int button,x,y,x1,x2,y1,y2,prevx2,prevy2;
	float r;
	int c=cur_color;
	setdrawmode(XOR_MODE);
	
	getmousepos(&button,&x,&y);
	set_color(15-c);
			if(button==1)
			{
				setdrawmode(XOR_MODE);
				hidemouseptr();
				getmousepos(&button,&x,&y);
				x1=x;y1=y;
				x2=x;y2=y;
				r=(pow((x1-x2),2))+(pow((y1-y2),2));
				r=pow(r,0.5);
				r=r/2.0;
				bcircle((x1+x2)/2,(y1+y2)/2,r,op);
				while(button==1)
				{
					prevx2=x2;prevy2=y2;
					getmousepos(&button,&x,&y);
					x2=x;y2=y;
					if(prevx2!=x2||prevy2!=y2)
					{
						r=(pow((x1-prevx2),2))+(pow((y1-prevy2),2));
						r=pow(r,0.5);
						r=r/2.0;
						bcircle((x1+prevx2)/2,(y1+prevy2)/2,r,op);
						r=(pow((x1-x2),2))+(pow((y1-y2),2));
						r=pow(r,0.5);
						r=r/2.0;
						bcircle((x1+x2)/2,(y1+y2)/2,r,op);
					}
				}
				set_color(c);
				setdrawmode(COPY_MODE);
				bcircle((x1+x2)/2,(y1+y2)/2,r,op);
				showmouseptr();
		}


}
