#if BUILDING_DLL
	#define DLLIMPORT __declspec(dllexport)
#else
	#define DLLIMPORT __declspec(dllimport)
#endif
// ʵ��һ���������
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
//using namespace std;

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
DLLIMPORT void InitAssembleInstructions(std::vector<std::string>& assIns);
DLLIMPORT void InitInstrctionArgNumber(std::map<BinIns, int>& insArgNum);
DLLIMPORT void InitBinaryToAssemble(const std::vector<std::string>& assIns, std::map<BinIns, std::string>& binToIns);
DLLIMPORT void ReadBinary(std::vector<std::string>& bin, std::string&readFileName);
DLLIMPORT void Display(const std::vector<std::string>& bar, std::string&writeFileName);
DLLIMPORT void BinaryToAssemble(const std::vector<std::string>& bin,
                                std::vector<std::string>& ass,
                                const std::map<BinIns, std::string>& binToIns,
                                std::map<BinIns, int>& insArgNum);
DLLIMPORT std::string StringToNum(const std::string& str);
DLLIMPORT void BinaryToDec(std::vector<std::string>& bin);
DLLIMPORT std::vector<std::string> Disassembly(std::string DfileName/*,string OfileName*/);
