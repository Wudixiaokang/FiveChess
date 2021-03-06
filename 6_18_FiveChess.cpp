// FiveChess.cpp : Defines the entry point for the console application.
// 	 circle(x*SIDE + BL,y*SIDE + BH,QI+2);

#include <iostream>
#include <graphics.h>  
#include "stdio.h"
#include <conio.h>
#include <io.h> 
#include "time.h"

#define ZL 700  //总边长
#define ZH 700
#define CL 640  //次右下点
#define CH 680
#define BL 40  //次左上点
#define BH 80
#define SIDE 40 //格子边长
#define QI 17 // 棋子半径
#define DB 1
#define MAXN 200000000
#define TWO 20
#define VTWO 5
#define THREE 100
#define VTHREE 40
#define FOUR 2000
#define VFOUR 100
#define DE 5

long GuJia(long m);
void GuJia2(long num[10][3][3],long map[20][20]);
long Must(long x,long y,long m);  //第一层判断是否为必走子之一，当前为M走
long Max(long top,long alpha,long beta);
long Min(long top,long alpha,long beta);
FILE *input,*output;
long end_win=0,end_lose=0,end_frist=0,end_lian=0,end_now=0;

typedef struct
{
  long x,y,sc,z;
}node;
typedef struct hashnode
{       
        __int64 var1,var2;
		long win;
		struct hashnode *next;
}hashnode;
node dui[250],dui2[250];
long rear,front;
long xn,yn,now,win,over;
long map[20][20],bian[9][2]={{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}};  // 1为白子（人），2为红子（电脑）
node mapt[500];  //存所有棋子
long tn;        
long left[20][20],up[20][20],lxie[20][20],rxie[20][20];
long zor3[200][10][3][3];
__int64 zor1[3][16][16],zor2[3][16][16];
long nnx=-1,nny=-1;
long score[3][10][3]={ { {0,0,0},  
                         {0,2,3},
					     {0,20,20},
					     {0,10000,41000},
                         {0,20000,200000000}
				       },
					   { {0,0,0},
					     {0,3,4},
					     {0,50,50},
					     {0,4000,6000},
					     {0,10000,200000000}
					   },
					   { {0,0,0},
					     {0,0,0},
						 {0,1,2},
						 {0,3,4},
						 {0,5,6}
					   }
					 } ;


long ok(long x,long y)
{
     if(x>=0 && x<=xn && y>=0 && y<=yn) return 1;
	 else return 0;
}

void move(long x,long y,long m)
{
	 map[y][x] = m;  
	 mapt[++tn].x = x; mapt[tn].y = y; mapt[tn].z = m;
}

void recover(long x,long y)
{
	 map[y][x] = 0;
	 tn--;
}

void print(long map[20][20])
{
     long i,j;
	 for(i=0;i<=yn;i++,printf("\n")) 
	   for(j=0;j<=xn;j++) 
	     printf("%ld ",map[i][j]);
     printf("\n====================================\n");
}
void kp(node dui[],long a,long b)
{
     long i=a,j=b,k;
	 
     if(i<j)
	 {  k = rand()%(b-a) + a;
	    dui[0] = dui[k]; dui[k] = dui[i]; dui[i] = dui[0];
	    while(i<j)
		{  while(i<j && dui[0].sc <= dui[j].sc) j--;
		   dui[i] = dui[j];
		   while(i<j && dui[0].sc >= dui[i].sc) i++;
		   dui[j] = dui[i];
		} 
		dui[i] = dui[0];
		kp(dui,a,i-1);
		kp(dui,i+1,b);
	 }
}
void PrintQiPan(long x,long y,long m)
{    
     char ss[200]="\0";
	 itoa(GuJia(m),ss,10);
     outtextxy(90 , 30 , ss);
}
void HuaQi(long x,long y,long cl)
{
     
  if(cl == 1) { setcolor(WHITE); setfillstyle(WHITE, SOLID_FILL, NULL); }
  else { setcolor(RED);  setfillstyle(RED, SOLID_FILL, NULL); }
     
  fillcircle(BL + x*SIDE , BH + y*SIDE , QI);

}
void QiPan()
{
	 long i,j,xx,yy;
	 setcolor(LIGHTGRAY);

	 for(i=0;i<=xn;i++) line(BL + i*SIDE , BH , BL + i*SIDE , CH);
	 for(i=0;i<=yn;i++) line(BL , BH + i*SIDE , CL , BH + i*SIDE);

	 for(i=0;i<=xn;i++) 
	   for(j=0;j<=yn;j++)
		 if(map[j][i] != 0)
		   HuaQi(i,j,map[j][i]);

	  if(ok(nnx,nny))
      {  
	     xx = nnx*SIDE + BL;  yy = nny*SIDE + BH;		
		 setcolor(WHITE);    
         circle(xx , yy , QI + 3);
	  }

      setcolor(WHITE);
	  if(over == 1) 
	  {   char ss[200]="白棋必胜!";
          outtextxy(300 , 30 , ss);
	  }
	  if(over == 2)
	  {   char ss[200]="红棋必胜!";
	      outtextxy(300 , 30 , ss);
	  }
}

