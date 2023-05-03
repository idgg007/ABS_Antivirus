#include<graphics.h>
#include<string>
#define VOID void
class Button {
	private:
		struct Text {
			std::string text;
			color_t wordColor;
			int wordSize;
			int pos_x;
			int pos_y;
		};
		int state = 0;
		int FPS_state;
	public:
		int FPS;
		int speed;
		Text text;
		color_t linesColor;
		color_t filldColor;
		int lenth_x;
		int lenth_y;
		int position_x;
		int position_y;
		VOID DrawButton();
		bool UpdateButton(int, int, bool);
};
VOID Button::DrawButton() {
	setcolor(filldColor);
	setfillcolor(filldColor);
	bar(position_x, position_y, position_x + lenth_x, position_y + lenth_y);
	setcolor(linesColor);
	line(position_x, position_y, position_x + lenth_x, position_y);
	line(position_x, position_y, position_x, position_y + lenth_y);
	line(position_x + lenth_x, position_y, position_x + lenth_x, position_y + lenth_y);
	line(position_x, position_y + lenth_y, position_x + lenth_x, position_y + lenth_y);
	setcolor(text.wordColor);
	setfont(text.wordSize, 0, "UTF-8");
	setbkmode(TRANSPARENT);
	outtextxy(text.pos_x, text.pos_y, text.text.c_str());
}
bool Button::UpdateButton(int mousePosition_x, int mousePosition_y, bool isMouse) {
	if (mousePosition_x < position_x + lenth_x &&
	    mousePosition_x > position_x &&
	    mousePosition_y < position_y + lenth_y &&
	    mousePosition_y > position_y && isMouse) {
		return 1;
	}
	if (mousePosition_x < position_x + lenth_x &&
	    mousePosition_x > position_x &&
	    mousePosition_y < position_y + lenth_y &&
	    mousePosition_y > position_y) {
		if (FPS_state > 0) {
			FPS_state--;
			return 0;
		}
		FPS_state = FPS / speed;
		if (state <= lenth_x + lenth_y) {
			int x1, y1, x2, y2;
			if (state < lenth_y) {
				x1 = position_x;
				y1 = position_y + lenth_y - state;
			} else {
				x1 = position_x + state - lenth_y;
				y1 = position_y;
			}
			if (state < lenth_x) {
				x2 = position_x + state;
				y2 = position_y + lenth_y;
			} else {
				x2 = position_x + lenth_x;
				y2 = position_y - state + lenth_x + lenth_y;
			}
			state++;
			setcolor(linesColor);
			line(x1, y1, x2, y2);
		}
		setcolor(text.wordColor);
		setfont(text.wordSize, 0, "UTF-8");
		setbkmode(TRANSPARENT);
		outtextxy(text.pos_x, text.pos_y, text.text.c_str());
		return 0;
	} else {
		state = 0;
		DrawButton();
		return 0;
	}
}
