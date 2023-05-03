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
		if (Name.size() > 1) {
			ofstream write("VNC.josn", ios::trunc);
			write << filePath << endl;
			write << Name;
			string FileName;
			FileName.clear();
			for (int j = 0, lenth_filePath = filePath.size(); j < lenth_filePath; j++)
				if (filePath[j] == '\\' || filePath[j] == '/')
					FileName.clear();
				else
					FileName += filePath[j];
			KillProcess(FileName.c_str());
			system("start ABSVirusWarning.exe");
			return 1;
		}
	}
	return 0;
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
