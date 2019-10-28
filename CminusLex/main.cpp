#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

#include "globals.h"

ifstream source;

int main(int argc, char * argv[])
{
	/* ��������Ҫ������������<��������> <Դ���ļ���>*/
	if (argc != 2)
	{
		cerr << "usage: " << argv[0] << " <filename>" << endl;
		exit(1);
	}

	/* ���Ϻ�׺.cminus */
	string sourceFile = argv[1];
	if (sourceFile.find(".") == string::npos)
		sourceFile += ".cminus";

	/* ���Դ��ļ� */
	source.open(sourceFile);
	if (!source)
	{
		cerr << "File " << sourceFile << " not found" << endl;
		exit(2);
	}

	/* �ʷ����� */
	while (getToken() != ENDFILE);
	source.close();

	return 0;
}