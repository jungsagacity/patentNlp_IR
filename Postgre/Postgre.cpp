#include "stdafx.h"
#include "postgre.h"
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include<windows.h>
#include <ctime>


/*
	功能	：	获取满足条件的专利数据IPC主分类号、标题以及摘要信息

	参数	：	（1）PGconn * conn	#表格所在的数据库连接句柄
				（2）char * pg_sql	#查询语句	
				（3）vector<pair<string,string>> &patent	#存放专利数据结果

	返回值	：	bool	#false（1）、数据库连接不成功；（2）、查询语句语法错误；
						#true 正确的返回结果集

	修改记录：	（1）2013.4.11 吴军 第一次创建
*/
bool  CPostgre::getPatent(PGconn * conn,char * pg_sql,vector<pair<string,string>> &patent)
{
	PGresult *res = NULL;//存放数据库操作（如查询，插入，更新等）后反馈结果信息		
	//PGconn * connIPC=NULL;
	//connIPC = PQconnectdb("dbname=chinapatent user=postgres password=123456 host=localhost port=5432 ");	
	
	pair<string,string> patentTemp;
	if(PQstatus(conn) == CONNECTION_OK)
	{
		//cout<<"成功连接数据库chinapatent......\n";
	}
	else
	{
		cout<<PQerrorMessage(conn);	
		//cout<<"连接数据库失败......\n";
		return false;
	}


	//1、读取IPC数据库中ipc3表中获取所有小类的名称
	//char* pg_sql = "select ic1,ti,ab from chinapatent where ad >= '1985-01-01 00:00:00'::timestamp without time zone AND ad <= '2000-07-01 00:00:00'::timestamp without time zone and ic1 like '%B02B%' limit 1000 ";
	res = PQexec(conn,pg_sql);//查询数据库IPC获取所有的4级IPC号	
	if( PQresultStatus( res ) != PGRES_TUPLES_OK)
	{
		cout<<PQerrorMessage(conn);
		//cout<<"查询数据库有问题.....\n";
		
		PQclear(res);		
		//cout<<"成功断开数据库......\n";
		return false;
	}

	
	int patentNum = PQntuples(res);//查询到满足条件的元组数目
		

	for(int i=0; i<patentNum; i++)
	{		
		char *restemp = new char[1000];
		restemp = PQgetvalue(res,i,0);
		//UTF8ToGBK(restemp);
		patentTemp.first = restemp;
	
		restemp = PQgetvalue(res,i,1);
		//UTF8ToGBK(restemp);	
		patentTemp.second = restemp;

		restemp = PQgetvalue(res,i,2);
		//UTF8ToGBK(restemp);
		patentTemp.second += restemp;	
		//cout<<patentTemp.second<<"\n\n";
		
		patent.push_back(patentTemp);//获取满足条件的元组的第一列所有结果，ipc
		
	}

	PQclear(res);
	return true;

}

/*
	功能	：	获取所有IPC小类名字符串向量组

	参数	：	（1）vector<string> &subClass	#子类名构成的字符串数组		
				（2）PGconn * conn	#表格所在的数据库连接

	返回值	：	bool	#false（1）、数据库连接不成功；（2）、查询语句语法错误
						#true 正常返回所有子类字符串结果集	
	修改记录：	（1）2013.4.11 吴军 第一次创建
*/
bool  CPostgre::getSubClass(vector<string> &subClass,PGconn * conn)
{
	PGresult *res = NULL;//存放数据库操作（如查询，插入，更新等）后反馈结果信息		
	//PGconn * conn=NULL;
	//conn = PQconnectdb("dbname=IPC user=postgres password=123456 host=localhost port=5432 ");	
	
	
	if(PQstatus(conn) == CONNECTION_OK)
	{
		//cout<<"成功连接数据库......\n";
	}
	else
	{
		cout<<PQerrorMessage(conn);	
		//cout<<"连接数据库失败......\n";
		return false;
	}


	//1、读取IPC数据库中ipc3表中获取所有小类的名称
	char* pg_sql = "select ipc from ipc3  ";
	res = PQexec(conn,pg_sql);//查询数据库IPC获取所有的4级IPC号	
	if( PQresultStatus( res ) != PGRES_TUPLES_OK)
	{
		cout<<PQerrorMessage(conn);
		//cout<<"查询数据库有问题.....\n";
		
		PQclear(res);		
		return false;
	}

	
	int subClassNum = PQntuples(res);//查询到满足条件的元组数目
	for(int i=0; i<subClassNum; i++)
	{		
		stringstream s;
		s<<strupr(PQgetvalue(res,i,0));
		subClass.push_back(s.str());//获取满足条件的元组的第一列所有结果，ipc
	}

	PQclear(res);
	return true;

}

