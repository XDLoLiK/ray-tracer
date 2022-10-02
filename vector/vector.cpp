#include "vector.hpp"

Vector3::Vector3(long double x, long double y, long double z):
	x_(x),
	y_(y),
	z_(z),
	length_(sqrtl(powl(x, 2) + powl(y, 2) + powl(z, 2)))
{

}

Vector3::Vector3(long double x0, long double y0, long double z0,
	             long double x1, long double y1, long double z1):
	x_(x1 - x0),
	y_(y1 - y0),
	z_(z1 - z0)
{

}

Vector3 &Vector3::operator*=(long double scale)
{
	x_ *= scale;
	y_ *= scale;
	z_ *= scale;

	length_ = sqrtl(powl(x_, 2) + powl(y_, 2) + powl(z_, 2));

	return *this;
}

Vector3 operator*(const Vector3 &vec, long double num)
{
	Vector3 newVec = vec;
	newVec *= num;
	return newVec;
}

Vector3 operator*(long double num, const Vector3 &vec)
{
	return vec * num;
}

long double dotProduct(const Vector3 &vec1, const Vector3 &vec2)
{
	return vec1.x() * vec2.x() + vec1.y() * vec2.y() + vec1.z() * vec2.z();
}

Vector3 crossProduct(const Vector3 &vec1, const Vector3 &vec2)
{
	long double newX = vec1.y() * vec2.z() - vec1.z() * vec2.y();
	long double newY = vec1.z() * vec2.x() - vec1.x() * vec2.z();
	long double newZ = vec1.x() * vec2.y() - vec1.y() * vec2.x();

	return Vector3(newX, newY, newZ);
}

Vector3 &Vector3::operator+=(const Vector3 &vec)
{
	x_ += vec.x();
	y_ += vec.y();
	z_ += vec.z();

	length_ = sqrtl(powl(x_, 2) + powl(y_, 2) + powl(z_, 2));

	return *this;
}

Vector3 operator+(const Vector3 &vec1, const Vector3 &vec2)
{
	Vector3 newVec = vec1;
	newVec += vec2;
	return newVec;
}

Vector3 &Vector3::operator-=(const Vector3 &vec)
{
	x_ -= vec.x();
	y_ -= vec.y();
	z_ -= vec.z();

	length_ = sqrtl(powl(x_, 2) + powl(y_, 2) + powl(z_, 2));

	return *this;	
}

Vector3 operator-(const Vector3 &vec1, const Vector3 &vec2)
{
	Vector3 newVec = vec1;
	newVec -= vec2;
	return newVec;	
}

Vector3 operator-(const Vector3 &vec)
{
	return -1 * vec;
}

Vector3 normalized(const Vector3 &vec)
{
	return Vector3(vec.x() / vec.length(),
		           vec.y() / vec.length(),
		           vec.z() / vec.length());
}

long double Vector3::x() const
{
	return x_;
}

long double Vector3::y() const
{
	return y_;
}

long double Vector3::z() const
{
	return z_;
}

long double Vector3::length() const
{
	return length_;
}

void Vector3::normalize()
{
	x_ /= length_;
	y_ /= length_;
	z_ /= length_;
	length_ = 1;
}

long double cos(const Vector3 &vec1, const Vector3 &vec2)
{
	long double numerator   = vec1.x() * vec2.x() + vec1.y() * vec2.y() + vec1.z() * vec2.z();
	long double denominator = vec1.length() * vec2.length();
	return equals(denominator, 0.0) ? INFINITY : numerator / denominator;
}

Vector3 reflect(const Vector3 &direction, const Vector3 &normal)
{
	if (dotProduct(direction, normal) < 0.0)
		return reflect(direction, -normal);

	/* Assuming all vectors are normalized */
	return direction + normal * 2.0 * dotProduct(direction, normal);
}

// Vector3 refract(const Vector3 &I, const Vector3 &N, long double eta_t, long double eta_i=1.f) { // Snell's law
//     long double cosi = - std::max(-1.f, std::min(1.f, I*N));
//     if (cosi<0) return refract(I, -N, eta_i, eta_t); // if the ray comes from the inside the object, swap the air and the media
//     long double eta = eta_i / eta_t;
//     long double k = 1 - eta*eta*(1 - cosi*cosi);
//     return k<0 ? vec3{1,0,0} : I*eta + N*(eta*cosi - std::sqrt(k)); // k<0 = total reflection, no ray to refract. I refract it anyways, this has no physical meaning
// }

Vector3 refract(const Vector3 &direction, const Vector3 &normal, long double n1, long double n2)
{
	if (equals(n1, 0.0l) || equals(n2, 0.0l))
		return Vector3(0.0, 0.0, 0.0);

	/* Assuming all vectors are normalized */
	long double cosAlpha = dotProduct(normal, direction);
	if (cosAlpha > 0.0l)
		return refract(direction, -normal, n2, n1);

	long double relation = n1 / n2;
	Vector3 refractedPerp = relation * (direction + cosAlpha * normal);
	
	long double l = refractedPerp.length();
	Vector3 refractedParallel = -sqrtl(1.0l - l * l) * normal;

	return refractedPerp + refractedParallel;
}
