
#include "lGooScalarAnimate.h"

void lGooScalarAnimateParam::SetBeginValue()
{
	AnimState->SetParam(BeginValue);
}

void lGooScalarAnimateParam::SetEndValue()
{
	AnimState->SetParam(EndValue);
}

void lGooScalarAnimateParam::SetInitialValue()
{
	AnimState->SetParam(InitialValue);
}

void lGooScalarAnimateParam::SetCurrentValue(lmScalar time_elapsed)
{
	AnimState->SetParam(BeginValue + time_elapsed * dValue);
}

void lGooScalarAnimateParam::IncreaseParam(lmScalar dt)
{
	if((BeginValue < EndValue && AnimState->GetParam() < EndValue) ||
	   (BeginValue > EndValue && AnimState->GetParam() > EndValue))
	{
		AnimState->IncreaseParam(dValue * dt);
	}
}

lGooScalarAnimateParam::lGooScalarAnimateParam(liGooScalarAnimState *anim_state,lmScalar begin_value,lmScalar end_value,lmScalar duration)
	:AnimState(anim_state),BeginValue(begin_value),EndValue(end_value)
{
	//Üres függvénytörzs
	InitialValue = AnimState->GetParam();
	dValue = (EndValue - BeginValue) / duration;
}

lGooScalarAnimateParam::~lGooScalarAnimateParam()
{
	//Üres függvénytörzs
}
