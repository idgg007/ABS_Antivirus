#include <graphics.h>
#include <ege/sys_edit.h>
#include <fstream>
#include <vector>
#include "BadButton.h"
using namespace std;
PIMAGE filePicture;
Button deal,cancel;
mouse_msg mouse;
void SetButton(){
	deal.speed=2;
	deal.FPS=1;
	cancel.speed=2;
	cancel.FPS=1;
	deal.position_x=230;
	deal.position_y=100;
	cancel.position_x=380;
	cancel.position_y=100;
	deal.lenth_x=100;
	deal.lenth_y=40;
	cancel.lenth_x=100;
	cancel.lenth_y=40;
	deal.filldColor=RGB(220,220,220);
	deal.linesColor=RGB(0,122,255);
	cancel.filldColor=RGB(220,220,220);
	cancel.linesColor=RGB(150,150,150);
	deal.text.text="立即恢复";
	cancel.text.text="信任修改";
	deal.text.wordColor=RGB(50,50,50);
	deal.text.wordSize=20;
	cancel.text.wordColor=RGB(50,50,50);
	cancel.text.wordSize=20;
	deal.text.pos_x=240;
	deal.text.pos_y=110;
	cancel.text.pos_x=390;
	cancel.text.pos_y=110;
}
int main()
{
	initgraph(500,155);
	setcolor(RGB(0,122,255));
	setfillcolor(RGB(0,122,255));
	setbkcolor(RGB(255,255,255));
	filePicture=newimage();
	getimage(filePicture,"ui\\fileProtect.png");
	bar(0,0,155,155);
	putimage(0,0,filePicture);
	for(int i=155;i<133/2+155;i++){
		setcolor(RGB((i-155)*3,122+2*(i-155),255));
		setfillcolor(RGB((i-155)*3,122+2*(i-155),255));
		line(i,0,i,155);
	}
	setfont(20,0,"楷体");
	setcolor(RGB(100,100,255));
	outtextxy(270,10,"保护文件夹已被更改");
	SetButton();
	SetButton();
	deal.DrawButton();
	cancel.DrawButton();
	string fileFullPath;
	ifstream read("FOC.josn");
	getline(read,fileFullPath);
	sys_edit editPath;
	editPath.create(1);
	editPath.move(250,40);
	editPath.size(200,50);
	editPath.setmaxlen(200);
	//editPath.setfocus();
	editPath.setbgcolor(RGB(245,245,245));
	editPath.setcolor(RGB(50,50,50));
	editPath.setfont(15,0,"Ink Free");
	editPath.settext(fileFullPath.c_str());
	editPath.visible(1);
	editPath.setreadonly(1);
	while(1){
		if(mousemsg()){
			mouse=getmouse();
		}
		if(deal.UpdateButton(mouse.x,mouse.y,mouse.is_down())){
			remove(fileFullPath.c_str());
			return 0;
		}
		if(cancel.UpdateButton(mouse.x,mouse.y,mouse.is_down())){
			return 0;
		}
		deal.UpdateButton(mouse.x,mouse.y,0);
		cancel.UpdateButton(mouse.x,mouse.y,0);
		deal.UpdateButton(mouse.x,mouse.y,0);
		cancel.UpdateButton(mouse.x,mouse.y,0);
		deal.UpdateButton(mouse.x,mouse.y,0);
		cancel.UpdateButton(mouse.x,mouse.y,0);
		delay_fps(60);
	}
	getch();
	return 0;
}
