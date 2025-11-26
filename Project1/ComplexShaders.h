#pragma once
#ifndef COMPLEXSHADERS_H
#define COMPLEXSHADERS_H
#include "Frame.h"
#include "General.h"
#include "Matrix.h"
#include <stdio.h>
#include <math.h>

/*class ShaderC1 : public Shader {
	Point p1, p2;
public:
	ShaderC1(unsigned char alp, double x1, double y1, double x2, double y2);
	Color GetColor(double x, double y) override;
};

ShaderC1::ShaderC1(unsigned char alp, double x1, double y1, double x2, double y2) : Shader(alp) {
	p1 = Point(x1, y1);
	p2 = Point(x2, y2);
}

Color ShaderC1::GetColor(double x, double y) {
	double help_l = getDistanceBetweenPoints(p1.getX(), p1.getY(), p2.getX(), p2.getY()) / 2;

	Point np1(0, 0);
	Point np2(0, -2 * help_l);

	Affine trans;
	trans.addTransfer(-p1.getX(), -p1.getY());

	double alpha = acos(p2.getX() - p1.getX());
	if (p1.getY() < p2.getY()) {
		alpha += 2 * (M_PI - alpha);
	}
	Affine rot;
	rot.addRotation(M_PI / 2 - alpha);
	Point p (x, y);
	p = rot * trans * p;

	double dist_from_p1 = getDistanceBetweenPoints(p.getX(), p.getY(), np1.getX(), np1.getY());
	double dist_from_p2 = getDistanceBetweenPoints(p.getX(), p.getY(), np2.getX(), np2.getY());

	if (dist_from_p1 > help_l / 2 && dist_from_p1 < help_l * 3 / 2 && p.getY() >= 0) {
		return Color(255, 255, 0);
	}
	else if (dist_from_p2 > help_l / 2 && dist_from_p2 < help_l * 3 / 2 && p.getY() <= -help_l * 2) {
		return Color(0, 255, 255);
	}
	else if (p.getY() < 0 && p.getY() > -help_l * 2 && p.getX() < -help_l / 2 && p.getX() > -help_l * 3 / 2) {
		return Color(255, 0, 255);
	}
	else {
		return Color(120, 25, 68);
	}
}

/*class ShaderF1 : public Shader {
	Point p1, p2;
	Point np1, np2;
	Affine trans, rot;
public:
	ShaderF1(unsigned char alp, double x1, double y1, double x2, double y2);
	Color GetColor(double x, double y) override;
};

ShaderF1::ShaderF1(unsigned char alp, double x1, double y1, double x2, double y2) : Shader(alp) {
	p1 = Point(x1, y1);
	p2 = Point(x2, y2);
	double help_l = getDistanceBetweenPoints(p1.getX(), p1.getY(), p2.getX(), p2.getY());
	np1 = Point(0, 0);
	np2 = Point(help_l, 0);
	trans.addTransfer(-p1.getX(), -p1.getY());
	double alpha = acos((p2.getX() - p1.getX()) / help_l);
	if (p1.getY() < p2.getY()) {
		alpha += 2 * (M_PI - alpha);
	}
	rot.addRotation(alpha);
}

Color ShaderF1::GetColor(double x, double y) {
	double help_l = getDistanceBetweenPoints(p1.getX(), p1.getY(), p2.getX(), p2.getY());

	Point p(x, y);
	p = trans * p;
	p = rot * p;

	double dist_from_p1 = getDistanceBetweenPoints(p.getX(), p.getY(), np1.getX(), np1.getY());
	double dist_from_p2 = getDistanceBetweenPoints(p.getX(), p.getY(), np2.getX(), np2.getY());

	if (dist_from_p1 > help_l / 2 && dist_from_p1 < help_l * 3 / 2 && p.getX() <= 0) {
		return Color(255, 255, 0);
	}
	else if (dist_from_p2 > help_l / 2 && dist_from_p2 < help_l * 3 / 2 && p.getX() >= help_l) {
		return Color(0, 255, 255);
	}
	else if (p.getY() > -help_l * 2 && p.getY() < help_l * 3 && p.getX() < help_l && p.getX() > 0) {
		return Color(255, 0, 255);
	}
	else {
		return Color(120, 25, 68);
	}
}*/


class ShaderC2 : public Shader {
	Point p1, p2;
	Point np1, np2;
	Affine trans, rot;
	Color col;
	double width_percent;
public:
	ShaderC2(unsigned char alp, double x1, double y1, double x2, double y2, Color c, double w);
	Color GetColor(double x, double y) override;
};

ShaderC2::ShaderC2(unsigned char alp, double x1, double y1, double x2, double y2, Color c, double w) : Shader(alp) {
	p1 = Point(x1, y1);
	p2 = Point(x2, y2);
	double help_ll = getDistanceBetweenPoints(p1.getX(), p1.getY(), p2.getX(), p2.getY());
	np1 = Point(0, 0);
	np2 = Point(0, -help_ll);
	trans.addTransfer(-p1.getX(), -p1.getY());
	double alpha = acos((p2.getX() - p1.getX()) / help_ll);
	if (p1.getY() < p2.getY()) {
		alpha += 2 * (M_PI - alpha);
	}
	rot.addRotation(alpha - M_PI / 2);
	col = c;
	width_percent = w;
}

