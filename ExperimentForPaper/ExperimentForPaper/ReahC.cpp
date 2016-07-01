#include<fstream>
#include<iostream>
#include"ReadC.h"

using namespace std;

ReadC::ReadC(string name)
{
	Cname=name;
	allContent=readallfile(name);
	string newContent=removeComment(allContent);

	findFunctions(newContent,name);
	
	findStruct(newContent);
	
	getGlobalArea(newContent);
	
	findGlobal(newContent,name);
	
}

ReadC::~ReadC()
{
}

bool ReadC::isFunction(string content,int pos)
{
	bool isFunc1=true;
	bool isFunc2=true;
	int right=content.substr(0,pos).find(")");
	if(right==string::npos)
		return false;
	for(int i=pos-1;i>=0;i--)		//判断是否为函数
	{
		if(content.at(i)=='\n')
			continue;
		if(content.at(i)==')')
		{
			int haveChar=i+1;
			string name="";
			
			int left=i-1;
			int iright=1;
			for(;left>=0;left--)
			{
				if(content.at(left)==')')
					iright++;
				if(content.at(left)=='(')
					iright--;
				if(iright==0)
					break;
			}
			int iname=left-1;
			if(left!=string::npos)
			{
				while(iname>=0&&!((content.at(iname)>=65&&content.at(iname)<=90)||(content.at(iname)>=97&&content.at(iname)<=122)||(content.at(iname)>=48&&content.at(iname)<=57)||content.at(iname)=='_'))
					iname--;
				left=iname;
				while(iname>=0&&((content.at(iname)>=65&&content.at(iname)<=90)||(content.at(iname)>=97&&content.at(iname)<=122)||(content.at(iname)>=48&&content.at(iname)<=57)||content.at(iname)=='_'))
					iname--;
				if(content.substr(iname+1,left-iname)=="switch"||content.substr(iname+1,left-iname)=="while"||content.substr(iname+1,left-iname)=="if"||content.substr(iname+1,left-iname)=="else if"||content.substr(iname+1,left-iname)=="for")
					isFunc1=false;
			}

			while((haveChar<pos)&&!((content.at(haveChar)>=65&&content.at(haveChar)<=90)||(content.at(haveChar)>=97&&content.at(haveChar)<=122)||(content.at(haveChar)>=48&&content.at(haveChar)<=57)||content.at(haveChar)=='_'))
				haveChar++;
			if(haveChar==pos)
			{
				isFunc2=true;
				break;
			}else
			{
				isFunc2=false;
				break;
			}
		}
		if(content.at(i)==';')
		{
			int j=i+1;
			for(;j<pos;j++)
			{
				if(!(content.at(j)==' '||content.at(j)=='\t'||content.at(j)=='\n'))
				{
					isFunc1=false;
					break;
				}
			}
			break;
		}
	}
	return isFunc1&&isFunc2;
}

string ReadC::removeComment(string content)
{
	int lcom=content.find("/*");
	int rcom=0;
	int comCount=0;
	while(lcom!=string::npos)
	{
		comCount++;
		int i=lcom+2;
		for(;i<content.length()-1;i++)
		{
			if(content.at(i)=='/'&&content.at(i+1)=='*')
				comCount++;
			if(content.at(i)=='*'&&content.at(i+1)=='/')
				comCount--;
			if(comCount==0)
				break;
		}
		rcom=i+1;
		content=content.erase(lcom,rcom-lcom+1);
		if(lcom+1<content.length())
			lcom=content.find("/*",lcom);
		else
			break;
	}
	return content;
}

