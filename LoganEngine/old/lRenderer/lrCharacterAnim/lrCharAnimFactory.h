#ifndef LR_CHAR_FACTORY_H
#define LR_CHAR_FACTORY_H

#include "../../lInterfaces/lResourceManager/liCharAnimFactory.h"

#include "lrCharacter.h"
#include "lrKeyframeAnimation.h"

class lrCharAnimFactory : public liCharAnimFactory
{
public:

	virtual liCharacter *CreateCharacter(liSkeleton &bind_pose_skeleton,unsigned int num_meshes) override
	{
		return new lrCharacter(bind_pose_skeleton,num_meshes);
	}

	virtual liKeyframeAnimation *CreateKeyframeAnimation(unsigned int frame_rate,unsigned int num_frames,unsigned int num_bones) override
	{
		return new lrKeyframeAnimation(frame_rate,num_frames,num_bones);
	}

	lrCharAnimFactory(){}
	virtual ~lrCharAnimFactory()override{}
};

#endif // LR_CHAR_FACTORY_H