void HuaNow(long x,long y)
{
     long xx,yy,i;
     setcolor(WHITE);
     xx = x*SIDE + BL;  yy = y*SIDE + BH;
     for(i=1;i<8;i+=2)
	 { 
	   line(xx + 21*bian[i][0] , yy + 21*bian[i][1] , xx + 15*bian[i][0] , yy + 15*bian[i][1]);
	 }
}

void Speak(long m,long num) //m胜
{
     setfont(20, 0,"宋体"); 
	 setcolor(WHITE);
     char s1[200],s2[200],s3[200];
	 if(m == 1) { strcpy(s1,"你");   strcpy(s3,"电脑刚刚被你打败了!");}
	 else  { strcpy(s1,"电脑"); strcpy(s3,"你刚刚被电脑打败了!");  }
	 outtextxy(40,20,s3);

	 strcpy(s2,s1); 
     if(num == 1) strcat(s2,"拿到了第一滴血!");
	 if(num == 2) strcat(s2,"完成了一次双杀!");
	 if(num == 3) strcat(s2,"正在大杀特杀!");
	 if(num == 4) strcat(s2,"已经主宰比赛了!");
	 if(num == 5) strcat(s2,"已经杀人如麻!");
	 if(num == 6) strcat(s2,"已经无人能档!");
	 if(num == 7) strcat(s2,"已经杀得变态了!");
	 if(num == 8) strcat(s2,"已经根妖怪一样了!");
	 if(num == 9) strcat(s2,"已经如同神一般!");
	 if(num >= 10) strcat(s2,"已经超越神的杀戮,拜托谁去打败他吧!");

	 if(num > 0 && ( num != 1 || end_frist == 1 ))
      outtextxy(40,40,s2);
}
void LoadRecord()
{
     input = fopen("data.dll","r");
	 fscanf(input,"%ld %ld %ld %ld %ld",&end_win,&end_lose,&end_frist,&end_lian,&end_now); 
	 fclose(input);
}
void SaveRecord()
{
     output = fopen("data.dll","w");
	 fprintf(output,"%ld %ld %ld %ld %ld",end_win,end_lose,end_frist,end_lian,end_now);
	 fclose(output); 
}
void init()
{
	 long i,j,k,r,w;
	 long t;

     nnx=-1,nny=-1,over=0,tn=0;

	 t = time(NULL);
	 srand(t);
	 memset(map,0,sizeof(map));
	  
	 xn = (CL-BL) / SIDE;
	 yn = (CH-BH) / SIDE;
	 memset(map,0,sizeof(map));
	 now = 1 ; win = 0;

	 LoadRecord();
	 cleardevice();
     FlushMouseMsgBuffer();
	 if(DB) { move(7,7,2);  now = 1;}
	 //load();
     QiPan(); 
}
void ClearOld(long x,long y)
{
     long xx,yy,i;
     setcolor(BLACK);
     xx = x*SIDE + BL;  yy = y*SIDE + BH;
     for(i=1;i<8;i+=2)
	 { 
	   line(xx + 21*bian[i][0] , yy + 21*bian[i][1] , xx + 15*bian[i][0] , yy + 15*bian[i][1]);
	 }
}
void MoveMouse(long x,long y,long oldx,long oldy)
{    
     ClearOld(oldx,oldy);
     HuaNow(x,y);
}
void PressMouse(long x,long y)
{
     if(map[y][x] != 0) return ;

     move(x,y,now);
	 now = now%2 +1;
   
     cleardevice();
	 QiPan();
	 HuaNow(x,y);
}

