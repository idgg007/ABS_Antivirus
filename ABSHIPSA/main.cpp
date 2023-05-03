#include "ScanFile.h"
#include "VirusJudge.h"
#include <cstring>
using namespace std;
map<string, bool> trustProgramsPath;
vector<Virus>virRecord;
void LoadTrust() {
	ifstream read("Trust.ini");
	string add;
	while (getline(read, add))
		trustProgramsPath[add] = 1;
}
string ScanFile_Simple(const string&fileName) {
	fstream read(fileName.c_str(), ios::in | ios::binary);
	char in;
	string name = "";
	name.clear();
	for (int i = 0, length_V = virRecord.size(); i < length_V; i++) {
		virRecord[i].ClearMaping();
	}
	while (read.read(&in, sizeof(in))) {
		for (int i = 0, length_V = virRecord.size(); i < length_V; i++) {
			name = virRecord[i].AddNew(in);
			if (name != "") {
				return name;
			}
		}
	}
	read.close();
	return "";
}
void LoadBitFeaures() {
	ifstream read("BitVir.ini");
	vector<string>allName;
	allName.clear();
	string in;
	while (getline(read, in)) {
		allName.push_back(in);
	}
	read.close();
	for (int i = 0, length_A = allName.size(); i < length_A; i++) {
		char firstPath[10];
		sprintf(firstPath, "BitVir\\%d", i + 1);
		for (int j = 1;; j++) {
			char secondPath[10];
			sprintf(secondPath, "\\%d.vir", j);
			string fullPath = firstPath;
			fullPath += secondPath;
			ifstream appear(fullPath);
			if (appear.fail()) {
				break;
			}
			appear.close();
			Virus newF;
			newF.SetName(allName[i]);
			newF.SetSize(64);
			newF.LoadFeatures(fullPath);
			virRecord.push_back(newF);
		}
	}
}
DWORD WINAPI MonitorFileThreadProc(LPVOID lParam) {
	char *pszDirectory = (char *)lParam;
	BOOL bRet = FALSE;
	BYTE Buffer[1024] = { 0 };

	FILE_NOTIFY_INFORMATION *pBuffer = (FILE_NOTIFY_INFORMATION *)Buffer;
	DWORD dwByteReturn = 0;
	HANDLE hFile = CreateFile(pszDirectory, FILE_LIST_DIRECTORY, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
	                          NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
		return 1;

	while (TRUE) {
		ZeroMemory(Buffer, sizeof(Buffer));
		// 设置监控目录回调函数
		bRet = ReadDirectoryChangesW(hFile, &Buffer, sizeof(Buffer), TRUE,
		                             FILE_NOTIFY_CHANGE_FILE_NAME |          // 修改文件名
		                             FILE_NOTIFY_CHANGE_ATTRIBUTES |         // 修改文件属性
		                             FILE_NOTIFY_CHANGE_LAST_WRITE,          // 最后一次写入
		                             &dwByteReturn, NULL, NULL);
		if (TRUE == bRet) {
			char szFileName[MAX_PATH] = { 0 };

			// 将宽字符转换成窄字符,宽字节字符串转多字节字符串
			WideCharToMultiByte(CP_ACP, 0, pBuffer->FileName, (pBuffer->FileNameLength / 2),
			                    szFileName, MAX_PATH, NULL, NULL);

			// 将路径与文件连接成完整文件路径
			char FullFilePath[1024] = { 0 };
			strncpy(FullFilePath, pszDirectory, strlen(pszDirectory));
			strcat(FullFilePath, szFileName);

			switch (pBuffer->Action) {
				case FILE_ACTION_ADDED: {
					/*bool against=0;
					for(const string&rule:protectFolder)
						if(strstr(FullFilePath,rule.c_str())){
							against=1;
							//删除文件
							break;
						}
					if(against)
						break;*/
					string suffix;
					for (int i = 0, lenth_Path = strlen(FullFilePath); i < lenth_Path; i++)
						if (FullFilePath[i] == '.')
							suffix.clear();
						else
							suffix += FullFilePath[i] <= 'Z' && FullFilePath[i] >= 'A' ? FullFilePath[i] + ' ' : FullFilePath[i];
					if ((suffix == "exe" || suffix == "txt") && trustProgramsPath[FullFilePath] == 0) {
						if (!ScanFile(FullFilePath)) {
							string name = ScanFile_Simple(FullFilePath);
							if (name != "") {
								ofstream write("VNC.josn", ios::trunc);
								write << FullFilePath << endl;
								write << name;
								string FileName;
								FileName.clear();
								for (int j = 0, lenth_filePath = strlen(FullFilePath); j < lenth_filePath; j++)
									if (FullFilePath[j] == '\\' || FullFilePath[j] == '/')
										FileName.clear();
									else
										FileName += FullFilePath[j];
								KillProcess(FileName.c_str());
								system("start ABSVirusWarning.exe");
							}
						}
					}
					//printf("添加: %s \n", FullFilePath);
					break;
				}
				case FILE_ACTION_REMOVED: {//只会触犯文件规则
					/*int against=0;
					for(const string&rule:protectFolder)
						if(strstr(FullFilePath,rule.c_str())){
							against=1;
							//还原文件
							break;
						}
					if(against)
						break;
					for(const string &rule:protectFile){
						if(rule==FullFilePath){
							//还原文件
							break;
						}
					}*/
					//printf("删除: %s \n", FullFilePath);
					break;
				}
				case FILE_ACTION_MODIFIED: {//删除文件夹归于修改
					string suffix;
					for (int i = 0, lenth_Path = strlen(FullFilePath); i < lenth_Path; i++)
						if (FullFilePath[i] == '.')
							suffix.clear();
						else
							suffix += FullFilePath[i] <= 'Z' && FullFilePath[i] >= 'A' ? FullFilePath[i] + ' ' : FullFilePath[i];
					if ((suffix == "exe" || suffix == "txt") && trustProgramsPath[FullFilePath] == 0) {
						if (!ScanFile(FullFilePath)) {
							string name = ScanFile_Simple(FullFilePath);
							if (name != "") {
								ofstream write("VNC.josn", ios::trunc);
								write << FullFilePath << endl;
								write << name;
								string FileName;
								FileName.clear();
								for (int j = 0, lenth_filePath = strlen(FullFilePath); j < lenth_filePath; j++)
									if (FullFilePath[j] == '\\' || FullFilePath[j] == '/')
										FileName.clear();
								else
									FileName += FullFilePath[j];
								KillProcess(FileName.c_str());
								system("start ABSVirusWarning.exe");
							}
						}
					}
					//printf("修改: %s \n", FullFilePath);
					break;
				}
				case FILE_ACTION_RENAMED_OLD_NAME: {
					/*printf("重命名: %s", szFileName);*/
					if (0 != pBuffer->NextEntryOffset) {
						FILE_NOTIFY_INFORMATION *tmpBuffer = (FILE_NOTIFY_INFORMATION *)
						                                     (/*(DWORD)*/pBuffer + pBuffer->NextEntryOffset);
						switch (tmpBuffer->Action) {
							case FILE_ACTION_RENAMED_NEW_NAME: {
								ZeroMemory(szFileName, MAX_PATH);
								WideCharToMultiByte(CP_ACP, 0, tmpBuffer->FileName,
								                    (tmpBuffer->FileNameLength / 2),
								                    szFileName, MAX_PATH, NULL, NULL);
								//printf(" -> %s \n", szFileName);
								break;
							}
						}
					}
					break;
				}
				case FILE_ACTION_RENAMED_NEW_NAME: {
					//printf("重命名(new): %s \n", FullFilePath);
					break;
				}
			}
		}
	}
	CloseHandle(hFile);
	return 0;
}
int main() {
	//开机自动隐藏窗口
	HWND hwnd;
	hwnd = FindWindow("ConsoleWindowClass", NULL); //找到当前窗口句柄
	if (hwnd) {
		ShowOwnedPopups(hwnd, SW_HIDE); //显示或隐藏由指定窗口所有的全部弹出式窗口
		ShowWindow(hwnd, SW_HIDE); //控制窗口的可见性
		//WinExec 函数: 控制窗口的显示形式
		//假如开机自动运行: C:\\WINDOWS\\SYSTEM32\\KeyboardRec.exe
		WinExec("C:\\WINDOWS\\SYSTEM32\\KeyboardRec.exe", SW_HIDE);
	}//*/
	LoadTrust();
	LoadBitFeaures();
	LoadVirusFeatures();
	LoadVirusName();
	char *pszDirectory = "C:\\";
	HANDLE hThread = CreateThread(NULL, 0, MonitorFileThreadProc, pszDirectory, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	return 0;
}
