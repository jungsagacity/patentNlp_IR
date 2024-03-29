
#ifndef POSTGRE_H
#define POSTGRE_H

#include "libpq-fe.h"
#include <string>
#include <vector>
#include <map>
using namespace std;


class CPostgre
{
	vector<string> stopWordFlag;
public:
	bool CPostgre::getPatent(PGconn * conn,char * pg_sql,vector<pair<string,string>> &patent);
	bool CPostgre::getSubClass(vector<string> &subClass,PGconn * conn);
	bool CPostgre::createTable(vector<string> &subClass,PGconn * conn);
	bool CPostgre::deleteAllTableData(vector<string> &subClass,PGconn * conn);
	void CPostgre::UTF8ToGBK(string &strUtf8);
	void CPostgre::GBKToUTF8(string &strGBK);
	void CPostgre::split(string &ictclasResult,string splitChar,vector<string> &result,vector<string> &stopWordFlags);
	PGconn * CPostgre::connectDatabase(const char * host);
	bool CPostgre::insertTerm(string &ipc3,map<string,int>::iterator map_begin,map<string,int>::iterator map_end,PGconn * conn);
	void CPostgre::upper2Lower(string &str);
	void CPostgre::getTermFre(vector<string>::iterator begin, vector<string>::iterator end, map<string,int> &TermFreMap);
	bool CPostgre::weedStopWord(vector<string> &stopWordFlag,string &object );
	void CPostgre::generateStopWordFlags( vector<string> &stopWordFlags);
};

#endif