#define ON 1
#define OFF 0
#define YES 1
#define NO 2
#define CANCEL 3
#define CLOSE 4
#define OK 5
#include<math.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<graphics.h>
#include<dos.h>
#include<alloc.h>

char *menu[]= {" Algo"," Region"," Color"," Car"," Width"," Tranformation"," LineFill","  Clipping"};
char *menuvA[]= {"Simple DDA","Symmetric DDA","Bresenham","MidpointLine","Circle","Ellipse"};
char *menuvQ[]= {"-X +Y","+X +Y","+X -Y","-X -Y"};
char *menuvCol[]= {"Yellow","Orange","Red","Green","Black"};
char *menuvCar[]= {"Sound","No Sound"};
char *menuvW[]= {"Size 3","Size5"};
char *menuvT[]= {"Tran.Line","Tran.poly","Reflection","Scaling","Rotation"};
char *menuvS[]= {"3","4","5","6","7"};
char *menuvL[]= {"Weiler","Hodgeman","LineClip"};

union REGS i,o;
struct SREGS s;

void getcoordinates_(int *x_,int *y_);
void getcoordinates(int *x,int *y);
void circle_(int r, int x_o,int y_o,int color);
void DDALine(int x1,int y1,int x2,int y2,int iColor,int quadrant);
void SymDDALine(int x1,int y1,int x2,int y2,int iColor,int quadrant);
void bresenham(int x1,int y1,int x2,int y2,int color,int quadrant);
void midpoint(int x1,int y1,int x2,int y2,int color,int quadrant);
void pattern(int quandrant,int x1,int x2,int y1,int y2);
void car();

//initalisation of mouse................
void flood(int x,int y, int fill_col, int old_col)
{
	if(getpixel(x,y)==7)
	{
	delay(1);
	putpixel(x,y,fill_col);
	flood(x+1,y,fill_col,old_col);
	flood(x-1,y,fill_col,old_col);
	flood(x,y+1,fill_col,old_col);
	flood(x,y-1,fill_col,old_col);
	}
}
initmouse()
{
    i.x.ax=0;
    int86(0x33,&i,&o);
    return(o.x.ax);
}

showmouseptr()
{
    i.x.ax=1;
    int86(0x33,&i,&o);
    return(o.x.ax);
}

hidemouseptr()
{
    i.x.ax=2;
    int86(0x33,&i,&o);
    return(o.x.ax);
}
////// /------------/////
//Function to get mouse position
getmousepos(int *button,int *x,int *y)
{
    i.x.ax=3;
    int86(0x33,&i,&o);
    *button=o.x.bx;
    *x=o.x.cx;
    *y=o.x.dx;
}

////////-----------///////
//Function to display a corner button
corner_button(int x,int y)
{
    setcolor(WHITE);
    setfillstyle(SOLID_FILL,7);
    settextstyle(SMALL_FONT,0,6);
    rectangle(x-5,y-3,x+8,y+8);
    setcolor(8);
    rectangle(x-4,y-2,x+9,y+9);
    bar(x-4,y-2,x+8,y+8);
}
//////////-------////////////
//Function to display a close button
close_button(int x,int y)
{
    corner_button(x,y);
    setcolor(BLACK);
    outtextxy(x-1,y-9,"x");
    outtextxy(x-2,y-9,"x");
}                                ///////---------////////////
//Function to display  a minimise button
minimise_button(int x,int y)
{
    corner_button(x,y);
    setcolor(WHITE);
    outtextxy(x-1,y-9,"_");
    setcolor(BLACK);
    outtextxy(x-1,y-10,"_");
}                           ////------------/////////
//Function to display a maximise button
maximize_button(int x,int y)
{
    corner_button(x,y);
    setcolor(WHITE);
    line(x-2,y-1,x+6,y-1);
    setcolor(BLACK);
    rectangle(x-2,y,x+6,y+7);
}                                ///////-------//////

