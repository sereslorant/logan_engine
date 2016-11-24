#ifndef LI_BLEND_FUNC_H
#define LI_BLEND_FUNC_H

class liBlendFunc
{
public:
	virtual void Reset() = 0;

	virtual void Update(double dt) = 0;
	virtual double GetWeight() = 0;

	liBlendFunc(){}
	virtual ~liBlendFunc() {}
};

#endif // LI_BLEND_FUNC_H
