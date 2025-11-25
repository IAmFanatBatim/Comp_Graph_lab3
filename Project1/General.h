#pragma once
#ifndef GENERAL_H
#define GENERAL_H
#include "Color.h"

const double M_PI = 3.141592653589793;

//Возвращает большее из двух вещественных значений x, y
double max2(double x, double y);

//Возвращает меньшее из двух вещественных значений x, y
double min2(double x, double y);

//Вспомогательная функция, обменивающая две переменные a, b произвольного типа (с размером type_size) значениями
void swapVoid(void* a, void* b, int type_size);

//Вспомогательная функция, показывающая, как далеко и с какой стороны от прямой, проходящей через точки (x1, y1), (x2, y2), находится точка (x, y)
int getDistanceFromLine(int x, int y, int x1, int y1, int x2, int y2);

//Вспомогательная функция, показывающая, как далеко и с какой стороны от окружности, с центром в точке (0, 0) и радиусом r, находится точка (x, y)
int getDistanceFromCircle(int y, int x, int r);

//Вспомогательная функция, показывающая, как далеко и с какой стороны от прямой, проходящей через точки (x1, y1), (x2, y2), находится точка (x, y)
double getDistanceFromLineDoub(double x, double y, double x1, double y1, double x2, double y2);

//Вспомогательная функция, показывающая, как далеко и с какой стороны от окружности, с центром в точке (0, 0) и радиусом r, находится точка (x, y)
double getDistanceFromCircleDoub(double x, double y, double xc, double yc, double r);

//Вспомогательная функция, возвращающая "истина", если точка (x, y) находится внутри треугольника из точек (x1, y1), (x2, y2), (x3, y3), и "ложь" в противном случае
bool isPointInTriangle(double x, double y, double x1, double y1, double x2, double y2, double x3, double y3);

//Вспомогательная функция, возвращающая "истина", если точка (x, y) находится внутри круга с центром (xc, yc) и радиусом r, и "ложь" в противном случае
bool isPointInCircle(double x, double y, double xc, double yc, double r);

//Вспомогательная функция, перезаписывающая координаты некой точки (x, y) так, что она оказывается повернутой относительно точки (cx, cy) по часовой стрелке на angle радиан
void rotatePoint(double* x, double* y, int cx, int cy, double angle);

//Вспомогательная функция, возвращающая расстояние между точками (x1, y1) и (x2, y2)
double getDistanceBetweenPoints(double x1, double y1, double x2, double y2);

//Вспомогательная функция, записывающая по адресам px, py координаты точки - проекции (x, y) на прямую (x1, y1) - (x2, y2)
void getProjectionPoint(double* px, double* py, double x, double y, double x1, double y1, double x2, double y2);

//Вспомогательная функция, возвращающая структуру Color, хранящую цвет, аналогичный цвету со значениями (h_comp, s_comp, v_comp, alpha) в цветовой модели HSV
Color HSVtoRGB(double h_comp, double s_comp, double v_comp, unsigned char alpha);
#endif
