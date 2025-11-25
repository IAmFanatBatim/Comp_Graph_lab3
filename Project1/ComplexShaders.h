#pragma once
#ifndef COMPLEXSHADERS_H
#define COMPLEXSHADERS_H
#include "Frame.h"
#include "General.h"
#include "Matrix.h"
#include <stdio.h>
#include <math.h>

class ShaderC1 : public Shader {
	Point* p1, * p2;
public:
	ShaderC1(unsigned char alp, Point* point1, Point* point2);
	Color GetColor(double x, double y) override;
};

ShaderC1::ShaderC1(unsigned char alp, Point* point1, Point* point2) : Shader(alp) {
	p1 = new Point(point1->getX(), point1->getY());
	p2 = new Point(point2->getX(), point2->getY());
}

Color ShaderC1::GetColor(double x, double y) {
	double help_l = getDistanceBetweenPoints(p1->getX(), p1->getY(), p2->getX(), p2->getY()) / 2;

	Point* np1 = new Point(0, 0);
	Point* np2 = new Point(0, -2 * help_l);

	Affine trans;
	trans.addTransfer(-p1->getX(), -p1->getY());

	double alpha = acos(p2->getX() - p1->getX());
	if (p1->getY() < p2->getY()) {
		alpha += 2 * (M_PI - alpha);
	}
	Affine rot;
	rot.addRotation(M_PI / 2 - alpha);
	Point* p = new Point(x, y);
	*p = rot * trans * *p;

	double dist_from_p1 = getDistanceBetweenPoints(p->getX(), p->getY(), np1->getX(), np1->getY());
	double dist_from_p2 = getDistanceBetweenPoints(p->getX(), p->getY(), np2->getX(), np2->getY());

	if (dist_from_p1 > help_l / 2 && dist_from_p1 < help_l * 3 / 2 && p->getY() >= 0) {
		return Color(255, 255, 0);
	}
	else if (dist_from_p2 > help_l / 2 && dist_from_p2 < help_l * 3 / 2 && p->getY() <= -help_l * 2) {
		return Color(0, 255, 255);
	}
	else if (p->getY() < 0 && p->getY() > -help_l * 2 && p->getX() < -help_l / 2 && p->getX() > -help_l * 3 / 2) {
		return Color(255, 0, 255);
	}
	else {
		return Color(120, 25, 68);
	}
}

class ShaderF1 : public Shader {
	Point* p1, * p2;
public:
	ShaderF1(unsigned char alp, Point* point1, Point* point2);
	Color GetColor(double x, double y) override;
};

ShaderF1::ShaderF1(unsigned char alp, Point* point1, Point* point2) : Shader(alp) {
	p1 = new Point(point1->getX(), point1->getY());
	p2 = new Point(point2->getX(), point2->getY());
}

Color ShaderF1::GetColor(double x, double y) {
	double help_l = getDistanceBetweenPoints(p1->getX(), p1->getY(), p2->getX(), p2->getY());
	Point* np1 = new Point(0, 0);
	Point* np2 = new Point(help_l, 0);

	Affine trans;
	trans.addTransfer(-p1->getX(), -p1->getY());

	double alpha = acos(p2->getX() - p1->getX());
	if (p1->getY() < p2->getY()) {
		alpha += 2 * (M_PI - alpha);
	}
	Affine rot;
	rot.addRotation(-alpha);
	Point* p = new Point(x, y);
	*p = trans * *p;

	double dist_from_p1 = getDistanceBetweenPoints(p->getX(), p->getY(), np1->getX(), np1->getY());
	double dist_from_p2 = getDistanceBetweenPoints(p->getX(), p->getY(), np2->getX(), np2->getY());

	if (dist_from_p1 > help_l / 2 && dist_from_p1 < help_l * 3 / 2 && p->getX() <= 0) {
		return Color(255, 255, 0);
	}
	else if (dist_from_p2 > help_l / 2 && dist_from_p2 < help_l * 3 / 2 && p->getX() >= help_l) {
		return Color(0, 255, 255);
	}
	else if (p->getY() > -help_l * 2 && p->getY() < help_l * 3 && p->getX() < help_l && p->getX() > 0) {
		return Color(255, 0, 255);
	}
	else {
		return Color(120, 25, 68);
	}
}


