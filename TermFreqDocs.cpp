


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

	//����
	stopWordFlags.push_back("����");// ����
	stopWordFlags.push_back("��");// ��
	
	
}