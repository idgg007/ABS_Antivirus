#ifndef BADSWITCH_H
#define BADSWITCH_H
#include<graphics.h>
class Switch {
	private:
		color_t lineColor;		//边缘的颜色
		color_t openColor;		//开启时颜色
		color_t closeColor;		//关闭时颜色
		color_t RodColor;		//操纵杆颜色
		int positionX, positionY;
		int SizeX, SizeY;
		bool State;
	public:
		void SetColor(color_t lineC, color_t openC, color_t closeC, color_t RodC);
		void SetSize(int sizeX, int sizeY);
		void Move(int x, int y);
		bool Show(int x, int y, bool isDown);
		void SetState(bool state);
};
void Switch::SetState(bool state){
	State=state;
}
bool Switch::Show(int x, int y, bool isDown) {
	if (x > positionX && x < positionX + SizeX &&
	    y > positionY && y < positionY + SizeY&&isDown) {
		State=!State;
	}
	if(State){
		setcolor(openColor);
		setfillcolor(openColor);
	}else{
		setcolor(closeColor);
		setfillcolor(closeColor);
	}
	bar(positionX,positionY,positionX+SizeX,positionY+SizeY);
	setcolor(lineColor);
	setfillcolor(lineColor);
	line(positionX,positionY,positionX,positionY+SizeY);
	line(positionX,positionY+SizeY,positionX+SizeX,positionY+SizeY);
	line(positionX+SizeX,positionY+SizeY,positionX+SizeX,positionY);
	line(positionX+SizeX,positionY,positionX,positionY);
	if(State){
		setcolor(RodColor);
		setfillcolor(RodColor);
		bar(positionX+SizeX-SizeY+2,positionY+2,positionX+SizeX-1,positionY+SizeY-1);
	}else{
		setcolor(RodColor);
		setfillcolor(RodColor);
		bar(positionX+2,positionY+2,positionX+SizeY-1,positionY+SizeY-1);
	}
	return State;
}
void Switch::Move(int x, int y) {
	State=0;
	positionX = x;
	positionY = y;
}
void Switch::SetSize(int sizeX, int sizeY) {
	SizeX = sizeX;
	SizeY = sizeY;
}
void Switch::SetColor(color_t lineC, color_t openC, color_t closeC, color_t RodC) {
	lineColor = lineC;
	openColor = openC;
	closeColor = closeC;
	RodColor = RodC;
}

#endif
