#ifndef LI_CHAR_ANIM_FACTORY_H
#define LI_CHAR_ANIM_FACTORY_H

#include "../lRenderer/lrCharacterAnim/liCharacter.h"
#include "../lRenderer/lrCharacterAnim/liKeyframeAnimation.h"

class liCharAnimFactory
{
public:
	virtual liCharacter *CreateCharacter(liSkeleton &bind_pose_skeleton,unsigned int num_meshes) = 0;
	virtual liKeyframeAnimation *CreateKeyframeAnimation(unsigned int frame_rate,unsigned int num_frames,unsigned int num_bones) = 0;

	liCharAnimFactory(){}
	virtual ~liCharAnimFactory(){}
};

#endif // LI_CHAR_ANIM_FACTORY_H