Color ShaderC2::GetColor(double x, double y) {
	double help_l = getDistanceBetweenPoints(p1.getX(), p1.getY(), p2.getX(), p2.getY()) / 2;

	Point p(x, y);
	p = trans * p;
	p = rot * p;

	double dist_lc1 = getDistanceFromCircleDoub(p.getX(), p.getY(), np1.getX(), np1.getY(), help_l * 3 / 2);
	double dist_lc2 = getDistanceFromCircleDoub(p.getX(), p.getY(), np2.getX(), np2.getY(), help_l * 3 / 2);

	double dist_sc1 = getDistanceFromCircleDoub(p.getX(), p.getY(), np1.getX(), np1.getY(), help_l / 2);
	double dist_sc2 = getDistanceFromCircleDoub(p.getX(), p.getY(), np2.getX(), np2.getY(), help_l / 2);

	double dist_sc3 = getDistanceFromCircleDoub(p.getX(), p.getY(), np1.getX() + help_l, np1.getY(), help_l / 2);
	double dist_sc4 = getDistanceFromCircleDoub(p.getX(), p.getY(), np2.getX() + help_l, np2.getY(), help_l / 2);

	double dist_l1 = fabs(p.getX() - (np1.getX() - help_l / 2));
	double dist_l2 = fabs(p.getX() - (np1.getX() - help_l * 3 / 2));

	if (p.getY() < 0) {
		dist_lc1 = DBL_MAX;
		dist_sc1 = DBL_MAX;
	} else {
		dist_sc3 = DBL_MAX;
		dist_l1 = DBL_MAX;
		dist_l2 = DBL_MAX;
	}

	if (p.getY() > -help_l * 2) {
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
	
	double width = help_l / width_percent;

	if (min_dist < width) {
		Color res_col = col;
		double max_mul = 255 / min2(col.red, min(col.green, col.blue));
		res_col.red = min2(255, col.red * (min_dist * (-max_mul+0.5)/width + max_mul));
		res_col.green = min2(255, col.green * (min_dist * (-max_mul+ 0.5) / width + max_mul));
		res_col.blue = min2(255, col.blue * (min_dist * (-max_mul+ 0.5) / width + max_mul));
		return res_col;
	} else {
		return Color(0, 0, 0, 0);
	}

}

class ShaderF2 : public Shader {
	Point p1, p2;
	Point np1, np2;
	Affine trans, rot;
	Color col;
	double grid_size;
	Shader* back;
public:
	ShaderF2(unsigned char alp, double x1, double y1, double x2, double y2, Color c, double g, Shader* b);
	Color GetColor(double x, double y) override;
};

ShaderF2::ShaderF2(unsigned char alp, double x1, double y1, double x2, double y2, Color c, double g, Shader* b) : Shader(alp) {
	p1 = Point(x1, y1);
	p2 = Point(x2, y2);
	double help_l = getDistanceBetweenPoints(p1.getX(), p1.getY(), p2.getX(), p2.getY());
	np1 = Point(0, 0);
	np2 = Point(help_l, 0);
	trans.addTransfer(-p1.getX(), -p1.getY());
	double alpha = acos((p2.getX() - p1.getX()) / help_l);
	if (p1.getY() < p2.getY()) {
		alpha += 2 * (M_PI - alpha);
	}
	rot.addRotation(alpha);
	col = c;
	grid_size = g;
	back = b;
}

Color ShaderF2::GetColor(double x, double y) {
	double help_l = getDistanceBetweenPoints(p1.getX(), p1.getY(), p2.getX(), p2.getY());

	Point p(x, y);
	p = trans * p;
	p = rot * p;

	double dist_from_p1 = getDistanceBetweenPoints(p.getX(), p.getY(), np1.getX(), np1.getY());
	double dist_from_p2 = getDistanceBetweenPoints(p.getX(), p.getY(), np2.getX(), np2.getY());

	if (dist_from_p1 > help_l / 2 && dist_from_p1 < help_l * 3 / 2 && p.getX() <= 0
		|| dist_from_p2 > help_l / 2 && dist_from_p2 < help_l * 3 / 2 && p.getX() >= help_l
		|| p.getY() > -help_l * 2 && p.getY() < help_l * 3 && p.getX() < help_l && p.getX() > 0) 
	{
		double c = grid_size * sqrt(2);
		double c2 = grid_size * 1.05;
		Affine rot45;
		rot45.addRotation(M_PI / 4);
		p = rot45 * p;
		p = Point(fabs(fmod(p.getX(), c)), fabs(fmod(p.getY(), c)));

		double dist1 = getDistanceBetweenPoints(p.getX(), p.getY(), 0, 0);
		double dist2 = getDistanceBetweenPoints(p.getX(), p.getY(), 0, c);
		double dist3 = getDistanceBetweenPoints(p.getX(), p.getY(), c, c);
		double dist4 = getDistanceBetweenPoints(p.getX(), p.getY(), c, 0);
		if ((dist1 < c2 || dist3 < c2) && (dist2 < c2 || dist4 < c2))
		{
			return col;
		}
		else {
			return back->GetColor(x, y);
		}
	}
	else {
		return Color(0, 0, 0, 0);
	}
}

#endif COMPLEXSHADERS_H
