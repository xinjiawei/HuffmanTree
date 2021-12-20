#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "windows.h"




	typedef struct
	{
		int weight;
		char Data; //存放节点的字符
		int Parent, Lchild, Rchild;
	} HTNode, *HuffmanTree;

	typedef char **HuffmanCode;
	void HuffmanCoding(HuffmanTree &, char *, int *, int);
	void select(HuffmanTree HT, int j, int *s1, int *s2); //选择父母为 0且权值最小的两个结点
	void Initialization();								  // 初始化赫夫曼树
	void Coding();										  //赫夫曼编码
	void Decoding();									  //赫夫曼译码
	void find(HuffmanTree & HT, char *code, char *text, int i, int m);
	HuffmanTree HT;
	int n = 0; //赫夫曼树叶子结点数目

	void set_console_color(unsigned short color_index)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_index);
	}
	//------------------------------------------------------------------------------
	//主函数

	int main()
	{
		char T;
		while (1)
		{
			printf("\n");
			set_console_color(1);
			printf("\t\t\t\t-----------------赫夫曼编码/译码器-----------------\n");
			set_console_color(6);
			printf("\t\t\t\t建立赫夫曼树-------------------------------------I\n");
			printf("\t\t\t\t进行赫夫曼编码-----------------------------------C\n");
			printf("\t\t\t\t进行赫夫曼译码-----------------------------------D\n");
			printf("\t\t\t\t退出此程序---------------------------------------Q\n");
			printf("\t\t\t\t请输入（I C D Q）                          \n\t\t\t\t");
			scanf("%c", &T);
			switch (T)
			{							
			case 'I':				
				Initialization();				
				break ;				
			case 'C':				
				Coding();			
				break ;				
			case 'D':				
				Decoding();			
				break ;				
			case 'Q':				
				exit(1);				
			default		:				
				printf(	"\t\t\t\t输入错误 \n" );		
		}							
		getchar();
	}
	return	0;
}

//------------------------------------------------------------------------------
                   //初始化函数，输入n个字符及其对应的权值，根据权值建立赫夫曼树

void Initialization()
{
	
	FILE *fp;

	set_console_color(2); // 存放 n个字符
	printf("\t\t\t\t请输入字符的个数:");	
	scanf("%d",&n);

	int i, w[2*n + 1]; //存放字符的权值,动态存储
	char character[2*n + 1]; //动态存储
	//int i, w[n + 1];		 //存放字符的权值，静态存储
	//char character[n + 1];	 //静态存储

	set_console_color(2);
	printf("\t\t\t\t请输入%d个字符及权值:\n\t\t\t\t" ,n);
	for(i=0;i<n;i++)					
	{					
		char  b=getchar();  		
		scanf("%c" ,&character[i]);			
		scanf("%d",&w[i]);								 // 输入字符和对应的权值
	}					
	HuffmanCoding(HT,character,w,n);							     // 建立赫夫曼树
	if ((fp=fopen("data.txt","w"))==NULL)
		printf("\t\t\t\tdata.txt	打开失败 \n"  );	
	for(i=1;i<=2*n-1;i++)				
	{					
		if(fwrite(&(HT[i].Data),sizeof (char),1,fp)!=1)
			printf(	"\t\t\t\t写入文件失败  \n");	
		if(fwrite(&(HT[i].weight),sizeof(int),1,fp)!=1)
			printf(	"\t\t\t\t写入文件失败  \n");	
		if(fwrite(&(HT[i].Parent),sizeof(int),1,fp)!=1)
			printf(	"\t\t\t\t写入文件失败  \n");	
		if(fwrite(&(HT[i].Lchild),sizeof(int),1,fp)!=1)
			printf(	"\t\t\t\t写入文件失败  \n");	
		if(fwrite(&(HT[i].Rchild),sizeof(int),1,fp)!=1)
			printf(	"\t\t\t\t写入文件失败  \n");		
	}
	set_console_color(3);
	printf("\t\t\t\t赫夫曼树已建立，并已存于 <<<<data.txt>>>>\n");
	fclose(fp);					

}

//------------------------------------------------------------------------------
															//建立赫夫曼树的算法

void HuffmanCoding(HuffmanTree &HT,char *character, int *w, int n)
{
	int	m,i,s1,s2;
	HuffmanTree p;
	if(n<=1)
		return  ;
	m=2*n-1;
	HT=(HuffmanTree)malloc((m+1)* sizeof(HTNode));
	for(p=HT+1,i=1;i<=n;++i,++p,++character,++w)					   // 赋初值 
	{
		p->Data=*character;
		p->weight=*w;
		p->Parent=0;
		p->Lchild=0;
		p->Rchild=0;
	} 
	for(;i<=m;++i,++p)								   // 给后面的节点赋初值为 0 
	{
		p->Data=0;
		p->weight=0;
		p->Parent=0;
		p->Lchild=0;
		p->Rchild=0;
	} 
	for(i=n+1;i<=m;++i)										   // 生成新节点
	{
		select(HT,i-1,&s1,&s2);	
		HT[s1].Parent=i;HT[s2].Parent=i;
		HT[i].Lchild=s1;HT[i].Rchild=s2;
		HT[i].weight=HT[s1].weight+HT[s2].weight;
	} 
}

//------------------------------------------------------------------------------
											  //选择父母为 0且权值最小的两个结点

