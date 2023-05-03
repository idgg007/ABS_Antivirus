#include"Disassembly.h"
#include<windows.h>
#include<TlHelp32.h>
#include<set>
class ABSVirus {
	private:
		vector<set<string>> features;
	public:
		string virusName;
		string JudgePrograme(vector<string> fileDisassembly);		//判断是否为病毒
		void AddFeature(set<string> feature);					//添加新特征
};
vector<ABSVirus> Viruses;
/******************************
 *  @brief     结束进程
 *  @param     szImageName:进程名
 *  @note      头文件： #include <Windows.h>
 *  @Sample usage:	KillProcess(进程名);
 * @author     xbebhxx3
 * @version    2.0
 * @date       2022/3/15
# Copyright (c) 2022-2077 xbebhxx3
******************************/
void KillProcess(const char* szImageName) {
	PROCESSENTRY32 pe = {sizeof(PROCESSENTRY32) }; //获得进程列表
	HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//拍摄快照
	BOOL bRet = Process32First(hProcess, &pe); //检索快照中第一个进程信息
	while (bRet) { //判断不是最后一个进程，历遍所有
		if (lstrcmp(szImageName, pe.szExeFile) == 0) { //判断是不是要结束的进程
			TerminateProcess(OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe.th32ProcessID), 0); //打开进程并杀死
		}
		bRet = Process32Next(hProcess, &pe); //下一个进程
	}
	return;
}
void LoadVirusFeatures() {
	int totalVirus = 1;
	for (int i = 1;; i++) {
		bool inEnd = 0;
		char Path[20];
		ABSVirus ADD;
		for (int j = 1;; j++) {
			string newFeature;
			set<string> AllFeatures;
			sprintf(Path, "Vir\\%d\\%d.vir", i, j);
			ifstream read(Path);
			while (getline(read, newFeature))
				AllFeatures.insert(newFeature);
			if (!AllFeatures.size()) {					//判断是否已经遍历完
				if (j == 1)					//无新病毒
					inEnd = 1;
				break;
			}
			ADD.AddFeature(AllFeatures);
		}
		if (inEnd)
			break;
		Viruses.push_back(ADD);
	}
}
void LoadVirusName() {					//执行时LoadVirusName 必须放在LoadVirusFeatures 后
	ifstream read("Vir.ini");
	string Name;
	int i = 0;
	while (getline(read, Name)) {
		Viruses[i].virusName = Name;
		i++;
	}
}
bool ScanFile(string filePath) {
	vector<string> fileDisassembly = Disassembly(filePath);
	for (int i = 0, lenth = Viruses.size(); i < lenth; i++) {
		string Name = Viruses[i].JudgePrograme(fileDisassembly);
		if (Name.size() > 1){
			return 1;
		}
	}
	return 0;
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
/*ABSVirus函数编写*/
string ABSVirus::JudgePrograme(vector<string> fileDisassembly) {
	set<string>Allfeatures;
	for (int i = 0, lenth = fileDisassembly.size(); i < lenth; i++)
		Allfeatures.insert(fileDisassembly[i]);
	for (int i = 0, lenth = features.size(); i < lenth; i++) {
		bool danger = 1;
		for (const string&feature : features[i])
			if (!Allfeatures.count(feature)) {
				danger = 0;
				break;
			}
		if (danger) {
			return virusName;
		}
	}
	return "";
}
void ABSVirus::AddFeature(set<string> feature) {
	features.push_back(feature);
}
