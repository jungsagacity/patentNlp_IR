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

	map<string,string> patent  ;//�洢IPC�ź� ����+ժҪ�ַ�����
	vector<string> splitRes;//�洢ÿһ���ַ����ִʺ�Ľ��
	vector<string> stopWordFlags;//ͣ�ôʼ��ϱ�־			
	map<string,int> mpSplitResult;	//��ÿһ���ִʽ����ĵ��ڵĴ�Ƶͳ��	
	map<string,struct freDocAttr> resMap;
	TermFreqDocs t;
	CPostgre pg;

	t.generateStopWordFlags(stopWordFlags);//����ͣ�ôʼ���

	start = clock();//����ִ�г�ʼʱ����
	
	
	string hostchinapatent = "dbname=chinapatent user=postgres password=123456 host=localhost port=5432";
	PGconn * conn = pg.connectDatabase(hostchinapatent.c_str()); //����chinapatent���ݿ�

	string hostsubclassterm = "dbname=subclass_term user=postgres password=123456 host=localhost port=5432";
	PGconn * connsubclassterm = pg.connectDatabase(hostsubclassterm.c_str()); //����subclass_term���ݿ�
	
	char * sql = "select ic1,ti,ab from chinapatent where ad >= '2000-01-01 00:00:00'::timestamp without time zone AND ad <= '2010-12-31 00:00:00'::timestamp without time zone \
				  and pa like '%��Ϊ%'; ";

	string sqlTemp = sql;
	pg.GBKToUTF8(sqlTemp);	

	PGresult *respg = PQexec(conn,sqlTemp.c_str());

	end = clock();
	cout<<"����ר����Ŀ��"<<PQntuples(respg)<<"\n";
	cout<<"���ݿ��ѯ��ʱ��"<<1000*(end-start)/CLOCKS_PER_SEC<<" ms\n";
	
	//�򿪴�Ž���ļ�
	int lineMaxSize = 50;
	ifstream file;
	const char * fileName = "log2.txt";
	file.open(fileName);	
	char *line = new char[lineMaxSize];
	struct freDocAttr fda;
	
	//map<string,struct freDocAttr> resMap;
	vector<string> resVec;
	//1����ȡ�洢������ļ�
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


	start = clock();//����ִ�г�ʼʱ����
	if( PQresultStatus( respg ) != PGRES_TUPLES_OK)
	{
		cout<<PQerrorMessage(conn);		
		PQclear(respg);		
		
		return false;
	}
	int patentNum = PQntuples(respg);//��ѯ������������Ԫ����Ŀ
	
	for(int i = 0; i < patentNum; i++)
	{		
		string sPatent;
		pair<string,string> patentTemp;

		patentTemp.first = PQgetvalue(respg,i,0);
		patentTemp.second = PQgetvalue(respg,i,1);
		patentTemp.second +=  PQgetvalue(respg,i,2);
	
		t.UTF8ToGBK(patentTemp.second);		
		patent.insert(patentTemp);//��ȡ����������Ԫ��ĵ�һ�����н����ipc	
		
		
		if(i!=0 && i%10 == 0)//ÿ��ȡ10��ר���Ϳ�ʼ���зִʺʹ洢���Ӷ���߷ִʺʹ洢��Ч�ʡ���������10Ϊ��ѵĳ���
		{
			map<string, string>::iterator patent_begin = patent.begin();

			while( patent_begin != patent.end())
			{
				string strTemp;
				sResult = NLPIR_ParagraphProcess(patent_begin->second.c_str(),1);//�ִʺ�����Ϊһ���ַ�����ʽ	
				strTemp = sResult;//���ִʽ��char * ת��Ϊ string ���д���
								
				t.split(strTemp," ",splitRes,stopWordFlags);//�Էִʽ���ַ��������з֣������˵�ͣ�ôʣ��зֵı�־�ǿո��ַ�

				vector<string>::iterator splitRes_begin = splitRes.begin(),splitRes_end = splitRes.end();				
				t.getTermFre(splitRes_begin,splitRes_end,mpSplitResult);//�Էִʼ��Ͻ����ĵ��ڵĴ�Ƶͳ��
				

				string tablename = patent_begin->first.substr(0,4);//ȡIPC��ǰ4���ַ�����Ϊ�������	
				
				map<string,int>::iterator mp_begin = mpSplitResult.begin();
				map<string,int>::iterator mp_end = mpSplitResult.end();
				t.getTermFreDocs(mp_begin, mp_end,resMap);

				splitRes.clear();//���ÿ��ר���ķִ�
				mpSplitResult.clear();//���ÿ��ר���Ĵ�Ƶ�Լ���Ƶͳ��				
				
				patent_begin++;
			}
			patent.clear();//ÿ����10��ר���Ͷ��������Ͻ������
			
			
		}
	}	

	
	NLPIR_Exit();


	//3���������Ľ�����ǵ�Դ�ļ���
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
	cout<<"�ܺ�ʱ��ʱ: "<<1000*(end-start)/CLOCKS_PER_SEC<<" ms\n";


	
	return 0;
	
}



