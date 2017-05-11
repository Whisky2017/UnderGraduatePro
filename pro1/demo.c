#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define Max 20000
#define NUM 10

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers



typedef struct ArcCell
{
	int adj;
}ArcCell;
typedef struct VertexType
{
	int number;
	char *sight;
	char *description;
}VertexType;
typedef struct
{
	VertexType vex[NUM];
	ArcCell arcs[NUM][NUM];
	int vexnum,arcnum;
}MGraph;
MGraph G;
void display();
void NextValue(int);
void HaMiTonian(int);
char SearchMenu();
void search();
char Menu();
void output(int sight1,int sight2);
void ShortestPath(int num);
void narrate();
void CreateUDN(int v,int a);
void main();

int x[11]={0};
int P[NUM][NUM];
long int D[NUM];

void main()
{
    system("color 0f");//属于stdio.h
	system("mode con: cols=100 lines=100");//属于stdio.h
	int v0,v1;
	char ck;
	CreateUDN(NUM,11);     //景点个数加1   //调用CreateUNM()函数
	do
	{
		ck=Menu();//调用Menu()
		switch(ck)
		{
		case '1':
			system("cls");
			narrate();//调用narrate函数
			printf("\n\n\t\tO(∩_∩)O温馨提示：输入00退出此环节\n");
M:printf("\n\t\t请选择起点景点(0～9)：");
  scanf("%d",&v0);
  if(v0==00)break;
  printf("\t\t请选择终点景点(0～9)：");              //景点编号
  scanf("%d",&v1);
  if(v0>9||v1>9)
  {
	  printf("\n\t\tO(∩_∩)O请重新输入v0,v1:\n");
	  goto M;
  }
  ShortestPath(v0);    //调用ShortestPath()函数
  output(v0,v1);       //调用output()函数
  goto M;
  break;
		case '2':
			search();//调用search()函数
			//system("ditu.jpg");
			break;
		case '3':
			system("cls");
			narrate();//调用 narrate()函数
			x[0]=1;            //此处必须为x[0]=1        实际为x[0]=true
			HaMiTonian(1);
			//调用哈密尔顿函数  遍历应该从第一个景点开始！！！如果想要修改，必须从HaMiTonian()和NextValue()内部修改
       // case '4' :break;
			printf("\n\n\t\t\t\t请按Enter继续...\n");
			getchar();
			getchar();
			break;
		};
	}while(ck!='00');
}

void CreateUDN(int v, int a)
{
	int i,j;
	G.vexnum=v;
	G.arcnum=a;
	for(i=0;i<G.vexnum;++i) G.vex[i].number=i;
	G.vex[0].sight="南大门";
	G.vex[0].description="华中科技大学正门，门口可见巨大的毛主席汉白玉雕像。";
	G.vex[1].sight="光谷体育馆";
	G.vex[1].description="武汉市内最大、最正规体育馆。是各类比赛及演唱会等文体活动的首选之地。";
	G.vex[2].sight="西十二教学楼";
	G.vex[2].description="建成时为亚洲最大教学楼，正门处立有巨大老子雕像。";
	G.vex[3].sight="东九教学楼";
	G.vex[3].description="建成时为亚洲最大教学楼，正门处立有巨大孔子雕像。";
	G.vex[4].sight="青年园";
	G.vex[4].description="校园内的丛林小径，环境优雅。";
	G.vex[5].sight="名人园";
	G.vex[5].description="多位国内外名人的浮雕和名人警句。";
	G.vex[6].sight="东校区图书馆";
	G.vex[6].description="东校区与主校区由“绝望坡”相连，现已成为我校三大校区之一，东校区图书馆历史悠久，藏书过万。";
	G.vex[7].sight="华中工学院建校纪念碑";
	G.vex[7].description="位于青年园内，造型别致，被同学们戏称“大学四年顶个球”。";
	G.vex[8].sight="西体";
	G.vex[8].description="原主校区体育馆，历史悠久。";
	G.vex[9].sight="南一楼";
	G.vex[9].description="计算机学院主要阵地。";
	for(i=0;i<G.vexnum;++i)
		for(j=0;j<G.vexnum;++j)
		G.arcs[i][j].adj=Max;
		G.arcs[0][1].adj=G.arcs[1][0].adj=400;
		G.arcs[0][9].adj=G.arcs[9][0].adj=130;
		G.arcs[0][2].adj=G.arcs[2][0].adj=570;
		G.arcs[2][4].adj=G.arcs[4][2].adj=450;
		G.arcs[8][4].adj=G.arcs[4][8].adj=230;
		G.arcs[7][4].adj=G.arcs[4][7].adj=150;
		G.arcs[8][7].adj=G.arcs[7][8].adj=320;
		G.arcs[7][9].adj=G.arcs[9][7].adj=740;
		G.arcs[9][5].adj=G.arcs[5][9].adj=540;
		G.arcs[5][1].adj=G.arcs[1][5].adj=400;
		G.arcs[1][3].adj=G.arcs[3][1].adj=1700;
		G.arcs[3][6].adj=G.arcs[6][3].adj=960;
}

