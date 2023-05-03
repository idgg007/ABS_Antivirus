#include<graphics.h>
using namespace std;
class ProgressBar {
	private:
		struct ColorShow {
			int R, G, B;
		};
		ColorShow barTopColor, barBottomColor;
		color_t lineColor, bkColor;
		int lineX1 = 0, lineY1 = 0, lineX2 = 0, lineY2 = 0, lineLenthX, lineLenthY;
		int BarPositionX = 0, BarPositionY = 0, barLenth, barWide;
	public:
		void SetSize(int lenthX, int lenthY);
		void SetPosition(int positionX, int positionY);
		void SetBar(int _barLenth, int _barWide);
		void SetColor(color_t _lineColor, color_t _bkColor, int TR, int TG, int TB, int BR, int BG, int BB);
		void Show(double progress);
};
void ProgressBar::SetSize(int lenthX, int lenthY) {
	lineLenthX = lenthX;
	lineLenthY = lenthY;
}
void ProgressBar::SetPosition(int positionX, int positionY) {
	lineX1 = positionX;
	lineY1 = positionY;
}
void ProgressBar::SetBar(int _barLenth, int _barWide) {
	barLenth = _barLenth;
	barWide = _barWide;
}
void ProgressBar::SetColor(color_t _lineColor, color_t _bkColor, int TR, int TG, int TB, int BR, int BG, int BB) {
	lineColor = _lineColor;
	bkColor = _bkColor;
	barTopColor = {TR, TG, TB};
	barBottomColor = {BR, BG, BB};
}
void ProgressBar::Show(double progress) {
	lineX2 = lineX1 + lineLenthX;
	lineY2 = lineY1 + lineLenthY;
	BarPositionX = lineX1 + lineLenthX / 2 - barLenth / 2;
	BarPositionY = lineY1 + lineLenthY / 2 - barWide / 2;
	setcolor(bkColor);
	setfillcolor(bkColor);
	bar(lineX1, lineY1, lineX2, lineY2);
	setcolor(lineColor);
	setfillcolor(lineColor);
	line(lineX1, lineY1, lineX1, lineY2);
	line(lineX1, lineY1, lineX2, lineY1);
	line(lineX2, lineY1, lineX2, lineY2);
	line(lineX1, lineY2, lineX2, lineY2);
	int lenth = int(barLenth * progress);
	for (int i = BarPositionY; i <= BarPositionY + barWide; i++) {
		setcolor(RGB(barTopColor.R + (barBottomColor.R - barTopColor.R) / barWide * (i - BarPositionY),
		             barTopColor.G + (barBottomColor.G - barTopColor.G) / barWide * (i - BarPositionY),
		             barTopColor.B + (barBottomColor.B - barTopColor.B) / barWide * (i - BarPositionY)));
		line(BarPositionX, i, lenth + BarPositionX, i);
	}
}