//Function to draw main window
main_g_window(char *str,int x1,int y1,int x2,int y2,int c)
{
    int button,x,y;
    setcolor(BLACK);
    rectangle(x1,y1+1,x2+1,y2+1);
    setcolor(8);
    rectangle(x1,y1+1,x2,y2);
    setcolor(WHITE);
    rectangle(x1,y1,x2-1,y2-1);
    setfillstyle(SOLID_FILL,7);
    bar(x1+1,y1+1,x2-1,y2-1);
    setfillstyle(SOLID_FILL,c);
    bar(x1+3,y1+3,x2-3,y1+19);
    settextstyle(SMALL_FONT,0,5);
    setcolor(WHITE);
    outtextxy(x1+25,y1+2,str);
    setcolor(YELLOW);
    outtextxy(x1+7,y1,"/");
    outtextxy(x1+8,y1,"/");
    outtextxy(x1+9,y1+2,"/");
    outtextxy(x1+10,y1+2,"/");
    outtextxy(x1+11,y1+4,"/");
    outtextxy(x1+12,y1+4,"/");
    minimise_button(x2-48,y1+8);
    maximize_button(x2-32,y1+8);
    close_button(x2-15,y1+8);
    setfillstyle(SOLID_FILL,7);
    bar(x1+5,y1+42,x2-7,y2-23);
    setcolor(8);
    line(x1+4,y1+41,x2-5,y1+41);
    line(x1+4,y1+41,x1+4,y2-21);
    setcolor(BLACK);
    line(x1+5,y1+42,x2-6,y1+42);
    line(x1+5,y1+42,x1+5,y2-22);
    setcolor(WHITE);
    line(x1+4,y2-20,x2-5,y2-20);
    line(x2-4,y1+41,x2-4,y2-20);

    settextstyle(SMALL_FONT,0,4);
    setcolor(BLACK);

    line(315,55,320,50);
    line(325,55,320,50);
    line(320,50,320,450);
    outtextxy(330,60,"Y-axis");

    //line passing from mid way of the screen vertically (y aixs)
    line(315,445,320,450);
    line(325,445,320,450);
///////////////////////////////////////////////////////////////////////////
    line(25,235,20,240);
    line(25,245,20,240);
    line(20,240,620,240);
    outtextxy(550,225,"X-axis");
    //line passing from mid way of the screen horizontally    (x axis)
    line(615,235,620,240);
    line(615,245,620,240);
    getmousepos(&button,&x,&y);

    if(x>=x2-20 && x<=x2-6 && y>=y1+5 && y<=y1+17)
    {
	if((button & 1)==1)
	{
	    hidemouseptr();
	    exit(0);
	}
//..exit from the project on clicking the close button in top right corner
    }

}
///////////----------------/////////////////////
//Function to display horizontal menu
displaymenuh(char **menu,int count,int x1,int y1)
{
    int i,tw,xc;
    xc=x1;
    setfillstyle(SOLID_FILL,7);
    bar(x1-3,y1,x1+500,y1+17);
    settextstyle(SMALL_FONT,0,4);
    setcolor(BLACK);
    for(i=0; i<count; i++)
    {
	tw=textwidth(menu[i]);
	outtextxy(xc,y1+2,menu[i]);
	xc=xc+tw+10;
    }
}
//////////--------////////////
//Function to highlight menu item
highlight (char **menu,int ch,int x1,int y1)
{
    int xc=x1,tw,tw1,i;
    for(i=1; i<=ch; i++)
    {
	xc=xc+textwidth(menu[i-1])+10;
    }

    tw=textwidth(menu[ch-1]);
    setcolor(WHITE);
    line(xc-10-tw,y1,xc,y1);
    line(xc-10-tw,y1,xc-10-tw,y1+17);
    setcolor(8);
    line(xc-10-tw,y1+17,xc,y1+17);
    line(xc,y1,xc,y1+17);
}
//////--------------///////////////
//Function to highlight selected menu
highlight_select(char **menu,int ch,int x1,int y1)
{
    int xc=x1,tw,tw1,i;
    for(i=1; i<=ch; i++)
    {
	xc=xc+textwidth(menu[i-1])+10;
    }

    tw=textwidth(menu[ch-1]);
    setcolor(8);
    line(xc-10-tw,y1,xc,y1);
    line(xc-10-tw,y1,xc-10-tw,y1+17);
    setcolor(WHITE);
    line(xc-10-tw,y1+17,xc,y1+17);
    line(xc,y1,xc,y1+17);
}
//////--------------///////////////
//Function to dehighlight menu item
dehighlight(char **menu,int ch,int x1,int y1)
{
    int xc=x1,tw,tw1,i;
    for(i=1; i<=ch; i++)
    {
	xc=xc+textwidth(menu[i-1])+10;
    }

    tw=textwidth(menu[ch-1]);
    setcolor(7);
    line(xc-10-tw,y1,xc,y1);
    line(xc-10-tw,y1,xc-10-tw,y1+17);
    setcolor(7);
    line(xc-10-tw,y1+17,xc,y1+17);
    line(xc,y1,xc,y1+17);
}
//////--------------///////////////
//Function to get mouse response on horizontal menu
getresponseh(char **menu,int count,int x1,int y1)
{
    int choice=1,prevchoice=0,x,y,x2,y2,button;
    int in,i,h,tw,xc;

    h=textheight(menu[0]);
    y2=y1+h+6;
    x2=x1;

    displaymenuh(menu,count,x1,y1);

    for(i=0; i<count; i++)
    {
	x2=x2+textwidth(menu[i])+10;
    }

    showmouseptr();
    while(1)
    {

	getmousepos(&button,&x,&y);
	getmousepos(&button,&x,&y);
        //if mouse goes out of view then hide it
	if(x>=630-20 && x<=630-6 && y>=5+5 && y<=5+17)
	{
	    if((button & 1)==1)
	    {
		hidemouseptr();
		exit(0);
	    }
	}

        settextstyle(SMALL_FONT,0,4);
	//if position of mouse is in horizontal menu
	if(x>=x1 && x<=x2 && y>=y1 && y<=y2)
	{
	    in=1;
	    xc=x1;
	    for(i=1; i<=count; i++) //FOR ALL THE ITEMS IN HORIZONTAL MENU

	    {
		settextstyle(SMALL_FONT,0,4);
		xc=xc+textwidth(menu[i-1])+10;
                if(x<=xc)
		{
		    choice=i;
		    break;
		}
	    }

	    if(prevchoice!=choice)
            {
                hidemouseptr();

		if(prevchoice)
		    dehighlight(menu,prevchoice,x1,y1);

		highlight(menu,choice,x1,y1);
		prevchoice=choice;
		showmouseptr();
	    }

            if((button & 1)==1)
            {
		outtextxy(400,450,"YES");

		while((button & 1)==1)
		    getmousepos(&button,&x,&y);

		if(x>=x1 && x<=x2 && y>=y1 && y<=y2)
		{
                    outtextxy(500,450,"NO***");
                    hidemouseptr();
		    highlight_select(menu,choice,x1,y1);
		    return(choice);
		}
	    }


	}
	else
	{
            if(in==1)
	    {
		in=0;
		prevchoice=0;
		hidemouseptr();
		dehighlight(menu,choice,x1,y1);
		showmouseptr();
	    }
	}
    }
}
//Function to display vertical menu
displaymenuv(char **menu,int count,int width,int x1,int y1,int bk_color)
{
    int i,h;
    setcolor(0);
    h=textheight(menu[0]);
    rectangle(x1-5,y1-10,x1+width+12,y1+count*(h+10)+10);
    setcolor(8);
    rectangle(x1-5,y1-10,x1+width+11,y1+count*(h+10)+9);
    setcolor(WHITE);
    rectangle(x1-5,y1-10,x1+width+10,y1+count*(h+10)+8);
    setfillstyle(SOLID_FILL,bk_color);
    bar(x1-4,y1-9,x1+width+10,y1+count*(h+10)+8);
    settextstyle(SMALL_FONT,0,4);
    setcolor(BLACK);
    for(i=0; i<count; i++)
    {
	outtextxy(x1+10,y1+i*(h+10),menu[i]);
    }
}
//Function to hoghlight vertivcal menu
highlightv(char **menu,int ch,int width,int x1,int y1,int color)
{
    int h;
    h=textheight(menu[0]);
    setfillstyle(SOLID_FILL,color);
    bar(x1,y1+(ch-1)*(h+10),x1+width+9,y1+(ch-1)*(h+10)+12);
    setcolor(WHITE);
    outtextxy(x1+10,y1+(ch-1)*(h+10),menu[ch-1]);
}
//Function to dehighlight vertical menu
dehighlightv(char **menu,int ch,int width,int x1,int y1,int bk_color)
{
    int h;
    h=textheight(menu[0]);
    setfillstyle(SOLID_FILL,bk_color);
    bar(x1,y1+(ch-1)*(h+10),x1+width+9,y1+(ch-1)*(h+10)+12);
    setcolor(BLACK);
    outtextxy(x1+10,y1+(ch-1)*(h+10),menu[ch-1]);
}

//Function to get mouse response on vertival menu
getresponsev(char **menu,int count,int width,int x1,int y1,int color,int bk_color)
{
    int choice=1,prevchoice=0,x,y,x2,y2,button,in,i,h,tw,xc,area;
    long int *buffer;

    h=textheight(menu[0]);
    y2=y1+count*(h+10);
    x2=x1+width+10;
    hidemouseptr();

    //we measure size of suitable area of screen to be
    //regenerated when vmenu closed
    area=imagesize(x1-6,y1-11,x1+width+13,y1+count*(h+10)+10);

    //allocate suitable memory as per size measured
    buffer=(long int *)malloc(area);

    //get image of the measured boundary  stored in allocated memory
    getimage(x1-6,y1-11,x1+width+13,y1+count*(h+10)+10,buffer);

    //now display menu
    displaymenuv(menu,count,width,x1,y1,bk_color);

    showmouseptr();

    settextstyle(SMALL_FONT,0,4);
    while(1)
    {
	getmousepos(&button,&x,&y);
	if(x<x1 || x>x2 || y<y1 || y>y2)   //when mouse is out of focus
	{
            if((button & 1)==1)             //of vertical menu opened
	    {
		//and if button is clicked
		hidemouseptr();
		putimage(x1-6,y1-11,buffer,COPY_PUT);
                //the original image is displayed
                free(buffer);
		//corresponding mem is freed
		return(0);
	    }
	}

	if(x>=x1 && x<=x2 && y>=y1 && y<=y2)
	{
	    in=1;
	    for(i=1; i<=count; i++)
	    {
                if(y<=y1+i*(h+10))    //hovering within the v_menu
		{
		    choice=i;      //opened the index corsp to
                    break;
		}
	    }            		//  menu item is assigned to choice

	    if(prevchoice!=choice)
            {
                hidemouseptr();  //hilighting and dehilighying menu items
		//within v_menu on mouse hover when menu open
		if(prevchoice)
		    dehighlightv(menu,prevchoice,width,x1,y1,bk_color);
		//first dehilght the previous menu item

		highlightv(menu,choice,width,x1,y1,color);
		//then hilight the new item

		prevchoice=choice;//update value of var prevchoice

		showmouseptr();
	    }

            if((button & 1)==1)
	    {
		outtextxy(450,450,"In VMENU");

		while((button & 1)==1)
                    getmousepos(&button,&x,&y);

		if(x>=x1 && x<=x2 && y>=y1 && y<=y2)
		{
		    outtextxy(350,450,"OUTMENU_V");
		    hidemouseptr();
		    putimage(x1-6,y1-11,buffer,COPY_PUT);
		    free(buffer); //vertical menu escapes
		    return(choice);
		}
	    }


	}
	else    //when v_menu is displayed and mouse hovers out of focus
        {
	    if(in==1)
	    {
		in=0;
		prevchoice=0;
                hidemouseptr();
		dehighlightv(menu,choice,width,x1,y1,bk_color);
		showmouseptr();
	    }
	}

    }//while ends
}
///__________////
//Function to display drop down menu
drop_down_menu(char **menu,int count,int x1,int y1,int color,int bk_color)
{
    int i,width=0;
    for(i=0; i<count; i++)
    {
        if(textwidth(menu[i])>width)
	{
	    width=textwidth(menu[i]);
	}
    }
    width=width+30;
    showmouseptr();
    getresponsev(menu,count,width,x1,y1,color,bk_color);
}
//_______________dda//
void DDALine(int x1,int y1,int x2,int y2,int iColor,int quadrant)
{
//    setbkcolor(6);
    outtextxy(50,454,"SimpleDDA");
    float dX,dY,iSteps;   //local declaration section
    float xInc,yInc,iCount,x,y;

    dX = x2 - x1;     //calculaing the distance b/w x cord.
    dY = y2 - y1;     //calculaing the distance b/w y cord.

    x = x1;
    y = y1;
    if (fabs(dX) > fabs(dY))
    {
	//if the  absolute value of floating point number dX
        iSteps = fabs(dX); //is greater than dY we assign this value to

    }   // variable meant as counter for the iterations to be made

    else
    {
	iSteps = fabs(dY);      //note iSteps=line length estimate
    }


    xInc = fabs(dX)/iSteps; //calulating x increment and y increment
    yInc = fabs(dY)/iSteps;
    /*  xInc = (dX)/iSteps; //calulating x increment and y increment

      yInc = (dY)/iSteps;*/


//    circle(x,y,1);   //drawing circle on the first point

    x+=0.5;

//    fprintf(fp,"This file has the cordinates formed as per calculation performed using simple DDA");
    for (iCount=1; iCount<=iSteps; iCount++)
    {
	if(quadrant==0)
	{
	    outtextxy(320,260,"No such Quadrant");
	    break;
	}

	if(quadrant==1)
        {
            y-=yInc;    //in first quadrant
	    x-=xInc;
        }
        if(quadrant==2)
	{
	    y-=yInc;    //in second quadrant
	    x+=xInc;
	}
	if(quadrant==3 )
        {
	    y+=yInc;    //in third quadrant
	    x+=xInc;
	}
	if(quadrant==4)
	{
	    y+=yInc;    //in fourth quadrant
	    x-=xInc;
	}


        if(int(iCount)%2==0)
	    continue;
        putpixel(floor(x),floor(y),iColor);
//	fprintf(fp,"x=%f and y=%f\n",floor(x-320),fabs(y-240));

    }

//    circle(x2,y2,1);



    return;
}

