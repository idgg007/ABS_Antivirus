#include <iostream>
#include <map>
#include <windows.h>
#include "VirusJudge.h"
using namespace std;
int length, tot;
string instruction;
vector<queue<char>>feaures;
vector<queue<char>>newF;
map<queue<char>, int>mapping;
map<queue<char>, int>now;
void AddFeaures(const string&path) {
	tot++;
	fstream read(path.c_str(), ios::in | ios::binary);
	queue<char>newFeaures;
	char in;
	while (!newFeaures.empty()) {
		newFeaures.pop();
	}
	newF.clear();
	now.clear();
	while (read.read(&in, sizeof(in))) {
		newFeaures.push(in);
		if (newFeaures.size() == length) {
			if ((mapping[newFeaures] && now[newFeaures] == 0) || tot == 1) {
				newF.push_back(newFeaures);
				now[newFeaures] = 1;
				while (!newFeaures.empty()) {
					newFeaures.pop();
				}
				newFeaures.push(1);
			}
			newFeaures.pop();
		}
	}
	feaures = newF;
	mapping = now;
}
void SearchFile(const char *pszDirectory, const bool &FAST) {
	// 搜索指定类型文件
	char *pszFileName = NULL;
	char *pTempSrc = NULL;
	WIN32_FIND_DATA FileData = { 0 };

	// 申请动态内存
	pszFileName = new char[2048];
	pTempSrc = new char[2048];

	// 构造搜索文件类型字符串 *.* 表示搜索所有文件类型
	wsprintf(pszFileName, "%s\\*.*", pszDirectory);

	HANDLE hFile = ::FindFirstFile(pszFileName, &FileData);
	if (INVALID_HANDLE_VALUE != hFile) {
		do {
			// 过滤掉当前目录"." 和上一层目录".."
			if ('.' == FileData.cFileName[0])
				continue;

			// 拼接文件路径
			wsprintf(pTempSrc, "%s\\%s", pszDirectory, FileData.cFileName);
			// 判断是否是目录还是文件
			if (FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				SearchFile(pTempSrc, FAST);   // 如果是目录则继续递归
			else {
				char drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
				_splitpath(pTempSrc, drive, dir, fname, ext);
				// 如果是文件并且后缀为.exe则输出具体路径
				if (FAST) {
					if (strcmp(ext, ".exe") == 0) {
						AddFeaures(pTempSrc);/*printf("%s \n", pTempSrc);*/
					}
				} else {
					AddFeaures(pTempSrc);
				}
			}
		} while (::FindNextFile(hFile, &FileData));
	}
	FindClose(hFile);
	delete[]pTempSrc;
	delete[]pszFileName;
}
void Print(const int&i) {
	fstream write("ans.vir", ios::out | ios::app | ios::binary);
	queue<char>de = feaures[i];
	while (!de.empty()) {
		char out = de.front();
		write.write(&out, sizeof(out));
		de.pop();
	}
}
int main() {
	while (1) {
		cout << ">>";
		getline(cin, instruction);
		if (instruction == "add full") {
			cout << "Path>>";
			getline(cin, instruction);
			SearchFile(instruction.c_str(), 0);
		} else if (instruction == "add exe") {
			cout << "Path>>";
			getline(cin, instruction);
			SearchFile(instruction.c_str(), 1);
		} else if (instruction == "add file") {
			cout << "Path>>";
			getline(cin, instruction);
			AddFeaures(instruction);
		} else if (instruction == "save") {
			for (int i = 0, se = feaures.size(); i < se; i++) {
				Print(i);
			}
		} else if (instruction == "Load") {
			cout << "none\n";
		} else if (instruction == "setSize") {
			cout << ">>";
			cin >> length;
			cout << length;
			getchar();
		} else {
			cout << "notFound\n";
		}
	}
}