void narrate()
{
	int i,k=0;
	printf("\n\t\t***************欢  迎  使  用  校  园  导  游  系  统*************\n");
	printf("\n\t\t          ******************华中科技大学**********************    \n");
	printf("\n\t\t__________________________________________________________________\n");
	printf("                                              景点名称\n");
	printf("\t\t__________________________________________________________________\n");
	for(i=0;i<NUM;i++)
	{
		printf("\t\t【%2d】%-15s\t",i,G.vex[i].sight);
		if(i%2==((NUM-1)%2))printf("\n");//奇数编号景点在右边，偶数编号景点在左边
		k=k+1;
	}
	printf("\t\t_________________________________________________________________\n");
}

void ShortestPath(int num)
{
	int v,w,i,t;
	int final[NUM];
	int min;
	for(v=0;v<NUM;v++)
	{
		final[v]=0;
		D[v]=G.arcs[num][v].adj;
		for(w=0;w<NUM;w++)
			P[v][w]=0;
		if(D[v]<20000)
		{
			P[v][num]=1;
			P[v][v]=1;
		}
	}
	D[num]=0;
	final[num]=1;
	for(i=0;i<NUM;++i)
	{
		min=Max;
		for(w=0;w<NUM;++w)
			if(!final[w])
				if(D[w]<min)
				{
					v=w;
					min=D[w];
				}
				final[v]=1;
				for(w=0;w<NUM;++w)
					if(!final[w]&&((min+G.arcs[v][w].adj)<D[w]))
					{
						D[w]=min+G.arcs[v][w].adj;
						for(t=0;t<NUM;t++)
							P[w][t]=P[v][t];
						P[w][w]=1;
					}
	}
}

void output(int sight1, int sight2)
{
	int a,b,c,d,q=0;
	a=sight2;
	if(a!=sight1)
	{
		printf("\n\t\t\t从(  %s  )到(  %s  )的最短路径是:\n\n",G.vex[sight1].sight,G.vex[sight2].sight);
		printf("\t\t\t%s",G.vex[sight1].sight);
		d=sight1;
		for(c=0;c<NUM;++c)
		{
gate:;
	 P[a][sight1]=0;
	 for(b=0;b<NUM;b++)
	 {
		 if(G.arcs[d][b].adj<20000&&P[a][b])
		 {
			 printf("-->%s",G.vex[b].sight);
			 q=q+1;
			 P[a][b]=0;
			 d=b;
			 if(q%10==0) printf("\n");
			 goto gate;
		 }
     }
		}
		printf("\t(最短距离为 %dm.)\n\t",D[a]);
	}
}

