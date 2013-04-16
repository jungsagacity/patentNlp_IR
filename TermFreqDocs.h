#include "stdafx.h"
#include "TermFreqDocs.h"

#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include<windows.h>
#include <ctime>

class TermFreqDocs
{
public:
	void TermFreqDocs::upper2Lower(string &str);
	void TermFreqDocs::getTermFre(vector<string>::iterator begin, vector<string>::iterator end, map<string,int> &TermFreMap);
}