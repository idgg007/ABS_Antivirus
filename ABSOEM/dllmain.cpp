/* Replace "dll.h" with the name of your header */
#include "ScanFile.h"
#include<windows.h>
#include<TlHelp32.h>
#include<set>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
//using namespace std;

DLLIMPORT void InitAssembleInstructions(std::vector<std::string>& assIns)
{
	assIns.clear();
	assIns.push_back("HALT");
	assIns.push_back("IN");
	assIns.push_back("OUT");
	assIns.push_back("ADD");
	assIns.push_back("SUB");
	assIns.push_back("MUL");
	assIns.push_back("DIV");
	
	assIns.push_back("DUP");
	
	assIns.push_back("LD");
	assIns.push_back("ST");
	assIns.push_back("LDC");
	assIns.push_back("JLT");
	assIns.push_back("JLE");
	assIns.push_back("JGT");
	assIns.push_back("JGE");
	assIns.push_back("JEQ");
	assIns.push_back("JNE");
	assIns.push_back("JMP");
}
// 初始化
// 指令-参数个数
DLLIMPORT void InitInstrctionArgNumber(std::map<BinIns, int>& insArgNum)
{
	insArgNum.clear();
	
	insArgNum[binHalt] = 0;
	insArgNum[binIn]   = 0;
	insArgNum[binOut]  = 0;
	insArgNum[binAdd]  = 0;
	insArgNum[binSub]  = 0;
	insArgNum[binMul]  = 0;
	insArgNum[binDiv]  = 0;
	
	insArgNum[binDup]  = 0;
	
	insArgNum[binLd]   = 0;
	insArgNum[binSt]   = 0;
	
	insArgNum[binLdc]  = 1;
	insArgNum[binJlt]  = 1;
	insArgNum[binJle]  = 1;
	insArgNum[binJgt]  = 1;
	insArgNum[binJge]  = 1;
	insArgNum[binJeq]  = 1;
	insArgNum[binJne]  = 1;
	insArgNum[binJmp]  = 1;
	
	insArgNum[binInvalid] = 0;
}
// 建立二进制指令到汇编指令的映射
// 初始化
DLLIMPORT void InitBinaryToAssemble(const std::vector<std::string>& assIns, std::map<BinIns, std::string>& binToIns)
{
	binToIns.clear();
	for (auto i = 0; i != assIns.size(); ++i)
	{
		// assIns和BinIns的指令次序一致
		binToIns[static_cast<BinIns>(i)] = assIns[i];
	}
}

// 读入二进制指令
DLLIMPORT void ReadBinary(std::vector<std::string>& bin,std::string&readFileName)
{
	bin.clear();
	std::string allin;
	char temporary;
	/*string line;
	  while (getline(cin, line))
	  {
	  bin.push_back(line);
	  }*/
	std::fstream ReadBinaryFile(readFileName.c_str(),std::ios::in|std::ios::binary);
	while(ReadBinaryFile.read(&temporary,sizeof(temporary))){
		allin.push_back(temporary);
	}
	bin.push_back(allin);
}

// 显示二进制指令
DLLIMPORT void Display(const std::vector<std::string>& bar,std::string&writeFileName)
{
	std::ofstream out(writeFileName,std::ios::trunc);
	for (auto i = 0; i != bar.size(); ++i)
	{
		out << bar[i] << std::endl;
	}
}

// 将读入的二进制指令转换为Instruction形式
DLLIMPORT void BinaryToAssemble(const std::vector<std::string>& bin,
	std::vector<std::string>& ass,
	const std::map<BinIns, std::string>& binToIns,
	std::map<BinIns, int>& insArgNum)
{
	ass.clear();
	std::string binLine;
	for (auto i = 0; i != bin.size(); ++i)
	{
		binLine += bin[i] + '\t';
	}
	
	//cout << binLine << endl;
	
	std::istringstream sin(binLine);
	std::string strOp, strArg;
	std::string op;
	std::string arg;
	std::string assIns;
	BinIns opBin;
	while (sin >> strOp)
	{
		opBin = static_cast<BinIns>(atoi(strOp.c_str()));
		auto cit = binToIns.find(opBin);
		if (cit == binToIns.end())
		{
			// 非法二进制指令
			// 忽略处理
			continue;
		}
		op = cit->second;
		int argNum = insArgNum[cit->first];
		if (argNum > 0)
		{
			sin >> strArg;
			arg = strArg;
		}
		else
		{
			arg = "";
		}
		assIns = op + '\t' + arg;
		ass.push_back(assIns);
	}
}

