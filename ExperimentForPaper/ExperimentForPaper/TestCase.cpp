#include<fstream>
#include<iostream>

#include"TestCase.h"
#include"ReadH.h"
#include"ReadC.h"
#include"ClusteringAlgorithm.h"

using namespace std;

TestCase::TestCase()
{
	testCaseMenu="..//testcases_menu.txt";
	casePath;
	testCaseName="";
	//fv=new FeatureVector();

	fstream menuFile;
	menuFile.open(testCaseMenu.c_str());
	getline(menuFile,casePath);

	int i;
	for(i=int(casePath.length())-1;i>=0&&casePath.at(i)!='/';i--);
	testCaseName=casePath.substr(i+1,casePath.length()-1-i);
	
	cout<<"TestCase: "<<testCaseName<<"  start...."<<endl;
	menuFile.close();

}

void TestCase::dfsFolder(string path)
{
	#ifdef WIN32
	struct _finddata_t fileinfo;
	int hFile = 0;
	string strFind = path+"//*.*";
	hFile	= int(_findfirst (strFind.c_str(), &fileinfo ));
	if(hFile!=-1L)
	{
		do{
			 //判断是否有子目录
			if (fileinfo.attrib & _A_SUBDIR)    
			{
				//这个语句很重要
				if( (strcmp(fileinfo.name,".") != 0 ) &&(strcmp(fileinfo.name,"..") != 0))   
				{
					string newPath = path + "//" + fileinfo.name;
					dfsFolder(newPath);
				}
			}
			else  
			{
				string				strFileName, strC_ext,  strH_ext; 
				strFileName = fileinfo.name;
				int iLen			= int(strFileName.length());
				
				if(iLen>=2)
				{
					strH_ext		= strFileName.substr ( iLen-2, 2 );
					strC_ext	= strFileName.substr ( iLen-2, 2 );
				}
				else
				{
					strC_ext		= "";
					strH_ext     = "";
				}
				if ( strC_ext == ".c" )   
				{  
					aryC_fileName.push_back(path+"//"+strFileName);
				} 
				else if ( strH_ext == ".h")
				{
					aryH_fileName.push_back(path+"//"+strFileName);
				}
			}
		}while (_findnext(hFile, &fileinfo) == 0);

		
	}
	_findclose( hFile );

	#else
	DIR              *pDir ;  
        struct dirent    *ent  ;  
        int               i=0  ;  
        char              childpath[512];  
  
        pDir=opendir(path.c_str());  
        memset(childpath,0,sizeof(childpath));  
  
  
        while((ent=readdir(pDir))!=NULL)  
        {  
  
           if(ent->d_type & DT_DIR)  
           {  
  
              if(strcmp(ent->d_name,".")==0 || strcmp(ent->d_name,"..")==0)  
                       continue;  
  
              sprintf(childpath,"%s/%s",path.c_str(),ent->d_name);  
              //printf("path:%s/n",childpath);  
  
              dfsFolder(string(childpath));  
  
           }  
	   else
	   {
		if(strstr(ent->d_name, "cpp")!=NULL)
             	{
                   arycpp_fileName.push_back(path+"//"+ent->d_name);
                   //cout<<"add:"<<path+"//"+ent->d_name<<endl;
             	}
	        if(strstr(ent->d_name, "h")!=NULL)
             	{
                   aryh_fileName.push_back(path+"//"+ent->d_name);
                   //cout<<"add:"<<path+"//"+ent->d_name<<endl;
             	}
	   }
        }  
	#endif
}

