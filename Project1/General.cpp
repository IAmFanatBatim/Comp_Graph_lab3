#pragma once
#ifndef GENERAL_CPP
#define GENERAL_CPP
#include "General.h"
#include <malloc.h>
#include <windows.h>
#include <math.h>

//Возвращает большее из двух вещественных значений x, y
double max2(double x, double y) {
	return (x > y) ? x : y;
}

//Возвращает меньшее из двух вещественных значений x, y
double min2(double x, double y) {
	return (x < y) ? x : y;
}

//Вспомогательная функция, обменивающая две переменные a, b произвольного типа (с размером type_size) значениями
void swapVoid(void* a, void* b, int type_size) {
	void* temp = malloc(type_size);
	memcpy(temp, a, type_size);
	memcpy(a, b, type_size);
	memcpy(b, temp, type_size);
}

//Вспомогательная функция, показывающая, как далеко и с какой стороны от прямой, проходящей через точки (x1, y1), (x2, y2), находится точка (x, y)
int getDistanceFromLine(int x, int y, int x1, int y1, int x2, int y2) {
	return (x - x1) * (y2 - y1) - (y - y1) * (x2 - x1);
}

//Вспомогательная функция, показывающая, как далеко и с какой стороны от окружности, с центром в точке (0, 0) и радиусом r, находится точка (x, y)
int getDistanceFromCircle(int y, int x, int r) {
	return x * x + y * y - r * r;
}

//Вспомогательная функция, показывающая, как далеко, проходящей через точки (x1, y1), (x2, y2), находится точка (x, y)
double getDistanceFromLineDoub(double x, double y, double x1, double y1, double x2, double y2) {
	return fabs((x - x1) * (y2 - y1) - (y - y1) * (x2 - x1));
}

//Вспомогательная функция, показывающая, как далеко и с какой стороны от окружности, с центром в точке (0, 0) и радиусом r, находится точка (x, y)
double getDistanceFromCircleDoub(double x, double y, double xc, double yc, double r) {
	return fabs(sqrt((x - xc) * (x - xc) + (y - yc) * (y - yc)) - r);
}

//Вспомогательная функция, возвращающая "истина", если точка (x, y) находится внутри треугольника из точек (x1, y1), (x2, y2), (x3, y3), и "ложь" в противном случае
bool isPointInTriangle(double x, double y, double x1, double y1, double x2, double y2, double x3, double y3) {
	if (y2 > y1) {
		swapVoid(&x1, &x2, sizeof(double));
		swapVoid(&y1, &y2, sizeof(double));
	}
	if (y3 > y2) {
		swapVoid(&x2, &x3, sizeof(double));
		swapVoid(&y2, &y3, sizeof(double));
	}
	if (y2 > y1) {
		swapVoid(&x1, &x2, sizeof(double));
		swapVoid(&y1, &y2, sizeof(double));
	}
	double s1 = (y2 - y3) * (x - x3) + (x3 - x2) * (y - y3);
	double s2 = (y3 - y1) * (x - x3) + (x1 - x3) * (y - y3);
	double s3 = (y1 - y2) * (x - x2) + (x2 - x1) * (y - y2);
	double s = (y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3);
	return s1 / s >= 0 && s2 / s >= 0 && s3 / s >= 0;
}

//Вспомогательная функция, возвращающая "истина", если точка (x, y) находится внутри круга с центром (xc, yc) и радиусом r, и "ложь" в противном случае
bool isPointInCircle(double x, double y, double xc, double yc, double r) {
	return (x - xc) * (x - xc) + (y - yc) * (y - yc) <= r * r;
}

//Вспомогательная функция, перезаписывающая координаты некой точки (x, y) так, что она оказывается повернутой относительно точки (cx, cy) по часовой стрелке на angle радиан
void rotatePoint(double* x, double* y, int cx, int cy, double angle) {
	double new_x = (*(x)-cx) * cos(angle) - (*(y)-cy) * sin(angle) + cx;
	double new_y = (*(x)-cx) * sin(angle) + (*(y)-cy) * cos(angle) + cy;
	*x = new_x;
	*y = new_y;
}

//Вспомогательная функция, возвращающая расстояние между точками (x1, y1) и (x2, y2)
double getDistanceBetweenPoints(double x1, double y1, double x2, double y2) {
	return sqrt((y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1));
}

//Вспомогательная функция, записывающая по адресам px, py координаты точки - проекции (x, y) на прямую (x1, y1) - (x2, y2)
void getProjectionPoint(double* px, double* py, double x, double y, double x1, double y1, double x2, double y2) {
	double normal_x = x2 - x1;
	double normal_y = y2 - y1;
	double lambda_num = normal_x * (x1 - x) + normal_y * (y1 - y);
	double lambda_den = normal_x * normal_x + normal_y * normal_y;
	double lambda = -lambda_num / lambda_den;
	*px = x1 + normal_x * lambda;
	*py = y1 + normal_y * lambda;
}

//Вспомогательная функция, возвращающая структуру Color, хранящую цвет, аналогичный цвету со значениями (h_comp, s_comp, v_comp, alpha) в цветовой модели HSV
Color HSVtoRGB(double h_comp, double s_comp, double v_comp, unsigned char alpha) {
	int h_streak = int(h_comp / 60) % 6;
	double f = h_comp / 60 - (int)(h_comp / 60);

	v_comp = v_comp * 255;
	int v = (int)(v_comp);
	int p = (int)(v_comp * (1 - s_comp));
	int q = (int)(v_comp * (1 - f * s_comp));
	int t = (int)(v_comp * (1 - (1 - f) * s_comp));

	if (h_streak == 0) {
		return Color(v, t, p, alpha);
	}
	else if (h_streak == 1) {
		return Color(q, v, p, alpha);
	}
	else if (h_streak == 2) {
		return Color(p, v, t, alpha);
	}
	else if (h_streak == 3) {
		return Color(p, q, v, alpha);
	}
	else if (h_streak == 4) {
		return Color(t, p, v, alpha);
	}
	else {
		return Color(v, p, q, alpha);
	}
}

#endif