Function* ReadC::dealWithOldC(string content,int pos,int left,int right)
{
	//提取函数参数
	Function *func=new Function();
	string param="";
	string funcname;
	string functype;

	param=content.substr(right+1,pos-right-1);	
	
	//提取函数类型和函数名，判断是否写在了多行
	int k=left-1;
	while(content.at(k)!='\n'&&k>=2)
		k--;
	int t=k-1;
	k=k+1;
	string temp1="";
	string temp2="";
	string temp3="";
	bool isMultiLines=true;
	while(content.at(k)==' '||content.at(k)=='\t'||content.at(k)=='*')
		k++;
	while((content.at(k)>=65&&content.at(k)<=90)||(content.at(k)>=97&&content.at(k)<=122)||(content.at(k)>=48&&content.at(k)<=57)||content.at(k)=='_')
	{
		temp1+=content.at(k);
		k++;
	}
	while(content.at(k)==' '||content.at(k)=='\t'||content.at(k)=='*')
		k++;
	while((content.at(k)>=65&&content.at(k)<=90)||(content.at(k)>=97&&content.at(k)<=122)||(content.at(k)>=48&&content.at(k)<=57)||content.at(k)=='_')
	{
		isMultiLines=false;
		temp2+=content.at(k);
		k++;
	}
	while(content.at(k)==' '||content.at(k)=='\t'||content.at(k)=='*')
		k++;
	while((content.at(k)>=65&&content.at(k)<=90)||(content.at(k)>=97&&content.at(k)<=122)||(content.at(k)>=48&&content.at(k)<=57)||content.at(k)=='_')
	{
		temp3+=content.at(k);
		k++;
	}

	if(!isMultiLines)		//一行
	{
		if(temp3=="")
		{
		functype=temp1;
		funcname=temp2;
		}else
		{
			functype=temp2;
			funcname=temp3;
		}
	}else
	{
		funcname=temp1;
		temp2="";
		temp3="";
		while(content.at(t)!='\n'&&t>=0)
			t--;
		t=t+1;
		while(content.at(t)==' '||content.at(t)=='\t'||content.at(t)=='*')
			t++;
		while((content.at(t)>=65&&content.at(t)<=90)||(content.at(t)>=97&&content.at(t)<=122)||(content.at(t)>=48&&content.at(t)<=57)||content.at(t)=='_')
		{
			temp2+=content.at(t);
			t++;
		}
		while(content.at(t)==' '||content.at(t)=='\t'||content.at(t)=='*')
			t++;
		while((content.at(t)>=65&&content.at(t)<=90)||(content.at(t)>=97&&content.at(t)<=122)||(content.at(t)>=48&&content.at(t)<=57)||content.at(t)=='_')
		{
			temp3+=content.at(t);
			t++;
		}
		if(temp3=="")
			functype=temp2;
		else
			functype=temp3;

	}

	func->parameters=param;
	func->funcName=funcname;
	func->returnType=functype;

	return func;

}

Function* ReadC::dealWithNewC(string content,int pos,int left,int right)
{
	//提取函数参数
	Function *func=new Function();
	string param="";
	string funcname;
	string functype;

	param=content.substr(left+1,right-left-1);		
	

	//提取函数类型和函数名，判断是否写在了多行
	int k=left-1;
	while(content.at(k)!='\n'&&k>=2)
		k--;
	int t=k-1;
	k=k+1;
	string temp1="";
	string temp2="";
	string temp3="";
	bool isMultiLines=true;
	while(content.at(k)==' '||content.at(k)=='\t'||content.at(k)=='*')
		k++;
	while((content.at(k)>=65&&content.at(k)<=90)||(content.at(k)>=97&&content.at(k)<=122)||(content.at(k)>=48&&content.at(k)<=57)||content.at(k)=='_')
	{
		temp1+=content.at(k);
		k++;
	}
	while(content.at(k)==' '||content.at(k)=='\t'||content.at(k)=='*')
		k++;
	while((content.at(k)>=65&&content.at(k)<=90)||(content.at(k)>=97&&content.at(k)<=122)||(content.at(k)>=48&&content.at(k)<=57)||content.at(k)=='_')
	{
		isMultiLines=false;
		temp2+=content.at(k);
		k++;
	}
	while(content.at(k)==' '||content.at(k)=='\t'||content.at(k)=='*')
		k++;
	while((content.at(k)>=65&&content.at(k)<=90)||(content.at(k)>=97&&content.at(k)<=122)||(content.at(k)>=48&&content.at(k)<=57)||content.at(k)=='_')
	{
		temp3+=content.at(k);
		k++;
	}

	if(!isMultiLines)		//一行
	{
		if(temp3=="")
		{
		functype=temp1;
		funcname=temp2;
		}else
		{
			functype=temp2;
			funcname=temp3;
		}
	}else
	{
		funcname=temp1;
		temp2="";
		temp3="";
		while(content.at(t)!='\n'&&t>=0)
			t--;
		t=t+1;
		while(content.at(t)==' '||content.at(t)=='\t'||content.at(t)=='*')
			t++;
		while((content.at(t)>=65&&content.at(t)<=90)||(content.at(t)>=97&&content.at(t)<=122)||(content.at(t)>=48&&content.at(t)<=57)||content.at(t)=='_')
		{
			temp2+=content.at(t);
			t++;
		}
		while(content.at(t)==' '||content.at(t)=='\t'||content.at(t)=='*')
			t++;
		while((content.at(t)>=65&&content.at(t)<=90)||(content.at(t)>=97&&content.at(t)<=122)||(content.at(t)>=48&&content.at(t)<=57)||content.at(t)=='_')
		{
			temp3+=content.at(t);
			t++;
		}
		if(temp3=="")
			functype=temp2;
		else
			functype=temp3;

	}

	func->parameters=param;
	func->funcName=funcname;
	func->returnType=functype;

	return func;

}

