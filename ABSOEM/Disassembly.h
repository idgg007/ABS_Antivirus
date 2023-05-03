#if BUILDING_DLL
	#define DLLIMPORT __declspec(dllexport)
#else
	#define DLLIMPORT __declspec(dllimport)
#endif
// 实现一个反汇编器
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