long Pan()
{
	 long i,j,k,r,w;
	 memset(left,0,sizeof(left));
	 memset(up,0,sizeof(up));
     memset(lxie,0,sizeof(lxie));
	 memset(rxie,0,sizeof(rxie));

	 for(i=0;i<=yn;i++) 
	   for(j=0;j<=xn;j++)
	   {
		 if(i != 0 && map[i][j] != 0 && map[i-1][j] == map[i][j] ) up[i][j] = up[i-1][j] + 1;
		 if(j != 0 && map[i][j] != 0 && map[i][j-1] == map[i][j] ) left[i][j] = left[i][j-1] + 1;
		 if(i != 0 && j != 0 && map[i][j] != 0 && map[i][j] == map[i-1][j-1] ) lxie[i][j] = lxie[i-1][j-1] + 1;
		 if(i != 0 && j != xn && map[i][j] != 0 && map[i][j] == map[i-1][j+1] ) rxie[i][j] = rxie[i-1][j+1] + 1;

		 if(up[i][j] == 4 || left[i][j] == 4 || lxie[i][j] == 4 || rxie[i][j] == 4) 
		 { 
		   win = map[i][j];
		   return 1;
		 }
	   }

     return 0;
}
void End()
{
     setcolor(WHITE);
	 setfont(30, 0,"宋体");
     if(win == 1) 
	 {  
	    end_win++; 
		end_frist++;
		if(end_now == 1) { end_lian++; }
		else { end_now = 1; end_lian = 1;}
	    char ss[100] = " you win! ";
        outtextxy(270, 30, ss);
		Speak(1,end_lian);
	 }
	 else 
	 {
	    end_lose++;
		end_frist++;
		if(end_now == 2) { end_lian++; }
		else { end_now = 2; end_lian = 1;}
	    char ss[100] = " you lose! ";
		outtextxy(270, 30, ss);
		Speak(2,end_lian);
	 }
	 setfont(30, 0,"宋体");
	 char sh[100] = "请按回车键重来";
	 outtextxy(250 , 80 , sh);

	 SaveRecord();
	 setfont(20, 0,"宋体");
	 char ss[100] = "当前比分：";  outtextxy(500 , 20 , ss);
	 itoa(end_win,ss,10);   itoa(end_lose,sh,10);
	 strcat(ss," : ");  strcat(ss,sh);
	 outtextxy(600 , 20 , ss);

	  outtextxy(530 , 60 ,"朽木出品");
}
long GuJia(long m) //m 为正，下步H走
{
     long i,j,k,h,t,t2,sum=0,mm,hh;
	 long xnn,ynn,xnn1,ynn1,xnn2,ynn2,xx,yy;
	 long dir[50][50][9],num[10][3][3],v[50][50][5];
     h = m%2 + 1;

	 memset(dir,0,sizeof(dir));
	 memset(num,0,sizeof(num));
	 memset(v,0,sizeof(v));
     
     for(i=-1;i<=yn+1;i++) 
	   for(j=-1;j<=xn+1;j++)
	     for(t=6;t<=9;t++)
	     {  
		    k = t%8;
		    xx = j + bian[k][0]; yy = i + bian[k][1];
			if( ok(xx,yy) && map[yy][xx] != 0 )
			{
			   if( ok(j,i) == 0 || ( map[i][j] != 0 && map[i][j] != map[yy][xx]) )
			   {  
			      v[yy][xx][ k%4 ]++;   
			      num[ dir[yy][xx][ k%4 ] ][ map[yy][xx] ][ v[yy][xx][ k%4 ] ]++;

				  if( ok(j,i) )  //不同子相遇
				  {
				    v[i][j][ k%4 ]++;
					dir[i][j][ k%4 ] = 1;
				  }
			   }
			   else if(map[i][j] == 0) //遇空子
			   {
			      num[ dir[yy][xx][ k%4 ] ][ map[yy][xx] ][ v[yy][xx][ k%4 ] ]++;
			   }
			   else if(map[i][j] == map[yy][xx]) //遇相同子
			   {
			      v[i][j][ k%4 ] = v[yy][xx][ k%4 ];
                  dir[i][j][ k%4 ] = dir[yy][xx][ k%4 ] + 1;
			   }
			}
            else if( ok(xx,yy) && map[yy][xx] == 0)
			{ 
			   if(ok(j,i) != 0 && map[i][j] != 0) //遇空子
			   {  
			      dir[i][j][ k%4 ] = 1; 
			   }
			}
			else if( ok(xx,yy) == 0) 
			{ 
			   if( ok(j,i) != 0 && map[i][j] != 0) //遇边界
			   {  
			      v[i][j][ k%4 ]++;
			      dir[i][j][ k%4 ] = 1; 
			   }
			}
	     }
     
	 for(i=5;i<=9;i++)
	   for(j=0;j<=2;j++) 
	     if(num[i][h][j] > 0) return -MAXN-2;
	     else if(num[i][m][j] > 0) return MAXN+2;

      for(j=0;j<=1;j++)
	    if(num[4][h][j] > 0) return -MAXN-1;
      
      if(num[4][m][0] > 0) return MAXN+1;
   
      if(num[3][h][0] > 0 && num[4][m][1] == 0) return -MAXN;
	  
	  if(num[3][m][0] + num[4][m][1] >= 2) sum += 999999;       

	  for(i=1;i<=4;i++) 
	    for(j=0;j<=1;j++)
	    { 
	       sum += num[i][m][j] * score[j][i][1];
	       sum -= num[i][h][j] * score[j][i][2];
	    }
      return sum; 
}