int* ReadC::findRight(string content,int pos,int tright,int lastBodyEnd)
{
	//pos-->{ 
	int paramBrace[2]={0};
	int fen=content.find(";",tright);
	bool isParam=true;
	for(int i=tright+1;i<fen;i++)
	{
		if(!(content.at(i)==' '||content.at(i)=='\t'||content.at(i)=='\n'))
		{
			isParam=false;
			break;
		}
	}
	while(isParam)
	{
		while(content.at(tright)!='\n')tright--;
		int enter=tright;
		tright=content.rfind(')',tright-1);
		string mycontent=content.substr(tright+1,enter-tright-1);
		fen=mycontent.find(";");
		if(fen!=string::npos)
		{
			for(int i=tright+1;i<fen+tright+1;i++)
			{
				if(!(content.at(i)==' '||content.at(i)=='\t'||content.at(i)=='\n'))
				{
					isParam=false;
					break;
				}
			}

		}else
			isParam=false;
	}
	
	paramBrace[1]=tright;
	int r=tright-1;
	int iright=1;
	for(;r>lastBodyEnd;r--)
	{
		if(content.at(r)==')')
			iright++;
		if(content.at(r)=='(')
			iright--;
		if(iright==0)
			break;
	}
	paramBrace[0]=r;
	
	return paramBrace;
}

void ReadC::findFunctions(string content,string name)
{
	int pos=int(content.find("{"));
	int lastBodyEnd=0;
	while(pos!=string::npos)
	{
		bool isFunc=isFunction(content,pos);
		int ileft=1;
		int bodyStart=pos+1;
		int bodyEnd=0;
		for(bodyEnd=pos+1;bodyEnd<content.length();bodyEnd++)
		{
			if(content.at(bodyEnd)=='{')
				ileft++;
			else if(content.at(bodyEnd)=='}')
				ileft--;
			if(ileft==0)
				break;
		}

		if(isFunc)
		{
			Function *func=new Function();

			//先处理函数返回类型、函数名称和函数的参数,类型与参数都有可能为空
			int right=pos-1;
			while(content.at(right)!=')')right--;

			int left=right-1;
			int iright=1;
			for(;left>lastBodyEnd;left--)
			{
				if(content.at(left)==')')
					iright++;
				if(content.at(left)=='(')
					iright--;
				if(iright==0)
					break;
			}

			string temp=content.substr(right+1,pos-right-1);///) right和{ pos之间
			int isOldC=int(temp.find(";"));
			if(isOldC!=string::npos)   //古老C写法
			{
				int *paramBrace=findRight(content,pos,right,lastBodyEnd);		//tem---> ),pos--> {
				left=paramBrace[0];
				right=paramBrace[1];
				func=dealWithOldC(content,pos,left,right);
			}
			else		//现代C写法
			{
				func=dealWithNewC(content,pos,left,right);
			}
			//再处理函数体
			func->funcBody=content.substr(bodyStart,bodyEnd-bodyStart);
			func->InCFile=name;

			map<string,Function*>::iterator iter;
			iter=mapForFunc.find(func->funcName);
			if(iter==mapForFunc.end())
			{
				mapForFunc.insert(pair<string,Function*>(func->funcName,func));
			}
		}
		lastBodyEnd=bodyEnd;
		if(bodyEnd+1<content.length())
			pos=content.find("{",bodyEnd+1);
		else
			break;
	}

}

void ReadC::findStruct(string content)
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
				CforStruct.push_back(structName);
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
					CforStruct.push_back(structName);
				
			}
		}
		if(spos+1<content.length())
			spos=int(content.find("struct",spos+1));
		else 
			break;
	}

}

