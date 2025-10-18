// This is my custom vector implementation. Also includes a point implementation, just because
// they're related. Cleaner.


#ifndef VECTOR_H
#define VECTOR_H


// Includes
#include <cmath>


// User includes


struct Point3D
{
	double x;
	double y;
	double z;

	Point3D operator+(const Point3D& p) const
	{
		return Point3D {x + p.x, y + p.y, z + p.z};
	}
};


struct Vector3D
{
	double x;
	double y;
	double z;

	Vector3D operator+(const Vector3D& v) const
	{
		return Vector3D {x + v.x, y + v.y, z + v.z};
	}

	Vector3D operator-(const Vector3D& v) const
	{
		return Vector3D {x - v.x, y - v.y, z - v.z};
	}

	Vector3D operator*(double scalar) const
	{
		return Vector3D {x * scalar, y * scalar, z * scalar};
	}

	Vector3D operator/(double scalar) const
	{
		return Vector3D {x / scalar, y / scalar, z / scalar};
	}

	double dot(const Vector3D& v) const
	{
		return ((x * v.x) + (y * v.y) + (z * v.z));
	}

	Vector3D cross(const Vector3D& v) const
	{
		return Vector3D {y * v.z - z * v.y,
				 z * v.x - x * v.z,
				 x * v.y - y * v.x};
	}

	double magnitude() const
	{
		return std::sqrt((x*x) + (y*y) + (z*z));
	}

	Vector3D normalize() const
	{
		double len = magnitude();
		if (len == 0) return Vector3D {0, 0, 0};
		return *this / len;
	}

	Vector3D matrixMultiply(const double matrix[3][3]) const
	{
		return Vector3D {
		    matrix[0][0] * this -> x + matrix[0][1] * this -> y + matrix[0][2] * this -> z,
		    matrix[1][0] * this -> x + matrix[1][1] * this -> y + matrix[1][2] * this -> z,
		    matrix[2][0] * this -> x + matrix[2][1] * this -> y + matrix[2][2] * this -> z};
	}	    
};


struct Ray
{
	Point3D origin;
	Vector3D vector;
};


#endif