char Menu()
{
	char c;
	int flag;
	do{
		flag=1;
		system("cls");
		narrate();      //调用 narrate()函数
		printf("\n");
		printf("\t\t\t┏━━━━━━━━━━━━━━━━┓\n");
		printf("\t\t\t┃      【1】查询景点路径         ┃\n");
		printf("\t\t\t┃      【2】查询景点信息         ┃\n");
		printf("\t\t\t┃      【3】推荐参观路线         ┃\n");
		printf("\t\t\t┃      【00】退出                ┃\n");
		printf("\t\t\t┗━━━━━━━━━━━━━━━━┛\n");
		printf("\t\t\t\t请输入您的选择：");
		scanf("%c",&c);
		if(c=='1'||c=='2'||c=='3'||c=='00')
			flag=0;
	}while(flag);
	return c;
}

void search()
{
	int num;
	int i;
	char c;
	char name[20];
	char a[4]="100";
	do
	{
		system("cls");
		c=SearchMenu();    //调用SearchMenu()函数
		switch (c)
		{
		case '1':
			system("cls");
			narrate();      //调用 narrate()函数
			printf("\n\t\t\tO(∩_∩)O温馨提示：输入00退出此环节\n");
N: printf("\n\t\t请输入您要查找的景点编号：");
   scanf("%d",&num);
   if(num!=00)
   {
	   for(i=0;i<NUM;i++)
	   {
		   if(num==G.vex[i].number)
		   {
			   printf("\n\t\t您要查找景点信息如下:");
			   printf("\n\n\t\t\t%-25s\n",G.vex[i].description);
			   break;
		   }//if语句
	   }//for语句
	   if(i==NUM)
	   {
		   printf("\n\t\t\t!!!没*有*找*到!!!\n");
	   }//if语句
	   goto N;
   }//if语句
   break;
		case '2':
			system("cls");
			narrate();     //调用 narrate()函数
			printf("\n\t\t\tO(∩_∩)O温馨提示：输入00退出此环节\n");
S:printf("\n\t\t请输入您要查找的景点名称：");
  scanf("%s",name);
  if(!strcmp(name,a))break;
  for(i=0;i<NUM;i++)
  {
	  if(!strcmp(name,G.vex[i].sight))
	  {
		  printf("\n\t\t您要查找景点信息如下:");
		  printf("\n\n\t\t\t%-25s\n",G.vex[i].description);
		  break;
	  }//if语句
  }//for语句
  if(i==NUM)
  {
	  printf("\n\t\t\t!!!没*有*找*到!!!\n");
  }//if语句
  goto S;
  break;
		}//switch语句
	}while(c!='00');//do while 语句
}

char SearchMenu()
{
	char c;
	int flag;
	do{
		flag=1;
		system("cls");
		narrate();      //调用 narrate()函数
		printf("\n");
		printf("\t\t\t┏━━━━━━━━━━━━━━━━┓\n");
		printf("\t\t\t┃      【1】按照景点编号查询     ┃\n");
		printf("\t\t\t┃      【2】按照景点名称查询     ┃\n");
		printf("\t\t\t┃      【00】返回                ┃\n");
		printf("\t\t\t┗━━━━━━━━━━━━━━━━┛\n");
		printf("\t\t\t\t请输入您的选择：");
		scanf("%c",&c);
		if(c=='1'||c=='2'||c=='00')
			flag=0;
	}while(flag);
	return c;
}

void HaMiTonian(int m)
{
if(m>10)   return;
L: NextValue(m);//调用NextValue()函数
   if(x[m]==0)
	   return;
   if(m==9&&G.arcs[0][x[10]-1].adj!=20000)//景点个数减1     景点个数加1
	   display();//调用display()函数
   else
	   HaMiTonian(m+1);//调用HaMiTonian()函数
   goto   L;
}

void NextValue(int k)
{
int j;
l:x[k]=(x[k]+1)%11;//景点个数加1
  if(x[k]==0)
	  return;
  if(G.arcs[x[k-1]-1][x[k]-1].adj!=20000)
  {
	  for(j=0;j<k;j++)
		  if(x[j]==x[k])
			  goto l;
		  return;
  }
  else
	  goto l;
}

void display()
{

	int i=0;
	printf("\n\n");
	for(i=0;i<10;i++)  // 景点个数
		printf("%s->",G.vex[x[i]-1].sight);
	printf("校园出口");
	printf("\n");
}
