#pragma once
#ifndef FRAME_CPP
#define FRAME_CPP
#include "Frame.h"
#include <math.h>
#include "General.h"

//Конструктор изображения с заданной шириной и высотой
Frame::Frame(unsigned w, unsigned h) {
	width = w;
	height = h;

	data = new Color[w * h];
	mat = new Color * [h];
	Color white(255, 255, 255, 0);
	for (int i = 0; i < h; i++) {
		mat[i] = data + i * w;
		for (int j = 0; j < w; j++) {
			mat[i][j] = white;
		}
	}
}

//Возвращает цвет, в который окрашен пиксель с координатами x, y
Color Frame::GetPixel(int x, int y) {
	return mat[y][x];
}

//Окрашивает в заданный цвет swatch пиксель с координатами x, y
void Frame::SetPixel(int x, int y, Color swatch) {
	mat[y][x] = swatch;
}

//Накладывает цвет swatch поверх пикселя с координатами x, y (с учетом прозрачности)
void Frame::OverlayColor(int x, int y, Color swatch) {
	Color written = GetPixel(x, y);
	double a = (double) swatch.alpha / 255;
	swatch.red = swatch.red * a + written.red * (1 - a);
	swatch.green = swatch.green * a + written.green * (1 - a);
	swatch.blue = swatch.blue * a + written.blue * (1 - a);
	SetPixel(x, y, swatch);
}

//Возвращает ширину изображения
int Frame::GetWidth() {
	return width;
}
//Возвращает высоту изображения
int Frame::GetHeight() {
	return height;
}

//Рисует на изображении отрезок между точками (x1, y1), (x2, y2), окрашивая его в заданный цвет swatch, используя алгоритм Брезенхейма
void Frame::DrawLine(int x1, int y1, int x2, int y2, Color swatch) {
	//если delta_x < delta_y, то двигаться необходимо по оси y, а не x - меняем оси местами и отмечаем это в флаге was_changed
	bool was_changed = false;
	if ((x1 - x2 < y1 - y2) ^ (x1 - x2 < y2 - y1)) {
		swapVoid(&y1, &x1, sizeof(int));
		swapVoid(&x2, &y2, sizeof(int));
		was_changed = true;
	}
	//если точка (x1, y1) находится правее, чем (x2, y2), меняем точки местами
	if (x1 > x2) {
		swapVoid(&x1, &x2, sizeof(int));
		swapVoid(&y1, &y2, sizeof(int));
	}
	//если точка (x1, y1) находится выше, чем (x2, y2), будем уменьшать, а не увеличивать y на 1
	char y_inc = (y1 < y2) ? 1 : -1;
	//Начальная точка (x1, y1)
	int x = x1;
	int y = y1;
	//Считаем изначальное значение dis_delta - разницу между расстояниями до прямой точек (x, y) и (x, y+1)
	//Поскольку точка (x, y) в самом начале алгоритма и так лежит на прямой, расстояние до нее можно не считать
	int dis_delta = getDistanceFromLine(x, y + y_inc, x1, y1, x2, y2);
	//Выполняем цикл, пока x не сравняется с x2
	while (x <= x2) {
		//Если разница больше 0 (когда y должен возрастать) и меньше 0 (когда y должен убывать), изменяем y на один, с учетом этого корректируем значение dis_delta
		if (dis_delta * y_inc > 0) {
			y += y_inc;
			dis_delta += 2 * (x1 - x2) * y_inc;
		}
		//Если в начале оси были поменяны местами, отзеркаливаем координаты точки, которую нужно закрасить, иначе оставляем без изменения
		if (was_changed) {
			SetPixel(y, x, swatch);
		}
		else {
			SetPixel(x, y, swatch);
		}
		//Приращиваем x на один, с учетом этого корректируем значение dis_delta
		x += 1;
		dis_delta += 2 * (y2 - y1);
	}
}

