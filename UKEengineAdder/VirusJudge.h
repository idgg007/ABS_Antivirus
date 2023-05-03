#include<fstream>
#include<queue>
#include<vector>
#include<string>
using namespace std;
class Virus{
private:
	string virusName;
	vector<queue<char>>features;
	vector<bool>maping;
	queue<char>nowFeatures;
	int length,totalSame;
public:
	string AddNew(const char&newData);
	void LoadFeatures(const string&filePath);
	void SetName(const string&name);
	void SetSize(const int&sizeOfVirus);
	void ClearMaping();
};
string Virus::AddNew(const char&newData){
	nowFeatures.push(newData);
	if(nowFeatures.size()==length){
		for(int i=0,all=features.size();i<all;i++){
			if(nowFeatures==features[i]&&maping[i]==0){
				maping[i]=1;
				totalSame++;
			}
		}
		if(totalSame>=features.size()){
			return virusName;
		}
		nowFeatures.pop();
	}
	return "";
}
void Virus::LoadFeatures(const string&filePath){
	fstream read(filePath.c_str(),ios::in|ios::binary);
	char in;
	queue<char>newFeatures;
	while(read.read(&in,sizeof(in))){
		newFeatures.push(in);
		if(newFeatures.size()==length){
			maping.push_back(0);
			features.push_back(newFeatures);
			while(!newFeatures.empty()){
				newFeatures.pop();
			}
		}
	}
}
void Virus::SetName(const string&name){
	virusName=name;
}
void Virus::SetSize(const int&SizeOfVirus){
	length=SizeOfVirus;
}
void Virus::ClearMaping(){
	maping.assign(features.size(),0);
	while(!nowFeatures.empty()){
		nowFeatures.pop();
	}
	totalSame=0;
}
