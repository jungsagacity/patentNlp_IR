// NLPIR.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "lib/NLPIR.h"
#include <stdio.h>
#include <string.h>
#include "Postgre/postgre.h"
#include "TermFreqDocs.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include <map>
#pragma comment(lib, "lib/NLPIR.lib")
//
int main(int argc, char* argv[])
{
	
	
	const char * sResult;
	if(!NLPIR_Init()) 
	{
		printf("Init fails\n");
		return -1;
	}

	//ofstream log;
	//log.open("F:\\VS8_Projects\\NLPIR\\log1.txt");

	clock_t start,end;

	map<string,string> patent  ;//存储IPC号和 标题+摘要字符串对
	vector<string> splitRes;//存储每一个字符串分词后的结果
	vector<string> stopWordFlags;//停用词集合标志			
	map<string,int> mpSplitResult;	//对每一个分词进行文档内的词频统计	
	map<string,struct freDocAttr> resMap;
	TermFreqDocs t;
	CPostgre pg;

	t.generateStopWordFlags(stopWordFlags);//生成停用词集合

	start = clock();//程序执行初始时间标记
	
	
	string hostchinapatent = "dbname=chinapatent user=postgres password=123456 host=localhost port=5432";
	PGconn * conn = pg.connectDatabase(hostchinapatent.c_str()); //连接chinapatent数据库

	string hostsubclassterm = "dbname=subclass_term user=postgres password=123456 host=localhost port=5432";
	PGconn * connsubclassterm = pg.connectDatabase(hostsubclassterm.c_str()); //连接subclass_term数据库
	
	char * sql = "select ic1,ti,ab from chinapatent where ad >= '2000-01-01 00:00:00'::timestamp without time zone AND ad <= '2010-12-31 00:00:00'::timestamp without time zone \
				  and pa like '%华为%'; ";

	string sqlTemp = sql;
	pg.GBKToUTF8(sqlTemp);	

	PGresult *respg = PQexec(conn,sqlTemp.c_str());

	end = clock();
	cout<<"满足专利数目："<<PQntuples(respg)<<"\n";
	cout<<"数据库查询耗时："<<1000*(end-start)/CLOCKS_PER_SEC<<" ms\n";
	
	//打开存放结果文件
	int lineMaxSize = 50;
	ifstream file;
	const char * fileName = "log2.txt";
	file.open(fileName);	
	char *line = new char[lineMaxSize];
	struct freDocAttr fda;
	
	//map<string,struct freDocAttr> resMap;
	vector<string> resVec;
	//1、读取存储结果的文件
	while(file.getline(line,lineMaxSize))
	{
		string sline = line;
		if(sline.size() == 0) break;
		t.split(sline,",",resVec,stopWordFlags);
		
		fda.freq = atoi(resVec.at(1).c_str());
		fda.docs = atoi(resVec.at(2).c_str());
		fda.attr = atoi(resVec.at(3).c_str());

		resMap.insert( make_pair(resVec.at(0), fda) );

		resVec.clear();
	}
	file.close();	

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


	start = clock();//程序执行初始时间标记
	if( PQresultStatus( respg ) != PGRES_TUPLES_OK)
	{
		cout<<PQerrorMessage(conn);		
		PQclear(respg);		
		
		return false;
	}
	int patentNum = PQntuples(respg);//查询到满足条件的元组数目
	
	for(int i = 0; i < patentNum; i++)
	{		
		string sPatent;
		pair<string,string> patentTemp;

		patentTemp.first = PQgetvalue(respg,i,0);
		patentTemp.second = PQgetvalue(respg,i,1);
		patentTemp.second +=  PQgetvalue(respg,i,2);
	
		t.UTF8ToGBK(patentTemp.second);		
		patent.insert(patentTemp);//获取满足条件的元组的第一列所有结果，ipc	
		
		
		if(i!=0 && i%10 == 0)//每获取10条专利就开始进行分词和存储，从而提高分词和存储的效率。经过试验10为最佳的长度
		{
			map<string, string>::iterator patent_begin = patent.begin();

			while( patent_begin != patent.end())
			{
				string strTemp;
				sResult = NLPIR_ParagraphProcess(patent_begin->second.c_str(),1);//分词后结果，为一个字符串形式	
				strTemp = sResult;//将分词结果char * 转换为 string 进行处理
								
				t.split(strTemp," ",splitRes,stopWordFlags);//对分词结果字符串进行切分，并过滤掉停用词，切分的标志是空格字符

				vector<string>::iterator splitRes_begin = splitRes.begin(),splitRes_end = splitRes.end();				
				t.getTermFre(splitRes_begin,splitRes_end,mpSplitResult);//对分词集合进行文档内的词频统计
				

				string tablename = patent_begin->first.substr(0,4);//取IPC的前4个字符，作为主题表名	
				
				map<string,int>::iterator mp_begin = mpSplitResult.begin();
				map<string,int>::iterator mp_end = mpSplitResult.end();
				t.getTermFreDocs(mp_begin, mp_end,resMap);

				splitRes.clear();//清空每条专利的分词
				mpSplitResult.clear();//清空每条专利的词频以及词频统计				
				
				patent_begin++;
			}
			patent.clear();//每处理10条专利就对向量集合进行清空
			
			
		}
	}	

	
	NLPIR_Exit();


	//3、将处理后的结果覆盖掉源文件；
	ofstream ofile;
	ofile.open(fileName,ios::trunc);
	
	map<string,struct freDocAttr>::iterator it = resMap.begin();
	while(it != resMap.end())
	{
		stringstream sTemp;	

		ofile<<it->first<<",";

		sTemp<<it->second.freq;
		ofile<<sTemp.str()<<",";
		sTemp.str("");
		

		sTemp<<it->second.docs;
		ofile<<sTemp.str()<<",";
		sTemp.str("");

		sTemp<<it->second.attr;
		ofile<<sTemp.str()<<"\n";
		sTemp.str("");

		it++;
	}
	
	ofile.close();

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++

	end = clock();
	cout<<"总耗时耗时: "<<1000*(end-start)/CLOCKS_PER_SEC<<" ms\n";


	
	return 0;
	
}



