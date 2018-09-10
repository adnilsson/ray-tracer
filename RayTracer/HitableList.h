#ifndef HITABLELISTH
#define HITABLELISTH

#include "Hitable.h"

class HitableList : public Hitable{
public:
	Hitable **list;
	int list_size;

	HitableList() {};
	HitableList(Hitable **l, int n): list(l), list_size(n) {};
	virtual ~HitableList();

	virtual bool hit(const Ray &r, float t_min, float t_max, hit_record &rec) const;
};

#endif