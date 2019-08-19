/*************************************************************************
	> File Name:		5.cpp
	> File Category: Course Design
	> Author: 		lttzz
	> Mail: 			3344517687@qq.com 
 ************************************************************************/

#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

//判断是否段首(以开头两个空格作为是否段首的标准),是则改为大写
int main(int argc, char* argv[])
{
	//cout << argv[1] << " " << argv[2] << endl;
	ifstream ifs;
	ofstream ofs;

	ifs.open(argv[1]);
	ofs.open(argv[2]);

	if (!ifs || !ofs)
	{
		cerr << "Fail to open file." << endl;
	}

	char tmp[1005];
	while (ifs.getline(tmp, 1000, '\n'))
	{
		if (tmp[0] == ' ' && tmp[1] == ' ')		//行首
		{
			if (tmp[2] >= 'a' && tmp[2] <= 'z')
			{
				tmp[2] -= 32;
			}
			if (tmp[2] == '\'' || tmp[2] == '\"')		//对话开头引号过滤
			{
				if (tmp[3] >= 'a' && tmp[3] <= 'z')
				{
					tmp[3] -= 32;
				}
			}
		}
		ofs << tmp << endl;
		memset(tmp, 0, sizeof(int));
	}

	ifs.close();
	ofs.close();

	return 0;
}
