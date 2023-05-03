// ʵ��һ���������
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

// ������ָ��ṹ��
// ָ����+������

struct Instruction {
	BinIns op;  // ָ����ֻռһ���ֽ�
	int    arg; // ��������ռ�ĸ��ֽ�
};

// ö�����͵Ķ�����ָ�

// ö������˵����
// enum���涨�����ö��������
// �������ڲ��Ǹ�ö�����Ϳ���ȡ��ֵ

// ��ʼ�����ָ�
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

// ��ʼ��
// ָ��-��������
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

// ����������ָ����ָ���ӳ��
// ��ʼ��
void InitBinaryToAssemble(const vector<string>& assIns, map<BinIns, string>& binToIns) {
	binToIns.clear();
	for (auto i = 0; i != assIns.size(); ++i) {
		// assIns��BinIns��ָ�����һ��
		binToIns[static_cast<BinIns>(i)] = assIns[i];
	}
}

// ���������ָ��
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

// ��ʾ������ָ��
void Display(const vector<string>& bar, string&writeFileName) {
	ofstream out(writeFileName, ios::trunc);
	for (auto i = 0; i != bar.size(); ++i) {
		out << bar[i] << endl;
	}
}

// ������Ķ�����ָ��ת��ΪInstruction��ʽ
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
			// �Ƿ�������ָ��
			// ���Դ���
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

// �������ַ���Ϊʮ�����ַ���
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

// ������ָ��ת��Ϊʮ����ָ��
// �������Ķ�����ָ��Ϊ�����Ʊ�����ʽ�����
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
	// ���ָ�
	vector<string> assIns;
	InitAssembleInstructions(assIns);

	// ������ָ��-����������
	map<BinIns, int> insArgNum;
	InitInstrctionArgNumber(insArgNum);

	// ���ָ������Ƶ�ӳ��
	map<BinIns, string> binToAss;
	InitBinaryToAssemble(assIns, binToAss);

	vector<string> bin; // �������Ķ�����ָ��
	ReadBinary(bin, DfileName);
	BinaryToDec(bin);
	//cout << endl;
	//Display(bin);
	//cout << endl;

	vector<string> ass; // ����ת����Ļ��ָ��
	BinaryToAssemble(bin, ass, binToAss, insArgNum);

	return ass;
	//Display(ass,OfileName);

	//cout << endl;
}
