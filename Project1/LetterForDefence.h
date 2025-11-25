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
	Point* p1, * p2;			//Опорные точки
public:
	void changeCoord(double x, double y);
	void changeRad(double kx, double ky);
	void rotateTo(double a);
	virtual int drawIt(Frame* sheet) = 0;
};

//Класс для задания буквы в имени, состоящей из линий, которую нужно нариосвать согласно варианту
class LetterC : public Letter {
public:
	LetterC(double s, double x, double y, double a);
	int drawIt(Frame* sheet) override;
};

//Класс для задания буквы в имени, состоящей из линий, которую нужно нариосвать согласно варианту
class LetterF : public Letter {
public:
	LetterF(double s, double x, double y, double a);
	int drawIt(Frame* sheet) override;
};

//Изменение расположения фигуры
void Letter::changeCoord(double dx, double dy) {
	Affine rot;
	rot.addTransfer(dx, dy);
	*p1 = rot * *p1;
	*p2 = rot * *p2;
}

//Изменение размера фигуры
void Letter::changeRad(double kx, double ky) {
	Affine rot;
	rot.addScaling(kx, ky);
	*p1 = rot * *p1;
	*p2 = rot * *p2;
}

//Поворот лучей на a радиан по часовой стрелке
void Letter::rotateTo(double a) {
	Affine rot;
	rot.addRotation(a);
	*p1 = rot * *p1;
	*p2 = rot * *p2;
}

//Конструктор фигру с произвольными параметрами
LetterC::LetterC(double s, double x, double y, double a) {
	p1 = new Point(x, y + s / 4);
	p2 = new Point(x, y - s / 4);
	if (a != 0) {
		rotateTo(a);
	}
}

//Рисует фигуру на изоюражении, адрес которого передан как аргумент
int LetterC::drawIt(Frame* sheet) {
	double help_ll = getDistanceBetweenPoints(p1->getX(), p1->getY(), p2->getX(), p2->getY());

	//Пропускает этап рисования, если радиус слишком мал, чтобы вместить все детали, и это может вызвать ошибку
	if (help_ll < 2) {
		return 1;
	}

	ShaderC2 sh(255, p1, p2);

	sheet->FillTriangle(
		min2(p1->getX(), p2->getX()) - help_ll, min2(p1->getY(), p2->getY()) - help_ll,
		min2(p1->getX(), p2->getX()) - help_ll, max2(p1->getY(), p2->getY()) + help_ll,
		max2(p1->getX(), p2->getX()) + help_ll, max2(p1->getY(), p2->getY()) + help_ll, sh);
	sheet->FillTriangle(
		min2(p1->getX(), p2->getX()) - help_ll, min2(p1->getY(), p2->getY()) - help_ll,
		max2(p1->getX(), p2->getX()) + help_ll, min2(p1->getY(), p2->getY()) - help_ll,
		max2(p1->getX(), p2->getX()) + help_ll, max2(p1->getY(), p2->getY()) + help_ll, sh);
	return 0;
}

//Конструктор фигру с произвольными параметрами
LetterF::LetterF(double s, double x, double y, double a) {
	p1 = new Point(x - s / 10, y + s / 10);
	p2 = new Point(x + s / 10, y + s / 10);
	if (a != 0) {
		rotateTo(a);
	}
}

//Рисует фигуру на изоюражении, адрес которого передан как аргумент
int LetterF::drawIt(Frame* sheet) {
	double help_l = getDistanceBetweenPoints(p1->getX(), p1->getY(), p2->getX(), p2->getY());

	//Пропускает этап рисования, если радиус слишком мал, чтобы вместить все детали, и это может вызвать ошибку
	if (help_l < 2) {
		return 1;
	}

	ShaderF1 sh(255, p1, p2);

	sheet->FillTriangle(
		min2(p1->getX(), p2->getX()) - 2 * help_l, min2(p1->getY(), p2->getY()) - 3 * help_l,
		min2(p1->getX(), p2->getX()) - 2 * help_l, max2(p1->getY(), p2->getY()) + 3 * help_l,
		max2(p1->getX(), p2->getX()) + 2 * help_l, max2(p1->getY(), p2->getY()) + 3 * help_l, sh);
	sheet->FillTriangle(
		min2(p1->getX(), p2->getX()) - 2 * help_l, min2(p1->getY(), p2->getY()) - 3 * help_l,
		max2(p1->getX(), p2->getX()) + 2 * help_l, min2(p1->getY(), p2->getY()) - 3 * help_l,
		max2(p1->getX(), p2->getX()) + 2 * help_l, max2(p1->getY(), p2->getY()) + 3 * help_l, sh);
	return 0;
}

#endif LETTERS_H
#pragma once
