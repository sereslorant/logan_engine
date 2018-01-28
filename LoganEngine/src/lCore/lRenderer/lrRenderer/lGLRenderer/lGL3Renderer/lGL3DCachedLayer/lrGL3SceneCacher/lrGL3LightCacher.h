#ifndef LR_GL3_LIGHT_CACHER_H
#define LR_GL3_LIGHT_CACHER_H

#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lGL3DCachedLayer/lrGL3SceneCache/lrGLData/lrGLLightArray.h>

class lrGL3LightCacher
{
private:
	lrGLLightArray *Lights= nullptr;
	unsigned int Next;
	
public:
	
	bool Initialized()
	{
		return Lights != nullptr;
	}
	
	void Initialize(lrGLLightArray *lights)
	{
		Lights = lights;
		Next = 0;
	}
	
	void SetNextLight(const li3DLight &light)
	{
		if(Initialized())
		{
			Lights->SetLight(light,Next);
			Next++;
		}
	}
	
	lrGL3LightCacher()
	{}
	
	~lrGL3LightCacher()
	{}
	/*
	 * End of class
	 */
};

#endif // LR_GL3_LIGHT_CACHER_H