//Рисует на изображении окружность с  радиусом r с центром в точке точками (xc, yc), окрашивая его в заданный цвет swatch, используя алгоритм Брезенхейма
void Frame::DrawCircle(int xc, int yc, int r, Color swatch) {
	//Начальная точка - верхняя точка окружности, (0, -y) (y с минусом, ведь для изображений начало координат находится в левом верхнем углу)
	int x = 0;
	//Считаем изначальное значение dis_delta - разницу между расстояниями до окружности точек (x, y) и (x, y+1)
	//Поскольку точка (x, y) в самом начале алгоритма и так лежит на окружности, расстояние до нее можно не считать
	int y = r * (-1);
	int dis_delta = getDistanceFromCircle(x, y + 1, r);
	//Выполняем цикл, пока x не станет больше, чем -y (это будет значить, что мы вышли за пределы сектора 1/8 окружности)
	while (x < -y) {
		//Если разница больше 0, приращиваем y, с учетом этого корректируем значение dis_delta
		if (dis_delta > 0) {
			dis_delta += 4 * y + 4;
			y += 1;
		}
		//Закрашиваем точку как в указанных координатах, так и в соответствующих точках в остальных 7 секторах окружности (меняя знаки x и y в разных комбинациях)
		SetPixel(xc + x, yc + y, swatch);
		SetPixel(xc - x, yc + y, swatch);
		SetPixel(xc + x, yc - y, swatch);
		SetPixel(xc - x, yc - y, swatch);

		SetPixel(xc + y, yc + x, swatch);
		SetPixel(xc - y, yc + x, swatch);
		SetPixel(xc + y, yc - x, swatch);
		SetPixel(xc - y, yc - x, swatch);

		//Приращиваем x на один, с учетом этого корректируем значение dis_delta
		dis_delta += 2 * (2 * x + 1);
		x += 1;
	}
}

//Рисует на изображении треугольник с вершинами в точках (x1, y1), (x2, y2), (x3, y3), окрашивая его в заданный цвет swatch
void Frame::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, Color swatch) {
	//Треугольник состоит из трех отрезков, так что просто рисуем ранее написанной функцией DrawLine три отрезка между разными парами точек
	DrawLine(x1, y1, x2, y2, swatch);
	DrawLine(x1, y1, x3, y3, swatch);
	DrawLine(x2, y2, x3, y3, swatch);
}

