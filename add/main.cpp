#include "Disassembly.h"
#include <algorithm>
#include <windows.h>
struct Data {
	string information;
	int times;
	bool operator<(const Data&rhs)const {
		return times > rhs.times;
	}
};
vector<Data> link;
map<string, int>ink;
void Add(string information, int times) {
	if (!ink[information]) {
		ink[information] = link.size();
		Data add;
		add.information = information;
		add.times = times;
		link.push_back(add);
	} else
		link[ink[information]].times += times;
}
void SearchFile(const char *pszDirectory, const bool &FAST, vector<string>&allPath) {
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
				SearchFile(pTempSrc, FAST, allPath);   // 如果是目录则继续递归
			else {
				char drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
				_splitpath(pTempSrc, drive, dir, fname, ext);
				// 如果是文件并且后缀为.exe则输出具体路径
				if (FAST) {
					if (strcmp(ext, ".exe") == 0 ||
					    strcmp(ext, ".dll") == 0 ||
					    strcmp(ext, ".bin") == 0) {
						allPath.push_back(pTempSrc);/*printf("%s \n", pTempSrc);*/
					}
				} else {
					allPath.push_back(pTempSrc);
				}
			}
		} while (::FindNextFile(hFile, &FileData));
	}
	FindClose(hFile);
	delete[]pTempSrc;
	delete[]pszFileName;
}
int main() {
	cout << "ABS>>";
	while (1) {
		string instruction;
		getline(cin, instruction);
		if (instruction == "save") {
			ofstream write("record.txt", ios::trunc);
			for (int i = 0, lenth = link.size(); i < lenth; i++) {
				write << link[i].information << endl;
				write << link[i].times << endl;
			}
			cout << "completed!\nABS>>";
		} else if (instruction == "read") {
			cout << "Path>>";
			getline(cin, instruction);
			ifstream read(instruction);
			Data newData;
			while (getline(read, newData.information)) {
				read >> newData.times;
				Add(newData.information, newData.times);
			}
			cout << "completed!\nABS>>";
		} else if (instruction == "add") {
			map<string, bool>local;
			local.clear();
			cout << "Path>>";
			getline(cin, instruction);
			vector<string> dis = Disassembly(instruction);
			for (int i = 0, lenth = dis.size(); i < lenth; i++)
				if (!local[dis[i]])
					Add(dis[i], 1), local[dis[i]] = 1;
			cout << "completed!\nABS>>";
		} else if (instruction == "exit") {
			return 0;
		} else if (instruction == "show") {
			for (int i = 0, lenth = link.size(); i < lenth; i++) {
				cout << link[i].information << endl;
				cout << link[i].times << endl;
			}
			cout << "\nABS>>";
		} else if (instruction == "sort") {
			sort(link.begin(), link.end());
			ink.clear();
			for (int i = 0, lenth = link.size(); i < lenth; i++)
				ink[link[i].information] = i;
			cout << "completed!\nABS>>";
		} else if (instruction == "addP") {
			vector<string>paths;
			cout << "Path>>";
			getline(cin, instruction);
			SearchFile(instruction.c_str(),1,paths);
			for (int i = 0, length = paths.size(); i < length; i++) {
				map<string, bool>local;
				local.clear();
				vector<string> dis = Disassembly(paths[i]);
				for (int i = 0, lenth = dis.size(); i < lenth; i++)
					if (!local[dis[i]])
						Add(dis[i], 1), local[dis[i]] = 1;
			}
			cout << "completed!\nABS>>";
		} else if(instruction=="addF"){
			vector<string>paths;
			cout << "Path>>";
			getline(cin, instruction);
			SearchFile(instruction.c_str(),0,paths);
			for (int i = 0, length = paths.size(); i < length; i++) {
				map<string, bool>local;
				local.clear();
				vector<string> dis = Disassembly(paths[i]);
				for (int i = 0, lenth = dis.size(); i < lenth; i++)
					if (!local[dis[i]])
						Add(dis[i], 1), local[dis[i]] = 1;
			}
			cout << "completed!\nABS>>";
		}else{
			cout << "meaning less instruction\nABS>>";
		}
	}
	return 0;
}
