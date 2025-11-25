#pragma once
#ifndef SHADERS_CPP
#define SHADERS_CPP
#include "Shaders.h"
#include "General.h"
#include <math.h>

Shader::Shader(unsigned char alp) {
	alpha = alp;
}

unsigned char Shader::getAlpha() {
	return alpha;
}

void Shader::changeAlpha(unsigned char alp) {
	alpha += alp;
}

Color Shader::GetColor(double x, double y) {
	return Color(255, 255, 255, alpha);
}

Plain::Plain(unsigned char alp, Color c) : Shader(alp) {
	col = c;
}

Color Plain::GetColor(double x, double y) {
	return col;
}

Barimetric::Barimetric(unsigned char alp, double xp1, double yp1, double xp2, double yp2, double xp3, double yp3, Color c1, Color c2, Color c3) : Shader(alp) {
	x1 = xp1;
	y1 = yp1;
	x2 = xp2;
	y2 = yp2;
	x3 = xp3;
	y3 = yp3;
	col1 = c1;
	col2 = c2;
	col3 = c3;
	//Меняем точки местами так, чтобы по оси y точка один была нижней, точка 3 - верхней
	if (y2 > y1) {
		swapVoid(&x1, &x2, sizeof(double));
		swapVoid(&y1, &y2, sizeof(double));
		swapVoid(&col1, &col2, sizeof(Color));
	}
	if (y3 > y2) {
		swapVoid(&x2, &x3, sizeof(double));
		swapVoid(&y2, &y3, sizeof(double));
		swapVoid(&col2, &col3, sizeof(Color));
	}
	if (y2 > y1) {
		swapVoid(&x1, &x2, sizeof(double));
		swapVoid(&y1, &y2, sizeof(double));
		swapVoid(&col1, &col2, sizeof(Color));
	}
}

Color Barimetric::GetColor(double x, double y) {
	//Вычисляем площади треугольников, которые формирует точка (x, y) со стороной напротив точки (x1, y1), (x2, y2), (x3, y3)
	double s1 = (y2 - y3)*(x - x3) + (x3 - x2)*(y - y3);
	double s2 = (y3 - y1) * (x - x3) + (x1 - x3) * (y - y3);
	double s3 = (y1 - y2) * (x - x2) + (x2 - x1) * (y - y2);
	//Вычисляем площадь треугольника, образовнаного точками (x1, y1), (x2, y2), (x3, y3)
	double s = (y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3);
	//Компоненты результирующего цвета — сумма компонент исходных цветов, но каждая умножена на отношение площади треугольника, образованного точкой и противолежащей "своей" вершине стороной, к исходному треугольнику
	char result_red = (col1.red * s1 + col2.red * s2 + col3.red * s3) / s;
	char result_green = (col1.green * s1 + col2.green * s2 + col3.green * s3) / s;
	char result_blue = (col1.blue * s1 + col2.blue * s2 + col3.blue * s3) / s;
	return Color(result_red, result_green, result_blue, alpha);
}

Radial::Radial(unsigned char alp, double xc, double yc, double hrr, double sp, Color c1, Color c2) : Shader(alp) {
	center_x = xc;
	center_y = yc;
	half_ring_rad = hrr;
	start_phase = sp;
	col1 = c1;
	col2 = c2;
}

Color Radial::GetColor(double x, double y) {
	//Вычисляется расстояние от точки до центра окружностей
	double distance = getDistanceBetweenPoints(center_x, center_y, x, y);
	//Мы получаем остаток от деления этого числа на расстояние между двумя пиками одного цвета
	double cur_iter_distance = fmod(distance + start_phase * 2 * half_ring_rad, 2 * half_ring_rad);
	double coeff;
	//На основании чего выводится коэффициент... 
	if (cur_iter_distance > half_ring_rad) {
		coeff = (half_ring_rad - fmod(cur_iter_distance, half_ring_rad)) / half_ring_rad;
	} else {
		coeff = cur_iter_distance / half_ring_rad;
	}
	//...на который умножается в итоге каждая компонента одного цвета; вторая умножается на число 1 - coeff
	char result_red = (col1.red * coeff + col2.red * (1-coeff));
	char result_green = (col1.green * coeff + col2.green * (1-coeff));
	char result_blue = (col1.blue * coeff + col2.blue * (1-coeff));

	return Color(result_red, result_green, result_blue, alpha);
}

Conical::Conical(unsigned char alp, double xc, double yc, double r, double v, double a, double sts) : Shader(alp) {
	center_x = xc;
	center_y = yc;
	rad = r;
	v_comp = v;
	angle = a;
	stable_s = sts;
}

Color Conical::GetColor(double x, double y) {
	//Находится расстрояние от центра сектора до точки
	double cur_rad = getDistanceBetweenPoints(center_x, center_y, x, y);
	double s_comp;
	//На его оснвое вычисляется S-компонента, если она не была задана как стабильная
	if (stable_s == -1) {
		s_comp = cur_rad / rad;
	} else {
		s_comp = stable_s;
	}
	//По соотношению разниц x и y координат находится тангенс угла между точкой, центром, горизональной осью
	double h_comp = atan2((y - center_y), (x - center_x));
	//А потом вычисляется угол, с учетом того, в какой тетверти находится точка, и нужно ли повернуть сектор при заливке
	if (h_comp < 0) {
		h_comp += 2 * M_PI;
	}
	h_comp = fmod(h_comp * 180 / M_PI + angle, 360);
	//Когда все компоненты получены, можно перевести цвет из модели HSV в модель RGB
	return HSVtoRGB(h_comp, s_comp, v_comp, alpha);
}

Checkerboard::Checkerboard(unsigned char alp, int cs, double a, Color c1, Color c2) : Shader(alp) {
	cell_size = cs;
	angle = a;
	col1 = c1;
	col2 = c2;
	col1.alpha = alpha;
	col2.alpha = alpha;
}

Color Checkerboard::GetColor(double x, double y) {
	//Угол поворота сетки переводится в радианы
	double angle_in_rads = angle * M_PI / 180;
	//Точка поворачивается на этот угол относительноначала координат
	rotatePoint(&x, &y, 0, 0, angle_in_rads);
	//Вычисляется, в каком квадрате она лежит, и выбирается один из двух цветов
	if ((fabs(fmod(x, 2 * cell_size)) * x > cell_size * x) ^ (fabs(fmod(y, 2 * cell_size)) * y > cell_size * y)) {
		return col1;
	}
	else {
		return col2;
	}
}

AllGradient::AllGradient(unsigned char alp, double px1, double py1, double px2, double py2, double v, double a, double sts) : Shader(alp) {
	x1 = px1;
	y1 = py1;
	x2 = px2;
	y2 = py2;
	v_comp = v;
	angle = a;
	stable_s = sts;
}

Color AllGradient::GetColor(double x, double y) {
	//Находим проекцию точки на отрезок, соединяющую "концы" градиента
	double projection_x, projection_y;
	getProjectionPoint(&projection_x, &projection_y, x, y, x1, y1, x2, y2);
	//По тому, насколько далеко от начала отрезка находится проекция, вычисляем компоненту тона H 
	double dist = getDistanceBetweenPoints(projection_x, projection_y, x1, y1);
	double full_dist = getDistanceBetweenPoints(x2, y2, x1, y1);
	double h_comp = fmod(dist / full_dist * 360 + angle, 360);
	//Когда все компоненты получены, можно перевести цвет из модели HSV в модель RGB
	return HSVtoRGB(h_comp, stable_s, v_comp, alpha);
}

#endif