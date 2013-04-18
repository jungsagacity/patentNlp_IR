#include "stdafx.h"
#include "TermFreqDocs.h"

/*
	����	��	��ȡ�ļ���ÿһ�д�ŵ�������term,tf,docs,attr�Կո��������ȡÿһ��֮��
				��ÿһ�е����ݽ��зֽ⣬��term,tf,docs��ŵ�map<string,pair<int,int>>���͵ı����У�
				�ñ�����Ϊһ��ȫ�ֱ��������β��д��ݽ�����

	����	��	��1��map<string,int>::iterator itBegin	# �ִʺ����ʹ�Ƶ���ϵĵ�һ��������
				��2��map<string,int>::iterator itEnd    # �ִʺ����ʹ�Ƶ���ϵ����һ��Ԫ�ص���һ��������
				
	����ֵ	��	void

	�޸ļ�¼��	��1��2013.4.18 ��� ��һ�δ���
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
	//2����ר�����ݽ��зִʺ�Ľ�����д���
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
	����	��	����д�ַ�תСд�ַ���������A-Z

	����	��	��1��string &str	# ��ת���������ַ���,ͬʱ��Դ�ַ����н����޸�
				
	����ֵ	��	void

	�޸ļ�¼��	��1��2013.4.16 ��� ��һ�δ���
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
	����	��	�Էִʺ���ַ�����������ȥ�أ���ͳ��ÿһ��������һ���ĵ��г��ֵ�Ƶ��

	����	��	��1��vector<string>::iterator begin	# �ִʺ���ַ�����������ʼ������
				��2��vector<string>::iterator end	# �ִʺ���ַ���������ĩ�˵���һ��������
				��3��map<string,int> &TermFreMap	#��vs�ַ������������ȥ�غʹ�Ƶͳ�ƺ�Ľ����mapĩ����һ����������

	����ֵ	��	void

	�޸ļ�¼��	��1��2013.4.11 ��� ����
				��2��2012.4.15 ��� ��һ���޸ģ����洢�ִʺʹ�Ƶ�Ľ���洢�ṹ��vector<pair<string,int>> ת��Ϊmap<string,int>
*/
void TermFreqDocs::getTermFre(vector<string>::iterator begin, vector<string>::iterator end, map<string,int> &TermFreMap)
{
	
	while(begin != end)
	{
		++TermFreMap[*begin++];//����map���±��ѯ�ص㣬����±��Ӧ��ֵ�����ڣ������һ���µ�Ԫ�أ�������ֵΪĬ�ϳ�ʼ��ֵ������int��Ϊ0		
	}
	
}

/*
	����	��	�ж�ĳ�ִʴ����Ƿ�����ͣ�ôʴ���

	����	��	��1��vector<string> &stopWordFlag	#ͣ�ôʴ��Լ���
				��2��string &object		#������Ŀ���ַ���

	����ֵ	��	bool	#false  ����÷ִʴ��Բ���ͣ�ôʴ��Լ�����
						#true	�÷ִʴ�����������ͣ�ôʴ��Լ�����һ��

	�޸ļ�¼��	��1��2013.4.11 ��� ��һ�δ���
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
	����	��	����ͣ�ôʴ��Լ���

	����	��	��1��vector<string> &stopWordFlag	#ͣ�ôʴ��Լ���
				��2��string &object		#������Ŀ���ַ���

	����ֵ	��	bool	#false  ����÷ִʴ��Բ���ͣ�ôʴ��Լ�����
						#true	�÷ִʴ�����������ͣ�ôʴ��Լ�����һ��

	�޸ļ�¼��	��1��2013.4.11 ��� ��һ�δ���
*/

void TermFreqDocs::generateStopWordFlags( vector<string> &stopWordFlags)
{
	//����
	stopWordFlags.push_back("/e");//̾��
	stopWordFlags.push_back("/c");//����
	stopWordFlags.push_back("/d");//����
	stopWordFlags.push_back("/f");//��λ��
	stopWordFlags.push_back("/o");//������
	stopWordFlags.push_back("/p");//���
	stopWordFlags.push_back("/q");//����
	stopWordFlags.push_back("/r");//����
	stopWordFlags.push_back("/u");//����
	stopWordFlags.push_back("/w");//������������
	stopWordFlags.push_back("/y");//������
	stopWordFlags.push_back("/vshi");// ��
	stopWordFlags.push_back("/vyou");// ��
	stopWordFlags.push_back("/vf");// ��λ����
	stopWordFlags.push_back("/m");// ��λ����
	stopWordFlags.push_back("/x");// δ֪��������
	stopWordFlags.push_back("/t");// δ֪��������
	stopWordFlags.push_back("/v");// δ֪��������
	stopWordFlags.push_back("/a");// δ֪��������

	//����
	stopWordFlags.push_back("����");// ����
	stopWordFlags.push_back("��");// ��
	
	
}



