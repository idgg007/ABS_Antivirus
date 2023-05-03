#include <Windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <string>
using namespace std;
vector<string>ProtectFile;
vector<string>ProtectFolder;
void LoadProtectFolder(){
	ifstream read("ProtectF.ini");
	string rd;
	while(getline(read,rd)){
		ProtectFolder.push_back(rd);
	}
}
void LoadProtectFile(){
	ifstream read("Protect.ini");
	string rd;
	while(getline(read,rd)){
		cout<<rd<<"\n";
		ProtectFile.push_back(rd);
	}
}
bool JudgeFile(const string File1,const string File2){
	fstream readFile1(File1.c_str(),ios::binary);
	fstream readFile2(File2.c_str(),ios::binary);
	char f1,f2;
	while(readFile1.read(&f1,sizeof(f1))&&readFile2.read(&f2,sizeof(f2))){
		if(f1!=f2){
			return 0;
		}
	}
	return 1;
}
int copy_file(const char* SourceFile, const char* TargetFile)
{
	// 创建 std::fstream 流对象
	ifstream in;
	ofstream out;
	
	try {
		// 打开源文件
		in.open(SourceFile, std::ios::binary);
		// 打开源文件失败
		if (in.fail()) {
			cout << "Error 1: Fail to open the source file." << std::endl;
			// 关闭文件对象
			in.close();
			out.close();
			return 0;
		}
		out.open(TargetFile, std::ios::binary|std::ios::trunc);
		if (out.fail()) {
			cout << "Error 2: Fail to create the new file." << std::endl;
			in.close();
			out.close();
			return 0;
		} else {
			out << in.rdbuf();
			out.close();
			in.close();
			return 1;
		}
		
	}
	catch (std::exception& E){
		cout << E.what() << std::endl;
		return 1;
	}
}//*/
void GetAllBackups(){
	for(int i=0,length=ProtectFile.size();i<length;i++){
		char path[20];
		sprintf(path,"backups\\%d.bkp",i);
		copy_file(ProtectFile[i].c_str(),path);
	}
}
DWORD WINAPI MonitorFileThreadProc(LPVOID lParam)
{
	char *pszDirectory = (char *)lParam;
	BOOL bRet = FALSE;
	BYTE Buffer[1024] = { 0 };
	
	FILE_NOTIFY_INFORMATION *pBuffer = (FILE_NOTIFY_INFORMATION *)Buffer;
	DWORD dwByteReturn = 0;
	HANDLE hFile = CreateFile(pszDirectory, FILE_LIST_DIRECTORY, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
		return 1;
	
	while (TRUE)
	{
		ZeroMemory(Buffer, sizeof(Buffer));
		// 设置监控目录回调函数
		bRet = ReadDirectoryChangesW(hFile,&Buffer,sizeof(Buffer),TRUE,
			FILE_NOTIFY_CHANGE_FILE_NAME |          // 修改文件名
			FILE_NOTIFY_CHANGE_ATTRIBUTES |         // 修改文件属性
			FILE_NOTIFY_CHANGE_LAST_WRITE,          // 最后一次写入
			&dwByteReturn, NULL, NULL);
		if (TRUE == bRet)
		{
			char szFileName[MAX_PATH] = { 0 };
			
			// 将宽字符转换成窄字符,宽字节字符串转多字节字符串
			WideCharToMultiByte(CP_ACP,0,pBuffer->FileName,(pBuffer->FileNameLength / 2),
				szFileName,MAX_PATH,NULL,NULL);
			
			// 将路径与文件连接成完整文件路径
			char FullFilePath[1024] = { 0 };
			strncpy(FullFilePath, pszDirectory, strlen(pszDirectory));
			strcat(FullFilePath, szFileName);
			
			switch (pBuffer->Action)
			{
			case FILE_ACTION_ADDED:
				{
					for(int i=0,lengthFloder=ProtectFolder.size();i<lengthFloder;i++){
						if(ProtectFolder[i].size()<strlen(FullFilePath)){
							int flag=1;
							for(int j=0,lengthP=ProtectFolder[i].size();j<lengthP;j++){
								if(FullFilePath[j]!=ProtectFolder[i][j]){
									flag=0;
									break;
								}
							}
							if(flag){
								//报警
								ofstream write("FOC.josn",ios::trunc);
								write<<FullFilePath;
								write.close();
								system("start ABSFolderWarning.exe");
								break;
							}
						}
					}
					printf("添加: %s \n", FullFilePath);
					break;
				}
			case FILE_ACTION_REMOVED:
				{
					int file=-1;
					for(int i=0,lenth=ProtectFile.size();i<lenth;i++){
						int flag=1;
						for(int j=0,length=ProtectFile[i].size();j<length&&FullFilePath[j];j++){
							if(FullFilePath[j]!=ProtectFile[i][j]){
								flag=0;
								break;
							}
						}
						if(flag){
							file=i;
							break;
						}
					}
					if(file!=-1){
						//报警
						//char path[20];
						//sprintf(path,"backups\\%d.bkp",file);
						//copy_file(path,ProtectFile[file].c_str());
						ofstream write("FNC.josn",ios::trunc);
						write<<file;
						write.close();
						system("start ABSFileWarning.exe");
					}
					printf("删除: %s \n", FullFilePath);
					break;
				}
			case FILE_ACTION_MODIFIED:
				{
					int file=-1;
					for(int i=0,lenth=ProtectFile.size();i<lenth;i++){
						int flag=1;
						for(int j=0,length=ProtectFile[i].size();j<length&&FullFilePath[j];j++){
							if(FullFilePath[j]!=ProtectFile[i][j]){
								flag=0;
								break;
							}
						}
						if(flag){
							file=i;
							break;
						}
					}
					cout<<file<<"\n";
					if(file!=-1){
						if(strlen(FullFilePath)!=ProtectFile[file].size()){
							ifstream in;
							in.open(ProtectFile[file].c_str(),ios::binary);
							if(!in.fail()){
								break;
							}
						}
						//报警
						//char path[20];
						//sprintf(path,"backups\\%d.bkp",file);
						//copy_file(path,ProtectFile[file].c_str());
						if(JudgeFile(FullFilePath,ProtectFile[file])){
							break;
						}
						ofstream write("FNC.josn",ios::trunc);
						write<<file;
						write.close();
						system("start ABSFileWarning.exe");
					}
					printf("修改: %s \n", FullFilePath);
					break;
				}
			case FILE_ACTION_RENAMED_OLD_NAME:
				{
					string oldName=szFileName;
					printf("重命名: %s", szFileName);
					if (0 != pBuffer->NextEntryOffset)
					{
						FILE_NOTIFY_INFORMATION *tmpBuffer = (FILE_NOTIFY_INFORMATION *)
						(pBuffer + pBuffer->NextEntryOffset);
						switch (tmpBuffer->Action)
						{
						case FILE_ACTION_RENAMED_NEW_NAME:
							{
								ZeroMemory(szFileName, MAX_PATH);
								WideCharToMultiByte(CP_ACP,0,tmpBuffer->FileName,
									(tmpBuffer->FileNameLength / 2),
									szFileName,MAX_PATH,NULL,NULL);
								printf(" -> %s \n", szFileName);
								int file=-1;
								for(int i=0,lenth=ProtectFile.size();i<lenth;i++){
									int flag=1;
									for(int j=0,length=ProtectFile[i].size();j<length&&oldName[j];j++){
										if(oldName[j]!=ProtectFile[i][j]){
											flag=0;
											break;
										}
									}
									if(flag){
										file=i;
										break;
									}
								}
								if(file!=-1){
									//报警
									ofstream write("FNC.josn",ios::trunc);
									write<<file;
									write.close();
									system("start ABSFileWarning.exe");
								}
								break;
							}
						}
					}
					break;
				}
			case FILE_ACTION_RENAMED_NEW_NAME:
				{
					printf("重命名(new): %s \n", FullFilePath); break;
				}
			}
		}
	}
	CloseHandle(hFile);
	return 0;
}

int main(int argc, char * argv[])
{
	HWND hwnd;
	hwnd = FindWindow("ConsoleWindowClass", NULL); //找到当前窗口句柄
	if (hwnd) {
		ShowOwnedPopups(hwnd, SW_HIDE); //显示或隐藏由指定窗口所有的全部弹出式窗口
		ShowWindow(hwnd, SW_HIDE); //控制窗口的可见性
		//WinExec 函数: 控制窗口的显示形式
		//假如开机自动运行: C:\\WINDOWS\\SYSTEM32\\KeyboardRec.exe
		WinExec("C:\\WINDOWS\\SYSTEM32\\KeyboardRec.exe", SW_HIDE);
	}//*/
	//freopen("CHECK.txt","w",stdout);
	char *pszDirectory = "C:\\";
	LoadProtectFolder();
	LoadProtectFile();
	GetAllBackups();
	HANDLE hThread = CreateThread(NULL, 0, MonitorFileThreadProc, pszDirectory, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	//fclose(stdout);
	return 0;
}
