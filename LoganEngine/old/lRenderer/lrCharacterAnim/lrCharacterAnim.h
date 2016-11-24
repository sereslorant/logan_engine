#ifndef LR_CHARACTER_ANIM_H
#define LR_CHARACTER_ANIM_H

#include "../../lInterfaces/lRenderer/lrCharacterAnim/liCharacterAnim.h"
#include "../../lInterfaces/lRenderer/lrCharacterAnim/liKeyframeAnimation.h"

#include <vector>
#include <map>

class lrCharacterAnim : public liCharacterAnim
{
private:

	class lAnimStateTransition : public liEventCallback
	{
	private:
		lrCharacterAnim *CharacterAnim;

		bool ResetState;
		liCharacterAnimState *NextState;

	public:

		virtual void Callback() override
		{
			CharacterAnim->CurrentState = NextState;

			if(ResetState)
			{
				NextState->Reset();
			}
		}

		lAnimStateTransition(lrCharacterAnim *character_anim,bool reset_state,liCharacterAnimState *next_state)
			:CharacterAnim(character_anim),ResetState(reset_state),NextState(next_state)
		{

		}

		virtual ~lAnimStateTransition() override
		{

		}
	};

	std::vector<liCharacterAnimState *>  KeyframeAnimStates;
	std::vector<lAnimStateTransition *> StateTransitions;
	std::vector<liEvent *> Events;

	std::map<std::string,liEvent *> EventMap;

	liCharacterAnimState *BeginState;
	liCharacterAnimState *CurrentState;

public:

	virtual void FireEvent(const std::string &key) override
	{
		auto I = EventMap.find(key);

		if(I != EventMap.end())
		{
			I->second->Fire();
		}
	}

	virtual void Reset() override
	{
		CurrentState = BeginState;
	}

	virtual void Update(double dt) override
	{
		CurrentState->Update(dt);
	}

	virtual void GetSkeleton(liSkeleton *destination) override
	{
		CurrentState->GetSkeleton(destination);
	}

	lrCharacterAnim(liKeyframeAnimation *keyframe_anims[]);

	~lrCharacterAnim()
	{
		for(liCharacterAnimState *I : KeyframeAnimStates)
		{
			delete I;
		}

		for(lAnimStateTransition *I : StateTransitions)
		{
			delete I;
		}

		for(liEvent *I : Events)
		{
			delete I;
		}
	}
};

#endif // LR_CHARACTER_ANIM_H
