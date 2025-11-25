#pragma once
#ifndef SHADERS_H
#define SHADERS_H
#include "Color.h"

//Класс для задания "шейдера"
class Shader {
protected:
	unsigned char alpha;			//Значение прозрачности (альфа-канала) будущей заливки (единственный общий параметр для всех шейдеров)
public:
	Shader(unsigned char alp);		//Конструктор, создающий пустой шейдер с заданной прозрачностью
	unsigned char getAlpha();		//Метод, возвращающий значение альфа-канала шейдера
	void changeAlpha(unsigned char alp);		//Метод, меняющий значение альфа-канала шейдера на новое alp 
	virtual Color GetColor(double x, double y);	//Метод, возвращающий цвет точки (x, y) (переопределяется для каждого шейдера)
};

//Далее каждый подкласс класса SHader будет иметь два метода: конструктор, задающий, помимо прозрачности, все параметры, характерные для этого шейдера, и переопределенный метод Color GetColor(x, y)

//Подкласс для шейдера, осуществляющего однотонную заливку
class Plain : public Shader {
	Color col;						//Цвет, которым осуществляется заливка
public:
	Plain(unsigned char alp, Color c);
	Color GetColor(double x, double y) override;
};

//Подкласс для шейдера, осуществляющего бариметрическую заливку
class Barimetric : public Shader {
	double x1; double y1;		//x- и y-координаты первой вершины треугольника
	double x2; double y2;		//x- и y-координаты второй вершины треугольника
	double x3; double y3;		//x- и y-координаты третьей вершины треугольника
	Color col1;					//Цвет, наиболее интенсивный у первой вершины
	Color col2;					//Цвет, наиболее интенсивный у второй вершины
	Color col3;					//Цвет, наиболее интенсивный у третьей вершины
public:
	Barimetric(unsigned char alp, double xp1, double yp1, double xp2, double yp2, double xp3, double yp3, Color c1, Color c2, Color c3);
	Color GetColor(double x, double y) override;
};

//Подкласс для шейдера, осуществляющего радиальную заливку
class Radial : public Shader {
	double center_x; double center_y;	//x- и y-координаты центра концентрических окружностей
	double half_ring_rad;				//Толщина "полукольца", где цвет проходит от максимально интенсивного цвета №1 к максимально интенсивному цвету №2
	double start_phase;					//Сдвиг по фазе; 0, если в центре максимально интенсивный первый цвет-
	Color col1;							//Цвет №1
	Color col2;							//Цвет №2
public:
	Radial(unsigned char alp, double xc, double yc, double hrr, double sp, Color c1, Color c2);
	Color GetColor(double x, double y) override;
};

//Подкласс для шейдера, осуществляющего секторную (коническую) заливку
class Conical : public Shader {
	double center_x; double center_y;	//x- и y-координаты центра среза HSV-пирамиды
	double rad;							//Радиус среза HSV-пирамиды
	double v_comp;						//Компонента V (высота, на которой берется срез)
	double angle;						//Угол поворота среза; если нулевой, красный цвет будет находиться в правой части
	double stable_s;					//Компонента S (удаление от оси пирамиды); если задана при создании, будет стабильной, если не задана, будет меняться от центра к краю, как на настоящем срезе
public:
	Conical(unsigned char alp, double xc, double yc, double r, double v, double a, double sts = -1);
	Color GetColor(double x, double y) override;
};

//Подкласс для шейдера, осуществляющего шахматную заливку
class Checkerboard : public Shader {
	int cell_size;			//Размер клетки
	double angle;			//Угол наклона осей
	Color col1;				//Цвет №1
	Color col2;				//Цвет №2
public:
	Checkerboard(unsigned char alp, int cs, double a, Color c1, Color c2);
	Color GetColor(double x, double y) override;
};

//Подкласс для шейдера, осуществляющего градиентную заливку
class AllGradient : public Shader {
	double x1; double y1;	//x- и y-координаты первой точки, соединяющей "концы" градиента
	double x2; double y2;	//x- и y-координаты второй точки, соединяющей "концы" градиента
	double v_comp;			//Компонента V (высота, на которой берется срез)
	double angle;		//Угол сдвига градиента; если нулевой, он будет начинаться и заканчиваться красным цветом
	double stable_s;		//Компонента S (удаление от оси пирамиды)
public:
	AllGradient(unsigned char alp, double px1, double py1, double px2, double py2, double v, double a, double sts);
	Color GetColor(double x, double y) override;
};

#endif