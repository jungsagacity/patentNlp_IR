#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <windows.h>
#include <ctime>
#include <string>
#include <map>
using namespace std;


//�ṹ���Ŵ���Ĵ�Ƶ�������ô�����ĵ��飬�ô�������
struct freDocAttr
{
	int freq;
	int docs;
	int attr;
};


class TermFreqDocs
{
public:

	TermFreqDocs()
	{
		generateStopWordFlags(stopWord);
	}

	void TermFreqDocs::upper2Lower(string &str);
	void TermFreqDocs::UTF8ToGBK(string &strUtf8);
	void TermFreqDocs::GBKToUTF8(string &strGBK);
	void TermFreqDocs::getTermFreDocs(map<string,int>::iterator itBegin, map<string,int>::iterator itEnd, map<string,struct freDocAttr> &resMap);	
	bool TermFreqDocs::weedStopWord(vector<string> &stopWordFlag,string &object );
	void TermFreqDocs::getTermFre(vector<string>::iterator begin, vector<string>::iterator end, map<string,int> &TermFreMap);
	void TermFreqDocs::generateStopWordFlags( vector<string> &stopWordFlags);
	void TermFreqDocs::split(string &ictclasResult,string splitChar,vector<string> &result,vector<string> &stopWordFlags);
	
private:
	vector<string> stopWord;

};