long Must(long x,long y,long m)  //第一层判断是否为必走子之一，当前为M走
{
     long i,j,h,xm,ym,r,w;
	 long num[10][3][3];

	 if(m == 1) h = 2; 
	 else h = 1;
     
	 move(x,y,h);
	 GuJia2(num,map);
     for(i=5;i<=9;i++) 
	   for(j=0;j<=2;j++) 
	     if(num[i][h][j] > 0) 
		 { recover(x,y);  return 1; }
	 if(num[4][h][0] > 0 && num[3][m][0] + num[3][m][1] + num[4][m][0] + num[4][m][1] == 0) {recover(x,y); return 1;}
     recover(x,y);
  
     move(x,y,m);
	 if(GuJia(m) >= MAXN) { recover(x,y); return 1;}
     recover(x,y);
	 return 0;
}

long mx=0,my=0,xx=10,yy=10;

long Min(long top,long alpha,long beta)
{
     long i,j,k,xm,ym,m,xmm,ymm,h,r,w,dn=0;
	 long var,t=0,fin;
	 __int64 var1,var2;
	 long v[20][20];
	 node dui[300];
     
     memset(v,0,sizeof(v));
	 if(top%2 == 1) { m = 2; h = 1;}
	 else { m = 1; h =2 ;}
	 if(top == DE)
	   return GuJia(2);

         for(r=1;r<=tn;r++)
	       for(k=0;k<=7;k++)
		   {  i = mapt[r].y;     j = mapt[r].x;
		      xm = j+bian[k][0]; ym = i+bian[k][1];
		      if(ok(xm,ym) && map[ym][xm] == 0 && v[ym][xm] == 0)
              { 
		         if( Must(xm,ym,1) )
				 { 
				    dui[++dn].x = xm;  dui[dn].y = ym;
					move(xm,ym,m);
					dui[dn].sc = -GuJia(1);
					recover(xm,ym);
					t = 1;
				 }
				 v[ym][xm] = 1;
		      } 
		   }
     memset(v,0,sizeof(v));
     if(t == 0)
           for(r=1;r<=tn;r++)
	         for(k=0;k<=7;k++)
		     {  i = mapt[r].y;     j = mapt[r].x;
		        xm = j+bian[k][0]; ym = i+bian[k][1];
		        if(ok(xm,ym) && map[ym][xm] == 0 && v[ym][xm] == 0)
                { 
		           dui[++dn].x = xm;  
			   	   dui[dn].y = ym;
				   move(xm,ym,m);
				   dui[dn].sc = -GuJia(1);
				   recover(xm,ym);
				   v[ym][xm] = 1;
		        } 

			  /*xmm = xm+bian[k][0];   ymm = ym+bian[k][1];
			  if(ok(xm,ym) && map[ym][xm] == 0 && ok(xmm,ymm) && v[ymm][xmm] == 0 && map[ymm][xmm]==0)
			  { 
			     dui[++dn].x = xmm;
				 dui[dn].y = ymm;
				 move(xmm,ymm,m);
				 dui[dn].sc = -GuJia(1);
				 recover(xmm,ymm);
				 v[ymm][xmm] = 1;
			  }*/
               
		     }
     kp(dui,1,dn);

	 for(i=1;i<=dn;i++)
	 {
	    xm = dui[i].x;  ym = dui[i].y;
		move(xm,ym,m);
		if(dui[i].sc >= MAXN) 
		{ 
		  recover(xm,ym); 
		  continue; 
		}
		if(dui[i].sc <= -MAXN-2) 
		{  
		   recover(xm,ym); 
		   return -MAXN-2; 
		}

		var = Max(top+1 , beta , alpha);
		recover(xm,ym);

		if(var <= beta) return beta;
		if(var < alpha) alpha = var;
	 }
    if(alpha >= MAXN+3) alpha = MAXN+2;
    return alpha; 
}