/*
	����	��	����һ������ʽ���ַ������в�֣������ַ���������,����ȥ����ͣ�ô�

	����	��	��1��string &ictclasResult # �п�Ժ�ִʽ��
				��2��const string &pattern #���ƥ����ʽ
				��3��vector<string> &result #��Ų�ֺ�Ľ��
				
	����ֵ	��	void

	�޸ļ�¼��	��1��2013.4.11 ��� ��һ�δ���
*/
void TermFreqDocs::split(string &ictclasResult,string splitChar,vector<string> &result,vector<string> &stopWordFlags)
{
	int pos = 0;//ƥ���ַ�������ʼλ��	
	int size = ictclasResult.size();	
	string term;

	for(int i=0; i<size; i++)//ѭ��ƥ��
	{				
		pos = ictclasResult.find(splitChar,i);//�ҵ��ո��ַ����ڵ�λ��

		if( pos != ictclasResult.npos )//�ַ����д��ڿո�
		{	
			if( pos != i)//�ų������ո�����
			{
				term = ictclasResult.substr(i,pos-i);
				if(!weedStopWord(stopWordFlags,term))
				{
					int pos1 = term.find('/',0);
					if( (pos1 != 2))
					{
						//term = term.substr(0,pos1);	
						result.push_back(term);//���ַ���ѹ��������
					}							
				}			
			}
			

		}else//�ַ�����û�пո�
		{
			term = ictclasResult.substr(i,size-i);//��ʣ����ַ���ȫ����Ϊһ��term��ŵ�������
			result.push_back(term);//���ַ���ѹ��������
			break;//�ӵ�ǰλ�ÿ�ʼ���ң�֪���ַ����Ľ�������û��ƥ�䣬�����ѭ��ƥ��
		}		
		i = pos;//���¶�λ���ҵ�λ�ã����¸����ҵ�λ��Ϊ���οո��λ�ã�����i++���㣬ָ�򱾴οո����һ��λ��
	}

	
}

/*
	����	��	��UTF8������ַ���ת��ΪGBK���롣��Ҫע������ݿ��ж�ȡ�������ַ�����gbk����

	����	��	��1��string &strUtf8 # ��ת�����ַ�����ͬʱת����Ľ��������ַ�����		
				
	����ֵ	��	

	�޸ļ�¼��	��1��2013.4.11 ��� ��һ�δ���
*/
void TermFreqDocs::UTF8ToGBK(string &strUtf8)
{
	int len=MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, NULL,0);
    wchar_t * wszGBK = new wchar_t[len];
    memset(wszGBK,0,len);
	MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, wszGBK, len); //ת��Ϊunicode

    len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
    char *szGBK=new char[len + 1];//�����ַ��������������һ��Ϊ\0��������Ҫ������һ���ֽڵĿռ�
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte (CP_ACP, 0, wszGBK, -1, szGBK, len, NULL,NULL);//ת��ΪGBK

    strUtf8 = szGBK;
    delete[] szGBK;
    delete[] wszGBK;
}



/*
	����	��	��GBK������ַ���ת��ΪUTF8���롣��Ҫע������ݿ��ж�ȡ�������ַ�����gbk����

	����	��	��1��string &strGBK # ����ת���Ľ��			
				
	����ֵ	��	void

	�޸ļ�¼��	��1��2013.4.11 ��� ��һ�δ���
*/
void TermFreqDocs::GBKToUTF8(string &strGBK)  
{  
    string strOutUTF8 = "";  
    WCHAR * str1;  
    int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);  
    str1 = new WCHAR[n];  
    MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);  
    n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);  //ת��Ϊunicode
    char * str2 = new char[n];  
    WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);  //ת��ΪUTF8
    strGBK = str2;  

    //delete[]str1;      
    //delete[]str2;  
    
   
} 

