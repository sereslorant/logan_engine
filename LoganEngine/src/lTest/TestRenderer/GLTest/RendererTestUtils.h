#ifndef GL3_TEST_UTILS_H
#define GL3_TEST_UTILS_H

lrmStaticMesh *CreateSphereMesh()
{
	lrmStaticMesh *StaticMesh = new lrmStaticMesh;
	lrmResourceManager::GenerateSphere(*StaticMesh,64,32);
	
	return StaticMesh;
}

bool LoadSkeletalMesh(std::vector<lrmSkeletalMesh> &skeletal_meshes,md5SkeletonExtrinsic &bind_pose_skeleton,md5SkeletonHierarchy &skeleton)
{
	md5File File;
	
	std::fstream mesh;
	mesh.open("bob_lamp_update_export.md5mesh");
	
	if(!mesh.is_open())
	{
		std::cout << "Error couldn't open mesh file!" << std::endl;
		return false;
	}
	
	md5LoadFile(mesh,File);
	
	skeletal_meshes.resize(File.Meshes.size());
	for(unsigned int i=0;i < skeletal_meshes.size();i++)
	{
		md5ExtractSkeletalMesh(File,true,i,skeletal_meshes[i]);
	}
	
	md5Utility::Skeleton_YUp(File.BindPoseSkeleton,bind_pose_skeleton);
	
	skeleton = File.BindPoseSkeletonMetadata;
	
	return true;
}

bool LoadAnim(std::vector<md5SkeletonExtrinsic> &frames,md5SkeletonHierarchy &anim_skeleton_metadata)
{
	md5Anim Anim;
	
	std::fstream anim;
	anim.open("retardaltkodik_with_lamp.md5anim");
	
	if(!anim.is_open())
	{
		std::cout << "Error couldn't open anim file!" << std::endl;
		return false;
	}
	
	md5LoadAnim(anim,Anim);
	
	frames.resize(Anim.NumFrames);
	for(unsigned int i=0;i < frames.size();i++)
	{
		md5ExtractAnimFrame(Anim,true,i,frames[i]);
	}
	
	anim_skeleton_metadata = Anim.SkeletonHierarchy;
	
	return true;
}

#endif // GL3_TEST_UTILS_H
