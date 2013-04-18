#include "stdafx.h"
#include "TermFreqDocs.h"

/*
	功能	：	读取文件，每一行存放的内容是term,tf,docs,attr以空格隔开，读取每一行之后，
				对每一行的内容进行分解，将term,tf,docs存放到map<string,pair<int,int>>类型的变量中；
				该变量作为一个全局变量，有形参中传递进来；

	参数	：	（1）map<string,int>::iterator itBegin	# 分词后词项和词频集合的第一个迭代器
				（2）map<string,int>::iterator itEnd    # 分词后词项和词频集合的最后一个元素的下一个迭代器
				
	返回值	：	void

	修改记录：	（1）2013.4.18 吴军 第一次创建
*/

void TermFreqDocs::getTermFreDocs(map<string,int>::iterator itBegin, map<string,int>::iterator itEnd, map<string,struct freDocAttr> &resMap)
{
	struct freDocAttr fda;	
	map<string,struct freDocAttr>::iterator it = resMap.begin();
	//while( it!=resMap.end() )
	//{
	//	cout<<it->first<<"\t";
	//	cout<<it->second.first<<"\t";
	//	cout<<it->second.second<<"\t"<<endl;

	//	it++;
	//}
	//2、对专利数据进行分词后的结果进行处理，
	while(itBegin != itEnd)
	{
		it = resMap.find(itBegin->first); 
		if( it != resMap.end())
		{	
			it->second.freq = it->second.freq + itBegin->second;
			it->second.docs = it->second.docs + 1;
			it->second.attr = it->second.attr;

		}else
		{
			fda.freq = itBegin->second;
			fda.docs = 1;
			fda.attr = 0;	
			resMap.insert( make_pair( itBegin->first, fda) );
		}
		
		
		itBegin++;
	}
	


}


/*
	功能	：	将大写字符转小写字符，仅限于A-Z

	参数	：	（1）string &str	# 待转换的引用字符串,同时在源字符串中进行修改
				
	返回值	：	void

	修改记录：	（1）2013.4.16 吴军 第一次创建
*/
void TermFreqDocs::upper2Lower(string &str)
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
void TermFreqDocs::getTermFre(vector<string>::iterator begin, vector<string>::iterator end, map<string,int> &TermFreMap)
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
bool TermFreqDocs::weedStopWord(vector<string> &stopWordFlag,string &object )
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

void TermFreqDocs::generateStopWordFlags( vector<string> &stopWordFlags)
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
	stopWordFlags.push_back("/t");// 未知数、符号
	stopWordFlags.push_back("/v");// 未知数、符号
	stopWordFlags.push_back("/a");// 未知数、符号

	//词项
	stopWordFlags.push_back("发明");// 发明
	stopWordFlags.push_back("本");// 本
	
	
}



/*
	功能	：	按照一定的形式对字符串进行查分，生成字符串向量组,并且去掉了停用词

	参数	：	（1）string &ictclasResult # 中科院分词结果
				（2）const string &pattern #拆分匹配表达式
				（3）vector<string> &result #存放拆分后的结果
				
	返回值	：	void

	修改记录：	（1）2013.4.11 吴军 第一次创建
*/
void TermFreqDocs::split(string &ictclasResult,string splitChar,vector<string> &result,vector<string> &stopWordFlags)
{
	int pos = 0;//匹配字符串的起始位置	
	int size = ictclasResult.size();	
	string term;

	for(int i=0; i<size; i++)//循环匹配
	{				
		pos = ictclasResult.find(splitChar,i);//找到空格字符所在的位置

		if( pos != ictclasResult.npos )//字符串中存在空格
		{	
			if( pos != i)//排除连续空格的情况
			{
				term = ictclasResult.substr(i,pos-i);
				if(!weedStopWord(stopWordFlags,term))
				{
					int pos1 = term.find('/',0);
					if( (pos1 != 2))
					{
						//term = term.substr(0,pos1);	
						result.push_back(term);//将字符串压入向量组
					}							
				}			
			}
			

		}else//字符串中没有空格
		{
			term = ictclasResult.substr(i,size-i);//将剩余的字符串全部作为一个term存放到向量中
			result.push_back(term);//将字符串压入向量组
			break;//从当前位置开始查找，知道字符串的结束，都没有匹配，则结束循环匹配
		}		
		i = pos;//重新定位查找的位置，即下个查找的位置为本次空格的位置，经过i++运算，指向本次空格的下一个位置
	}

	
}

/*
	功能	：	将UTF8编码的字符串转换为GBK编码。需要注意从数据库中读取出来的字符集是gbk编码

	参数	：	（1）string &strUtf8 # 带转换的字符串，同时转换后的结果存放在字符串中		
				
	返回值	：	

	修改记录：	（1）2013.4.11 吴军 第一次创建
*/
void TermFreqDocs::UTF8ToGBK(string &strUtf8)
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
    delete[] szGBK;
    delete[] wszGBK;
}



/*
	功能	：	将GBK编码的字符串转换为UTF8编码。需要注意从数据库中读取出来的字符集是gbk编码

	参数	：	（1）string &strGBK # 返回转码后的结果			
				
	返回值	：	void

	修改记录：	（1）2013.4.11 吴军 第一次创建
*/
void TermFreqDocs::GBKToUTF8(string &strGBK)  
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

