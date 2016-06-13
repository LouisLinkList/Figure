#include <iostream>
#include <vector>
using namespace std;
#define epsilon 1e-10
#define Pi 3.141592653589793

double sqr(double a) { return a*a; }

struct Point{
private:
	double x, y;
public:
	Point(){}
	Point(double _x, double _y) : x(_x), y(_y){}
	double GetX(){ return x; }
	double GetY(){ return y; }
	double Length(){ return sqrt(x*x + y*y); }
	double Mul(Point b){ return x*b.y - y*b.x; } // z координата векторного произведения
	//Point operator=(Point b){ x = b.x; y = b.y; return *this; }

	Point operator-(Point b){ return{ x - b.x, y - b.y }; }
	Point operator-=(Point b){ return *this = *this - b; }
	Point operator+(Point b) { return{ x + b.x, y + b.y }; }
	Point operator+=(Point b) { return *this = *this + b; }
	Point operator*(double b) { return{ x*b, y*b }; }
	Point operator*=(double b) { return *this = *this * b; }
	Point operator/(double b) { return{ x / b, y / b }; }
	Point operator/=(double b) { return *this = *this / b; }
	double operator*(Point b) { return x*b.x + y*b.y; } // скалярное произведение

	bool operator<(Point b){
		if (abs(x - b.x) > epsilon) // !=
			return (x < b.x);
		return (y < b.y);
	}
	bool operator>(Point b){ return !(*this < b); }
	bool operator == (Point b) { return abs(x - b.x) < epsilon && abs(y - b.y) < epsilon; }
	bool operator !=(Point b){ return !(*this == b); };
	Point Norm(){ return{ x / Length(), y / Length() }; }
};

class Segment{
public:
	Point a, b;
	Segment(){}
	Segment(Point _a, Point _b) : a(_a), b(_b){
		if (a > b){
			Point temp;
			temp = b;
			b = a;
			a = temp;
		}
	}
	vector<Point> IntersectionPoint(Segment c){
		vector<Point> result;
		Point test = c.a + (c.b - c.a)*(abs((b - a).Mul(c.a - a)) / abs((b - a).Mul(c.b - a) - (b - a).Mul(c.a - a)));
		if (PointOnSegment(test))
			result.push_back(test);
		return result;
	}
	bool PointOnSegment(Point c){ 
		if (abs(b.GetX() - a.GetX()) < epsilon  || abs(b.GetY() - a.GetY()) < epsilon)
			return (a < c || a == c) && (c < b || c == b);
		
		return abs((c.GetX() - a.GetX()) / (b.GetX() - a.GetX()) - (c.GetY() - a.GetY()) / (b.GetY() - a.GetY())) < epsilon && (a < c || a == c) && (c < b || c == b); 
	}
	bool SegmentIntersection(Segment c);
};

class Circle{
public:
	Point center;
	double r;
	Circle(){}
	Circle(Point _center, double _r) : center(_center), r(_r){}
	double Perimetr(){ return 2  *r * Pi; }
	vector<Point> IntersectionSegment(Segment AB){
		vector<Point> result;
		Point losos = AB.b - AB.a; // (0,2)
		Point Proj = losos*((center - AB.a) * losos) / (losos * losos) + AB.a; // (-1,0)

		if (((center - Proj).Length() - r) < epsilon){
			result.push_back(Proj);
			return result;
		}

		Point temp = losos.Norm() * sqrt(sqr(r) - sqr((center - Proj).Length())); // (-1,0)

		if (AB.PointOnSegment(temp))			
			result.push_back(temp);
		
		losos *= -1;
		temp = losos.Norm() * sqrt(sqr(r) - sqr((center - Proj).Length()));

		if (AB.PointOnSegment(temp))
			result.push_back(temp);
		
		return result;
	}
};

class Rectangle{
public:
	Point a, b, c, d;
	Rectangle(){}
	Rectangle(Point _a, Point _b, Point _c, Point _d) : a(_a), b(_b), c(_c), d(_d){}
	Rectangle(Point _a, Point _b, double len): a(_a) , b(_b){ 
		Point ort((b - a).GetY(), -(b - a).GetX());
		c = b + ort.Norm()*len;
		d = a + ort.Norm()*len;
	}
	bool Orthogonal(){ return abs((b - a)*(c - b)) < epsilon && abs((c - b)*(d - c)) < epsilon && abs((d - c)*(a - d)) < epsilon && abs((a - d)*(b - a)) < epsilon; }
	double Perimetr(){ return (b - a).Length() * 2 + (c - b).Length() * 2; }
};

class Quadrate : public Rectangle{
public:
	Quadrate(Point _a, Point _b, Point _c, Point _d) : Rectangle(_a, _b, _c, _d){}
	Quadrate(Point _a, Point _b) : Rectangle(_a, _b, (_b - _a).Length()){}
	double Perimetr() { return (a - b).Length() * 4; }
};


class Line {
public:
	Point a, b;
	Line(){}
	Line(Point _a, Point _b) : a(_a), b(_b){}
	Line(Point _b) : a({ 0, 0 }), b(_b){}
	bool PointOnLine(Point c){
		if (abs(b.GetX() - a.GetX()) < epsilon || abs(b.GetY() - a.GetY()) < epsilon)
			return abs(c.GetX() - a.GetX()) < epsilon || abs(c.GetY() - a.GetY()) < epsilon;  return abs((c.GetX() - a.GetX()) / (b.GetX() - a.GetX()) - (c.GetY() - a.GetY()) / (b.GetY() - a.GetY())) < epsilon;
	} 
	Point Projection(Point c){ return (b - a)*((c - a) * (b - a)) / ((b - a) * (b - a)) + a; } // Деление на ноль!!!
	Segment Projection(Segment c){
		if (Projection(c.a) > Projection(c.b))
			return{ Projection(c.b), Projection(c.a) };
		return{ Projection(c.a), Projection(c.b) };
	}
	Segment Projection(Circle O){
		Segment result(Projection(O.center) - (b - a)*O.r / (b - a).Length(), Projection(O.center) + (b - a)*O.r / (b - a).Length());
		if (result.a > result.b)
			return{ result.b, result.a };
		return result;
	}
	Segment Projection(Rectangle ABCD){
		Segment temp_s(Projection(ABCD.a), Projection(ABCD.b));
		if (temp_s.a > temp_s.b)
			temp_s = { temp_s.b, temp_s.a };
		if (Projection(ABCD.c) < temp_s.a) temp_s.a = Projection(ABCD.c);
		if (Projection(ABCD.c) > temp_s.b) temp_s.b = Projection(ABCD.c);
		if (Projection(ABCD.d) < temp_s.a) temp_s.a = Projection(ABCD.d);
		if (Projection(ABCD.d) > temp_s.b) temp_s.b = Projection(ABCD.d);		
		return temp_s;
	}
};


vector<Point> IntersectionRectangleSegment(Rectangle, Segment);