/*
	功能	：	创建以小类名命名的表

	参数	：	（1）vector<string> &subClass	#子类名构成的字符串数组		
				（2）PGconn * connIPC	#表格所在的数据库连接

	返回值	：	void

	修改记录：	（1）2013.4.11 吴军 第一次创建
*/
bool CPostgre::createTable(vector<string> &subClass,PGconn * conn)
{
	PGresult *res = NULL;//存放数据库操作（如查询，插入，更新等）后反馈结果信息		
	//conn = PQconnectdb("dbname=subclass_term user=postgres password=123456 host=localhost port=5432 ");		
	
	if(PQstatus(conn) == CONNECTION_OK)
	{
		cout<<"成功连接数据库......\n";
	}
	else
	{
		cout<<PQerrorMessage(conn);	
		cout<<"连接数据库失败......\n";
		
		return false;
	}

	for(int i=0; i< subClass.size(); i++)
	{
		string pg_sql = "create table " + subClass[i] + "(term varchar(32),docs int,tf int,attr int)";
		res = PQexec(conn,pg_sql.c_str());
		if( PQresultStatus( res ) != PGRES_COMMAND_OK)
		{
			cout<<PQerrorMessage(conn);
			cout<<"查询SQL语句语法错误.....\n";
			
			PQclear(res);
			return false;
		}
	}
	PQclear(res);	
	return true;

}

/*
	功能	：	删除所有存储分词结果的表中的数据

	参数	：	（1）vector<string> &subClass	#子类名称的字符串向量组	
				（2）PGconn * connIPC	#表格所在的数据库连接

	返回值	：	void

	修改记录：	（1）2013.4.11 吴军 第一次创建
*/
bool CPostgre::deleteAllTableData(vector<string> &subClass,PGconn * conn)
{
	PGresult *res = NULL;//存放数据库操作（如查询，插入，更新等）后反馈结果信息		
	
	//conn = PQconnectdb("dbname=subclass_term user=postgres password=123456 host=localhost port=5432 ");
		
	
	if(PQstatus(conn) == CONNECTION_OK)
	{
		cout<<"成功连接数据库......\n";
	}
	else
	{
		cout<<PQerrorMessage(conn);	
		cout<<"连接数据库失败......\n";
		
		return false;
	}

	for(int i=0; i< subClass.size(); i++)
	{
		string pg_sql = "delete from " + subClass[i];
		res = PQexec(conn,pg_sql.c_str());
		if( PQresultStatus( res ) != PGRES_COMMAND_OK)
		{
			cout<<PQerrorMessage(conn);
			cout<<"删除数据库IPC有问题.....\n";
			
			PQclear(res);			
			return false;
		}
	}

	PQclear(res);	
	return true;
}

/*
	功能	：	将UTF8编码的字符串转换为GBK编码。需要注意从数据库中读取出来的字符集是gbk编码

	参数	：	（1）string &strUtf8 # 带转换的字符串，同时转换后的结果存放在字符串中		
				
	返回值	：	

	修改记录：	（1）2013.4.11 吴军 第一次创建
*/
void CPostgre::UTF8ToGBK(string &strUtf8)
{
	int len=MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, NULL,0);
    wchar_t * wszGBK = new wchar_t[len];
    memset(wszGBK,0,len);
	MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, wszGBK, len); //转换为unicode

    len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
    char *szGBK=new char[len + 1];//对于字符串，则会存在最后一个为\0，所以需要多申请一个字节的空间
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte (CP_ACP, 0, wszGBK, -1, szGBK, len, NULL,NULL);//转换为GBK

    strUtf8 = szGBK;
    //delete[] szGBK;
    //delete[] wszGBK;
}



/*
	功能	：	将GBK编码的字符串转换为UTF8编码。需要注意从数据库中读取出来的字符集是gbk编码

	参数	：	（1）string &strGBK # 返回转码后的结果			
				
	返回值	：	void

	修改记录：	（1）2013.4.11 吴军 第一次创建
*/
void CPostgre::GBKToUTF8(string &strGBK)  
{  
    string strOutUTF8 = "";  
    WCHAR * str1;  
    int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);  
    str1 = new WCHAR[n];  
    MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);  
    n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);  //转换为unicode
    char * str2 = new char[n];  
    WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);  //转换为UTF8
    strGBK = str2;  

    //delete[]str1;      
    //delete[]str2;  
    
   
} 