//Рисует на изображении залитый треугольник с вершинами в точках (x1, y1), (x2, y2), (x3, y3), для определения цвета конкретного пиксел в заливке используя функцию defineCol
void Frame::FillTriangle(double x1, double y1, double x2, double y2, double x3, double y3, Shader& interpolator) {
	//Меняем точки местами так, чтобы по оси y точка один была нижней, точка 3 - верхней
	if (y1 > y2) {
		swapVoid(&x1, &x2, sizeof(double));
		swapVoid(&y1, &y2, sizeof(double));
	}
	if (y2 > y3) {
		swapVoid(&x2, &x3, sizeof(double));
		swapVoid(&y2, &y3, sizeof(double));
	}
	if (y1 > y2) {
		swapVoid(&x1, &x2, sizeof(double));
		swapVoid(&y1, &y2, sizeof(double));
	}

	//Получаем целые номера рядов, на которых находятся пиксели, "представляющие" точки (x1, y1), (x2, y2), (x3, y3)
	int Y1 = (int)(y1 + 0.5);
	int Y2 = (int)(y2 + 0.5);
	int Y3 = (int)(y3 + 0.5);

	//Получаем дробные координаты x для точек, принадлежащих прямым, составляющим верхнюю часть треугольника, и лежащим ровно на середине пикселей центрального ряда
	double xl = (Y2 + 0.5 - y2) * (x3 - x2) / (y3 - y2) + x2;
	double xr = (Y2 + 0.5 - y1) * (x3 - x1) / (y3 - y1) + x1;
	//Вычисляем прибавление к этим координатам в случае увеличения y на 1
	double xl_delta = (x3 - x2) / (y3 - y2);
	double xr_delta = (x3 - x1) / (y3 - y1);
	//Меняем левые и правые координаты местами так, чтобы левая была, как ни странно, левее; то же касается и прибавлений
	int swap_changed = xl > xr;
	if (swap_changed) {
		swapVoid(&xr, &xl, sizeof(double));
		swapVoid(&xr_delta, &xl_delta, sizeof(double));
	}
	//Перебираем координаты y центров рядов пикселей верхней части треугольника
	for (double Y = Y2 + 0.5; Y <= Y3; Y++) {
		//Вычисляем номера по x крайних пикселей
		int XL = int(xl + 0.5);
		int XR = int(xr + 0.5);
		//Если они выходят за рамки изображения, приравниваем их к 0 или ширине изображения, в зависимости от того, находятся ли они слева или справа
		if (XL < 0) XL = 0; if (XL > width) XL = width;
		if (XR < 0) XR = 0; if (XR > width) XR = width;
		//Закрашиваем все пиксели между крайними в этом ряду
		for (int X = XL; X < XR; X++) {
			OverlayColor(X, Y, interpolator.GetColor(X + 0.5, Y));
		}
		//Выполняем приращение координат крайних пикселей для следующего ряда
		xl += xl_delta;
		xr += xr_delta;
	}

	//Получаем дробные координаты x для точек, принадлежащих прямым, составляющим нижнюю часть треугольника, и лежащим ровно на середине пикселей самого нижнего ряда
	xl = (Y1 + 0.5 - y1) * (x2 - x1) / (y2 - y1) + x1;
	xr = (Y1 + 0.5 - y1) * (x3 - x1) / (y3 - y1) + x1;
	//Вычисляем прибавление к этим координатам в случае увеличения y на 1
	xl_delta = (x2 - x1) / (y2 - y1);
	xr_delta = (x3 - x1) / (y3 - y1);
	//Меняем левые и правые координаты местами так, чтобы левая была, как ни странно, левее; то же касается и прибавлений
	swap_changed = xl > xr;
	if (swap_changed) {
		swapVoid(&xr, &xl, sizeof(double));
		swapVoid(&xr_delta, &xl_delta, sizeof(double));
	}
	//Перебираем координаты y центров рядов пикселей нижней части треугольника
	for (double Y = Y1 + 0.5; Y <= Y2; Y++) {
		//Вычисляем номера по x крайних пикселей
		int XL = int(xl + 0.5);
		int XR = int(xr + 0.5);
		//Если они выходят за рамки изображения, приравниваем их к 0 или ширине изображения, в зависимости от того, находятся ли они слева или справа
		if (XL < 0) XL = 0; if (XL > width) XL = width;
		if (XR < 0) XR = 0; if (XR > width) XR = width;
		//Закрашиваем все пиксели между крайними в этом ряду
		for (int X = XL; X < XR; X++) {
			OverlayColor(X, Y, interpolator.GetColor(X + 0.5, Y));
		}
		//Выполняем приращение координат крайних пикселей для следующего ряда
		xl += xl_delta;
		xr += xr_delta;
	}
}

//Рисует на изображении круг с радиусом r с центром в точке точками (xc, yc), для определения цвета конкретного пикселя в заливке используя функцию defineCol
void Frame::FillCircle(double xc, double yc, double r, Shader& interpolator) {
	//Получаем целые номера рядов, на которых находится самая верхняя и самая нижняя точка круга
	int Y1 = (int)(yc - r + 0.5);
	int Y2 = (int)(yc + r + 0.5);
	//Получаем вспомогательное значение для вычисления координат x левых и крайних пикселей в ряду
	double pre_x = r * r - (Y1 + 0.5 - yc) * (Y1 + 0.5 - yc);
	//Перебираем координаты y центров рядов пикселей всего круга
	for (double Y = Y1 + 0.5; Y <= Y2; Y++) {
		//Вычисляем номера по x крайних пикселей
		int XL = (int)(-sqrt(pre_x) + xc + 0.5);
		int XR = (int)(sqrt(pre_x) + xc + 0.5);
		//Если они выходят за рамки изображения, приравниваем их к 0 или ширине изображения, в зависимости от того, находятся ли они слева или справа
		if (XL < 0) XL = 0; if (XL > width) XL = width;
		if (XR < 0) XR = 0; if (XR > width) XR = width;
		//Закрашиваем все пиксели между крайними в этом ряду
		for (int X = XL; X < XR; X++) {
			OverlayColor(X, Y, interpolator.GetColor(X+0.5, Y));
		}
		//Выполняем приращение вспомогательного значения для следующего ряда
		pre_x += 2 * yc - 2 * Y - 1;
	}
}

#endif