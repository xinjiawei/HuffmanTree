#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "windows.h"




	typedef struct
	{
		int weight;
		char Data; //��Žڵ���ַ�
		int Parent, Lchild, Rchild;
	} HTNode, *HuffmanTree;

	typedef char **HuffmanCode;
	void HuffmanCoding(HuffmanTree &, char *, int *, int);
	void select(HuffmanTree HT, int j, int *s1, int *s2); //ѡ��ĸΪ 0��Ȩֵ��С���������
	void Initialization();								  // ��ʼ���շ�����
	void Coding();										  //�շ�������
	void Decoding();									  //�շ�������
	void find(HuffmanTree & HT, char *code, char *text, int i, int m);
	HuffmanTree HT;
	int n = 0; //�շ�����Ҷ�ӽ����Ŀ

	void set_console_color(unsigned short color_index)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_index);
	}
	//------------------------------------------------------------------------------
	//������

	int main()
	{
		char T;
		while (1)
		{
			printf("\n");
			set_console_color(1);
			printf("\t\t\t\t-----------------�շ�������/������-----------------\n");
			set_console_color(6);
			printf("\t\t\t\t�����շ�����-------------------------------------I\n");
			printf("\t\t\t\t���кշ�������-----------------------------------C\n");
			printf("\t\t\t\t���кշ�������-----------------------------------D\n");
			printf("\t\t\t\t�˳��˳���---------------------------------------Q\n");
			printf("\t\t\t\t�����루I C D Q��                          \n\t\t\t\t");
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
				printf(	"\t\t\t\t������� \n" );		
		}							
		getchar();
	}
	return	0;
}

//------------------------------------------------------------------------------
                   //��ʼ������������n���ַ������Ӧ��Ȩֵ������Ȩֵ�����շ�����

void Initialization()
{
	
	FILE *fp;

	set_console_color(2); // ��� n���ַ�
	printf("\t\t\t\t�������ַ��ĸ���:");	
	scanf("%d",&n);

	int i, w[2*n + 1]; //����ַ���Ȩֵ,��̬�洢
	char character[2*n + 1]; //��̬�洢
	//int i, w[n + 1];		 //����ַ���Ȩֵ����̬�洢
	//char character[n + 1];	 //��̬�洢

	set_console_color(2);
	printf("\t\t\t\t������%d���ַ���Ȩֵ:\n\t\t\t\t" ,n);
	for(i=0;i<n;i++)					
	{					
		char  b=getchar();  		
		scanf("%c" ,&character[i]);			
		scanf("%d",&w[i]);								 // �����ַ��Ͷ�Ӧ��Ȩֵ
	}					
	HuffmanCoding(HT,character,w,n);							     // �����շ�����
	if ((fp=fopen("data.txt","w"))==NULL)
		printf("\t\t\t\tdata.txt	��ʧ�� \n"  );	
	for(i=1;i<=2*n-1;i++)				
	{					
		if(fwrite(&(HT[i].Data),sizeof (char),1,fp)!=1)
			printf(	"\t\t\t\tд���ļ�ʧ��  \n");	
		if(fwrite(&(HT[i].weight),sizeof(int),1,fp)!=1)
			printf(	"\t\t\t\tд���ļ�ʧ��  \n");	
		if(fwrite(&(HT[i].Parent),sizeof(int),1,fp)!=1)
			printf(	"\t\t\t\tд���ļ�ʧ��  \n");	
		if(fwrite(&(HT[i].Lchild),sizeof(int),1,fp)!=1)
			printf(	"\t\t\t\tд���ļ�ʧ��  \n");	
		if(fwrite(&(HT[i].Rchild),sizeof(int),1,fp)!=1)
			printf(	"\t\t\t\tд���ļ�ʧ��  \n");		
	}
	set_console_color(3);
	printf("\t\t\t\t�շ������ѽ��������Ѵ��� <<<<data.txt>>>>\n");
	fclose(fp);					

}

//------------------------------------------------------------------------------
															//�����շ��������㷨

void HuffmanCoding(HuffmanTree &HT,char *character, int *w, int n)
{
	int	m,i,s1,s2;
	HuffmanTree p;
	if(n<=1)
		return  ;
	m=2*n-1;
	HT=(HuffmanTree)malloc((m+1)* sizeof(HTNode));
	for(p=HT+1,i=1;i<=n;++i,++p,++character,++w)					   // ����ֵ 
	{
		p->Data=*character;
		p->weight=*w;
		p->Parent=0;
		p->Lchild=0;
		p->Rchild=0;
	} 
	for(;i<=m;++i,++p)								   // ������Ľڵ㸳��ֵΪ 0 
	{
		p->Data=0;
		p->weight=0;
		p->Parent=0;
		p->Lchild=0;
		p->Rchild=0;
	} 
	for(i=n+1;i<=m;++i)										   // �����½ڵ�
	{
		select(HT,i-1,&s1,&s2);	
		HT[s1].Parent=i;HT[s2].Parent=i;
		HT[i].Lchild=s1;HT[i].Rchild=s2;
		HT[i].weight=HT[s1].weight+HT[s2].weight;
	} 
}

