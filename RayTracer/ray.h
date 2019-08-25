#ifndef RAYH
#define RAYH

#ifndef EIGENH
#define EIGENH
	#include <Eigen/Core>
#endif


class Ray {
private:
	Eigen::Vector4f A, B;
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

	Ray() {};
	Ray(const Eigen::Vector4f& a, const Eigen::Vector4f& b) { A = a; B = b; }

	Eigen::Vector4f origin() const		{ return A; }
	Eigen::Vector4f direction() const	{ return B; }
	Eigen::Vector4f point_at_parameter(float t) const { return A + B * t; }

	void normalize_direction() { B.normalize(); }

};

#endif