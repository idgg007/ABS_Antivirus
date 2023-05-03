#include"VirusJudge.h"
#include<iostream>
#include<string>
using namespace std;
Virus vir;
int main(){
	vir.SetName("text");
	vir.SetSize(64);
	vir.ClearMaping();
	vir.LoadFeatures("ans.vir");
	string PATH;
	getline(cin,PATH);
	fstream read(PATH.c_str(),ios::in|ios::binary);
	char in;
	while(read.read(&in,sizeof(in))){
		string nec=vir.AddNew(in);
		if(nec!=""){
			cout<<nec;
			getchar();
			return 0;
		}
	}
}
