// 实现一个反汇编器
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

enum BinIns {
	binHalt, binIn, binOut, binAdd, binSub, binMul, binDiv,
	binDup,
	binLd, binSt, binLdc, binJlt, binJle, binJgt, binJge, binJeq, binJne, binJmp,
	binInvalid
};

// 二进制指令结构体
// 指令码+操作数

struct Instruction {
	BinIns op;  // 指令码只占一个字节
	int    arg; // 操作数，占四个字节
};

// 枚举类型的二进制指令集

// 枚举类型说明：
// enum后面定义的是枚举类型名
// 花括号内部是该枚举类型可以取的值

// 初始化汇编指令集
void InitAssembleInstructions(vector<string>& assIns) {
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
void InitInstrctionArgNumber(map<BinIns, int>& insArgNum) {
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
void InitBinaryToAssemble(const vector<string>& assIns, map<BinIns, string>& binToIns) {
	binToIns.clear();
	for (auto i = 0; i != assIns.size(); ++i) {
		// assIns和BinIns的指令次序一致
		binToIns[static_cast<BinIns>(i)] = assIns[i];
	}
}

// 读入二进制指令
void ReadBinary(vector<string>& bin, string&readFileName) {
	bin.clear();
	string allin;
	char temporary;
	/*string line;
	while (getline(cin, line))
	{
		bin.push_back(line);
	}*/
	fstream ReadBinaryFile(readFileName.c_str(), ios::in | ios::binary);
	while (ReadBinaryFile.read(&temporary, sizeof(temporary))) {
		allin.push_back(temporary);
	}
	bin.push_back(allin);
}

// 显示二进制指令
void Display(const vector<string>& bar, string&writeFileName) {
	ofstream out(writeFileName, ios::trunc);
	for (auto i = 0; i != bar.size(); ++i) {
		out << bar[i] << endl;
	}
}

// 将读入的二进制指令转换为Instruction形式
void BinaryToAssemble(const vector<string>& bin,
                      vector<string>& ass,
                      const map<BinIns, string>& binToIns,
                      map<BinIns, int>& insArgNum) {
	ass.clear();
	string binLine;
	for (auto i = 0; i != bin.size(); ++i) {
		binLine += bin[i] + '\t';
	}

	//cout << binLine << endl;

	istringstream sin(binLine);
	string strOp, strArg;
	string op;
	string arg;
	string assIns;
	BinIns opBin;
	while (sin >> strOp) {
		opBin = static_cast<BinIns>(atoi(strOp.c_str()));
		auto cit = binToIns.find(opBin);
		if (cit == binToIns.end()) {
			// 非法二进制指令
			// 忽略处理
			continue;
		}
		op = cit->second;
		int argNum = insArgNum[cit->first];
		if (argNum > 0) {
			sin >> strArg;
			arg = strArg;
		} else {
			arg = "";
		}
		assIns = op + '\t' + arg;
		ass.push_back(assIns);
	}
}

// 二进制字符串为十进制字符串
string StringToNum(const string& str) {
	string ret;
	int num = 0;
	for (auto i = 0; i != str.size(); ++i) {
		num = num * 2 + str[i] - '0';
	}
	char tmp[101];
	itoa(num, tmp, 10);
	ret = tmp;
	return ret;
}

// 二进制指令转换为十进制指令
// 针对输入的二进制指令为二进制编码形式的情况
void BinaryToDec(vector<string>& bin) {
	for (auto i = 0; i != bin.size(); ++i) {
		istringstream sin(bin[i]);
		string tmp, ins;
		while (sin >> tmp) {
			ins += StringToNum(tmp) + '\t';
		}
		bin[i] = ins;
	}
}

vector<string> Disassembly(string DfileName/*,string OfileName*/) {
	// 汇编指令集
	vector<string> assIns;
	InitAssembleInstructions(assIns);

	// 二进制指令-操作数个数
	map<BinIns, int> insArgNum;
	InitInstrctionArgNumber(insArgNum);

	// 汇编指令到二进制的映射
	map<BinIns, string> binToAss;
	InitBinaryToAssemble(assIns, binToAss);

	vector<string> bin; // 保存读入的二进制指令
	ReadBinary(bin, DfileName);
	BinaryToDec(bin);
	//cout << endl;
	//Display(bin);
	//cout << endl;

	vector<string> ass; // 保存转换后的汇编指令
	BinaryToAssemble(bin, ass, binToAss, insArgNum);

	return ass;
	//Display(ass,OfileName);

	//cout << endl;
}