void ReadC::getGlobalArea(string content)
{
	string newContent=content;
	int lastBodyEnd=0;
	int posbrace=newContent.find("{");
	bool isFunc=true;
	
	//cout<<newContent<<endl;
	while(posbrace!=string::npos)
	{
		//cout<<strForGlobal<<endl;
		int ileft=1;
		int bodyStart=posbrace+1;
		int bodyEnd=0;
		for(bodyEnd=posbrace+1;bodyEnd<content.length();bodyEnd++)
		{
			if(content.at(bodyEnd)=='{')
				ileft++;
			else if(content.at(bodyEnd)=='}')
				ileft--;
			if(ileft==0)
				break;
		}
		isFunc=isFunction(newContent,posbrace);
		int end;
		if(isFunc)
		{	
			int right=posbrace-1;		
			for(;right>=0&&newContent.at(right)!=')';right--);
			if(right>0)
			{
			string myGlobal=newContent.substr(right+1,posbrace-right-1);
			int fen=myGlobal.find(";");
			if(fen!=string::npos)
			{
				int *paramBrace=findRight(newContent,posbrace,right,lastBodyEnd);		//tem---> ),pos--> {
				right=paramBrace[1];
				newContent.erase(right+1,posbrace-right-1);
			}
			}
		}
		end=posbrace+1;
		
		if(end+1<newContent.length())
			posbrace=newContent.find("{",end);
		else
			break;
	}

	//cout<<newContent<<endl;

	int leftnum=0;
	for(int i=0;i<int(newContent.length());i++)
	{
		if(newContent.at(i)=='{')
		{
			int posfen;
			for(posfen=int(strForGlobal.length()-1);posfen>=0;posfen--)
			{
				if(strForGlobal.at(posfen)==';')
					break;
				if(strForGlobal.at(posfen)=='=')
				{
					bool isArrayAssign=true;
					for(int j=posfen+1;j<strForGlobal.length();j++)
					{
						if(!(strForGlobal.at(j)==' '||strForGlobal.at(j)=='\t'||strForGlobal.at(j)=='\n'))
						{
							isArrayAssign=false;
							break;
						}
					}
					if(!isArrayAssign)
						continue;
					else
					{
						posfen=posfen-1;
						break;
					}
				}

			}
	
			strForGlobal.erase(posfen+1,strForGlobal.length()-posfen-1);
			leftnum++;
		}
		else if(newContent.at(i)=='}')
			leftnum--;
		//else if(content.at(i)==';')
		//	start = i+1;

		if(leftnum==0)
		{
			if(newContent.at(i)!='}')
				strForGlobal += newContent.at(i);  //append(string(content.at(i)));
		}
		if(leftnum<0)
			break;
	}

}

void ReadC::findGlobal(string content,string name)
{
	string global="";
	int posfen=strForGlobal.find(';');
	//cout<<strForGlobal<<endl;
	while(posfen!=string::npos)
	{
		int start=posfen-1;
		if(start>=0)
		{
		bool isGlobal=true;
		for(;strForGlobal.at(start)!='\n'&&start>=0;start--)
		{
			if(strForGlobal.at(start)=='('||strForGlobal.at(start)==')'||start==0)
			{
				isGlobal=false;
				break;
			}
			
		}
		if(isGlobal)
		{
			int end=posfen-1;
			int equal=strForGlobal.substr(start+1,posfen-start-1).find("=");
			if(equal!=string::npos)
				end=equal+start;
			int iright=0;
			for(;end>start;end--)
			{
				if(strForGlobal.at(end)==' '||strForGlobal.at(end)=='\t')
					continue;
				if(strForGlobal.at(end)==']')
					iright++;
				if(strForGlobal.at(end)=='[')
					iright--;
				if((strForGlobal.at(end)>=65&&strForGlobal.at(end)<=90)||(strForGlobal.at(end)>=97&&strForGlobal.at(end)<=122)||(strForGlobal.at(end)>=48&&strForGlobal.at(end)<=57)||strForGlobal.at(end)=='_')
				{
					if(iright==0)
						break;
				}
			}
			start=end;
			while(start>=0&&((strForGlobal.at(start)>=65&&strForGlobal.at(start)<=90)||(strForGlobal.at(start)>=97&&strForGlobal.at(start)<=122)||(strForGlobal.at(start)>=48&&strForGlobal.at(start)<=57)||strForGlobal.at(start)=='_'))
				start--;
			global=strForGlobal.substr(start+1,end-start);
			map<string,string>::iterator iter;
			iter=mapForGlobal.find(global);
			if(iter==mapForGlobal.end())
				mapForGlobal.insert(pair<string,string>(global,name));
		}
		if(posfen+1<strForGlobal.length())
			posfen=strForGlobal.find(";",posfen+1);
		else 
			break;
		}else
			posfen=strForGlobal.find(";",posfen+1);
	}
	for(int i=0;i<CforStruct.size();i++)
	{
		if(mapForGlobal.find(CforStruct.at(i))!=mapForGlobal.end())
			mapForGlobal.erase(CforStruct.at(i));
	}


}

string ReadC::readallfile(string cPath)
{
    FILE * pFile;
    long lSize;
    char * buffer;
    int result;
    
#ifdef WIN32
	errno_t err;
	/* 若要一个byte不漏地读入整个文件，只能采用二进制方式打开 */ 
	err = fopen_s (&pFile, cPath.c_str(), "rb" );
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

void ReadC::setnext(ReadC *p)
{
	next = p;
}
ReadC* ReadC::getnext()
{
	return next;
}