void TestCase::readEachFile()
{
#ifdef WIN32
	runtime = ::GetTickCount();
#else
	gettimeofday(&runtime,NULL);
#endif
	fstream testcaseRecord;
	testcaseRecord.open("..//log//record.txt",ios::out);
	if(!testcaseRecord)
		cout<<"error open!"<<endl;

	for ( int i=0; i < int(aryH_fileName.size()); i++ )
	{
		ReadH *p		= new ReadH (aryH_fileName.at(i));

		int k;
		for(k=int(aryH_fileName.at(i).length())-1;aryH_fileName.at(i).at(k)!='/';k--);
		string headname = aryH_fileName.at(i).substr(k+1,aryH_fileName.at(i).length()-1-k);   //只取文件名，不要路径

		hmap.insert(pair <string,ReadH*>(headname, p));

		testcaseRecord<<p->Hname<<"文件中struct:\n";
		for(int j=0;j<p->HforStruct.size();j++)
		{
			testcaseRecord<<p->HforStruct.at(j)<<"\n";
		}
	}

	for ( int i=0; i < int(aryC_fileName.size()); i++ )
	{
		ReadC *p		= new ReadC (aryC_fileName.at(i));
		p->setnext(head);
		head = p;

		testcaseRecord<<p->Cname<<"文件中struct:"<<"\n";
		
		for(int j=0;j<p->CforStruct.size();j++)
		{
			testcaseRecord<<p->CforStruct.at(j)<<"\n";
		}

		testcaseRecord<<p->Cname<<"文件中functions:"<<"\n";
		map<string,Function*>::iterator iter2;
		for(iter2=p->mapForFunc.begin();iter2!=p->mapForFunc.end();iter2++)
		{
			testcaseRecord<<iter2->first<<"\n";
		}

		testcaseRecord<<p->Cname<<"文件中globals:"<<"\n";
		map<string,string>::iterator iter3;
		for(iter3=p->mapForGlobal.begin();iter3!=p->mapForGlobal.end();iter3++)
		{
			testcaseRecord<<iter3->first<<"\n";
		}
		testcaseRecord<<"\n";
	}

	testcaseRecord.close();
	
	cout<<"TestCase: "<<testCaseName<<"  end reading...."<<endl;
#ifdef WIN32
	int now = runtime;
	runtime = ::GetTickCount();
	now = runtime - now;
	char gettime[100];
	sprintf_s(gettime,"%d",now);
	writeTime += "readfile time : " + string(gettime)+"ms\n";
	allTime += now;
	cout<<writeTime<<endl;
#else
	timeval endtime;
    	gettimeofday(&endtime,NULL);
    	int now = 1000*(endtime.tv_sec - runtime.tv_sec) + (endtime.tv_usec - runtime.tv_usec)/1000; 
    	char gettime[100];
    	sprintf(gettime,"%d",now);
    	writeTime += "readfile time : " + string(gettime)+"ms\n";
    	allTime += now;
#endif

}

void TestCase::doReading()
{
	cout<<"TestCase: "<<testCaseName<<"  start reading...."<<endl;
#ifdef WIN32
    runtime = ::GetTickCount();
	dfsFolder(casePath);

	int now = runtime;
	runtime = ::GetTickCount();
	now = runtime - now;
	char gettime[100];
	sprintf_s(gettime,"%d",now);
	writeTime += "read path time : " + string(gettime)+"ms\n";
	allTime += now;
	cout<<writeTime<<endl;

#else
    gettimeofday(&runtime,NULL);
    dfsFolder(strTestPath);
    timeval endtime;
    gettimeofday(&endtime,NULL);
    int now = 1000*(endtime.tv_sec - runtime.tv_sec) + (endtime.tv_usec - runtime.tv_usec)/1000; 
    char gettime[100];
    sprintf(gettime,"%d",now);
    writeTime += "read path time : " + string(gettime)+"ms\n";
    allTime += now;
#endif
	readEachFile();
	

}

void TestCase::doParsing()
{
	doReading();

	cout<<"TestCase: "<<testCaseName<<"  start parsing...."<<endl;
#ifdef WIN32
    runtime = ::GetTickCount();
	int now = runtime;

 	Parsing *parse=new Parsing();
	/*
	FeatureVector* fv=NULL;
	
	fv=(FeatureVector*)malloc(sizeof(FeatureVector));
	 if (fv == NULL)
    {
        fputs ("Memory error",stderr); 
   
    }
	*/

	fv=parse->parseToFeatureVector(aryC_fileName.size(),aryH_fileName.size(),head,hmap);

	runtime = ::GetTickCount();
	now = runtime - now;
	char gettime[100];
	sprintf_s(gettime,"%d",now);
	writeTime += "parsing time : " + string(gettime)+"ms\n";
	allTime += now;
	cout<<writeTime<<endl;

#else
    gettimeofday(&runtime,NULL);
    dfsFolder(strTestPath);
    timeval endtime;
    gettimeofday(&endtime,NULL);
    int now = 1000*(endtime.tv_sec - runtime.tv_sec) + (endtime.tv_usec - runtime.tv_usec)/1000; 
    char gettime[100];
    sprintf(gettime,"%d",now);
    writeTime += "parsing time : " + string(gettime)+"ms\n";
    allTime += now;
#endif
}

void TestCase::doClustering()
{
	doParsing();

	cout<<"TestCase: "<<testCaseName<<"  start clustering...."<<endl;
#ifdef WIN32
    runtime = ::GetTickCount();
	int now = runtime;

	CAAlorithm *ca=new CAAlorithm(testCaseName);
	cout<<sizeof(*fv)<<endl;
	ca->runAlgorithm(fv);

	runtime = ::GetTickCount();
	now = runtime - now;
	char gettime[100];
	sprintf_s(gettime,"%d",now);
	writeTime += "clustering time : " + string(gettime)+"ms\n";
	allTime += now;
	cout<<writeTime<<endl;

#else
    gettimeofday(&runtime,NULL);
    dfsFolder(strTestPath);
    timeval endtime;
    gettimeofday(&endtime,NULL);
    int now = 1000*(endtime.tv_sec - runtime.tv_sec) + (endtime.tv_usec - runtime.tv_usec)/1000; 
    char gettime[100];
    sprintf(gettime,"%d",now);
    writeTime += "clustering time : " + string(gettime)+"ms\n";
    allTime += now;
#endif
	
}

void print()
{

}