/*
	功能	：	按照一定的形式对字符串进行查分，生成字符串向量组,并且去掉了停用词

	参数	：	（1）string &ictclasResult # 中科院分词结果
				（2）const string &pattern #拆分匹配表达式
				（3）vector<string> &result #存放拆分后的结果
				
	返回值	：	void

	修改记录：	（1）2013.4.11 吴军 第一次创建
*/
void CPostgre::split(string &ictclasResult,string splitChar,vector<string> &result,vector<string> &stopWordFlags)
{
	int pos = 0;//匹配字符串的起始位置	
	int size = ictclasResult.size();	
	
	for(int i=0; i<size; i++)//循环匹配
	{				
		pos = ictclasResult.find(splitChar,i);

		if( pos != ictclasResult.npos )
		{
			string term = ictclasResult.substr(i,pos-i);
			if(term.size( ) == 0)
			{
				continue;
			}else
			{
				if( !weedStopWord(stopWordFlags,term) )	
				{					
					result.push_back(term);//将字符串压入向量组
				}						
			}
			
			i = pos;	

		}
	}

	
}


/*
	功能	：	返回数据库连接句柄

	参数	：	（1）string &ipc3 # 应插入的数据库表表明；
				（2）vector<pair<string,int>> &mt #待插入的词项以及词频向量集合
				（3）PGconn * connIPC #数据库连接
				
	返回值	：	void

	修改记录：	（1）2013.4.11 吴军 第一次创建
*/

PGconn * CPostgre::connectDatabase(const char * host)
{
	//return PQconnectdb("dbname=subclass_term user=postgres password=123456 host=localhost port=5432 ");
	return PQconnectdb(host);
}



/*
	功能	：	将专利中标题和摘要分词后的词项和对应出现的频率插入到数据库中

	参数	：	（1）string &ipc3 # 应插入的数据库表表明；
				（2）map<string,int>::iterator map_it #待插入的词项以及词频向量集合
				（3）PGconn * connIPC #数据库连接
				
	返回值	：	void

	修改记录：	（1）2013.4.11 吴军 第一次创建
				（2）2013.4.15 吴军 第一次修改：通过map<string,int>::iterator map_begin和map_end访问存储分词和词频的map容器对象
				（3）2013.4.16 吴军 第二次修改，优化term插入数据库的效率问题。之前的方案是对每一条专利分词和词频统计之后，
				     先检查是否有带插入的term已经存在，如果存在则更新该条数据，否则插入该条数据，插入一条专利数据需要100多ms。
				     而优化的思路是，以空间换时间，将所有的term拷贝到内存中，在内存中更新数据；然后再一起插入到数据库中；
*/
bool CPostgre::insertTerm(string &ipc3,map<string,int>::iterator map_begin,map<string,int>::iterator map_end,PGconn * conn)
{
	PGresult *res = NULL;//存放数据库操作（如查询，插入，更新等）后反馈结果信息
	map<string, pair<int, int>> termFreqDocs;//存储数据库中词项、词频和包含该词项的文档数目	
	
	ofstream log1;
	log1.open("F:\\VS8_Projects\\NLPIR\\sql.txt",ios::app);
	
	upper2Lower(ipc3);//将专利数据中的IPC分类号转换为小写字符
	string sql_test = "select term,tf,docs from " + ipc3 ;
	res = PQexec(conn,sql_test.c_str());	
	if( PQresultStatus( res ) != PGRES_TUPLES_OK)
	{
		PQclear(res);			
		return false;
	}
	int termNum = PQntuples(res);
	for(int i=0; i < termNum; i++)
	{
		string resTerm = PQgetvalue(res,i,0);
		UTF8ToGBK(resTerm);
		termFreqDocs.insert( make_pair(resTerm,make_pair( atoi( PQgetvalue(res,i,1)), atoi(PQgetvalue(res,i,2)) ) ) );		
	}
	
	while(map_begin != map_end)
	{		
		termFreqDocs[map_begin->first].first += map_begin->second;//map_begin->second;//如果数据库中存在该此项，则词频进行累加，否则词频采用map_begin->first初始化
		termFreqDocs[map_begin->first].second +=  1;//如果数据库中存在该此项，则包含该次的文档数目docs进行累加，否则docs采用map_begin->second初始化
		map_begin++;
	}
	
	map<string, pair<int, int>>::iterator it_begin = termFreqDocs.begin();
	map<string, pair<int, int>>::iterator it_end = termFreqDocs.end();

	//sql_test = "delete from " + ipc3 ;//删除该表对应的所有数据
	//res = PQexec(conn,sql_test.c_str());	
	//if( PQresultStatus( res ) != PGRES_COMMAND_OK)
	//{
	//	PQclear(res);			
	//	return false;
	//}	
	
	while(it_begin != it_end)
	{	
		clock_t start,end;
		start = clock();

		stringstream tf,docs;		
		tf<<it_begin->second.first;//将词项整型转换为字符串，方便生成sql语句
		docs<<it_begin->second.second;//将文档频数转换为字符串		
		
		string term = it_begin->first;
		GBKToUTF8(term);//将gbk转为utf8
		sql_test = "insert into " + ipc3 + "(term,tf,docs,attr) values( '" + term + "'," + tf.str() + ","+docs.str()+",0);";
		log1<<sql_test<<"\n";
		//res = PQexec(conn,sql_test.c_str());

		//if( PQresultStatus( res ) != PGRES_COMMAND_OK)
		//{
		//	//string t = PQerrorMessage(conn);
		//	//UTF8ToGBK(t);		
		//	
		//	PQclear(res);			
		//	return false;
		//}
		
		it_begin++;
	}
	
	log1.close();
	return true;		
}

