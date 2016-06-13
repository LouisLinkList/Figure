#include <iostream>
#include "funk.h"
using namespace std;

ostream& operator<<(ostream& os, Point&  a){ return os << "x: " << a.GetX() << "  y: " << a.GetY() << '\n'; }
ostream& operator<<(ostream& os, Segment&  c){ return os << "Segment\n" << "A: " << c.a << "B: " << c.b; }
ostream& operator<<(ostream& os, Circle&  c){ return os << "Circle\n" << "Center: " << c.center << "Radius: " << c.r << '\n'; }
ostream& operator<<(ostream& os, Rectangle&  ABCD){ return 	os << "Rectangle\n" << "A: " << ABCD.a << "B: " << ABCD.b << "C: " << ABCD.c << "D: " << ABCD.d; }
ostream& operator<<(ostream& os, Quadrate&  ABCD){ return os << "Quadrate\n" << "A: " << ABCD.a << "B: " << ABCD.b << "C: " << ABCD.c << "D: " << ABCD.d; }
ostream& operator<<(ostream& os, vector<Point> & vect){
	if (vect.size()){
		os << "Vector:\n";
		for (size_t i = 0; i < vect.size(); ++i)
			os << vect[i];
		return os;
	}
	return os << "Vector.size == 0!\n";
}

vector<Point> IntersectionRectangleSegment(Rectangle ABCD, Segment AB){
	vector<Point> result , temp;

	temp = AB.IntersectionPoint({ ABCD.a, ABCD.b });
	for (size_t i = 0; i < temp.size(); ++i)
		result.push_back(temp[i]);

	temp = AB.IntersectionPoint({ ABCD.b, ABCD.c });
	for (size_t i = 0; i < temp.size(); ++i)
		result.push_back(temp[i]);

	temp = AB.IntersectionPoint({ ABCD.c, ABCD.d });
	for (size_t i = 0; i < temp.size(); ++i)
		result.push_back(temp[i]);

	temp = AB.IntersectionPoint({ ABCD.d, ABCD.a });
	for (size_t i = 0; i < temp.size(); ++i)
		result.push_back(temp[i]);

	return result;

}


vector<Point> IntersectionCircleRectangle(Circle O, Rectangle ABCD){
	vector<Point> result, temp;

	temp = O.IntersectionSegment({ ABCD.a, ABCD.b });

	for (size_t i = 0; i < temp.size(); ++i)
		result.push_back(temp[i]);

	temp = O.IntersectionSegment({ ABCD.b, ABCD.c });
	for (size_t i = 0; i < temp.size(); ++i)
		result.push_back(temp[i]);

	temp = O.IntersectionSegment({ ABCD.c, ABCD.d });
	for (size_t i = 0; i < temp.size(); ++i)
		result.push_back(temp[i]);

	temp = O.IntersectionSegment({ ABCD.d, ABCD.a });
	for (size_t i = 0; i < temp.size(); ++i)
		result.push_back(temp[i]);

	return result;
}

int main(){
	
	Rectangle ABCD({ -1, -3 }, { -1, 3 }, { 1, 3 }, { 1, -3 });
	Line l({ 1, 1 });
	Segment AB({1,1}, {3,3});
	Segment CD({1,3}, {3,1});
	Circle O({ 0, 0 }, 1);


	
	//cout << ABCD;

	cout << IntersectionCircleRectangle(O, ABCD);



	system("pause");
	return 0;
}