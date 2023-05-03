#include"BadButton.h"
#include<graphics.h>
#include<string>
#include<fstream>
using namespace std;
mouse_msg mouse;
PIMAGE VirusPicture;
string virusPath;
string virusName;
Button deal, trust;
void GetVirus() {
	ifstream read("VNC.josn");
	getline(read, virusPath);
	getline(read, virusName);
}
void DeleteVirus() {
	remove(virusPath.c_str());
}
void TrustFile() {
	ofstream write("Trust.ini", ios::app);
	for (int i = 0; virusPath[i]; i++) {
		write << virusPath[i];
	}
	write << "\n";
}
void LoadVirus() {
	VirusPicture = newimage();
	getimage(VirusPicture, "ui/1.jpg");
}
void PrintBack() {
	setbkcolor(RGB(240, 240, 240));
	setfillcolor(RGB(0, 0, 250));
	setcolor(RGB(0, 0, 250));
	bar(0, 0, 155, 155);
	putimage(0, 0, VirusPicture);
	for (int i = 155; i <= 200; i++) {
		setfillcolor(RGB((i - 155) * 5, (i - 155) * 5, 251));
		setcolor(RGB((i - 155) * 5, (i - 155) * 5, 251));
		line(i, 0, i, 156);
	}
	Sleep(20);
	setcolor(RGB(50, 50, 250));
	setfillcolor(RGB(50, 50, 250));
	setfont(10, 0, "宋体");
	setbkmode(TRANSPARENT);
	outtextrect(220, 30, 50, 10, "发现威胁:");
	outtextrect(220, 50, 50, 10, "威胁名称:");
	setcolor(RGB(220, 220, 220));
	setfillcolor(RGB(220, 220, 220));
	bar(270, 30, 450, 40);
	bar(270, 50, 450, 60);
	//设置按钮
	deal.FPS = 0;
	deal.speed = 9;
	deal.position_x = 220;
	deal.position_y = 100;
	deal.lenth_x = 90;
	deal.lenth_y = 40;
	deal.text.pos_x = 225;
	deal.text.pos_y = 110;
	deal.text.wordColor = RGB(0, 0, 255);
	deal.text.wordSize = 20;
	deal.text.text = "立即处理";
	deal.filldColor = RGB(100, 255, 100);
	deal.linesColor = RGB(0, 200, 0);
	deal.DrawButton();
	trust.FPS = 0;
	trust.speed = 9;
	trust.position_x = 330;
	trust.position_y = 100;
	trust.lenth_x = 90;
	trust.lenth_y = 40;
	trust.text.pos_x = 335;
	trust.text.pos_y = 110;
	trust.text.wordColor = RGB(0, 255, 0);
	trust.text.wordSize = 20;
	trust.text.text = "信任文件";
	trust.filldColor = RGB(170, 170, 170);
	trust.linesColor = RGB(210, 210, 210);
	trust.DrawButton();
	//
	GetVirus();
	setcolor(RGB(0, 0, 0));
	setfillcolor(RGB(0, 0, 0));
	setfont(10, 0, "宋体");
	setbkmode(TRANSPARENT);
	outtextrect(270, 30, 180, 10, virusPath.c_str());
	outtextrect(270, 50, 180, 10, virusName.c_str());
}
void NameChanging(string name) {
	setcolor(RGB(220, 220, 220));
	setfillcolor(RGB(220, 220, 220));
	bar(270, 30, 450, 40);
	setcolor(RGB(0, 0, 0));
	setfillcolor(RGB(0, 0, 0));
	setfont(10, 0, "宋体");
	setbkmode(TRANSPARENT);
	outtextrect(270, 30, 180, 10, name.c_str());
}
int main() {
	initgraph(500, 155);
	LoadVirus();
	PrintBack();
	string times = virusPath;
	int PAST;
	while (1) {
		if (mousemsg()) {
			mouse = getmouse();
		}
		if (mouse.x >= 270 && mouse.x <= 450
		    && mouse.y >= 30 && mouse.y <= 40) {
			if(PAST>1){
				PAST--;
			}else{
				if(times.size()>36){
					for (int i = 1; times[i]; i++) {
						times[i - 1] = times[i];
						times[i]=0;
					}
					times.pop_back();
				}
				NameChanging(times);
				PAST = 100;
			}
		} else if (PAST) {
			times = virusPath;
			NameChanging(times);
			PAST = 0;
		}
		if (deal.UpdateButton(mouse.x, mouse.y, mouse.is_down())) {
			DeleteVirus();
			return 0;
		}
		if (trust.UpdateButton(mouse.x, mouse.y, mouse.is_down())) {
			TrustFile();
			return 0;
		}
		delay_fps(375);
	}
	return 0;
}