void car()
{
    int maxx,midy;

    maxx = getmaxx();
    /* mid pixel in vertical axis */
    midy = getmaxy()/2;

    for (int i=0; i < maxx-10; i=i+5)
    {

	/* clears screen */
	cleardevice();
	setbkcolor(6);
        /* draw a white road */
        setcolor(WHITE);
	line(0, midy + 37, maxx, midy + 37);

	/* Draw Car */
	setcolor(YELLOW);
	setfillstyle(SOLID_FILL, RED);

	line(i, midy + 23, i, midy);
	line(i, midy, 40 + i, midy - 20);
	line(40 + i, midy - 20, 80 + i, midy - 20);
        line(80 + i, midy - 20, 100 + i, midy);
        line(100 + i, midy, 120 + i, midy);
	line(120 + i, midy, 120 + i, midy + 23);
	line(0 + i, midy + 23, 18 + i, midy + 23);
        arc(30 + i, midy + 23, 0, 180, 12);
	line(42 + i, midy + 23, 78 + i, midy + 23);
	arc(90 + i, midy + 23, 0, 180, 12);
	line(102 + i, midy + 23, 120 + i, midy + 23);
	line(28 + i, midy, 43 + i, midy - 15);
        line(43 + i, midy - 15, 57 + i, midy - 15);
	line(57 + i, midy - 15, 57 + i, midy);
	line(57 + i, midy, 28 + i, midy);
	line(62 + i, midy - 15, 77 + i, midy - 15);
	line(77 + i, midy - 15, 92 + i, midy);
	line(92 + i, midy, 62 + i, midy);
	line(62 + i, midy, 62 + i, midy - 15);
	floodfill(5 + i, midy + 22, YELLOW);
	setcolor(BLUE);
	setfillstyle(SOLID_FILL, DARKGRAY);
	/*  Draw Wheels */
        circle(30 + i, midy + 25, 9);
        circle(90 + i, midy + 25, 9);
	floodfill(30 + i, midy + 25, BLUE);
	floodfill(90 + i, midy + 25, BLUE);
        /* Add delay of 0.1 milli seconds */
	sound(300*((random(10)+1)));
	delay(100);
	nosound();
	delay(50);
    }
    return;
}

void midpoint(int x1,int y1,int x2,int y2,int color,int quadrant)
{
    //     setbkcolor(6);
    outtextxy(50,454,"MidPoint");

    int dx,dy,p,x,y,end;

    dx = abs(x2 - x1);
    dy = abs(y2 - y1);
    ////////////////////quadrant2///////////////////////
    if(quadrant==2||quadrant==4)
    {
	if(dx>dy)
	{
	    p = 2 * dy - dx;
	    if(x1 > x2)
	    {
                x = x2;
		y = y2;
		end = x1;
	    }
	    else
	    {
		x = x1;
		y = y1;
		end = x2;
	    }
	    putpixel(x, y, color);
            while(x < end)
	    {
		x = x + 1;
                if(p < 0)
		{
		    p = p + 2 * dy;
		}
		else
                {
                    y = y - 1;
		    p = p + 2 * (dy - dx);
		}
		putpixel(x, y, color);
//	fprintf(fp,"x=%d and y=%d\n",x-320,y-240);
	    }
	}
	else
	{
	    p = 2 * dx - dy;
            if(y2 > y1)
            {
		x = x2;
                y = y2;
                end = y1;
	    }
	    else
	    {
		x = x1;
                y = y1;
                end = y2;
	    }
	    putpixel(x, y, color);
	    while(y > end)
	    {
		y = y - 1;
		if(p < 0)
		{
		    p = p + 2 * dx;
		}
                else
		{
		    x = x + 1;
                    p = p + 2 * (dx - dy);
                }

		putpixel(x, y, color);
		//fprintf(fp,"x=%d and y=%d\n",x-320,y-240);
	    }

        }
    }
    else if(quadrant==1||quadrant==3)
    {
	/////////////////////quadrant 1 and 3////////////////////
	if(dx>dy)
	{
	    p = 2 * dy - dx;
	    if(x1 > x2)
	    {
		x = x1;
                y = y1;
		end = x2;
            }
	    else
	    {
		x = x2;
		y = y2;
		end = x1;
            }
            putpixel(x, y, color);
	    while(x > end)
	    {
		x = x - 1;
		if(p < 0)
		{
		    p = p + 2 * dy;
		}
		else
		{
		    y = y - 1;
                    p = p + 2 * (dy - dx);
		}
		putpixel(x, y, color);
                //	fprintf(fp,"x=%d and y=%d\n",x-320,y-240);
	    }
	}
	else
	{
            p = 2 * dx - dy;
            if(y2 > y1)
	    {
		x = x2;
		y = y2;
		end = y1;
	    }
	    else
	    {
		x = x1;
		y = y1;
                end = y2;
            }
	    putpixel(x, y, color);
            while(y > end)
            {
		y = y - 1;
		if(p < 0)
		{
		    p = p + 2 * dx;
                }
                else
		{
		    x = x - 1;
		    p = p + 2 * (dx - dy);
		}

		putpixel(x, y, color);
		// fprintf(fp,"x=%d and y=%d\n",x-320,y-240);
	    }

        }
    }
    return;
}

