#pragma once
#pragma once
#ifndef LETTERS_H
#define LETTERS_H
#include "Frame.h"
#include "General.h"
#include "Matrix.h"
#include "ComplexShaders.h"
#include <math.h>
#include <stdio.h>

//Класс для задания буквы в имени, состоящей из линий, которую нужно нариосвать согласно варианту
class Letter {
protected:
	Point p1, p2;			//Опорные точки
public:
	void changeCoord(double x, double y);
	void changeRad(double kx, double ky);
	void rotateTo(double a);
	virtual int drawIt(Frame* sheet) = 0;
};

//Класс для задания буквы в имени, состоящей из линий, которую нужно нариосвать согласно варианту
class LetterC : public Letter {
public:
	LetterC();
	void Rearrange(double s, double x, double y, double a);
	int drawIt(Frame* sheet) override;
};

//Класс для задания буквы в имени, состоящей из линий, которую нужно нариосвать согласно варианту
class LetterF : public Letter {
public:
	LetterF();
	void Rearrange(double s, double x, double y, double a);
	int drawIt(Frame* sheet) override;
};

//Изменение расположения фигуры
void Letter::changeCoord(double dx, double dy) {
	Affine trans;
	trans.addTransfer(dx, dy);
	p1 = trans * p1;
	p2 = trans * p2;
}

//Изменение размера фигуры
void Letter::changeRad(double kx, double ky) {
	double x = (p1.getX() + p2.getX()) / 2;
	double y = (p1.getY() + p2.getY()) / 2;
	changeCoord(-x, -y);
	Affine sc;
	sc.addScaling(kx, ky);
	p1 = sc * p1;
	p2 = sc * p2;
	changeCoord(x, y);
}

//Поворот лучей на a радиан по часовой стрелке
void Letter::rotateTo(double a) {
	double x = (p1.getX() + p2.getX()) / 2;
	double y = (p1.getY() + p2.getY()) / 2;
	changeCoord(-x, -y);
	Affine rot;
	rot.addRotation(a);
	p1 = rot * p1;
	p2 = rot * p2;
	changeCoord(x, y);
}

//Конструктор фигру с произвольными параметрами
LetterC::LetterC() {
	p1 = Point();
	p2 = Point();
}

//Конструктор фигру с произвольными параметрами
void LetterC::Rearrange(double s, double x, double y, double a) {
	p1 = Point(x, y + s / 4);
	p2 = Point(x, y - s / 4);
	if (a != 0) {
		rotateTo(a);
	}
}

//Рисует фигуру на изоюражении, адрес которого передан как аргумент
int LetterC::drawIt(Frame* sheet) {
	double help_ll = getDistanceBetweenPoints(p1.getX(), p1.getY(), p2.getX(), p2.getY());

	//Пропускает этап рисования, если радиус слишком мал, чтобы вместить все детали, и это может вызвать ошибку
	if (help_ll < 2) {
		return 1;
	}

	ShaderC2 sh(255, p1.getX(), p1.getY(), p2.getX(), p2.getY(), Color(1, 254, 234), 4);

	double alpha = acos((p2.getX() - p1.getX()) / help_ll);
	if (p1.getY() < p2.getY()) {
		alpha += 2 * (M_PI - alpha);
	}
	Affine rotbord;
	rotbord.addRotation(alpha - M_PI / 2);

	p1 = rotbord * p1;
	p2 = rotbord * p2;

	Point bord1(min2(p1.getX(), p2.getX()) - help_ll, min2(p1.getY(), p2.getY()) - help_ll);
	Point bord2(min2(p1.getX(), p2.getX()) - help_ll, max2(p1.getY(), p2.getY()) + help_ll);
	Point bord3(max2(p1.getX(), p2.getX()) + help_ll, max2(p1.getY(), p2.getY()) + help_ll);
	Point bord4(max2(p1.getX(), p2.getX()) + help_ll, min2(p1.getY(), p2.getY()) - help_ll);

	rotbord.addRotation(-(alpha - M_PI / 2));
	bord1 = rotbord * bord1;
	bord2 = rotbord * bord2;
	bord3 = rotbord * bord3;
	bord4 = rotbord * bord4;
	p1 = rotbord * p1;
	p2 = rotbord * p2;

	sheet->FillTriangle(bord1.getX(), bord1.getY(), bord2.getX(), bord2.getY(), bord3.getX(), bord3.getY(), sh);
	sheet->FillTriangle(bord1.getX(), bord1.getY(), bord4.getX(), bord4.getY(), bord3.getX(), bord3.getY(), sh);
}

LetterF::LetterF() {
	p1 = Point();
	p2 = Point();
}

//Конструктор фигру с произвольными параметрами
void LetterF::Rearrange(double s, double x, double y, double a) {
	p1 = Point(x - s / 10, y + s / 10);
	p2 = Point(x + s / 10, y + s / 10);
	if (a != 0) {
		rotateTo(a);
	}
}

//Рисует фигуру на изоюражении, адрес которого передан как аргумент
int LetterF::drawIt(Frame* sheet) {
	double help_l = getDistanceBetweenPoints(p1.getX(), p1.getY(), p2.getX(), p2.getY());

	//Пропускает этап рисования, если радиус слишком мал, чтобы вместить все детали, и это может вызвать ошибку
	if (help_l < 2) {
		return 1;
	}

	AllGradient stars(255, min2(p1.getX(), p2.getX()) - 2 * help_l, min2(p1.getY(), p2.getY()) - 3 * help_l,
		max2(p1.getX(), p2.getX()) + 2 * help_l, max2(p1.getY(), p2.getY()) + 3 * help_l,
		1, 0, 0.75);

	ShaderF2 sh(255, p1.getX(), p1.getY(), p2.getX(), p2.getY(), Color(105, 79, 176), help_l/2, &stars);

	double alpha = acos((p2.getX() - p1.getX()) / help_l);
	if (p1.getY() < p2.getY()) {
		alpha += 2 * (M_PI - alpha);
	}
	Affine rotbord;
	rotbord.addRotation(alpha);

	p1 = rotbord * p1;
	p2 = rotbord * p2;

	Point bord1(min2(p1.getX(), p2.getX()) - 2 * help_l, min2(p1.getY(), p2.getY()) - 3 * help_l);
	Point bord2(min2(p1.getX(), p2.getX()) - 2 * help_l, max2(p1.getY(), p2.getY()) + 3 * help_l);
	Point bord3(max2(p1.getX(), p2.getX()) + 2 * help_l, max2(p1.getY(), p2.getY()) + 3 * help_l);
	Point bord4(max2(p1.getX(), p2.getX()) + 2 * help_l, min2(p1.getY(), p2.getY()) - 3 * help_l);

	rotbord.addRotation(-alpha);
	bord1 = rotbord * bord1;
	bord2 = rotbord * bord2;
	bord3 = rotbord * bord3;
	bord4 = rotbord * bord4;
	p1 = rotbord * p1;
	p2 = rotbord * p2;

	sheet->FillTriangle(bord1.getX(), bord1.getY(), bord2.getX(), bord2.getY(), bord3.getX(), bord3.getY(), sh);
	sheet->FillTriangle(bord1.getX(), bord1.getY(), bord4.getX(), bord4.getY(), bord3.getX(), bord3.getY(), sh);
}

#endif LETTERS_H
#pragma once