/*
	功能	：	将大写字符转小写字符，仅限于A-Z

	参数	：	（1）string &str	# 待转换的引用字符串,同时在源字符串中进行修改
				
	返回值	：	void

	修改记录：	（1）2013.4.11 吴军 第一次创建
*/
void CPostgre::upper2Lower(string &str)
{	
	char cTemp;
	for(int i = 0; i < str.length(); i++)
	{
		cTemp = str.at(i);
		if(cTemp >= 'A' && cTemp <= 'Z')
		{
			str.at(i) = str.at(i) + 32;
		}
	}
}


/*
	功能	：	对分词后的字符串向量进行去重，并统计每一个词项在一个文档中出现的频率

	参数	：	（1）vector<string>::iterator begin	# 分词后的字符串向量组起始迭代器
				（2）vector<string>::iterator end	# 分词后的字符串向量组末端的下一个迭代器
				（3）map<string,int> &TermFreMap	#对vs字符串向量组进行去重和词频统计后的结果的map末端下一个迭代器，

	返回值	：	void

	修改记录：	（1）2013.4.11 吴军 创建
				（2）2012.4.15 吴军 第一次修改：将存储分词和词频的结果存储结构由vector<pair<string,int>> 转换为map<string,int>
*/
void CPostgre::getTermFre(vector<string>::iterator begin, vector<string>::iterator end, map<string,int> &TermFreMap)
{
	
	while(begin != end)
	{
		++TermFreMap[*begin++];//利用map的下标查询特点，如果下标对应的值不存在，则插入一个新的元素，并且置值为默认初始化值，比如int型为0		
	}
	
}

/*
	功能	：	判断某分词词性是否满足停用词词性

	参数	：	（1）vector<string> &stopWordFlag	#停用词词性集合
				（2）string &object		#待检查的目标字符串

	返回值	：	bool	#false  如果该分词词性不在停用词词性集合中
						#true	该分词词性正好属于停用词词性集合中一个

	修改记录：	（1）2013.4.11 吴军 第一次创建
*/
bool CPostgre::weedStopWord(vector<string> &stopWordFlag,string &object )
{

	int size = stopWordFlag.size();
	for(int i = 0; i < size; i++)
	{
		if( object.find(stopWordFlag.at(i).c_str(),0) != object.npos )
		{
			return true;
		}

	}


	return false;
}

/*
	功能	：	生成停用词词性集合

	参数	：	（1）vector<string> &stopWordFlag	#停用词词性集合
				（2）string &object		#待检查的目标字符串

	返回值	：	bool	#false  如果该分词词性不在停用词词性集合中
						#true	该分词词性正好属于停用词词性集合中一个

	修改记录：	（1）2013.4.11 吴军 第一次创建
*/

void CPostgre::generateStopWordFlags( vector<string> &stopWordFlags)
{
	//词性
	stopWordFlags.push_back("/e");//叹词
	stopWordFlags.push_back("/c");//连词
	stopWordFlags.push_back("/d");//副词
	stopWordFlags.push_back("/f");//方位词
	stopWordFlags.push_back("/o");//拟声词
	stopWordFlags.push_back("/p");//介词
	stopWordFlags.push_back("/q");//量词
	stopWordFlags.push_back("/r");//代词
	stopWordFlags.push_back("/u");//助词
	stopWordFlags.push_back("/w");//标点符号语气词
	stopWordFlags.push_back("/y");//语气词
	stopWordFlags.push_back("/vshi");// 是
	stopWordFlags.push_back("/vyou");// 有
	stopWordFlags.push_back("/vf");// 方位动词
	stopWordFlags.push_back("/m");// 方位动词
	stopWordFlags.push_back("/x");// 未知数、符号

	//词项
	stopWordFlags.push_back("发明");// 发明
	stopWordFlags.push_back("本");// 本
	
	
}