//------------------------------------------------------------------------------
											  //ѡ��ĸΪ 0��Ȩֵ��С���������

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
		*s1=i;									// �� s1Ϊ��С�������
	HT[*s1].Parent=1;	// ��ǰ�� HT[*s1].Parent	��ֵ 1�������Ҵ�С��㡢 �ж�����ʱ��Ӱ��
		
		
	for(i=1;i<=j;i++)
		if(HT[i].Parent==0)
		{
			*s2=i;
			break;
		} 					   // ��һ��û���ʹ��ľ�����ѭ�����Դ�Ϊ���Ƚ�
	for(;i<=j;i++)
		if((HT[i].Parent==0)&&(i!=*s1)&&(HT[i].weight<HT[*s2].weight))
		*s2=i;									  // ��weight ��С�Ľ��
}

//------------------------------------------------------------------------------
//���кշ�������

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
			printf("\t\t\t\tToBeTran.txt ��ʧ�� \n");	
		if((fw=fopen("CodeFile.txt","wb+"))==NULL) 	 // �������ļ����ж�/ д
			printf("\t\t\t\tCodeFile.txt ��ʧ�� \n");
		fscanf(fp,"%c",&temp);	// ��fp ��ָ���ļ��ж����һ���ַ���	temp����
		printf("\n\t\t\t\t ����Ϊ��\n");
		while(!feof(fp))
		{
			for(i=1;i<=n;i++)
				if (HT[i].Data==temp)
					break ;				// ���� 1�Žڵ���ͬ������ѭ��
			for(r=0;HC[i][r]!='\0';r++)	// �ںշ������в����ַ����ڵ�λ��
			{
				fputc(HC[i][r],fw);	// ���ַ���Ӧ�ı�������ļ�����	HC[i][r] д�� fw��ָʾ�ļ�
			 	set_console_color(4);
				printf("\n\t\t\t\t %c", HC[i][r]);
			}
				fscanf(fp, "%c", &temp); // ���ļ�������һ���ַ�
		}
		fclose(fw);
		fclose(fp);
		set_console_color(3);
		printf("\n\t\t\t\t ����data.txt	��ToBeTran.txt ����ɹ�������<<<<CodeFile.txt>>>>\n");
}

//------------------------------------------------------------------------------
//���кշ�������

void Decoding()
{
	FILE *fp,*fw;
	int	m,i;
	char *code,*text,*p;
	if((fp=fopen("CodeFile.txt","rb"))==NULL) 			     // ���ļ�����ֻ��
		printf("\t\t\t\tCodeFile.txt	��ʧ�� \n");
	if((fw=fopen("Textfile.txt","wb+"))==NULL) // �������ļ���  / д, ���ѽ��������ݻ����
		printf(	"\t\t\t\tTextfile.txt	��ʧ�� \n");
	code=(char*)malloc(sizeof(char));
	fscanf(fp,"%c",code);							 // ���ļ�����һ�� 0�� 1�ַ�
	for(i=1;!feof(fp);i++)
	{
		code=(char *)realloc(code,(i+1)*sizeof(char));	
		fscanf(fp,"%c",&code[i]);			     // ���ļ�������һ�� 0�� 1���ַ�
	}
	//	���� CodeFile.txt�ļ��е� 0�� 1��ȫ�����룬�����  code������
	text=(char *)malloc(100*sizeof(char));
	p=text;
	m=2*n-1;
	if(*code=='0')
		find(HT,code,text,HT[m].Lchild,m);				   // �Ӹ��ڵ����������
	else
		find(HT,code,text,HT[m].Rchild,m);				   // �Ӹ��ڵ����������
		
	for(i=0;p[i]!='\0';i++)		       // ������õ��ַ������ļ� Textfile.txt ��
		fputc(p[i],fw);						   	// �� p[i]д�� fw��ָʾ���ļ���
	fclose(fp);
	fclose(fw);
	set_console_color(3);
	printf("\n\t\t\t\t ����data.txt ��CodeFile.txt ����ɹ�������<<<<Textfile.txt>>>>\n");
}

//------------------------------------------------------------------------------
//����ʱ���� 01�ַ���Ѱ����ӦҶ�ӽڵ�ĵݹ��㷨

void find(HuffmanTree &HT,char *code,char *text,int i, int m)
{
	if(*code!='\0')											 // ������δ����
	{
		code++;
		if(HT[i].Lchild==0&&HT[i].Rchild==0)				   // ���ҵ�Ҷ�ӽڵ�
		{
			*text=HT[i].Data;					   	// ��Ҷ�ӽڵ��ַ�����text ��
			text++;
			if((*code=='0'))
				find(HT,code,text,HT[m].Lchild,m);	   // �����Ӹ��ڵ����������
			else
				find(HT,code,text,HT[m].Rchild,m);	   // �����Ӹ��ڵ����������
		}
		else	
 			if(*code=='0')
				find(HT,code,text,HT[i].Lchild,m);	     	 // �Ӹýڵ���������
			else
				find(HT,code,text,HT[i].Rchild,m);			 // �Ӹýڵ���������
	}
	else
		*text='\0';
}
