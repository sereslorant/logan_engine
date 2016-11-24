#ifndef LI_CHARACTER_ANIM_H
#define LI_CHARACTER_ANIM_H

#include "liSkeleton.h"

class liCharacterAnimState
{
public:

	virtual void Reset() = 0;
	virtual void Update(double dt) = 0;
	virtual void GetSkeleton(liSkeleton *destination) = 0;

	liCharacterAnimState(){}

	virtual ~liCharacterAnimState(){}
};

#include <string>

class liCharacterAnim : public liCharacterAnimState
{
public:
	virtual void FireEvent(const std::string &key) = 0;

	liCharacterAnim(){}
	virtual ~liCharacterAnim() override {}
};

#include "../../lGameMode/liEvent.h"

#endif // LI_CHARACTER_ANIM_H
