#pragma once
#ifndef FRAME_H
#define FRAME_H
#include "Shaders.h"

//Класс для задания изображения (кадра)
class Frame
{
	unsigned width;		//Ширина в пикселях
	unsigned height;	//Высота в пикселях
	Color** mat;		//Массив указателей на горизонтальные ряды пикселей
	Color* data;		//Массив, хранящий информацию о каждом пикселе изображения
public:
	Frame(unsigned w, unsigned h, Color back);
	Color GetPixel(int x, int y);
	void SetPixel(int x, int y, Color swatch);
	void OverlayColor(int x, int y, Color swatch);

	int GetWidth();
	int GetHeight();

	void DrawLine(int x1, int y1, int x2, int y2, Color swatch);
	void DrawCircle(int xc, int yc, int r, Color swatch);
	void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, Color swatch);

	void FillTriangle(double x1, double y1, double x2, double y2, double x3, double y3, Shader& interpolator);
	void FillCircle(double xc, double yc, double r, Shader& interpolator);
};

#endif FRAME_H
