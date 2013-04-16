


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

	//词项
	stopWordFlags.push_back("发明");// 发明
	stopWordFlags.push_back("本");// 本
	
	
}