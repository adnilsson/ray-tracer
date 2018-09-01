#ifndef RAYH
#define RAYH

#ifndef EIGENH
#define EIGENH
	#include <Eigen/Core>
#endif


class Ray {
private:
	Eigen::Vector3f A, B;
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

	Ray() {};
	Ray(const Eigen::Vector3f& a, const Eigen::Vector3f& b) { A = a; B = b; }

	Eigen::Vector3f origin() const		{ return A; }
	Eigen::Vector3f direction() const	{ return B; }
	Eigen::Vector3f point_at_parameter(float t) const { return A + B * t; }

	void normalize_direction() { B.normalize(); }

};

#endif