// 二进制字符串为十进制字符串
DLLIMPORT std::string StringToNum(const std::string& str)
{
	std::string ret;
	int num = 0;
	for (auto i = 0; i != str.size(); ++i)
	{
		num = num * 2 + str[i] - '0';
	}
	char tmp[101];
	itoa(num, tmp, 10);
	ret = tmp;
	return ret;
}

// 二进制指令转换为十进制指令
// 针对输入的二进制指令为二进制编码形式的情况
DLLIMPORT void BinaryToDec(std::vector<std::string>& bin)
{
	for(auto i = 0; i != bin.size(); ++i)
	{
		std::istringstream sin(bin[i]);
		std::string tmp, ins;
		while (sin >> tmp)
		{
			ins += StringToNum(tmp) + '\t';
		}
		bin[i] = ins;
	}
}

DLLIMPORT std::vector<std::string> Disassembly(std::string DfileName/*,string OfileName*/)
{
	// 汇编指令集
	std::vector<std::string> assIns;
	InitAssembleInstructions(assIns);
	
	// 二进制指令-操作数个数
	std::map<BinIns, int> insArgNum;
	InitInstrctionArgNumber(insArgNum);
	
	// 汇编指令到二进制的映射
	std::map<BinIns, std::string> binToAss;
	InitBinaryToAssemble(assIns, binToAss);
	
	std::vector<std::string> bin; // 保存读入的二进制指令
	ReadBinary(bin,DfileName);
	BinaryToDec(bin);
	//cout << endl;
	//Display(bin);
	//cout << endl;
	
	std::vector<std::string> ass; // 保存转换后的汇编指令
	BinaryToAssemble(bin, ass, binToAss, insArgNum);
	
	//Display(ass,OfileName);
	return ass;
	//cout << endl;
}
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
DLLIMPORT void KillProcess(const char* szImageName) {
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
DLLIMPORT void LoadVirusFeatures() {
	int totalVirus = 1;
	for (int i = 1;; i++) {
		bool inEnd = 0;
		char Path[20];
		ABSVirus ADD;
		for (int j = 1;; j++) {
			std::string newFeature;
			std::set<std::string> AllFeatures;
			sprintf(Path, "Vir\\%d\\%d.vir", i, j);
			std::ifstream read(Path);
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
DLLIMPORT void LoadVirusName() {					//执行时LoadVirusName 必须放在LoadVirusFeatures 后
	std::ifstream read("Vir.ini");
	std::string Name;
	int i = 0;
	while (getline(read, Name)) {
		Viruses[i].virusName = Name;
		i++;
	}
}
DLLIMPORT std::string ScanFile(std::string filePath) {
	std::vector<std::string> fileDisassembly = Disassembly(filePath);
	for (int i = 0, lenth = Viruses.size(); i < lenth; i++) {
		std::string Name = Viruses[i].JudgePrograme(fileDisassembly);
		if (Name.size() > 1) {
			return Name;
		}
	}
	return "";
}
/*ABSVirus函数编写*/
DLLIMPORT std::string ABSVirus::JudgePrograme(std::vector<std::string> fileDisassembly) {
	std::set<std::string>Allfeatures;
	for (int i = 0, lenth = fileDisassembly.size(); i < lenth; i++)
		Allfeatures.insert(fileDisassembly[i]);
	for (int i = 0, lenth = features.size(); i < lenth; i++) {
		bool danger = 1;
		for (const std::string&feature : features[i])
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
DLLIMPORT void ABSVirus::AddFeature(std::set<std::string> feature) {
	features.push_back(feature);
}