class ShaderC2 : public Shader {
	Point* p1, * p2;
public:
	ShaderC2(unsigned char alp, Point* point1, Point* point2);
	Color GetColor(double x, double y) override;
};

ShaderC2::ShaderC2(unsigned char alp, Point* point1, Point* point2) : Shader(alp) {
	p1 = new Point(point1->getX(), point1->getY());
	p2 = new Point(point2->getX(), point2->getY());
}

Color ShaderC2::GetColor(double x, double y) {
	double help_l = getDistanceBetweenPoints(p1->getX(), p1->getY(), p2->getX(), p2->getY()) / 2;

	Point* np1 = new Point(0, 0);
	Point* np2 = new Point(0, -2 * help_l);

	Affine trans;
	trans.addTransfer(-p1->getX(), -p1->getY());

	double alpha = acos(p2->getX() - p1->getX());
	if (p1->getY() < p2->getY()) {
		alpha += 2 * (M_PI - alpha);
	}
	Affine rot;
	rot.addRotation(M_PI / 2 - alpha);
	Point* p = new Point(x, y);
	*p = rot * trans * *p;

	double dist_lc1 = getDistanceFromCircleDoub(p->getX(), p->getY(), np1->getX(), np1->getY(), help_l * 3 / 2);
	double dist_lc2 = getDistanceFromCircleDoub(p->getX(), p->getY(), np2->getX(), np2->getY(), help_l * 3 / 2);

	double dist_sc1 = getDistanceFromCircleDoub(p->getX(), p->getY(), np1->getX(), np1->getY(), help_l / 2);
	double dist_sc2 = getDistanceFromCircleDoub(p->getX(), p->getY(), np2->getX(), np2->getY(), help_l / 2);

	double dist_sc3 = getDistanceFromCircleDoub(p->getX(), p->getY(), np1->getX() + help_l, np1->getY(), help_l / 2);
	double dist_sc4 = getDistanceFromCircleDoub(p->getX(), p->getY(), np2->getX() + help_l, np2->getY(), help_l / 2);

	double dist_l1 = fabs(p->getX() - (np1->getX() - help_l / 2));
	double dist_l2 = fabs(p->getX() - (np1->getX() - help_l * 3 / 2));

	if (p->getY() < 0) {
		dist_lc1 = DBL_MAX;
		dist_sc1 = DBL_MAX;
	} else {
		dist_sc3 = DBL_MAX;
		dist_l1 = DBL_MAX;
		dist_l2 = DBL_MAX;
	}

	if (p->getY() > -help_l * 2) {
		dist_lc2 = DBL_MAX;
		dist_sc2 = DBL_MAX;
	}
	else {
		dist_sc4 = DBL_MAX;
		dist_l1 = DBL_MAX;
		dist_l2 = DBL_MAX;
	}
	
	double dists[8] = {dist_lc1, dist_lc2, dist_sc1, dist_sc2, dist_sc3, dist_sc4, dist_l1, dist_l2};
	double min_dist = dist_lc1;
	for (int i = 1; i < 8; i++) {
		min_dist = min2(min_dist, dists[i]);
	}
	
	double width = help_l / 6;

	if (min_dist < width) {
		//Color base_color(16, 239, 106);
		Color base_color(242, 11, 249);
		double max_mul = 255 / min2(base_color.red, min(base_color.green, base_color.blue));
		base_color.red = min2(255, base_color.red * (min_dist * (-max_mul+0.5)/width + max_mul));
		base_color.green = min2(255, base_color.green * (min_dist * (-max_mul+ 0.5) / width + max_mul));
		base_color.blue = min2(255, base_color.blue * (min_dist * (-max_mul+ 0.5) / width + max_mul));
		return base_color;
	} else {
		return Color(120, 25, 68);
	}

}

#endif COMPLEXSHADERS_H
