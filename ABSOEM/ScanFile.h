#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif
#include"Disassembly.h"
#include<windows.h>
#include<TlHelp32.h>
#include<set>
class DLLIMPORT ABSVirus {
	private:
		std::vector<std::set<std::string> > features;
	public:
		std::string virusName;
		std::string JudgePrograme(std::vector<std::string> fileDisassembly);		//判断是否为病毒
		void AddFeature(std::set<std::string> feature);					//添加新特征
};
DLLIMPORT std::vector<ABSVirus> Viruses;
DLLIMPORT void KillProcess(const char* szImageName);
DLLIMPORT void LoadVirusFeatures();
DLLIMPORT void LoadVirusName();
DLLIMPORT std::string ScanFile(std::string filePath);
