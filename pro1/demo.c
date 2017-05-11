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
    system("color 0f");//����stdio.h
	system("mode con: cols=100 lines=100");//����stdio.h
	int v0,v1;
	char ck;
	CreateUDN(NUM,11);     //���������1   //����CreateUNM()����
	do
	{
		ck=Menu();//����Menu()
		switch(ck)
		{
		case '1':
			system("cls");
			narrate();//����narrate����
			printf("\n\n\t\tO(��_��)O��ܰ��ʾ������00�˳��˻���\n");
M:printf("\n\t\t��ѡ����㾰��(0��9)��");
  scanf("%d",&v0);
  if(v0==00)break;
  printf("\t\t��ѡ���յ㾰��(0��9)��");              //������
  scanf("%d",&v1);
  if(v0>9||v1>9)
  {
	  printf("\n\t\tO(��_��)O����������v0,v1:\n");
	  goto M;
  }
  ShortestPath(v0);    //����ShortestPath()����
  output(v0,v1);       //����output()����
  goto M;
  break;
		case '2':
			search();//����search()����
			//system("ditu.jpg");
			break;
		case '3':
			system("cls");
			narrate();//���� narrate()����
			x[0]=1;            //�˴�����Ϊx[0]=1        ʵ��Ϊx[0]=true
			HaMiTonian(1);
			//���ù��ܶ��ٺ���  ����Ӧ�ôӵ�һ�����㿪ʼ�����������Ҫ�޸ģ������HaMiTonian()��NextValue()�ڲ��޸�
       // case '4' :break;
			printf("\n\n\t\t\t\t�밴Enter����...\n");
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
	G.vex[0].sight="�ϴ���";
	G.vex[0].description="���пƼ���ѧ���ţ��ſڿɼ��޴��ë��ϯ���������";
	G.vex[1].sight="���������";
	G.vex[1].description="�人������������������ݡ��Ǹ���������ݳ������������ѡ֮�ء�";
	G.vex[2].sight="��ʮ����ѧ¥";
	G.vex[2].description="����ʱΪ��������ѧ¥�����Ŵ����о޴����ӵ���";
	G.vex[3].sight="���Ž�ѧ¥";
	G.vex[3].description="����ʱΪ��������ѧ¥�����Ŵ����о޴���ӵ���";
	G.vex[4].sight="����԰";
	G.vex[4].description="У԰�ڵĴ���С�����������š�";
	G.vex[5].sight="����԰";
	G.vex[5].description="��λ���������˵ĸ�������˾��䡣";
	G.vex[6].sight="��У��ͼ���";
	G.vex[6].description="��У������У���ɡ������¡����������ѳ�Ϊ��У����У��֮һ����У��ͼ�����ʷ�ƾã��������";
	G.vex[7].sight="���й�ѧԺ��У���";
	G.vex[7].description="λ������԰�ڣ����ͱ��£���ͬѧ��Ϸ�ơ���ѧ���궥���򡱡�";
	G.vex[8].sight="����";
	G.vex[8].description="ԭ��У�������ݣ���ʷ�ƾá�";
	G.vex[9].sight="��һ¥";
	G.vex[9].description="�����ѧԺ��Ҫ��ء�";
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
	printf("\n\t\t***************��  ӭ  ʹ  ��  У  ԰  ��  ��  ϵ  ͳ*************\n");
	printf("\n\t\t          ******************���пƼ���ѧ**********************    \n");
	printf("\n\t\t__________________________________________________________________\n");
	printf("                                              ��������\n");
	printf("\t\t__________________________________________________________________\n");
	for(i=0;i<NUM;i++)
	{
		printf("\t\t��%2d��%-15s\t",i,G.vex[i].sight);
		if(i%2==((NUM-1)%2))printf("\n");//������ž������ұߣ�ż����ž��������
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
		printf("\n\t\t\t��(  %s  )��(  %s  )�����·����:\n\n",G.vex[sight1].sight,G.vex[sight2].sight);
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
		printf("\t(��̾���Ϊ %dm.)\n\t",D[a]);
	}
}

char Menu()
{
	char c;
	int flag;
	do{
		flag=1;
		system("cls");
		narrate();      //���� narrate()����
		printf("\n");
		printf("\t\t\t������������������������������������\n");
		printf("\t\t\t��      ��1����ѯ����·��         ��\n");
		printf("\t\t\t��      ��2����ѯ������Ϣ         ��\n");
		printf("\t\t\t��      ��3���Ƽ��ι�·��         ��\n");
		printf("\t\t\t��      ��00���˳�                ��\n");
		printf("\t\t\t������������������������������������\n");
		printf("\t\t\t\t����������ѡ��");
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
		c=SearchMenu();    //����SearchMenu()����
		switch (c)
		{
		case '1':
			system("cls");
			narrate();      //���� narrate()����
			printf("\n\t\t\tO(��_��)O��ܰ��ʾ������00�˳��˻���\n");
N: printf("\n\t\t��������Ҫ���ҵľ����ţ�");
   scanf("%d",&num);
   if(num!=00)
   {
	   for(i=0;i<NUM;i++)
	   {
		   if(num==G.vex[i].number)
		   {
			   printf("\n\t\t��Ҫ���Ҿ�����Ϣ����:");
			   printf("\n\n\t\t\t%-25s\n",G.vex[i].description);
			   break;
		   }//if���
	   }//for���
	   if(i==NUM)
	   {
		   printf("\n\t\t\t!!!û*��*��*��!!!\n");
	   }//if���
	   goto N;
   }//if���
   break;
		case '2':
			system("cls");
			narrate();     //���� narrate()����
			printf("\n\t\t\tO(��_��)O��ܰ��ʾ������00�˳��˻���\n");
S:printf("\n\t\t��������Ҫ���ҵľ������ƣ�");
  scanf("%s",name);
  if(!strcmp(name,a))break;
  for(i=0;i<NUM;i++)
  {
	  if(!strcmp(name,G.vex[i].sight))
	  {
		  printf("\n\t\t��Ҫ���Ҿ�����Ϣ����:");
		  printf("\n\n\t\t\t%-25s\n",G.vex[i].description);
		  break;
	  }//if���
  }//for���
  if(i==NUM)
  {
	  printf("\n\t\t\t!!!û*��*��*��!!!\n");
  }//if���
  goto S;
  break;
		}//switch���
	}while(c!='00');//do while ���
}

char SearchMenu()
{
	char c;
	int flag;
	do{
		flag=1;
		system("cls");
		narrate();      //���� narrate()����
		printf("\n");
		printf("\t\t\t������������������������������������\n");
		printf("\t\t\t��      ��1�����վ����Ų�ѯ     ��\n");
		printf("\t\t\t��      ��2�����վ������Ʋ�ѯ     ��\n");
		printf("\t\t\t��      ��00������                ��\n");
		printf("\t\t\t������������������������������������\n");
		printf("\t\t\t\t����������ѡ��");
		scanf("%c",&c);
		if(c=='1'||c=='2'||c=='00')
			flag=0;
	}while(flag);
	return c;
}

void HaMiTonian(int m)
{
if(m>10)   return;
L: NextValue(m);//����NextValue()����
   if(x[m]==0)
	   return;
   if(m==9&&G.arcs[0][x[10]-1].adj!=20000)//���������1     ���������1
	   display();//����display()����
   else
	   HaMiTonian(m+1);//����HaMiTonian()����
   goto   L;
}

void NextValue(int k)
{
int j;
l:x[k]=(x[k]+1)%11;//���������1
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
	for(i=0;i<10;i++)  // �������
		printf("%s->",G.vex[x[i]-1].sight);
	printf("У԰����");
	printf("\n");
}
