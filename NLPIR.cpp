// NLPIR.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "lib/NLPIR.h"
#include <stdio.h>
#include <string.h>
#include "Postgre/postgre.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include <map>
#pragma comment(lib, "lib/NLPIR.lib")
//
int main(int argc, char* argv[])
{
	//Sample1: Sentence or paragraph lexical analysis with only one result
	
	const char * sResult;
	if(!NLPIR_Init()) 
	{
		printf("Init fails\n");
		return -1;
	}

	ofstream log;
	log.open("F:\\VS8_Projects\\NLPIR\\log1.txt");

	clock_t start,end;

	
	vector<string> splitRes;//�洢ÿһ���ַ����ִʺ�Ľ��
	vector<string> stopWordFlags;//ͣ�ôʼ��ϱ�־			
	map<string,int> mpSplitResult;	//��ÿһ���ִʽ����ĵ��ڵĴ�Ƶͳ��	
	vector<pair<string,string>> patent  ;//�洢IPC�ź� ����+ժҪ�ַ�����
	CPostgre pg;

	pg.generateStopWordFlags(stopWordFlags);//����ͣ�ôʼ���

	start = clock();//����ִ�г�ʼʱ����
	
	
	string hostchinapatent = "dbname=chinapatent user=postgres password=123456 host=localhost port=5432";
	PGconn * conn = pg.connectDatabase(hostchinapatent.c_str()); //����chinapatent���ݿ�

	string hostsubclassterm = "dbname=subclass_term user=postgres password=123456 host=localhost port=5432";
	PGconn * connsubclassterm = pg.connectDatabase(hostsubclassterm.c_str()); //����subclass_term���ݿ�
	
	char * sql = "select ic1,ti,ab from chinapatent where ad >= '2010-01-01 00:00:00'::timestamp without time zone AND ad <= '2010-12-31 00:00:00'::timestamp without time zone \
				  and ic1 like 'A01B%' ; ";

	PGresult *respg = PQexec(conn,sql);

	end = clock();
	log<<"���ݿ��ѯ��ʱ��"<<1000*(end-start)/CLOCKS_PER_SEC<<" s\n";

	start = clock();//����ִ�г�ʼʱ����
	if( PQresultStatus( respg ) != PGRES_TUPLES_OK)
	{
		log<<PQerrorMessage(conn);		
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
	
		pg.UTF8ToGBK(patentTemp.second);		
		patent.push_back(patentTemp);//��ȡ����������Ԫ��ĵ�һ�����н����ipc	

		
		if(i!=0 && i%100 == 0)//ÿ��ȡ10��ר���Ϳ�ʼ���зִʺʹ洢���Ӷ���߷ִʺʹ洢��Ч�ʡ���������10Ϊ��ѵĳ���
		{
			//log<<i<<"\n";


			vector<pair<string,string>>::iterator patent_begin = patent.begin();
			clock_t start1,end1;
			start1 = clock();
			while( patent_begin != patent.end())
			{
				string strTemp;
								
				
				sResult = NLPIR_ParagraphProcess(patent_begin->second.c_str(),1);//�ִʺ�����Ϊһ���ַ�����ʽ	
				strTemp = sResult;//���ִʽ��char * ת��Ϊ string ���д���
				//log<<strTemp<<end;
				
				
				//log<<"�ִʺ�ʱ: \t"<<1000*(end1-start1)/CLOCKS_PER_SEC<<" ms\n";

				
				pg.split(strTemp," ",splitRes,stopWordFlags);//�Էִʽ���ַ��������з֣������˵�ͣ�ôʣ��зֵı�־�ǿո��ַ�
				

				
				vector<string>::iterator splitRes_begin = splitRes.begin(),splitRes_end = splitRes.end();				
				pg.getTermFre(splitRes_begin,splitRes_end,mpSplitResult);//�Էִʼ��Ͻ����ĵ��ڵĴ�Ƶͳ��
				

				string tablename = patent_begin->first.substr(0,4);//ȡIPC��ǰ4���ַ�����Ϊ�������
				//cout<<"mpSplitResult.size =  "<<mpSplitResult.size()<<endl;
				
				clock_t start3,end3;
				start3 = clock();
				map<string,int>::iterator mp_begin = mpSplitResult.begin(),mp_end = mpSplitResult.end();
				if( !pg.insertTerm(tablename,mp_begin,mp_end,connsubclassterm))
				{
					//log<<"pg.insertTerm ִ��ʧ��\n";
					break;
				}
				end3 = clock();	
				log<<"ÿ��ר���������ݿ��ʱ: "<<1000*(end3-start3)/CLOCKS_PER_SEC<<" ms\n";
				
				
				splitRes.clear();//���ÿ��ר���ķִ�
				mpSplitResult.clear();//���ÿ��ר���Ĵ�Ƶ�Լ���Ƶͳ��				
				
				patent_begin++;
			}
			patent.clear();//ÿ����10��ר���Ͷ��������Ͻ������
			end1 = clock();
			log<<"ÿ10���ִʺ�ʱ: \t"<<1000*(end1-start1)/CLOCKS_PER_SEC<<" ms\n";
			
			//cout<<"patent.size = "<<patent.size()<<endl;
			
		}
	}	

	
	NLPIR_Exit();
	end = clock();

	log<<"�ܺ�ʱ��ʱ: "<<1000*(end-start)/CLOCKS_PER_SEC<<" ms\n";
	return 0;
	
}



