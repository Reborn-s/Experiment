#include"ReadH.h"
#include<iostream>
using namespace std;

ReadH::ReadH(string name)
{
	Hname=name;
	allContent=readallfile(name);
	findStruct(allContent);
}

ReadH::~ReadH()
{
}

void ReadH::findStruct(string content)
{
	//先处理typedef struct
	int pos=int(content.find("typedef"));
	string structName="";
	while(pos!=string::npos&&pos+int(strlen("typedef"))<content.length())
	{
		pos+=int(strlen("typedef"));
		int f=pos+1;
		while(!((content.at(f)>=65&&content.at(f)<=90)||(content.at(f)>=97&&content.at(f)<=122)))f++;
		if(content.at(f)=='s'&&content.at(f+1)=='t'&&content.at(f+2)=='r'&&content.at(f+3)=='u'&&content.at(f+4)=='c'&&content.at(f+5)=='t')
		{
		structName="";
		pos=f+6;
		int semipos=int(content.find(";",pos));
		string findArea=content.substr(pos,semipos-pos+1);		//避免typedef struct my_error_mgr * my_error_ptr;这种情况
		int ileft=0;
		int leftpos=int(findArea.find("{"));
		int i=0;
		int structEnd=0;
		if(leftpos!=string::npos){

			for(;i<leftpos;i++)
			{
				if((findArea.at(i)>=65&&findArea.at(i)<=90)||(findArea.at(i)>=97&&findArea.at(i)<=122)||(findArea.at(i)>=48&&findArea.at(i)<=57)||findArea.at(i)=='_')
				{
					structName+=findArea.at(i);
				}
			}
			ileft++;
			i=i+pos+1;
			for(;i<int(content.length());i++)
			{
				if(content.at(i)=='{')
					ileft++;
				else if(content.at(i)=='}')
					ileft--;

				if(ileft==0)
					break;
			}
			structEnd=int(content.find(';',i));
			if(structEnd!=string::npos)
			{
				string newStructName="";
				for(;i<structEnd;i++)
				{
					if((content.at(i)>=65&&content.at(i)<=90)||(content.at(i)>=97&&content.at(i)<=122)||(content.at(i)>=48&&content.at(i)<=57)||content.at(i)=='_')
					{
						newStructName+=content.at(i);
					}
				}
				structName=newStructName;
			}
			if(structName!="")
				HforStruct.push_back(structName);
			if(structEnd<content.length())
				pos=int(content.find("typedef",structEnd));
			else 
				break;
		}else
		{
			if(pos+1<content.length())
				pos=int(content.find("typedef",pos+1));
			else
				break;
		}
		
		}else
			if(pos+1<content.length())
				pos=int(content.find("typedef",pos+1));
			else
				break;
		
	}

	//再处理struct情况
	int spos=content.find("struct");
	while(spos!=string::npos&&spos+int(strlen("struct"))<content.length())
	{
		structName="";
		int sleftpos;
		int f=spos-1;
		
		while(!((content.at(f)>=65&&content.at(f)<=90)||(content.at(f)>=97&&content.at(f)<=122))&&f>=0)f--;

		if(f<8||content.substr(f-6,7)!="typedef")
		{
			spos+=int(strlen("struct"));
			int ssemipos=int(content.find(";",spos));
			string sfindArea=content.substr(spos,ssemipos-spos+1);		//避免typedef struct my_error_mgr * my_error_ptr;这种情况
			sleftpos=int(sfindArea.find("{"));
			bool isStruct=true;
			for(int k=0;k<int(sleftpos);k++)
			{
				if(!((sfindArea.at(k)>=65&&sfindArea.at(k)<=90)||(sfindArea.at(k)>=97&&sfindArea.at(k)<=122)||(sfindArea.at(k)>=48&&sfindArea.at(k)<=57)||sfindArea.at(k)=='_'
					||sfindArea.at(k)==' '||sfindArea.at(k)=='\t'||sfindArea.at(k)=='\n'))
				{
					isStruct=false;
					break;
				}
			}
			if(isStruct==true&&sleftpos!=string::npos){

				for(int j=0;j<sleftpos;j++)
				{
					if((sfindArea.at(j)>=65&&sfindArea.at(j)<=90)||(sfindArea.at(j)>=97&&sfindArea.at(j)<=122)||(sfindArea.at(j)>=48&&sfindArea.at(j)<=57)||sfindArea.at(j)=='_')
					{
						structName+=sfindArea.at(j);
					}
				}
				if(structName!="")
					HforStruct.push_back(structName);
				
			}
		}
		if(spos+1<content.length())
			spos=int(content.find("struct",spos+1));
		else 
			break;
	}

}

string ReadH::readallfile(string hPath)
{
	FILE * pFile;
    long lSize;
    char * buffer;
    int result;
    
#ifdef WIN32
	errno_t err;
	/* 若要一个byte不漏地读入整个文件，只能采用二进制方式打开 */ 
	err = fopen_s (&pFile, hPath.c_str(), "rb" );
    if (err!=0)
    {
        fputs ("File error",stderr);
        exit (1);
    }
#else
	pFile = fopen (name.c_str(), "rb" );
    if (pFile==NULL)
    {
        fputs ("File error",stderr);
        exit (1);
    }
#endif

    /* 获取文件大小 */
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);

    /* 分配内存存储整个文件 */ 
    buffer = (char*) malloc (sizeof(char)*lSize);
    if (buffer == NULL)
    {
        fputs ("Memory error",stderr); 
        exit (2);
    }

    /* 将文件拷贝到buffer中 */
    result = int(fread (buffer,1,lSize,pFile));
    if (result != lSize)
    {
        fputs ("Reading error",stderr);
        exit (3);
    }
    /* 现在整个文件已经在buffer中，可由标准输出打印内容 */
    //printf("%s", buffer); 

    /* 结束演示，关闭文件并释放内存 */
	string ans = string(buffer);
    fclose (pFile);
    free (buffer);
    return ans;
}