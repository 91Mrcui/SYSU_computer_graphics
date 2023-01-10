#ifndef UTILS_H
#define UTILS_H

#include <array>

#include "WindowsApp.h"

class Point2D
{
public:

	double x, y;
	std::array<Uint8, 3> color;

	Point2D(int _x, int _y) : x(_x), y(_y) {}

	Point2D operator*(const double &w) const
	{
		Point2D ans(x*w, y*w);
		return ans;
	}

	Point2D operator+(const Point2D &rhs) const
	{
		Point2D ans(x + rhs.x, y + rhs.y);
		return ans;
	}

	Point2D& operator+=(const Point2D &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	static Point2D lerp(const Point2D &p0, const Point2D &p1, const double &t)
	{
		// Linear interpolation
		double x = p0.x * (1.0 - t) + p1.x * t;
		double y = p0.y * (1.0 - t) + p1.y * t;
		return Point2D(x, y);
	}
};

class Line2D
{
public:
	double x0, y0, x1, y1;
	std::array<Uint8, 3> color;

	Line2D(int _x0, int _y0, int _x1, int _y1)
		:x0(_x0), y0(_y0), x1(_x1), y1(_y1) {}
};

class BezierCurve final
{
public:

	//Add/clear
	void clearAll();
	void clearControlPoints();
	void nextBezierCurve() { m_beizerPoints.push_back(std::vector<Point2D>()); }
	
	void addControlPoints(int x, int y);
	int numControlPoints() const { return m_controlPoints.size(); }

	void evaluate(const double &t);

	//Drawing
	void drawControlPointsAndLines(WindowsApp* winApp) const;
	void drawBezierCurvePoints(WindowsApp* winApp) const;
	void drawAuxiliaryLines(WindowsApp *winApp) const;

	
private:
	void generateAuxiliaryLines(const double &t);

	Point2D implementTask1(const std::vector<Point2D> &points, const double &t)const;
	Point2D implementTask2(const std::vector<Point2D> &points, const double &t) const;

	//task 1
	long long getFactorial(int n)const;

private:
	std::vector<Point2D> m_controlPoints;
	std::vector<Line2D> m_auxiliaryLines;
	std::vector<std::vector<Point2D>> m_beizerPoints;

};

#endif