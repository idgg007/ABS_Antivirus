#include <graphics.h>
#include <ege/sys_edit.h>
#include <cstring>
#include <wtypes.h>
#include <locale.h>
#include "BadSwitch.h"
#include "BadProgressBar.h"
#include "ScanFile.h"
#include "BadButton.h"
#include "VirusJudge.h"
vector<Virus>virRecord;
mouse_msg mouse;
PIMAGE UI, shield, Scan_P;
Button scan;
Button protect;
vector<string>dangerousFile;
//
//判断进程是否存在
//2012-09-10
string ScanFile_Simple(const string&fileName){
	fstream read(fileName.c_str(),ios::in|ios::binary);
	char in;
	string name="";
	name.clear();
	for(int i=0,length=virRecord.size();i<length;i++){
		virRecord[i].ClearMaping();
	}
	while(read.read(&in,sizeof(in))){
		for(int i=0,length=virRecord.size();i<length;i++){
			name=virRecord[i].AddNew(in);
			if(name!=""){
				return name;
			}
		}
	}
	return "";
}
void LoadBitFeaures(){
	ifstream read("BitVir.ini");
	vector<string>allName;
	allName.clear();
	string in;
	while(getline(read,in)){
		allName.push_back(in);
	}
	read.close();
	for(int i=0,length=allName.size();i<length;i++){
		char firstPath[10];
		sprintf(firstPath,"BitVir\\%d",i+1);
		for(int j=1;;j++){
			char secondPath[10];
			sprintf(secondPath,"\\%d.vir",j);
			string fullPath=firstPath;
			fullPath+=secondPath;
			ifstream appear(fullPath);
			if(appear.fail()){
				break;
			}
			appear.close();
			Virus newF;
			newF.ClearMaping();
			newF.SetName(allName[i]);
			newF.SetSize(64);
			newF.LoadFeatures(fullPath);
			virRecord.push_back(newF);
		}
	}
}
void ProtectUI();
char* wideCharToMultiByte(wchar_t* pWCStrKey)
{
	//第一次调用确认转换后单字节字符串的长度，用于开辟空间
	int pSize = WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), NULL, 0, NULL, NULL);
	char* pCStrKey = new char[pSize+1];
	//第二次调用将双字节字符串转换成单字节字符串
	WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), pCStrKey, pSize, NULL, NULL);
	pCStrKey[pSize] = '\0';
	return pCStrKey;
	
	//如果想要转换成string，直接赋值即可
	//string pKey = pCStrKey;
}
void AllScan();
BOOL FindProcess(string programName) {
	int i = 0;
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		i += 0;
	}
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);
	while (bMore) {
		//printf(" 进程名称：%s \n", pe32.szExeFile);
		if (stricmp(programName.c_str(), pe32.szExeFile) == 0) {
			//printf("进程运行中");
			i += 1;
		}
		bMore = ::Process32Next(hProcessSnap, &pe32);
	}
	if (i > 0) {       //大于1，排除自身
		return true;
	} else {
		return false;
	}
}//https://blog.csdn.net/zhensoft163/article/details/7967993
void ScanUI(const string path, const bool FAST) {
	setcolor(RGB(240, 240, 240));
	setfillcolor(RGB(240, 240, 240));
	bar(185, 5, 595, 595);
	putimage(195, 40, Scan_P);
	ProgressBar progressBar;
	progressBar.SetPosition(300, 60);
	progressBar.SetSize(250, 60);
	progressBar.SetBar(240, 50);
	progressBar.SetColor(RGB(255, 100, 100), RGB(240, 240, 240), 30, 255, 30, 200, 255, 200);
	progressBar.Show(0);
	setfont(18, 0, "UTF-8");
	setcolor(RGB(200, 30, 30));
	outtextxy(335, 150, L"准备中...");
	delay_fps(100);
	vector<string> needPath;
	needPath.clear();
	SearchFile(path.c_str(), FAST, needPath);
	dangerousFile.clear();
	for (int i = 0, lenth = needPath.size(); i < lenth; i++) {
		progressBar.Show(double(i + 1) / double(lenth));
		setcolor(RGB(240, 240, 240));
		setfillcolor(RGB(240, 240, 240));
		bar(200, 150, 600, 170);
		setcolor(RGB(200, 30, 30));
		setfont(10, 0, "UTF-8");
		if (needPath[i].size() <= 76) {
			outtextxy(200, 150, needPath[i].c_str());
		} else {
			string frontStr, backStr;
			frontStr.clear();
			backStr.clear();
			for (int j = 0; j < 51; j++)
				frontStr.push_back(needPath[i][j]);
			frontStr += "{...}";
			for (int j = needPath[i].size() - 20, len = needPath[i].size(); j < len; j++)
				backStr.push_back(needPath[i][j]);
			frontStr += backStr;
			outtextxy(200, 150, frontStr.c_str());
		}
		delay_fps(300);
		if (ScanFile(needPath[i]))
			dangerousFile.push_back(needPath[i]);
		else{
			string name=ScanFile_Simple(needPath[i]);
			if(name!=""){
				dangerousFile.push_back(needPath[i]);
			}
		}
	}//*/
	if (!dangerousFile.size()) {
		MessageBox(GetForegroundWindow(), "No Virus!", "ABS2.0", 0);
		return;
	}
	setcolor(RGB(240, 240, 240));
	setfillcolor(RGB(240, 240, 240));
	bar(200, 150, 600, 170);
	setfont(18, 0, "UTF-8");
	setcolor(RGB(80, 80, 255));
	outtextxy(350, 150, L"病毒：");
	setcolor(RGB(255, 255, 255));
	setfillcolor(RGB(255, 255, 255));
	bar(195, 170, 585, 500);
	Button deal, esc;
	wchar_t dealStr[]=L"立即处理";
	wchar_t escStr[]=L"暂不处理";
	deal.FPS = 1;
	deal.speed = 2;
	esc.FPS = 1;
	esc.speed = 2;
	deal.position_x = 200;
	deal.position_y = 525;
	deal.lenth_x = 150;
	deal.lenth_y = 50;
	deal.linesColor = RGB(150, 150, 255);
	deal.filldColor = RGB(230, 230, 230);
	deal.text.pos_x = 203;
	deal.text.pos_y = 533;
	deal.text.wordColor = RGB(70, 70, 150);
	deal.text.wordSize = 36;
	deal.text.text = wideCharToMultiByte(dealStr);
	esc.position_x = 400;
	esc.position_y = 525;
	esc.lenth_x = 150;
	esc.lenth_y = 50;
	esc.linesColor = RGB(170, 170, 170);
	esc.filldColor = RGB(230, 230, 230);
	esc.text.pos_x = 403;
	esc.text.pos_y = 533;
	esc.text.wordColor = RGB(100, 100, 100);
	esc.text.wordSize = 36;
	esc.text.text = wideCharToMultiByte(escStr);
	esc.DrawButton();
	deal.DrawButton();
	int showPosition = 0;
	setfont(15, 0, "UTF-8");
	setcolor(RGB(255, 200, 200));
	for (int i = 180; i < 500; i += 20) {
		line(195, i, 585, i);
	}
	line(530, 170, 530, 500);
	setcolor(RGB(0, 0, 0));
	for (int i = 0, lenth = min(int(dangerousFile.size()), 16); i < lenth; i++) { //显示威胁文件
		outtextrect(195, 182 + i * 20, 330, 20, dangerousFile[i].c_str());
	}
	setcolor(RGB(0, 0, 255));
	for (int i = 0, lenth = min(int(dangerousFile.size()), 16); i < lenth; i++) { //显示威胁文件
		outtextrect(535, 182 + i * 20, 50, 20, L"取消");
	}
	int changeLine = -1;
	string changeingStr;
	int times = 0;
	bool ifChanged = 0; //之前状态是否被改变
	mouse.x=0,mouse.y=0;
	mouse.msg = mouse_msg_move;
	while (1) {
		if (mousemsg()) {
			mouse = getmouse();
			if (mouse.x > 530 && mouse.y % 20 != 0 && mouse.is_up() && (mouse.y - 180) / 20 + showPosition < dangerousFile.size()) {
				for (int i = (mouse.y - 180) / 20 + showPosition + 1, lenth = dangerousFile.size(); i < lenth; i++) {
					dangerousFile[i - 1] = dangerousFile[i];
				}
				ifChanged = 1;
				dangerousFile.pop_back();
			}
		}
		if (mouse.x >= 195 && mouse.x <= 585 && mouse.y >= 170 && mouse.y <= 500) {
			if (mouse.wheel < 0 && showPosition < dangerousFile.size() - 16)
				showPosition++, ifChanged = 1 ,mouse.wheel=0;
			if (mouse.wheel > 0 && showPosition > 0)
				showPosition--, ifChanged = 1 ,mouse.wheel=0;
			if (ifChanged) {
				setcolor(RGB(255, 255, 255));
				setfillcolor(RGB(255, 255, 255));
				for (int i = 0; i < 16; i++) {
					bar(195, 181 + i * 20, 585, 199 + i * 20);
				}
				setcolor(RGB(255, 200, 200));
				line(530, 170, 530, 500);
				setcolor(RGB(0, 0, 0));
				setfont(15, 0, "UTF-8");
				for (int i = showPosition, lenth = min(int(dangerousFile.size()), 16 + showPosition); i < lenth; i++) {
					outtextrect(195, 182 + (i - showPosition) * 20, 330, 20, dangerousFile[i].c_str());
				}
				setcolor(RGB(0, 0, 255));
				for (int i = showPosition, lenth = min(int(dangerousFile.size()), 16 + showPosition); i < lenth; i++) { //显示威胁文件
					outtextrect(535, 182 + (i - showPosition) * 20, 50, 20, L"取消");
				}
				ifChanged = 0;
			}
			if (mouse.x < 530 && mouse.y % 20 != 0&&(mouse.y - 180) / 20 + showPosition < dangerousFile.size()) {
				if (changeLine != (mouse.y - 180) / 20 + showPosition) {
					changeingStr = dangerousFile[(mouse.y - 180) / 20 + showPosition];
					changeLine = (mouse.y - 180) / 20 + showPosition;
					ifChanged = 1;
				} else {
					if (times != 20) {
						times++;
					} else {
						setcolor(RGB(255, 255, 255));
						setfillcolor(RGB(255, 255, 255));
						bar(195, 181 + (changeLine - showPosition) * 20, 529, 199 + (changeLine - showPosition) * 20);
						if (changeingStr.size() > 41) {
							for (int i = 1, lenth = changeingStr.size(); i < lenth; i++)
								changeingStr[i - 1] = changeingStr[i];
							changeingStr.pop_back();
						}
						setcolor(RGB(0, 0, 0));
						setfont(15, 0, "UTF-8");
						outtextrect(195, 182 + (changeLine - showPosition) * 20, 330, 20, changeingStr.c_str());
						times = 1;
					}
				}
			} else {
				ifChanged = 1;
				changeLine = -1;
			}
		}
		if (esc.UpdateButton(mouse.x, mouse.y, mouse.is_down())) {
			return;
		}
		if (deal.UpdateButton(mouse.x, mouse.y, mouse.is_down())) {
			for (int i = 0, lenth = dangerousFile.size(); i < lenth; i++)
				remove(dangerousFile[i].c_str());
			MessageBox(GetForegroundWindow(), "completed", "ABS2.0", 0);
			return;
		}
		if (scan.UpdateButton(mouse.x, mouse.y, mouse.is_down())) {
			AllScan();
		}
		if (protect.UpdateButton(mouse.x, mouse.y, mouse.is_down())) {
			ProtectUI();
		}
		delay_fps(375);
	}
}
string getInPath(){
	wchar_t Path[200];
	inputbox_getline(L"ABS杀毒软件2.0",L"输入文件路径",Path,100);
	string PATH;
	PATH=wideCharToMultiByte(Path);
	return PATH;
}
void AllScan() {
clear:
	setcolor(RGB(240,240,240));
	setfillcolor(RGB(240,240,240));
	bar(185, 5, 595, 595);
	setfont(25, 0, "UTF-8");
	setcolor(RGB(0, 0, 0));
	outtextxy(300, 60, L"扫描您的计算机");
	setfont(18, 0, "UTF-8");
	setcolor(RGB(100, 100, 100));
	outtextxy(300, 90, L"及时发现病毒，保护您的计算机");
	delay_fps(60);
	//setcolor(RGB(0,0,0));//测试图片大小
	//setfillcolor(RGB(0,0,0));
	//bar(195,50,295,150);
	putimage(195, 40, Scan_P);
	Button FASTSCAN;
	Button COMPLETELYSCAN;
	Button custom;
	wchar_t FASTSCANstr[]=L"快速扫描";
	wchar_t COMPLETELYSCANstr[]=L"完全扫描";
	wchar_t customStr[]=L"自定义扫描";
	FASTSCAN.FPS = 1;
	FASTSCAN.speed = 2;
	COMPLETELYSCAN.FPS = 1;
	COMPLETELYSCAN.speed = 2;
	custom.FPS = 1;
	custom.speed = 2;
	FASTSCAN.position_x = 200;
	FASTSCAN.position_y = 190;
	FASTSCAN.lenth_x = 202;
	FASTSCAN.lenth_y = 40;
	FASTSCAN.linesColor = RGB(100, 100, 235);
	FASTSCAN.filldColor = RGB(235, 200, 200);
	FASTSCAN.text.pos_x = 245;
	FASTSCAN.text.pos_y = 198;
	FASTSCAN.text.wordColor = RGB(50, 50, 50);
	FASTSCAN.text.wordSize = 25;
	FASTSCAN.text.text = wideCharToMultiByte(FASTSCANstr);
	custom.position_x = 200;
	custom.position_y = 330;
	custom.lenth_x = 202;
	custom.lenth_y = 40;
	custom.linesColor = RGB(100, 100, 235);
	custom.filldColor = RGB(235, 200, 200);
	custom.text.pos_x = 233;
	custom.text.pos_y = 338;
	custom.text.wordColor = RGB(50, 50, 50);
	custom.text.wordSize = 25;
	custom.text.text = wideCharToMultiByte(customStr);
	COMPLETELYSCAN.position_x = 200;
	COMPLETELYSCAN.position_y = 260;
	COMPLETELYSCAN.lenth_x = 202;
	COMPLETELYSCAN.lenth_y = 40;
	COMPLETELYSCAN.linesColor = RGB(100, 100, 235);
	COMPLETELYSCAN.filldColor = RGB(235, 200, 200);
	COMPLETELYSCAN.text.pos_x = 245;
	COMPLETELYSCAN.text.pos_y = 266;
	COMPLETELYSCAN.text.wordColor = RGB(50, 50, 50);
	COMPLETELYSCAN.text.wordSize = 25;
	COMPLETELYSCAN.text.text = wideCharToMultiByte(COMPLETELYSCANstr);
	FASTSCAN.DrawButton();
	COMPLETELYSCAN.DrawButton();
	custom.DrawButton();
	setfont(13, 0, "UTF-8");
	setcolor(RGB(100, 100, 100));
	outtextxy(407, 203, L"只扫描User下的部分文件");
	outtextxy(407, 273, L"扫描C盘所有文件");
	delay_fps(60);
	while (1) {
		if (mousemsg())
			mouse = getmouse();
		if (FASTSCAN.UpdateButton(mouse.x, mouse.y, mouse.is_down())) {
			ScanUI("C:\\Users\\", 1);
			mouse.msg = mouse_msg_move;
			goto clear;
		}
		if (COMPLETELYSCAN.UpdateButton(mouse.x, mouse.y, mouse.is_down())) {
			ScanUI("C:\\", 0);
			mouse.msg = mouse_msg_move;
			goto clear;
		}
		if (custom.UpdateButton(mouse.x, mouse.y, mouse.is_down())) {
			string path=getInPath();
			ScanUI(path,0);
			goto clear;
		}
		if (scan.UpdateButton(mouse.x, mouse.y, mouse.is_down())) {
			AllScan();
		}
		if (protect.UpdateButton(mouse.x, mouse.y, mouse.is_down())) {
			ProtectUI();
		}
		FASTSCAN.UpdateButton(mouse.x, mouse.y, 0);
		COMPLETELYSCAN.UpdateButton(mouse.x, mouse.y, 0);
		custom.UpdateButton(mouse.x, mouse.y, 0);
		scan.UpdateButton(mouse.x, mouse.y, 0);
		protect.UpdateButton(mouse.x, mouse.y, 0);
		FASTSCAN.UpdateButton(mouse.x, mouse.y, 0);
		COMPLETELYSCAN.UpdateButton(mouse.x, mouse.y, 0);
		custom.UpdateButton(mouse.x, mouse.y, 0);
		scan.UpdateButton(mouse.x, mouse.y, 0);
		protect.UpdateButton(mouse.x, mouse.y, 0);
		FASTSCAN.UpdateButton(mouse.x, mouse.y, 0);
		COMPLETELYSCAN.UpdateButton(mouse.x, mouse.y, 0);
		custom.UpdateButton(mouse.x, mouse.y, 0);
		scan.UpdateButton(mouse.x, mouse.y, 0);
		protect.UpdateButton(mouse.x, mouse.y, 0);
		FASTSCAN.UpdateButton(mouse.x, mouse.y, 0);
		COMPLETELYSCAN.UpdateButton(mouse.x, mouse.y, 0);
		custom.UpdateButton(mouse.x, mouse.y, 0);
		scan.UpdateButton(mouse.x, mouse.y, 0);
		protect.UpdateButton(mouse.x, mouse.y, 0);
		FASTSCAN.UpdateButton(mouse.x, mouse.y, 0);
		COMPLETELYSCAN.UpdateButton(mouse.x, mouse.y, 0);
		custom.UpdateButton(mouse.x, mouse.y, 0);
		scan.UpdateButton(mouse.x, mouse.y, 0);
		protect.UpdateButton(mouse.x, mouse.y, 0);
		delay_fps(60);
	}
}
void SetButton() {
	wchar_t scanStr[]=L"查杀病毒";
	wchar_t protectStr[]=L"防护中心";
	scan.FPS = 1;
	scan.speed = 2;
	protect.FPS = 1;
	protect.speed = 2;
	scan.position_x = 10;
	scan.position_y = 190;
	scan.lenth_x = 150;
	scan.lenth_y = 50;
	scan.linesColor = RGB(255, 150, 150);
	scan.filldColor = RGB(245, 245, 245);
	scan.text.pos_x = 15;
	scan.text.pos_y = 197;
	scan.text.wordColor = RGB(100, 100, 100);
	scan.text.wordSize = 36;
	scan.text.text = wideCharToMultiByte(scanStr);
	protect.position_x = 10;
	protect.position_y = 260;
	protect.lenth_x = 150;
	protect.lenth_y = 50;
	protect.linesColor = RGB(255, 150, 150);
	protect.filldColor = RGB(245, 245, 245);
	protect.text.pos_x = 15;
	protect.text.pos_y = 267;
	protect.text.wordColor = RGB(100, 100, 100);
	protect.text.wordSize = 36;
	protect.text.text = wideCharToMultiByte(protectStr);
}
void ProtectUI(){
	Switch HIPSA;
	Switch HIPSB;
	Switch Memory;
	bool HIPSAState=FindProcess("ABSHIPSA.exe");
	bool HIPSBState=FindProcess("ABSHIPSB.exe");
	bool MemoryState=FindProcess("ABSMemory.exe");
	setcolor(RGB(240, 240, 240));
	setfillcolor(RGB(240, 240, 240));
	bar(185, 5, 595, 595);
	HIPSA.SetColor(RGB(200,200,200),RGB(100,255,100),RGB(0,122,255),RGB(100,100,100));
	HIPSB.SetColor(RGB(200,200,200),RGB(100,255,100),RGB(0,122,255),RGB(100,100,100));
	Memory.SetColor(RGB(200,200,200),RGB(100,255,100),RGB(0,122,255),RGB(100,100,100));
	HIPSA.SetSize(50,15);
	HIPSB.SetSize(50,15);
	Memory.SetSize(50,15);
	HIPSA.Move(500,22);
	HIPSB.Move(500,52);
	Memory.Move(500,82);
	HIPSA.SetState(HIPSAState);
	HIPSB.SetState(HIPSBState);
	Memory.SetState(MemoryState);
	HIPSA.Show(220,160,0);
	HIPSB.Show(200,160,0);
	Memory.Show(220,160,0);
	setfont(20,0,"楷体");
	setcolor(RGB(100,100,100));
	setfillcolor(RGB(100,100,100));
	outtextxy(200,20,L"监控所有活动的文件");
	outtextxy(200,50,L"保护系统文件");
	outtextxy(200,80,L"监控所有进程");
	while(1){
		if(mousemsg()){
			mouse=getmouse();
		}
		if(scan.UpdateButton(mouse.x,mouse.y,mouse.is_down())){
			AllScan();
		}
		if(HIPSAState!=HIPSA.Show(mouse.x,mouse.y,mouse.is_down())){
			HIPSAState=!HIPSAState;
			if(HIPSAState){
				system("start ABSHIPSA.exe");
			}else{
				KillProcess("ABSHIPSA.exe");
			}
			mouse.x=0;
		}
		if(HIPSBState!=HIPSB.Show(mouse.x,mouse.y,mouse.is_down())){
			HIPSBState=!HIPSBState;
			if(HIPSBState){
				system("start ABSHIPSB.exe");
			}else{
				KillProcess("ABSHIPSB.exe");
			}
			mouse.x=0;
		}
		if(MemoryState!=Memory.Show(mouse.x,mouse.y,mouse.is_down())){
			MemoryState=!MemoryState;
			if(MemoryState){
				system("start ABSMemory.exe");
			}else{
				KillProcess("ABSMemory.exe");
			}
			mouse.x=0;
		}
		scan.UpdateButton(mouse.x,mouse.y,0);
		scan.UpdateButton(mouse.x,mouse.y,0);
		scan.UpdateButton(mouse.x,mouse.y,0);
		scan.UpdateButton(mouse.x,mouse.y,0);
		scan.UpdateButton(mouse.x,mouse.y,0);
		delay_fps(60);
	}
}
int main() {
	LoadBitFeaures();
	LoadVirusFeatures();
	LoadVirusName();
	Button FastScan;
	initgraph(600, 600);
	setbkcolor(RGB(210, 210, 255));
	UI = newimage();
	shield = newimage();
	Scan_P = newimage();
	getimage(UI, "ui\\ui.png");
	getimage(shield, "ui\\shield.png");
	getimage(Scan_P, "ui\\Scan.jpg");
	putimage(10, 10, UI);
clean:
	setfillcolor(RGB(250, 230, 230));
	setcolor(RGB(250, 230, 230));
	bar(180, 0, 600, 600);
	SetButton();
	protect.DrawButton();
	scan.DrawButton();
	putimage(300, 50, shield);
	setfont(25, 0, "UTF-8");
	setcolor(RGB(190, 80, 80));
	outtextxy(275, 280, L"快速扫描您的计算机");
	outtextxy(300, 320, L"以保护您电脑");
	//设置按钮
	wchar_t fastScanStr[]=L"快速扫描";
	FastScan.FPS = 1;
	FastScan.speed = 2;
	FastScan.position_x = 300;
	FastScan.position_y = 400;
	FastScan.lenth_x = 180;
	FastScan.lenth_y = 50;
	FastScan.linesColor = RGB(150, 150, 255);
	FastScan.filldColor = RGB(200, 200, 255);
	FastScan.text.pos_x = 319;
	FastScan.text.pos_y = 407;
	FastScan.text.wordColor = RGB(0, 0, 255);
	FastScan.text.wordSize = 36;
	FastScan.text.text = wideCharToMultiByte(fastScanStr);
	FastScan.DrawButton();
	setfont(20, 0, "UTF-8");
	setcolor(RGB(170, 170, 170));
	outtextxy(10, 570, "idgg007 2022.6");
	while (1) {
		if (mousemsg())
			mouse = getmouse();
		if (protect.UpdateButton(mouse.x, mouse.y, mouse.is_down())) {
			ProtectUI();
		}
		if (scan.UpdateButton(mouse.x, mouse.y, mouse.is_down())) {
			AllScan();
		}
		if (FastScan.UpdateButton(mouse.x, mouse.y, mouse.is_down())) {
			ScanUI("C:\\Users\\", 1);
			goto clean;
		}
		protect.UpdateButton(mouse.x, mouse.y, 0);
		scan.UpdateButton(mouse.x, mouse.y, 0);
		FastScan.UpdateButton(mouse.x, mouse.y, 0);
		protect.UpdateButton(mouse.x, mouse.y, 0);
		scan.UpdateButton(mouse.x, mouse.y, 0);
		FastScan.UpdateButton(mouse.x, mouse.y, 0);
		protect.UpdateButton(mouse.x, mouse.y, 0);
		scan.UpdateButton(mouse.x, mouse.y, 0);
		FastScan.UpdateButton(mouse.x, mouse.y, 0);
		protect.UpdateButton(mouse.x, mouse.y, 0);
		scan.UpdateButton(mouse.x, mouse.y, 0);
		FastScan.UpdateButton(mouse.x, mouse.y, 0);
		protect.UpdateButton(mouse.x, mouse.y, 0);
		scan.UpdateButton(mouse.x, mouse.y, 0);
		FastScan.UpdateButton(mouse.x, mouse.y, 0);
		delay_fps(60);
	}
	getch();
	return 0;
}
