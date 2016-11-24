
#include "lrCharacterAnim.h"

#include "../../lGameModes/lEvent.h"

#include <iostream>

#include "lrLinearBlendFunc.h"

#include "lrKeyframeAnimState.h"
#include "lrSkeletonBlendState.h"
#include "lrCharacterAnimBlend.h"
#include "lrCharacterAnimPlay.h"

lrCharacterAnim::lrCharacterAnim(liKeyframeAnimation *keyframe_anims[])
{
	lrKeyframeAnimState *Standing = new lrKeyframeAnimState(keyframe_anims[0]);
	lrKeyframeAnimState *Step_Fwd = new lrKeyframeAnimState(keyframe_anims[1]);
	lrKeyframeAnimState *Run_Fwd  = new lrKeyframeAnimState(keyframe_anims[2]);

	liBlendFunc *StepBlendFunc = new lrLinearBlendFunc(4.0/24.0,0.0,1.0);
	liBlendFunc *RunBlendFunc  = new lrLinearBlendFunc(4.0/24.0,0.0,1.0);

	lrCharacterAnimBlend *Step_Blend = new lrCharacterAnimBlend(Standing,Step_Fwd,StepBlendFunc,4.0/24.0);
	lrCharacterAnimBlend *Run_Blend  = new lrCharacterAnimBlend(Step_Fwd,Run_Fwd,RunBlendFunc,4.0/24.0);

	liEvent *MoveForwardEvent = new lEvent;
	liEvent *MoveBackwardEvent = new lEvent;

	lAnimStateTransition *ToStepBlend = new lAnimStateTransition(this,true,Step_Blend);
	MoveForwardEvent->AddCallback(ToStepBlend);

	/*TMP!!!!!!!!!!!!*/
	MoveBackwardEvent->AddCallback(ToStepBlend);

	lAnimStateTransition *ToRunBlend = new lAnimStateTransition(this,true,Run_Blend);
	Step_Blend->GetEndEvent()->AddCallback(ToRunBlend);

	lAnimStateTransition *ToRun = new lAnimStateTransition(this,false,Run_Fwd);
	Run_Blend->GetEndEvent()->AddCallback(ToRun);

	liEvent *MoveLeftEvent	= new lEvent;
	liEvent *MoveRightEvent	= new lEvent;

	liEvent *StopMovingEvent = new lEvent;

	lAnimStateTransition *ToStand = new lAnimStateTransition(this,true,Standing);
	StopMovingEvent->AddCallback(ToStand);

	double StepTransitionTime = 0.25;

	double MoveTransitionTime1 = StepTransitionTime/4.0 - 0.1*StepTransitionTime;
	double MoveTransitionTime2 = 2.0*StepTransitionTime/4.0 - 0.1*StepTransitionTime;

	std::cout << "8==============D\n" << keyframe_anims[3]->Size() << std::endl;

	liSkeleton *StepSidewaysStanding = ((*keyframe_anims[3])[6]);

	liSkeleton *StepLeftStepping1 = ((*keyframe_anims[3])[8]);
	liSkeleton *StepLeftStepping2 = ((*keyframe_anims[3])[9]);

	liSkeleton *StepRightStepping1 = ((*keyframe_anims[3])[11]);
	liSkeleton *StepRightStepping2 = ((*keyframe_anims[3])[10]);

	liSkeleton *StepSideways = ((*keyframe_anims[3])[7]);

	/*
	 * Move left
	 */

	lrLinearBlendFunc *StepLeftBlend = new lrLinearBlendFunc(StepTransitionTime,0.0,1.0);
	lrSkeletonBlendState *StepLeft = new lrSkeletonBlendState(StepSidewaysStanding,StepLeftStepping1,StepLeftBlend,StepTransitionTime);

	lrLinearBlendFunc *MoveLeftBlend1 = new lrLinearBlendFunc(StepTransitionTime,0.0,1.0);
	lrSkeletonBlendState *MoveLeft1 = new lrSkeletonBlendState(StepLeftStepping1,StepSideways,MoveLeftBlend1,StepTransitionTime);

	lrLinearBlendFunc *MoveLeftBlend2 = new lrLinearBlendFunc(StepTransitionTime,0.0,1.0);
	lrSkeletonBlendState *MoveLeft2 = new lrSkeletonBlendState(StepSideways,StepLeftStepping2,MoveLeftBlend2,StepTransitionTime);

	lrLinearBlendFunc *MoveLeftBlend3 = new lrLinearBlendFunc(StepTransitionTime,0.0,1.0);
	lrSkeletonBlendState *MoveLeft3 = new lrSkeletonBlendState(StepLeftStepping2,StepLeftStepping1,MoveLeftBlend3,StepTransitionTime);

	lrLinearBlendFunc *StepLeftAnimBlend_StartBlend = new lrLinearBlendFunc(MoveTransitionTime1,0.0,1.0);
	lrCharacterAnimBlend *StepLeftAnimBlend_Start = new lrCharacterAnimBlend(Standing,StepLeft,StepLeftAnimBlend_StartBlend,MoveTransitionTime1);

	lrCharacterAnimPlay *StepLeftAnimPlay = new lrCharacterAnimPlay(StepLeft,MoveTransitionTime2);

	lrLinearBlendFunc *MoveLeftAnimBlend_StartBlend = new lrLinearBlendFunc(MoveTransitionTime1,0.0,1.0);
	lrCharacterAnimBlend *MoveLeftAnimBlend_Start = new lrCharacterAnimBlend(StepLeft,MoveLeft1,MoveLeftAnimBlend_StartBlend,MoveTransitionTime1);

	lrCharacterAnimPlay *MoveLeft1Play = new lrCharacterAnimPlay(MoveLeft1,MoveTransitionTime2);

	lrLinearBlendFunc *MoveLeftAnimBlend_1Blend = new lrLinearBlendFunc(MoveTransitionTime1,0.0,1.0);
	lrCharacterAnimBlend *MoveLeftAnimBlend_1 = new lrCharacterAnimBlend(MoveLeft1,MoveLeft2,MoveLeftAnimBlend_1Blend,MoveTransitionTime1);

	lrCharacterAnimPlay *MoveLeft2Play = new lrCharacterAnimPlay(MoveLeft2,MoveTransitionTime2);

	lrLinearBlendFunc *MoveLeftAnimBlend_2Blend = new lrLinearBlendFunc(MoveTransitionTime1,0.0,1.0);
	lrCharacterAnimBlend *MoveLeftAnimBlend_2 = new lrCharacterAnimBlend(MoveLeft2,MoveLeft3,MoveLeftAnimBlend_2Blend,MoveTransitionTime1);

	lrCharacterAnimPlay *MoveLeft3Play = new lrCharacterAnimPlay(MoveLeft3,MoveTransitionTime2);

	lrLinearBlendFunc *MoveLeftAnimBlend_3Blend = new lrLinearBlendFunc(MoveTransitionTime1,0.0,1.0);
	lrCharacterAnimBlend *MoveLeftAnimBlend_3 = new lrCharacterAnimBlend(MoveLeft3,MoveLeft1,MoveLeftAnimBlend_3Blend,MoveTransitionTime1);

	/*
	 * Állapotátmenetek
	 */

	lAnimStateTransition *ToStepLeft = new lAnimStateTransition(this,true,StepLeftAnimBlend_Start);
	MoveLeftEvent->AddCallback(ToStepLeft);

	lAnimStateTransition *ToStepLeftPlay = new lAnimStateTransition(this,true,StepLeftAnimPlay);
	StepLeftAnimBlend_Start->GetEndEvent()->AddCallback(ToStepLeftPlay);

	lAnimStateTransition *ToMoveLeft1 = new lAnimStateTransition(this,true,MoveLeftAnimBlend_Start);
	StepLeftAnimPlay->GetEndEvent()->AddCallback(ToMoveLeft1);

	lAnimStateTransition *ToMoveLeft1Play = new lAnimStateTransition(this,true,MoveLeft1Play);
	MoveLeftAnimBlend_Start->GetEndEvent()->AddCallback(ToMoveLeft1Play);
	MoveLeftAnimBlend_3->GetEndEvent()->AddCallback(ToMoveLeft1Play);

	lAnimStateTransition *ToMoveLeft2 = new lAnimStateTransition(this,true,MoveLeftAnimBlend_1);
	MoveLeft1Play->GetEndEvent()->AddCallback(ToMoveLeft2);

	lAnimStateTransition *ToMoveLeft2Play = new lAnimStateTransition(this,true,MoveLeft2Play);
	MoveLeftAnimBlend_1->GetEndEvent()->AddCallback(ToMoveLeft2Play);

	lAnimStateTransition *ToMoveLeft3 = new lAnimStateTransition(this,true,MoveLeftAnimBlend_2);
	MoveLeft2Play->GetEndEvent()->AddCallback(ToMoveLeft3);

	lAnimStateTransition *ToMoveLeft3Play = new lAnimStateTransition(this,true,MoveLeft3Play);
	MoveLeftAnimBlend_2->GetEndEvent()->AddCallback(ToMoveLeft3Play);

	lAnimStateTransition *ToMoveLeft4 = new lAnimStateTransition(this,true,MoveLeftAnimBlend_3);
	MoveLeft3Play->GetEndEvent()->AddCallback(ToMoveLeft4);



	/*
	 * Move right
	 */

	lrLinearBlendFunc *StepRightBlend = new lrLinearBlendFunc(StepTransitionTime,0.0,1.0);
	lrSkeletonBlendState *StepRight = new lrSkeletonBlendState(StepSidewaysStanding,StepRightStepping1,StepRightBlend,StepTransitionTime);

	lrLinearBlendFunc *MoveRightBlend1 = new lrLinearBlendFunc(StepTransitionTime,0.0,1.0);
	lrSkeletonBlendState *MoveRight1 = new lrSkeletonBlendState(StepRightStepping1,StepSideways,MoveRightBlend1,StepTransitionTime);

	lrLinearBlendFunc *MoveRightBlend2 = new lrLinearBlendFunc(StepTransitionTime,0.0,1.0);
	lrSkeletonBlendState *MoveRight2 = new lrSkeletonBlendState(StepSideways,StepRightStepping2,MoveRightBlend2,StepTransitionTime);

	lrLinearBlendFunc *MoveRightBlend3 = new lrLinearBlendFunc(StepTransitionTime,0.0,1.0);
	lrSkeletonBlendState *MoveRight3 = new lrSkeletonBlendState(StepRightStepping2,StepRightStepping1,MoveRightBlend3,StepTransitionTime);

	lrLinearBlendFunc *StepRightAnimBlend_StartBlend = new lrLinearBlendFunc(MoveTransitionTime1,0.0,1.0);
	lrCharacterAnimBlend *StepRightAnimBlend_Start = new lrCharacterAnimBlend(Standing,StepRight,StepRightAnimBlend_StartBlend,MoveTransitionTime1);

	lrCharacterAnimPlay *StepRightAnimPlay = new lrCharacterAnimPlay(StepRight,MoveTransitionTime2);

	lrLinearBlendFunc *MoveRightAnimBlend_StartBlend = new lrLinearBlendFunc(MoveTransitionTime1,0.0,1.0);
	lrCharacterAnimBlend *MoveRightAnimBlend_Start = new lrCharacterAnimBlend(StepRight,MoveRight1,MoveRightAnimBlend_StartBlend,MoveTransitionTime1);

	lrCharacterAnimPlay *MoveRight1Play = new lrCharacterAnimPlay(MoveRight1,MoveTransitionTime2);

	lrLinearBlendFunc *MoveRightAnimBlend_1Blend = new lrLinearBlendFunc(MoveTransitionTime1,0.0,1.0);
	lrCharacterAnimBlend *MoveRightAnimBlend_1 = new lrCharacterAnimBlend(MoveRight1,MoveRight2,MoveRightAnimBlend_1Blend,MoveTransitionTime1);

	lrCharacterAnimPlay *MoveRight2Play = new lrCharacterAnimPlay(MoveRight2,MoveTransitionTime2);

	lrLinearBlendFunc *MoveRightAnimBlend_2Blend = new lrLinearBlendFunc(MoveTransitionTime1,0.0,1.0);
	lrCharacterAnimBlend *MoveRightAnimBlend_2 = new lrCharacterAnimBlend(MoveRight2,MoveRight3,MoveRightAnimBlend_2Blend,MoveTransitionTime1);

	lrCharacterAnimPlay *MoveRight3Play = new lrCharacterAnimPlay(MoveRight3,MoveTransitionTime2);

	lrLinearBlendFunc *MoveRightAnimBlend_3Blend = new lrLinearBlendFunc(MoveTransitionTime1,0.0,1.0);
	lrCharacterAnimBlend *MoveRightAnimBlend_3 = new lrCharacterAnimBlend(MoveRight3,MoveRight1,MoveRightAnimBlend_3Blend,MoveTransitionTime1);

	/*
	 * Állapotátmenetek
	 */

	lAnimStateTransition *ToStepRight = new lAnimStateTransition(this,true,StepRightAnimBlend_Start);
	MoveRightEvent->AddCallback(ToStepRight);

	lAnimStateTransition *ToStepRightPlay = new lAnimStateTransition(this,true,StepRightAnimPlay);
	StepRightAnimBlend_Start->GetEndEvent()->AddCallback(ToStepRightPlay);

	lAnimStateTransition *ToMoveRight1 = new lAnimStateTransition(this,true,MoveRightAnimBlend_Start);
	StepRightAnimPlay->GetEndEvent()->AddCallback(ToMoveRight1);

	lAnimStateTransition *ToMoveRight1Play = new lAnimStateTransition(this,true,MoveRight1Play);
	MoveRightAnimBlend_Start->GetEndEvent()->AddCallback(ToMoveRight1Play);
	MoveRightAnimBlend_3->GetEndEvent()->AddCallback(ToMoveRight1Play);

	lAnimStateTransition *ToMoveRight2 = new lAnimStateTransition(this,true,MoveRightAnimBlend_1);
	MoveRight1Play->GetEndEvent()->AddCallback(ToMoveRight2);

	lAnimStateTransition *ToMoveRight2Play = new lAnimStateTransition(this,true,MoveRight2Play);
	MoveRightAnimBlend_1->GetEndEvent()->AddCallback(ToMoveRight2Play);

	lAnimStateTransition *ToMoveRight3 = new lAnimStateTransition(this,true,MoveRightAnimBlend_2);
	MoveRight2Play->GetEndEvent()->AddCallback(ToMoveRight3);

	lAnimStateTransition *ToMoveRight3Play = new lAnimStateTransition(this,true,MoveRight3Play);
	MoveRightAnimBlend_2->GetEndEvent()->AddCallback(ToMoveRight3Play);

	lAnimStateTransition *ToMoveRight4 = new lAnimStateTransition(this,true,MoveRightAnimBlend_3);
	MoveRight3Play->GetEndEvent()->AddCallback(ToMoveRight4);

	StateTransitions.resize(20);
	StateTransitions[0] = ToStepBlend;
	StateTransitions[1] = ToRunBlend;
	StateTransitions[2] = ToRun;
	StateTransitions[3] = ToStand;
	StateTransitions[4] = ToStepLeft;
	StateTransitions[5] = ToMoveLeft1;
	StateTransitions[6] = ToMoveLeft1Play;
	StateTransitions[7] = ToMoveLeft2;
	StateTransitions[8] = ToMoveLeft2Play;
	StateTransitions[9] = ToMoveLeft3;
	StateTransitions[10] = ToMoveLeft3Play;
	StateTransitions[11] = ToMoveLeft4;
	StateTransitions[12] = ToStepRight;
	StateTransitions[13] = ToMoveRight1;
	StateTransitions[14] = ToMoveRight1Play;
	StateTransitions[15] = ToMoveRight2;
	StateTransitions[16] = ToMoveRight2Play;
	StateTransitions[17] = ToMoveRight3;
	StateTransitions[18] = ToMoveRight3Play;
	StateTransitions[19] = ToMoveRight4;

	Events.resize(5);
	Events[0] = MoveForwardEvent;
	Events[1] = MoveBackwardEvent;
	Events[2] = MoveLeftEvent;
	Events[3] = MoveRightEvent;
	Events[4] = StopMovingEvent;

	EventMap["MoveForward"] = MoveForwardEvent;
	EventMap["MoveBackward"] = MoveBackwardEvent;
	EventMap["MoveLeft"] = MoveLeftEvent;
	EventMap["MoveRight"] = MoveRightEvent;
	EventMap["StopMoving"] = StopMovingEvent;

	KeyframeAnimStates.resize(29);
	KeyframeAnimStates[0] = Standing;
	KeyframeAnimStates[1] = Step_Fwd;
	KeyframeAnimStates[2] = Run_Fwd;
	KeyframeAnimStates[3] = Step_Blend;
	KeyframeAnimStates[4] = Run_Blend;
	KeyframeAnimStates[5] = StepLeft;
	KeyframeAnimStates[6] = MoveLeft1;
	KeyframeAnimStates[7] = MoveLeft2;
	KeyframeAnimStates[8] = MoveLeft3;
	KeyframeAnimStates[9] = StepLeftAnimBlend_Start;
	KeyframeAnimStates[10] = MoveLeftAnimBlend_Start;
	KeyframeAnimStates[11] = MoveLeftAnimBlend_1;
	KeyframeAnimStates[12] = MoveLeftAnimBlend_2;
	KeyframeAnimStates[13] = MoveLeftAnimBlend_3;
	KeyframeAnimStates[14] = MoveLeft1Play;
	KeyframeAnimStates[15] = MoveLeft2Play;
	KeyframeAnimStates[16] = MoveLeft3Play;
	KeyframeAnimStates[17] = StepRight;
	KeyframeAnimStates[18] = MoveRight1;
	KeyframeAnimStates[19] = MoveRight2;
	KeyframeAnimStates[20] = MoveRight3;
	KeyframeAnimStates[21] = StepRightAnimBlend_Start;
	KeyframeAnimStates[22] = MoveRightAnimBlend_Start;
	KeyframeAnimStates[23] = MoveRightAnimBlend_1;
	KeyframeAnimStates[24] = MoveRightAnimBlend_2;
	KeyframeAnimStates[25] = MoveRightAnimBlend_3;
	KeyframeAnimStates[26] = MoveRight1Play;
	KeyframeAnimStates[27] = MoveRight2Play;
	KeyframeAnimStates[28] = MoveRight3Play;

	BeginState = Standing;
	CurrentState = Standing;
}