long Max(long top,long alpha,long beta)
{
     long i,j,k,xm,ym,xmm,ymm,m,h,r,w,dn = 0;
	 long var,v[50][50],v2[50][50],t=0,fin;
	 __int64 var1,var2;
	 node dui[300];
	 
	 memset(v,0,sizeof(v));
	 memset(v2,0,sizeof(v2));

	 if(top%2 == 1) { m = 2; h = 1;}
	 else { m = 1; h = 2;}
	 if(top == DE)
	   return -GuJia(1); 
     
        //找是否存在必须走的地方。
	   
	     for(r=1;r<=tn;r++)
	       for(k=0;k<=7;k++)
		   {  i = mapt[r].y;   j = mapt[r].x;
		      xm = j+bian[k][0]; ym = i+bian[k][1];
		      if(ok(xm,ym) && map[ym][xm] == 0 && v[ym][xm] == 0)
              { 
		         if( Must(xm,ym,2) )
				 { 
				    dui[++dn].x = xm;  dui[dn].y = ym;
					move(xm,ym,m);
					dui[dn].sc = GuJia(2);
					recover(xm,ym);
					t = 1;
				 }
				 v[ym][xm] = 1;
		      } 


		   }

    memset(v,0,sizeof(v));
    if(t == 0)
	       for(r=1;r<=tn;r++)
		     for(k=0;k<=7;k++)
		     {  i = mapt[r].y;    j = mapt[r].x;
		        xm = j+bian[k][0];  ym = i+bian[k][1];
			    if(ok(xm,ym) && map[ym][xm] == 0 && v[ym][xm] == 0)
			    { 
			       dui[++dn].x = xm;
				   dui[dn].y = ym;
				   move(xm,ym,m);
				   dui[dn].sc = GuJia(2);
				   recover(xm,ym);
				   v[ym][xm] = 1;
			    }

			 /* xmm = xm+bian[k][0];   ymm = ym+bian[k][1];
			  if(ok(xm,ym) && map[ym][xm] == 0 && ok(xmm,ymm) && v[ymm][xmm] == 0 && map[ymm][xmm]==0)
			  { 
			     dui[++dn].x = xmm;
				 dui[dn].y = ymm;
				 move(xmm,ymm,m);
				 dui[dn].sc = GuJia(2);
				 recover(xmm,ymm);
				 v[ymm][xmm] = 1;
			  }*/
		     }
   
     kp(dui,1,dn);

     for(i=dn;i>=1;i--)
	 {  
	    xm = dui[i].x;  ym = dui[i].y; 
		move(xm,ym,m);
		if(dui[i].sc <= -MAXN  ) 
		{  
		   recover(xm,ym);
	       continue;
		}
		if(dui[i].sc >= MAXN+2)
		{  
	       if(top == 1) { xx = xm; yy = ym; }
		   recover(xm,ym);
		   return MAXN+2;
		}

		var = Min(top+1 , beta , alpha);
		recover(xm,ym);
        
		
		if(var >= beta) return beta;
		if(var > alpha )
		{  if(top == 1) { xx = xm; yy = ym; }
		   alpha = var;
		}
	 }
    if(alpha <= -MAXN-3) alpha++;
    return alpha;  
} 

void ComXia(long x,long y)
{
     long i,j;
	 move(x,y,now);
	 now = 1;
     nnx = x; nny = y;
	 cleardevice();
	 QiPan();
	 HuaNow(x,y);
}