void select(HuffmanTree HT,int j, int *s1, int *s2)
{
	int	i;
	for(i=1;i<=j;i++)
		if (HT[i].Parent==0)
		{
			*s1=i;
			break;
		} 
	for(;i<=j;i++)
		if((HT[i].Parent==0)&&(HT[i].weight<HT[*s1].weight))
		*s1=i;									// 则 s1为最小结点的序号
	HT[*s1].Parent=1;	// 提前给 HT[*s1].Parent	赋值 1，以免找次小结点、 判断条件时受影响
		
		
	for(i=1;i<=j;i++)
		if(HT[i].Parent==0)
		{
			*s2=i;
			break;
		} 					   // 有一个没访问过的就跳出循环，以此为起点比较
	for(;i<=j;i++)
		if((HT[i].Parent==0)&&(i!=*s1)&&(HT[i].weight<HT[*s2].weight))
		*s2=i;									  // 找weight 次小的结点
}

//------------------------------------------------------------------------------
//进行赫夫曼编码

void Coding()
{
	FILE *fp,*fw;
	int	i,f,c,r,start;
	char *cd;
	char temp;
	HuffmanCode HC;
	{
		HC=(HuffmanCode)malloc((n+1)* sizeof (char*));
		cd=(char *)malloc(n*sizeof(char));
		cd[n-1]='\0';
		for(i=1;i<=n;++i)
		{
			start=n-1;
			for(c=i,f=HT[i].Parent;f!=0;c=f,f=HT[f].Parent)
				if (HT[f].Lchild==c)
					cd[--start]='0';
				else
					cd[--start]='1';
			HC[i]=(char *)malloc((n-start)* sizeof(char));  
			strcpy(HC[i],&cd[start]);	
		}
		free(cd);
	}
		if((fp=fopen("ToBeTran.txt","rb"))==NULL) 
			printf("\t\t\t\tToBeTran.txt 打开失败 \n");	
		if((fw=fopen("CodeFile.txt","wb+"))==NULL) 	 // 建立新文件进行读/ 写
			printf("\t\t\t\tCodeFile.txt 打开失败 \n");
		fscanf(fp,"%c",&temp);	// 从fp 所指的文件中读入第一个字符到	temp变量
		printf("\n\t\t\t\t 编码为：\n");
		while(!feof(fp))
		{
			for(i=1;i<=n;i++)
				if (HT[i].Data==temp)
					break ;				// 若和 1号节点相同则跳出循环
			for(r=0;HC[i][r]!='\0';r++)	// 在赫夫曼树中查找字符所在的位置
			{
				fputc(HC[i][r],fw);	// 将字符对应的编码存入文件，把	HC[i][r] 写到 fw所指示文件
			 	set_console_color(4);
				printf("\n\t\t\t\t %c", HC[i][r]);
			}
				fscanf(fp, "%c", &temp); // 从文件读入下一个字符
		}
		fclose(fw);
		fclose(fp);
		set_console_color(3);
		printf("\n\t\t\t\t 根据data.txt	将ToBeTran.txt 编码成功，存于<<<<CodeFile.txt>>>>\n");
}

//------------------------------------------------------------------------------
//进行赫夫曼译码

void Decoding()
{
	FILE *fp,*fw;
	int	m,i;
	char *code,*text,*p;
	if((fp=fopen("CodeFile.txt","rb"))==NULL) 			     // 打开文件进行只读
		printf("\t\t\t\tCodeFile.txt	打开失败 \n");
	if((fw=fopen("Textfile.txt","wb+"))==NULL) // 建立新文件读  / 写, 若已建立，内容会清空
		printf(	"\t\t\t\tTextfile.txt	打开失败 \n");
	code=(char*)malloc(sizeof(char));
	fscanf(fp,"%c",code);							 // 从文件读入一个 0或 1字符
	for(i=1;!feof(fp);i++)
	{
		code=(char *)realloc(code,(i+1)*sizeof(char));	
		fscanf(fp,"%c",&code[i]);			     // 从文件读入下一个 0或 1的字符
	}
	//	至此 CodeFile.txt文件中的 0、 1已全部读入，存放在  code数组中
	text=(char *)malloc(100*sizeof(char));
	p=text;
	m=2*n-1;
	if(*code=='0')
		find(HT,code,text,HT[m].Lchild,m);				   // 从根节点的左子树找
	else
		find(HT,code,text,HT[m].Rchild,m);				   // 从根节点的右子树找
		
	for(i=0;p[i]!='\0';i++)		       // 把译码好的字符存入文件 Textfile.txt 中
		fputc(p[i],fw);						   	// 把 p[i]写到 fw所指示的文件中
	fclose(fp);
	fclose(fw);
	set_console_color(3);
	printf("\n\t\t\t\t 根据data.txt 将CodeFile.txt 译码成功，存于<<<<Textfile.txt>>>>\n");
}

//------------------------------------------------------------------------------
//译码时根据 01字符串寻找相应叶子节点的递归算法

void find(HuffmanTree &HT,char *code,char *text,int i, int m)
{
	if(*code!='\0')											 // 若译码未结束
	{
		code++;
		if(HT[i].Lchild==0&&HT[i].Rchild==0)				   // 若找到叶子节点
		{
			*text=HT[i].Data;					   	// 将叶子节点字符存入text 中
			text++;
			if((*code=='0'))
				find(HT,code,text,HT[m].Lchild,m);	   // 继续从根节点的左子树找
			else
				find(HT,code,text,HT[m].Rchild,m);	   // 继续从根节点的右子树找
		}
		else	
 			if(*code=='0')
				find(HT,code,text,HT[i].Lchild,m);	     	 // 从该节点左子树找
			else
				find(HT,code,text,HT[i].Rchild,m);			 // 从该节点右子树找
	}
	else
		*text='\0';
}
