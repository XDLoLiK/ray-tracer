#include "sphere.hpp"

Sphere::Sphere(long double x, long double y,
	           long double z, long double r, const Material &material):
	center_(Vector3(x, y, z)),
	radius_(r),
	material_(material)
{

}

Sphere::Sphere(const Vector3 &vec, long double r,
	           const Material &material):
	center_(vec),
	radius_(r),
	material_(material)
{

}

Sphere::~Sphere()
{
	radius_ = 0.0;
}

std::tuple<bool, long double> Sphere::intersect(const Vector3 &origin, const Vector3 &direction) const
{
	Vector3 v = center_ - origin;

	long double a = direction * direction;
	long double b = direction * v * 2;
	long double c = v * v - radius_ * radius_;

	auto [solutionsN, t1, t2] = solveQuadratic(a, b, c);

	if (solutionsN == SolutionsNumber::e_oneRoot)
		return {true, t1};

	if (solutionsN == SolutionsNumber::e_twoRoots)
		return {true, (std::abs(t1) < std::abs(t2)) ? t1 : t2};
	
	return {false, 0};
}

std::tuple<enum Sphere::SolutionsNumber, long double, long double> Sphere::solveQuadratic(long double a,
	                                                                                      long double b,
	                                                                                      long double c) const
{
	long double t1 = NAN, t2 = NAN;

	if (compare(a, 0.0) && compare(b, 0.0))
		return compare(c, 0.0) ? std::make_tuple(e_infRoots, t1, t2) : std::make_tuple(e_noRoots, t1, t2);

	if (compare(a, 0.0) && !compare(b, 0.0))
		return {e_oneRoot, t1 = -c / b, t2};

	long double discriminant2 = b * b - 4 * a * c;

	if (discriminant2 < 0)
		return {e_noRoots, t1, t2};

	if (compare(discriminant2, 0.0))
		return {e_oneRoot, t1 = -b / (2 * a), t2 = -b / (2 * a)};

	t1 = (-b + sqrtl(discriminant2)) / (2 * a);
	t2 = (-b - sqrtl(discriminant2)) / (2 * a);
	return {e_twoRoots, t1, t2};
}

Vector3 Sphere::normal(const Vector3 &point) const
{
	return normalized(point - center_);
}

long double Sphere::radius() const
{
	return radius_;
}

const Material &Sphere::material() const
{
	return material_;
}