void GuJia2(long num[10][3][3],long map[20][20]) //直接返回NUM数组的估价
{
     long i,j,k,h,t,t2,sum=0,mm,hh,m;
	 long xnn,ynn,xnn1,ynn1,xnn2,ynn2,xx,yy;
	 long dir[50][50][9],v[50][50][5];
     h = m%2 + 1;

	 memset(dir,0,sizeof(dir));
	 memset(num,0,10*3*3*4);
	 memset(v,0,sizeof(v));
     
     for(i=-1;i<=yn+1;i++) 
	   for(j=-1;j<=xn+1;j++)
	     for(t=6;t<=9;t++)
	     {  
		    k = t%8;
		    xx = j + bian[k][0]; yy = i + bian[k][1];

			if( ok(xx,yy) && map[yy][xx] != 0 )
			{
			   if( ok(j,i) == 0 || ( map[i][j] != 0 && map[i][j] != map[yy][xx]) )
			   {  
			      v[yy][xx][ k%4 ]++;   
			      num[ dir[yy][xx][ k%4 ] ][ map[yy][xx] ][ v[yy][xx][ k%4 ] ]++;

				  if( ok(j,i) )  //不同子相遇
				  {
				    v[i][j][ k%4 ]++;
					dir[i][j][ k%4 ] = 1;
				  }
			   }
			   else if(map[i][j] == 0) //遇空子
			   {
			      num[ dir[yy][xx][ k%4 ] ][ map[yy][xx] ][ v[yy][xx][ k%4 ] ]++;
			   }
			   else if(map[i][j] == map[yy][xx]) //遇相同子
			   {
			      v[i][j][ k%4 ] = v[yy][xx][ k%4 ];
                  dir[i][j][ k%4 ] = dir[yy][xx][ k%4 ] + 1;
			   }
			}
            else if( ok(xx,yy) && map[yy][xx] == 0)
			{ 
			   if(ok(j,i) != 0 && map[i][j] != 0) //遇空子
			   {  
			      dir[i][j][ k%4 ] = 1; 
			   }
			}
			else if( ok(xx,yy) == 0) 
			{ 
			   if( ok(j,i) != 0 && map[i][j] != 0) //遇边界
			   {  
			      v[i][j][ k%4 ]++;
			      dir[i][j][ k%4 ] = 1; 
			   }
			}
	     }

}

void Xia()  //当红子必败的时候，红子的走法
{
     long i,j,k,r,w,dn=0,ans=1,m=1,mm=-1;
	 long xm,ym,xmm,ymm,v[50][50],num[10][3][3],oldnum[10][3][3];
	 node dui[500];
     

	 memset(v,0,sizeof(v));
  
     GuJia2(oldnum,map);

     for(i=0;i<=yn;i++)
	   for(j=0;j<=xn;j++)
	     if(map[i][j] != 0) 
		   for(k=0;k<=7;k++)
		   {
		      xm = j+bian[k][0];  ym = i+bian[k][1];
			  if(ok(xm,ym) && map[ym][xm] == 0 && v[ym][xm] == 0)
			  { 
			     dui[++dn].x = xm;
				 dui[dn].y = ym;
				 move(xm,ym,m);
				 recover(xm,ym);
				 v[ym][xm] = 1;
			  }
		   }
     for(i=1;i<=dn;i++) 
	 {    
	    xm = dui[i].x;  ym = dui[i].y;
	    move(xm,ym,m);
	    GuJia2(num,map);
		recover(xm,ym);
		for(j=5;j<=9;j++)
		  for(k=0;k<=2;k++) 
		    if(num[j][1][k] > oldnum[j][1][k])
			{ ans = i; goto jiesu; }
        
		
		  if(num[4][1][0] > oldnum[4][1][0]) 
		  { ans = i; mm = 2 ; }

        if(num[3][1][0] > oldnum[3][1][0] && mm < 1) 
		{ ans = i;  mm = 1; }
		   
	 }   

     jiesu:;
	 xx = dui[ans].x;  yy = dui[ans].y;
}   
void solve()
{
     MOUSEMSG m;	
	 long i,j,k,r,w,x,y,ox=-1,oy=-1;
	 long nowx=-1,nowy=-1;
     while(1)


	 {
	    m = GetMouseMsg();
	    x = ( m.x - (BL-SIDE/2) ) / SIDE;
	    y = ( m.y - (BH-SIDE/2) ) / SIDE;
	    if( ok(x,y) )
		{  
	 	  switch(m.uMsg)
		  {
		  	   case WM_MOUSEMOVE:     //移动
				   if(  nowx != x || nowy != y )  MoveMouse(x,y,nowx,nowy);
				   break;

			   case WM_LBUTTONDOWN:  //按下
                    if(map[y][x] != 0) break;
			  	    PressMouse(x,y);
                    over = 0;
					if(Pan())break;

				    k = Max(1,-MAXN-3,MAXN+3);
                    
					if(k >= MAXN ) over =2;
					if(k <= -MAXN ) {  /*over =1; */ Xia(); }
				    ComXia(xx,yy);
				    break;
		  }
		  nowx = x; nowy = y;  
		  if( Pan() ) break;
		}
     }
	 End();
}

int main()
{
    char c;
    initgraph(ZL , ZH);
//	LoadHash();
	do
	{ 
	  
	  init();
	  solve();
	  fflush(stdin);
	  c = getchar() ;
	}
    while(c != 'q');
	closegraph();  
	return 0;
}