void bresenham(int x1,int y1,int x2,int y2,int color,int quadrant)
{
    //     setbkcolor(6);
    outtextxy(50,454,"Bresenham");

    int dx,dy,p,x,y,end;

    dx = abs(x2 - x1);
    dy = abs(y2 - y1);
    ////////////////////quadrant2///////////////////////
    if(quadrant==2||quadrant==4)
    {
	if(dx>dy)
	{
	    p = 2 * dy - dx;
	    if(x1 > x2)
            {
		x = x2;
                y = y2;
                end = x1;
	    }
	    else
	    {
		x = x1;
                y = y1;
                end = x2;
	    }
	    putpixel(x, y, color);
	    while(x < end)
	    {
		x = x + 1;
		if(p < 0)
		{
		    p = p + 2 * dy;
		}
                else
                {
		    y = y - 1;
                    p = p + 2 * (dy - dx);
                }
		if(x%2==0)
		    continue;
		putpixel(x, y, color);
		//	fprintf(fp,"x=%d and y=%d\n",x-320,y-240);
            }
        }
	else
	{
	    p = 2 * dx - dy;
	    if(y2 > y1)
	    {
		x = x2;
		y = y2;
		end = y1;
	    }
            else
            {
		x = x1;
                y = y1;
		end = y2;
	    }
	    putpixel(x, y, color);
	    while(y > end)
	    {
                y = y - 1;
		if(p < 0)
		{
		    p = p + 2 * dx;
		}
		else
		{
		    x = x + 1;
		    p = p + 2 * (dx - dy);
		}
		if(y%2==0)
                    continue;
                putpixel(x, y, color);
		//  fprintf(fp,"x=%d and y=%d\n",x-320,y-240);
            }

	}
    }
    else if(quadrant==1||quadrant==3)
    {
	/////////////////////quadrant 2////////////////////
	if(dx>dy)
	{
	    p = 2 * dy - dx;
	    if(x1 > x2)
	    {
		x = x1;
		y = y1;
		end = x2;
	    }
	    else
            {
                x = x2;
		y = y2;
		end = x1;
            }
	    putpixel(x, y, color);
	    while(x > end)
	    {
		x = x - 1;
                if(p < 0)
                {
		    p = p + 2 * dy;
		}
		else
		{
		    y = y - 1;
		    p = p + 2 * (dy - dx);
		}
		putpixel(x, y, color);
		//	fprintf(fp,"x=%d and y=%d\n",x-320,y-240);
            }
        }
	else
        {
            p = 2 * dx - dy;
	    if(y2 > y1)
	    {
		x = x2;
		y = y2;
		end = y1;
	    }
	    else
	    {
		x = x1;
		y = y1;
		end = y2;
	    }
	    putpixel(x, y, color);
	    while(y > end)
	    {
                y = y - 1;
                if(p < 0)
		{
                    p = p + 2 * dx;
                }
		else
		{
		    x = x - 1;
		    p = p + 2 * (dx - dy);
		}

		putpixel(x, y, color);
		//   fprintf(fp,"x=%d and y=%d\n",x-320,y-240);
	    }

	}
    }
    return;
}
void SymDDALine(int x1,int y1,int x2,int y2,int iColor,int quadrant)
{
    // setbkcolor(6);
    outtextxy(50,454,"SymmetricDDA");
    float dX,dY,iSteps;
    float xInc,yInc,iCount,x,y;

    dX = x2 - x1;  //delta x and y values
    dY = y2 - y1;

    //line length=1/e where e is 2^n it signifies number of iterations
    int n=1;
    if (fabs(dX) > fabs(dY))  //calcuating line length
    {
	while(1)
	{
	    if(pow(2,n-1)<=fabs(dX) && fabs(dX)<pow(2,n))
		break;

	    n++;
	}
        iSteps=pow(2,n);
    }
    else
    {
	//calculating line length
	while(1)
	{
	    if(pow(2,n-1)<=fabs(dY) && fabs(dY)<pow(2,n))
		break;

            n++;
	}
	iSteps=pow(2,n);

    }

    xInc = fabs(dX)/iSteps;
    yInc = fabs(dY)/iSteps;
//    printf("xInc=%f and yInc=%f",xInc,yInc);
    x = x1;
    y = y1;
    // circle(x1,y1,1);
    x+=0.5;
    y-=0.5;


    for (iCount=1; iCount<=iSteps; iCount++)
    {
	if(quadrant==1)
        {
	    y-=yInc;    //in first quadrant
	    x-=xInc;
	}
	if(quadrant==2)
	{
	    y-=yInc;    //in second quadrant
	    x+=xInc;
	}
	if(quadrant==3 )
	{
	    y+=yInc;    //in third quadrant
            x+=xInc;
	}
        if(quadrant==4)
        {
	    y+=yInc;    //in fourth quadrant
	    x-=xInc;
	}

	putpixel(floor(x),floor(y),iColor);
        //fprintf(fp,"x=%f and y=%f\n",floor(x-320),floor(fabs(y-240)));
    }

    //  circle(x2,y2,1);
    return;
}
void pattern(int quadrant,int x1,int x2,int y1,int y2)
{
    if(quadrant==2)
    {
	line(320+x1-2,240-y1-2,320+x1+2,240-y1+2);
	line(320+x1-2,240-y1+2,320+x1+2,240-y1+2);//lower triangle
	line(320+x1-2,240-y1+2,320+x1-2,240-y1-2);

	line(320+x2-2,240-y2-2,320+x2+2,240-y2-2);
	line(320+x2-2,240-y2-2,320+x2+2,240-y2+2);//upper triangle
	line(320+x2+2,240-y2+2,320+x2+2,240-y2-2);
    }
}
 void circle_(int r, int x_o,int y_o,int color)
{
    int p,x,y;
    p=1-r;
    x=0;
    y=r;
    while(x<=y)
    {
	putpixel(x_o+x,y_o-y,color);
	putpixel(x_o+x,y_o+y,color);
	putpixel(x_o-x,y_o-y,color);
	putpixel(x_o-x,y_o+y,color);
	putpixel(x_o+y,y_o-x,color);
	putpixel(x_o+y,y_o+x,color);
	putpixel(x_o-y,y_o+x,color);
	putpixel(x_o-y,y_o-x,color);
	if(p<0)
	{
	    p+=(2*x)+3;
	}
	else
	{
	    p+=2*(x-y)+5;
	    y--;
	}
	x++;
    }

}
void ellipse_(int xc,int yc,long rx, long ry,int iColor)
{
    int x,y;float p;



   //Region 1
   p=ry*ry-rx*rx*ry+rx*rx/4;
   x=0;y=ry;
   putpixel(xc+x,yc-y,iColor);
   putpixel(xc+x,yc+y,iColor);
   while(2.0*ry*ry*x <= 2.0*rx*rx*y)
   {
	if(p < 0)
	{
		x++;
		p = p+2*ry*ry*x+ry*ry;
	}
	else
	{
		x++;y--;
		p = p+2*ry*ry*x-2*rx*rx*y-ry*ry;
	}
	putpixel(xc+x,yc+y,iColor);
	putpixel(xc+x,yc-y,iColor);
	putpixel(xc-x,yc+y,iColor);
	putpixel(xc-x,yc-y,iColor);
   }

  //Region 2
   p=ry*ry*(x+0.5)*(x+0.5)+rx*rx*(y-1)*(y-1)-rx*rx*ry*ry;
   while(y > 0)
   {
	if(p <= 0)
	{
		x++;y--;
		p = p+2*ry*ry*x-2*rx*rx*y+rx*rx;
	}
	else
	{
		y--;
		p = p-2*rx*rx*y+rx*rx;
	}
	putpixel(xc+x,yc+y,iColor);
	putpixel(xc+x,yc-y,iColor);
	putpixel(xc-x,yc+y,iColor);
	putpixel(xc-x,yc-y,iColor);
   }

}


void getcoordinates(int *x_,int *y_)
{
    int button,x,y;


    while(1)
    {
	getmousepos(&button,&x,&y);
	if((button&1)==1)//left click
	{
	    *x_=x;
	    *y_=y;
	    //   printf("\n(%d %d) inside func",*x_,*y_);
	    break;
	}

    }
}
void getcoordinates_(int *x_,int *y_)
{
    int button,x,y;


    while(1)
    {
	getmousepos(&button,&x,&y);
	if((button&2)==2)   //right click
	{
	    *x_=x;
	    *y_=y;
	    //     printf("\n(%d %d)  inside func",*x_,*y_);
	    break;
	}

    }
}

int getradius(int xc,int ycc)
{
	    int r,x1,y1,x_,y_;
	    showmouseptr();

	    getcoordinates(&x_,&y_);
	    x1=x_-320;
	    y1=240-y_;

	    r=sqrt(((xc-x1)*(xc-x1))+((ycc-y1)*(ycc-y1)));
	    return r;


}

