#include <cmath>
#include <string>
#include <iostream>
#include "vector2f.h"

Vector2f& Vector2f::operator=(const Vector2f& rhs) {
  if ( this != &rhs ) {
	  v[0] = rhs[0];
	  v[1] = rhs[1];
  }
  return *this;
}

Vector2f::Vector2f(float x, float y) {
	v[0] = x;
	v[1] = y;
}

float &Vector2f::operator[](int index) {
	return v[index];
}

float Vector2f::operator[](int index) const {
	return v[index];
}

Vector2f Vector2f::operator*(float scale) const {
	return Vector2f(v[0] * scale, v[1] * scale);
}

Vector2f Vector2f::operator/(float scale) const {
  if ( scale < 0.001 && scale > -0.001 ) {
    throw std::string("scale too small in /");
  }
	return Vector2f(v[0] / scale, v[1] / scale);
}

Vector2f Vector2f::operator+(const Vector2f &other) const {
	return Vector2f(v[0] + other.v[0], v[1] + other.v[1]);
}

Vector2f Vector2f::operator-(const Vector2f &other) const {
	return Vector2f(v[0] - other.v[0], v[1] - other.v[1]);
}

Vector2f Vector2f::operator-() const {
	return Vector2f(-v[0], -v[1]);
}

const Vector2f &Vector2f::operator*=(float scale) {
	v[0] *= scale;
	v[1] *= scale;
	return *this;
}

const Vector2f &Vector2f::operator/=(float scale) {
  if ( scale < 0.001 && scale > -0.001 ) {
    throw std::string("scale too small in /=");
  }
	v[0] /= scale;
	v[1] /= scale;
	return *this;
}

const Vector2f &Vector2f::operator+=(const Vector2f &other) {
	v[0] += other.v[0];
	v[1] += other.v[1];
	return *this;
}

const Vector2f &Vector2f::operator-=(const Vector2f &other) {
	v[0] -= other.v[0];
	v[1] -= other.v[1];
	return *this;
}

float Vector2f::magnitude() const {
	return sqrt(v[0] * v[0] + v[1] * v[1]);
}

float Vector2f::magnitudeSquared() const {
	return v[0] * v[0] + v[1] * v[1];
}

Vector2f Vector2f::normalize() const {
	float m = sqrt(v[0] * v[0] + v[1] * v[1]);
  if ( m < 0.001 && m > -0.001 ) {
    throw std::string("Point too close in Vector2f::normalize");
  }
	return Vector2f(v[0] / m, v[1] / m);
}

float Vector2f::dot(const Vector2f &other) const {
	return v[0] * other.v[0] + v[1] * other.v[1] ;
}

Vector2f Vector2f::cross(const Vector2f &) const {
	throw std::string("Cross Product not implemented!"); 
}

Vector2f operator*(float scale, const Vector2f &v) {
	return v * scale;
}

std::ostream &operator<<(std::ostream &output, const Vector2f &v) {
	std::cout << '(' << v[0] << ", " << v[1]  << ')';
	return output;
}