int getradius_()
{
	    int r,x1,y1,x_,y_;
	    showmouseptr();

	    getcoordinates(&x_,&y_);
	    x1=x_-320;
	    y1=240-y_;

	    r=sqrt((x1*x1)+(y1*y1));
	    return r;


}
void sort(float sdy[],int h)
{
float temp;

for(int j=0;j<=h-1;j++)
 {
for(int i=0;i<h-1-j;i++)
  {
if(sdy[i]>sdy[i+1])
    {
temp=sdy[i];
sdy[i]=sdy[i+1];
sdy[i+1]=temp;
    }
  }
 }
}
///////////////////
struct ather
{
float x;
float y;
float io;
float vis;
};
struct ather z[20];

main()
{
    int gd=DETECT,gm,choice,i,countvT,count,countvA,countvQ,countvCol,countS,countvCar,countvW,countL;
    int choicev,xco=20,yco=27,xco1=22,yco1=56,xco2=60,yco2=56,ch;
    int x1=20,x2=100,y1=20,y2=120,iColor=2,quadrant=0,thick=3,x_,y_;
    int tra[3][3];
    initgraph(&gd,&gm,"c:\\tc\\bgi");
    setbkcolor(BLACK);
    main_g_window("PROJECT_YOGESHWAR",5,5,630,470,BLUE);
    if(initmouse()==0)
    {
	printf("\n Unable to initialise Mouse");
	exit(0);
    }

    showmouseptr();
    count=sizeof(menu)/sizeof(char*);
    countvA=sizeof(menuvA)/sizeof(char*);
    countvQ=sizeof(menuvQ)/sizeof(char*);
    countvCol=sizeof(menuvCol)/sizeof(char*);
    countvCar=sizeof(menuvCar)/sizeof(char*);
    countvW=sizeof(menuvW)/sizeof(char*);
    countvT=sizeof(menuvT)/sizeof(char*);
    countS=sizeof(menuvS)/sizeof(char*);
    countL=sizeof(menuvL)/sizeof(char*);
    settextstyle(SMALL_FONT,0,6);
    setcolor(BLACK);
    displaymenuh(menu,count,xco,yco);
    showmouseptr();

/////////////////////////////////////////////////////////////////////////////

    while(1)
    {
	choice=getresponseh(menu,count,xco,yco);

	switch(choice)
	{
	case 1:////////////////////////////algotirhms
	    choicev=drop_down_menu(menuvA,countvA,xco1,yco1,BLUE,7);
	    switch(choicev)
	    {
	    case 1://////////////////simple dda
		if(quadrant==1||quadrant==3)  //in first  quadrant    and third quadrant    //in first  quadrant    and third quadrant
		{

		    DDALine(320+x1-1,240-y1+1,320+x2-1,240-y2+1,iColor,quadrant);
		    DDALine(320+x1,240-y1,320+x2,240-y2,iColor,quadrant);//original
		    DDALine(320+x1+1,240-y1-1,320+x2+1,240-y2-1,iColor,quadrant);


		}
		if(quadrant==2||quadrant==4)//in second  quadrant and fourth quadrant
		{
		    if(thick==5)
			DDALine(320+x1-2,240-y1-2,320+x2-2,240-y2-2,iColor,quadrant);
		    DDALine(320+x1-1,240-y1-1,320+x2-1,240-y2-1,iColor,quadrant);
		    DDALine(320+x1,240-y1,320+x2,240-y2,iColor,quadrant);//original
		    DDALine(320+x1+1,240-y1+1,320+x2+1,240-y2+1,iColor,quadrant);
		    if(thick==5)
			DDALine(320+x1+2,240-y1+2,320+x2+2,240-y2+2,iColor,quadrant);
		    if(thick==5)
			pattern(2,x1,x2,y1,y2);
		}

		break;
	    case 2://///////////////symmetric dda
		if(quadrant==1||quadrant==3)  //in first  quadrant    and third quadrant    //in first  quadrant    and third quadrant
		{
		    SymDDALine(320+x1-1,240-y1+1,320+x2-1,240-y2+1,iColor,quadrant);
		    SymDDALine(320+x1,240-y1,320+x2,240-y2,iColor,quadrant);//original
		    SymDDALine(320+x1+1,240-y1-1,320+x2+1,240-y2-1,iColor,quadrant);

		}
		if(quadrant==2||quadrant==4)//in second  quadrant and fourth quadrant
		{
		    if(thick==5)
			SymDDALine(320+x1-2,240-y1-2,320+x2-2,240-y2-2,iColor,quadrant);
		    SymDDALine(320+x1-1,240-y1-1,320+x2-1,240-y2-1,iColor,quadrant);
		    SymDDALine(320+x1,240-y1,320+x2,240-y2,iColor,quadrant);//original
		    SymDDALine(320+x1+1,240-y1+1,320+x2+1,240-y2+1,iColor,quadrant);
		    if(thick==5)
			SymDDALine(320+x1+2,240-y1+2,320+x2+2,240-y2+2,iColor,quadrant);
		    if(thick==5)
			pattern(2,x1,x2,y1,y2);
		}

		break;
	    case 3://///////////////bresenham
		if(quadrant==2||quadrant==4)
		{
		    if(thick==5)
			bresenham(320+x1-2,240-y1-2,320+x2-2,240-y2-2,iColor,quadrant);
		    bresenham(320+x1-1,240-y1-1,320+x2-1,240-y2-1,iColor,quadrant);
		    bresenham(320+x1,240-y1,320+x2,240-y2,iColor,quadrant);
		    bresenham(320+x1+1,240-y1+1,320+x2+1,240-y2+1,iColor,quadrant);
		    if(thick==5)
			bresenham(320+x1+2,240-y1+2,320+x2+2,240-y2+2,iColor,quadrant);
//     line(320+x1,240-y1,320+x1,240);
//     line(320+x1,240-y1,320,240-y1);
//     line(320+x2,240-y2,320+x2,240);
//     line(320+x2,240-y2,320,240-y2);
//    putpixel(320+x1-1,240-y1+1,14);
//     putpixel(320+x2+1,240-y2-1,14);
		    if(thick==5)
			pattern(2,x1,x2,y1,y2);
		}
		if(quadrant==3||quadrant==1)
		{
		    bresenham(320+x1-1,240-y1+1,320+x2-1,240-y2+1,iColor,quadrant);
		    bresenham(320+x1,240-y1,320+x2,240-y2,iColor,quadrant);
		    bresenham(320+x1+1,240-y1-1,320+x2+1,240-y2-1,iColor,quadrant);
		    // delay(5000);
		    // car();
		}
	       break;

	    case 4://///////////////////midpoint
		if(quadrant==2||quadrant==4)
		{
		    if(thick==5)
			midpoint(320+x1-2,240-y1-2,320+x2-2,240-y2-2,iColor,quadrant);
		    midpoint(320+x1-1,240-y1-1,320+x2-1,240-y2-1,iColor,quadrant);
		    midpoint(320+x1,240-y1,320+x2,240-y2,iColor,quadrant);
		    midpoint(320+x1+1,240-y1+1,320+x2+1,240-y2+1,iColor,quadrant);
		    if(thick==5)
			midpoint(320+x1+2,240-y1+2,320+x2+2,240-y2+2,iColor,quadrant);
		    if(thick==5)
			pattern(2,x1,x2,y1,y2);
		}
		if(quadrant==3||quadrant==1)
		{
		    midpoint(320+x1-1,240-y1+1,320+x2-1,240-y2+1,iColor,quadrant);
		    midpoint(320+x1,240-y1,320+x2,240-y2,iColor,quadrant);
		    midpoint(320+x1+1,240-y1-1,320+x2+1,240-y2-1,iColor,quadrant);

		}

		break;
	    case 5://////////////////////////circle


		showmouseptr();

	    getcoordinates(&x_,&y_);
	    int xc=x_-320;
	    int yc=240-y_;

	    getcoordinates_(&x_,&y_);
	    int xcc=x_-320;
	    int ycc=240-y_;

			putpixel(318+xc,238-ycc,iColor);
			getch();
		int r=getradius(xc,ycc);

		setcolor(iColor);


		circle_(r,320+xc,240-ycc,iColor);
		outtextxy(20,449,"Circle drawing");
		getch();
		setcolor(iColor);
		setfillstyle(SOLID_FILL,iColor);
		floodfill(320+xc,240-ycc,iColor);
	     setfillstyle(SOLID_FILL,7);
	 /* draw the bar */
	    bar(10,452,628,468);

		break;

	    case 6:////////////////////ellipse
	    showmouseptr();

	    getcoordinates(&x_,&y_);
	    int xq=x_-320;
	    int yq=240-y_;

	    getcoordinates_(&x_,&y_);
	    int xp=x_-320;
	    int yp=240-y_;
	    putpixel(318+xq,238-yq,iColor);
	    getch();
	    getcoordinates(&x_,&y_);
	    x1=x_-320;
	    y1=240-y_;

	    getcoordinates_(&x_,&y_);
	    x2=x_-320;
	    y2=240-y_;

	    setcolor(iColor);


	    ellipse_(320+xq,240-yp,x2,y1,iColor);
	    outtextxy(10,454,"Click to choose center, then yradius and then x radius");
	    getch();
	    setcolor(iColor);
	    setfillstyle(SOLID_FILL,iColor);
	    floodfill(320+xq,240-yp,iColor);
	    getch();
	    setfillstyle(SOLID_FILL,7);
	 /* draw the bar */
	    bar(10,452,628,468);

	    break;


	    }
	    delay(2000);
	    setfillstyle(SOLID_FILL,7);
	    /* draw the bar */
	    bar(10,452,628,468);
	    break;

	case 2://///////////////////////quadrant

	    choicev=drop_down_menu(menuvQ,countvQ,xco2,yco2,BLUE,7);

	    switch(choicev)
	    {
	    case 1:
		quadrant=1;
		break;
	    case 2:
		quadrant=2;
		break;
	    case 3:
		quadrant=3;
		break;
	    case 4:
		quadrant=4;
		break;
	    }
	    outtextxy(10,454,"Enter the Coordinates::(x1,y1) and (x2,y2)");
	    delay(1500);
	    setfillstyle(SOLID_FILL,7);

	    /* draw the bar */
	    bar(10,452,628,468);

	    showmouseptr();
	    getcoordinates(&x_,&y_);
	    x1=x_-320;
	    y1=240-y_;

	    getcoordinates_(&x_,&y_);
	    x2=x_-320;
	    y2=240-y_;

//	  printf("%d %d %d %d",x1+320,240-y1,x2+320,240-y2);
	    break;

	case 3:
	    choicev=drop_down_menu(menuvCol,countvCol,100,yco2,BLUE,7);
	    switch(choicev)
	    {
	    case 1:
		iColor=14;
		break;
	    case 2:
		iColor=6;
		break;
	    case 3:
		iColor=12;
		break;
	    case 4:
		iColor=2;
		break;
	    case 5:
		iColor=0;
		break;
	    }

	    break;
	case 4:
	    choicev=drop_down_menu(menuvCar,countvCar,135,yco2,BLUE,7);
	    switch(choicev)
	    {
	    case 1:
		car();
		cleardevice();
		setbkcolor(BLACK);
		main_g_window("PROJECT_YOGESHWAR",5,5,630,470,BLUE);

		break;
	    case 2:
		car();
		cleardevice();
		setbkcolor(BLACK);
		main_g_window("PROJECT_YOGESHWAR",5,5,630,470,BLUE);

		break;
	    }
	    break;
	case 5:
	    choicev=drop_down_menu(menuvW,countvW,190,yco2,BLUE,7);
	    switch(choicev)
	    {
	    case 1:
		thick=3;
		break;
	    case 2:
		thick=5;
		break;


	    }
	 case 6:

	   /* putpixel(80,220,BLACK);
	    putpixel(380,220,BLACK);
	    putpixel(420,200,BLACK);*/
	    choicev=drop_down_menu(menuvT,countvT,240,yco1,BLUE,7);
	    switch(choicev)
	    {
	    case 1:outtextxy(20,449,"TRANSLATION HERE");
	    delay(2000);

	    setfillstyle(SOLID_FILL,7);
	    /* draw the bar */
	    bar(10,452,628,468);

	    showmouseptr();

	    getcoordinates(&x_,&y_);
	    int xl=x_-320;
	    int yl=240-y_;

	    getcoordinates_(&x_,&y_);
	    int xll=x_-320;
	    int yll=240-y_;

	    getcoordinates(&x_,&y_);
	    int xlll=x_-320;
	    int ylll=240-y_;

	    getcoordinates_(&x_,&y_);
	    int xllll=x_-320;
	    int yllll=240-y_;
	    setcolor(iColor);
	    line(320+xl,240-yll,320+xlll,240-yllll);
	    getch();
	    getcoordinates_(&x_,&y_);
	    int xv=x_-320;
	    int yv=240-y_;

	    int tx=xv-xlll;
	    int ty=yv-yllll;

	    setcolor(BLACK);
	    line(320+x1+tx,240-yll-ty,320+xlll+tx,240-yllll-ty);
	    getch();
	    break;

	    case 2:    showmouseptr();
	    outtextxy(20,449,"POLYGON TRANSLATION");
	    delay(2000);
	    setfillstyle(SOLID_FILL,7);
	    /* draw the bar */
	    bar(10,452,628,468);
	   /*
	    getcoordinates_(&x_,&y_);
	    int xr=x_-320;
	    int yr=240-y_;
	    line(320\,240,320+xr,240-yr);

	    int rd=(int) sqrt((xr*xr+yr*yr)*1.0);
	    setcolor(WHITE);

	    circle(320,240,rd);*/

	    int poly[10];
	    getcoordinates_(&x_,&y_);
	    int x1=x_;
	    poly[0]=x1;
	    getcoordinates(&x_,&y_);
	    int y1=y_;
	    poly[1]=y1;
	    getcoordinates_(&x_,&y_);
	    int x2=x_;
	    poly[2]=x2;
	    getcoordinates(&x_,&y_);
	    int y3=y_;
	    poly[3]=y3;
	    getcoordinates_(&x_,&y_);
	    int x4=x_;
	    poly[4]=x4;
	    getcoordinates(&x_,&y_);
	    int y5=y_;
	    poly[5]=y5;
	    getcoordinates_(&x_,&y_);
	    int x6=x_;
	    poly[6]=x6;
	    getcoordinates(&x_,&y_);
	    int y7=y_;
	    poly[7]=y7;
	    poly[8]=poly[0];
	    poly[9]=poly[1];
	    setcolor(iColor);
	    drawpoly(5,poly);

	    getcoordinates_(&x_,&y_);//right
	    int x8=x_;
	    getcoordinates(&x_,&y_); //left
	    int y10=y_;

	    int xmove=x8-x6;
	    int ymove=y10-y7;
	    for(i=0;i<=9;i++)
	    { if(i%2==0)
	      poly[i]+=xmove;
	      else
	      poly[i]+=ymove;
	    }
	    setcolor(RED);
	    drawpoly(5,poly);
	    break;
	    /////////////////////////////////
	    case 3:
		showmouseptr();
	    outtextxy(20,449,"REFLECTION");
	    delay(2000);
	    setfillstyle(SOLID_FILL,7);
	    /* draw the bar */
	    bar(10,452,628,468);
	    int polyp[10];
	    getcoordinates_(&x_,&y_);
	    int x11=x_;
	    polyp[0]=x11;
	    getcoordinates(&x_,&y_);
	    int y11=y_;
	    polyp[1]=y11;
	    getcoordinates_(&x_,&y_);
	    int x22=x_;
	    polyp[2]=x22;
	    getcoordinates(&x_,&y_);
	    int y33=y_;
	    polyp[3]=y33;
	    getcoordinates_(&x_,&y_);
	    int x44=x_;
	    polyp[4]=x44;
	    getcoordinates(&x_,&y_);
	    int y55=y_;
	    polyp[5]=y55;
	    getcoordinates_(&x_,&y_);
	    int x66=x_;
	    polyp[6]=x66;
	    getcoordinates(&x_,&y_);
	    int y77=y_;
	    polyp[7]=y77;
	    polyp[8]=polyp[0];
	    polyp[9]=polyp[1];
	    setcolor(iColor);
	    drawpoly(5,polyp);
	    ///////////////////////reflection about x axis////////////////
	    int polyg[10];
	    for(i=0;i<=9;i++)
	    {if(i%2==0)
	    polyp[i]-=320;
	    else
	    polyp[i]=240-polyp[i];
	    }
	    for(i=0;i<=9;i++)
	    {
	       if(i%2==0)
	       polyg[i]=320+polyp[i];
	       else
	       polyg[i]=240+polyp[i];
	    }
	    setcolor(RED);
	    drawpoly(5,polyg);
	   /////////////////////////////////////////////////////////////////
	    int polyv[10];
	    for(i=0;i<=9;i++)
	    {
	       if(i%2==0)
	       polyv[i]=320-polyp[i];
	       else
	       polyv[i]=240-polyp[i];
	    }
	    setcolor(BLACK);
	    drawpoly(5,polyv);
	    break;
	    case 4:
	    showmouseptr();
	    outtextxy(20,449,"SCALING BY A FACTOR OF 2 and 1/2");
	    delay(2000);
	    setfillstyle(SOLID_FILL,7);
	    /* draw the bar */
	    bar(10,452,628,468);

	    int polys[10];
	     int polyss[10];

	    getcoordinates_(&x_,&y_);  //right
	    int x111=x_;
	    polys[0]=x111-320;

	    getcoordinates(&x_,&y_);     //left
	    int y111=y_;
	    polys[1]=240-y111;

	    getcoordinates_(&x_,&y_);
	    int x222=x_;
	    polys[2]=x222-320;

	    getcoordinates(&x_,&y_);
	    int y333=y_;
	    polys[3]=240-y333;

	    getcoordinates_(&x_,&y_);
	    int x444=x_;
	    polys[4]=x444-320;

	    getcoordinates(&x_,&y_);
	    int y555=y_;
	    polys[5]=240-y555;

	    getcoordinates_(&x_,&y_);
	    int x666=x_;
	    polys[6]=x666-320;

	    getcoordinates(&x_,&y_);
	    int y777=y_;
	    polys[7]=240-y777;

	    polys[8]=polys[0];
	    polys[9]=polys[1];

	    for(i=0;i<=9;i++)
	    {
	    polyss[i]=polys[i];       //polys stores wrt 320 240
	    if(i%2==0)               //polyss stores wrt 0 0 top left
	    polyss[i]+=320;
	    else
	    polyss[i]=240-polys[i];
	    }
	    setcolor(iColor);
	    drawpoly(5,polyss);
	    getch();
	    int xmotion=polys[4];
	    int ymotion=polys[5];
	    //////////////////////////
	    for(i=0;i<=9;i++)
	    { if(i%2==0)
	    polys[i]-=xmotion;
	    else
		polys[i]-=ymotion;
	    }

	    for(i=0;i<=9;i++)
	    {
	    polyss[i]=polys[i];
	    if(i%2==0)
	    polyss[i]+=320;
	    else
	    polyss[i]=240-polys[i];
	    }

	    setcolor(WHITE);
	    drawpoly(5,polyss);
	    ///////////////////////////
	    getch();
	    polys[0]=2*polys[0];
	    polys[1]=2*polys[1];
	    polys[2]=2*polys[2];
	    polys[3]=2*polys[3];

	    polys[6]=2*polys[6];
	    polys[7]=2*polys[7];

	    polys[8]=polys[0];
	    polys[9]=polys[1];

	    for(i=0;i<=9;i++)
	    { if(i%2==0)
	    polys[i]+=xmotion;

	    else
	    polys[i]+=ymotion;
	    }

	    for(i=0;i<=9;i++)
	    {
	    polyss[i]=polys[i];
	    if(i%2==0)
	    polyss[i]+=320;
	    else
	    polyss[i]=240-polys[i];
	    }

	    setcolor(12);
	    drawpoly(5,polyss);

	    break;

	    case 5:
	    showmouseptr();
	    outtextxy(20,449,"Rotation about an arbitrary point");
	    delay(2000);
	    setfillstyle(SOLID_FILL,7);
	    /* draw the bar */
	    bar(10,452,628,468);

	    int  polyr[10],polyr_[10];
	    int  polyrr[10];

	    getcoordinates_(&x_,&y_);  //right
	    int x1111=x_;
	    polyr[0]=x1111-320;

	    getcoordinates(&x_,&y_);     //left
	    int y1111=y_;
	    polyr[1]=240-y1111;

	    getcoordinates_(&x_,&y_);
	    int x2222=x_;
	    polyr[2]=x2222-320;

	    getcoordinates(&x_,&y_);
	    int y3333=y_;
	    polyr[3]=240-y3333;

	    getcoordinates_(&x_,&y_);
	    int x4444=x_;
	    polyr[4]=x4444-320;

	    getcoordinates(&x_,&y_);
	    int y5555=y_;
	    polyr[5]=240-y5555;

	    getcoordinates_(&x_,&y_);
	    int x6666=x_;
	    polyr[6]=x6666-320;

	    getcoordinates(&x_,&y_);
	    int y7777=y_;
	    polyr[7]=240-y7777;

	    polyr[8]=polyr[0];
	    polyr[9]=polyr[1];

	    for(i=0;i<=9;i++)
	    {
	    polyrr[i]=polyr[i];               //polyr wrt 320 240 mid screen
	    if(i%2==0)
	    polyrr[i]+=320;                  //polyrr wrt 0 0 top left screen
	    else
	    polyrr[i]=240-polyr[i];
	    }

	    setcolor(iColor);    //draw with hardware value
	    drawpoly(5,polyrr);
		getch();


	    getcoordinates_(&x_,&y_);
	    int xooo=x_;
	    int orgxooo=xooo-320;   //arbitrary reference point

	    getcoordinates(&x_,&y_);
	    int yooo=y_;
	    int orgyooo=240-yooo;

	    getch();
	    line(xooo,40,xooo,400);
	    line(50,yooo,600,yooo);
	   // float t=1.57;
//	   for(int y=30;y<=360;y+=30)
//	   {int x,t=y;
	   for(int x=1;x<=5;x++)
	   {
	     int t=60;

	    for(i=0;i<=7;i++)
	    {
		if(i%2==0)
		polyr_[i]=(polyr[i]*cos((t*3.14)/180))-(polyr[i+1]*sin((t*3.14)/180))-(orgxooo*cos((t*3.14)/180))+(orgyooo*sin((t*3.14)/180))+orgxooo;
		//x'=xcost-ysint-hcost+ksint+h
		else
		polyr_[i]=(polyr[i-1]*sin((t*3.14)/180))+(polyr[i]*cos((t*3.14)/180))-(orgxooo*sin((t*3.14)/180))-(orgyooo*cos((t*3.14)/180))+orgyooo;
		//y'=xsint+ycost-hsint-kcost+k
	    }
	    polyr_[8]=polyr_[0];
	    polyr_[9]=polyr_[1];

	    for(i=0;i<=9;i++)
	     {
	     polyrr[i]=polyr_[i];               //polys wrt 3																																20 240 mid screen
	     if(i%2==0)
	     polyrr[i]+=320;                  //polyss wrt 0 0 top left screen
	     else
	     polyrr[i]=240-polyrr[i];
	     }
	     if(x==7)
	     setcolor(12);
	     else
	     setcolor(x);
	     drawpoly(5,polyrr);
	     for(i=0;i<=9;i++)
	     polyr[i]=polyr_[i];
	  //   getch();
	  delay(100);
	    }//end of foe
//	    }
		    ////////////////////////////////////////
	 break;
	}
	break;
	case 7:
	    int n,i,j,k,gd,gm,dy,dx;
	    int x,y,temp;
	    int a[20][2],xi[20];
	    float slope[20];
	    showmouseptr();
	    //outtextxy(200,240,"SCANLINE POLYGON FILLING UNDER CONSTRUCION");
	    outtextxy(20,449,"Enter number of edges");
	    delay(2000);
	    setfillstyle(SOLID_FILL,7);
	    bar(10,452,628,468);
	    choicev=drop_down_menu(menuvS,countS,360,yco1,BLUE,7);
	    switch(choicev)
	    {
	      case 1:
	      n=3;
	      break;
	      case 2:
	      n=4;
	      break;
	      case 3:
	      n=5;
	      break;
	      case 4:
	      n=6;
	      break;
	      case 5:
	      n=7;
	      break;
	    }
	    ////////////////////////////////////////////////////
	     showmouseptr();
	    for(i=0;i<n;i++)
	    {
	     getcoordinates_(&x_,&y_);  //right x
	     a[i][0]=x_;
	     getcoordinates(&x_,&y_);     //left y
	     a[i][1]=y_;
	     getch();
	     putpixel(a[i][0],a[i][1],0);
	   // printf("\tX%d Y%d : ",i,i);
	   // scanf("%d %d",&a[i][0],&a[i][1]);
	    }

	    a[n][0]=a[0][0];
	    a[n][1]=a[0][1];

	/*- draw polygon -*/
	//
	for(i=0;i<n;i++)
	{
	line(a[i][0],a[i][1],a[i+1][0],a[i+1][1]);
	}
	//
	for(i=0;i<n;i++)
	{
	dy=a[i+1][1]-a[i][1];
	dx=a[i+1][0]-a[i][0];

	if(dy==0) slope[i]=1.0;
	if(dx==0) slope[i]=0.0;

		if((dy!=0)&&(dx!=0)) /*- calculate inverse slope -*/
		slope[i]=(float) dx/dy;

	}
	//
	for(y=0;y< 480;y++)
	{
		k=0;
		for(i=0;i<n;i++)
		{
		if(((a[i][1]<=y)&&(a[i+1][1]>y))||((a[i][1]>y)&&(a[i+1][1]<=y)))
		{
			xi[k]=(int)(a[i][0]+slope[i]*(y-a[i][1]));
			k++;
		}
		}

		for(j=0;j<k-1;j++) /*- Arrange x-intersections in order -*/
		for(i=0;i<k-1;i++)
		{
		if(xi[i]>xi[i+1])
		{
		temp=xi[i];
		xi[i]=xi[i+1];
		xi[i+1]=temp;
		}
		}

		setcolor(35);
		for(i=0;i<k;i+=2)
		{
		line(xi[i],y,xi[i+1]+1,y);
		getch();
		}
	}
       ////////////////////////////////////////////////////////////
	break;
	case 8:////////////////////////////algotirhms
	    choicev=drop_down_menu(menuvL,countL,400,yco1,BLUE,7);
	    switch(choicev)
	    { case 1:
		float px[15]={0};
		float py[15]={0};
		float pdx[15],pdy[10];
		float outx[15]={0};
		float outy[15]={0};
		float xmin,ymin,xmax,ymax;
		float sdx[15],sdy[15];
		int s,m,w=0;
	    outtextxy(20,449,"Choose Coordinates of clipping window");
	    delay(2000);
	    setfillstyle(SOLID_FILL,7);
	    bar(10,452,628,468);
	    showmouseptr();
	    getcoordinates_(&x_,&y_);  //right
	    xmin=x_;
	    getcoordinates(&x_,&y_);     //left
	    ymin=y_;
	    getch();
	    putpixel(xmin,ymin,BLACK);
	    getcoordinates_(&x_,&y_);
	    xmax=x_;
	    getcoordinates(&x_,&y_);
	    ymax=y_;
	    getch();
	    putpixel(xmax,ymax,BLACK);

	    int xbeg,ybeg,xend,yend;
	    xbeg=xmin;
	    ybeg=ymin;
	    xend=xmax;
	    yend=ymax;

	    setcolor(15);
	    rectangle(xbeg,ybeg,xend,yend);

	    printf("%d, %d, %d, %d",xbeg,ybeg,xend,yend);

	    n=5;

	    outtextxy(20,449,"Choose Coordinates of object");
	    delay(2000);
	    setfillstyle(SOLID_FILL,7);
	    bar(10,452,628,468);

	    getcoordinates_(&x_,&y_);  //right
	    px[0]=x_-320;
	    getcoordinates(&x_,&y_);     //left
	    py[0]=240-y_;
	    getcoordinates_(&x_,&y_);
	    px[1]=x_-320;
	    getcoordinates(&x_,&y_);
	    py[1]=240-y_;
	    getcoordinates_(&x_,&y_);  //right
	    px[2]=x_-320;
	    getcoordinates(&x_,&y_);     //left
	    py[2]=240-y_;
	    getcoordinates_(&x_,&y_);
	    px[3]=x_-320;
	    getcoordinates(&x_,&y_);
	    py[3]=240-y_;
	    getcoordinates_(&x_,&y_);  //right
	    px[4]=x_-320;
	    getcoordinates(&x_,&y_);     //left
	    py[4]=240-y_;


	    //rectangle(320+xmin,240-ymax,320+xmax,240-ymin);
	    px[n]=px[0];
	    py[n]=py[0];

        for(s=0;s<n;s++)
	    {
		line(320+px[s],240-py[s],320+px[s+1],240-py[s+1]);}
		getch();
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		px[n]=px[0];
		py[n]=py[0];

		int l=0;
		for(m=0;m<n;m++)
		{
		if(px[m]>=xmin && px[m+1]<=xmin)
		  {
			pdx[m]=xmin;
			pdy[m]=py[m]+((py[m+1]-py[m])/(px[m+1]-px[m]))*(xmin-px[m]);
			outx[l]=pdx[m];
			outy[l]=pdy[m];
			z[l].io=1;
			l++;
		  }
		if(px[m]>=xmin && px[m+1]>=xmin)
		  {
			outx[l]=px[m+1];
			outy[l]=py[m+1];
			z[l].io=0;
			l++;
		   }
		if(px[m]<=xmin && px[m+1]>=xmin)
		  {
				  pdx[m]=xmin;

				  pdy[m]=py[m]+((py[m+1]-py[m])/(px[m+1]-px[m]))*(xmin-px[m]);
			  outx[l]=pdx[m];
			  outy[l]=pdy[m];
			  z[l].io=0;
			  l++;
			  outx[l]=px[m+1];
			  outy[l]=py[m+1];
			  z[l].io=0;
			  l++;
		   }
	     }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	outx[l]=outx[0];
	outy[l]=outy[0];

	setcolor(GREEN);

	for(i=0;i<l;i++)
	{
		if(outx[i]==xmin)
		{
		sdx[w]=outx[i];
		sdy[w]=outy[i];
		w++;
		}
	}
////////////////////////////////////////////////////////////
	sort(sdy,w);

	outx[l]=outx[0];
	outy[l]=outy[0];

	for(i=0;i<=l;i++)
	{
		z[i].x=outx[i];
		z[i].y=outy[i];
		z[i].vis=0;
	}

	s=0;
////////////////////////////////////////////////////////////////////
	for(m=0;m<=l-1;m++)
	{	outx[l]=outx[0];
		outy[l]=outy[0];
		sdx[w+1]=sdx[0];
		sdy[w+1]=sdy[0];

		if(z[s].io==0)
		{
			line(320+outx[s],240-outy[s],320+outx[s+1],240-outy[s+1]);
			z[s].vis=1;
			z[s+l].vis=1;
		}
	else if(z[s].io==1)
	{for(i=0;i<=w;i++)
	{
	if(sdy[i]==outy[s])
	{line(320+sdx[i],240-sdy[i],320+sdx[i+1],240-sdy[i+1]);
	z[s].vis=1;
	z[s+l].vis=1;
	break;
	}
	}
	for(int j=0;j<l;j++)
	{if(sdy[i+1]==z[j].y)
	{s=j;
	line(320+outx[s],240-outy[s],320+outx[s+1],240-outy[s+1]);
	z[s].vis=1;
	z[s+l].vis=1;
	break;
	}
	}
	}
	if(s<=l-1){s++;}
	else{s=0;}
	if(s==l)
	{s=0;
	}
	int p=s;
	while(z[s].vis == 1)
	{s++;
	if(s==p+l)
	{ break;
	}
	}
	}
		break;
		}
		break;

		}    //switch closed
     }//while closed
}
