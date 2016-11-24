#ifndef LR_GL_3_3_POST_PROCESSOR_H
#define LR_GL_3_3_POST_PROCESSOR_H


#include <list>


#include "../lrGLShader/lrGLPostProcessShader.h"
#include "../lrGLViewportSizeQuad.h"

#include "../lrGLPingPongBuffer.h"

class lrGL3_3Blur
{
private:
	lrGLPostProcessShader *BlurShader;
	lrGLViewportSizeQuad *ViewportSizeQuad;

public:

	void Apply(lrGLPingPongBuffer *target,unsigned int is_vertical)
	{
		lmVector2D PixelSize = {1.0/target->GetWidth(),1.0/target->GetHeight()};

		target->Enable();

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		glViewport(0,0,target->GetWidth(),target->GetHeight());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		BlurShader->UseProgram();

		glUniform2fv(glGetUniformLocation(BlurShader->GetProgramId(),"lr_PixelSize"),1,(const float*)&PixelSize);
		glUniform1i(glGetUniformLocation(BlurShader->GetProgramId(),"lr_IsVertical"),is_vertical);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,target->GetImg()->GetId());
		glUniform1i(glGetUniformLocation(BlurShader->GetProgramId(),"lr_PrevRenderedImg"),0);

		ViewportSizeQuad->Draw();

		BlurShader->DisableProgram();

		target->Disable();
	}

	lrGL3_3Blur(lrGLPostProcessShader *blur_shader,lrGLViewportSizeQuad *viewport_size_quad)
		:BlurShader(blur_shader),ViewportSizeQuad(viewport_size_quad)//,Width(width),Height(height),PixelSize(1.0/width,1.0/height)
	{
		//Üres függvénytörzs
	}

	virtual ~lrGL3_3Blur()
	{
		//Üres függvénytörzs
	}

	/*
	 * Class vége
	 */
};


#include "../lrGLShader/lrGLPostProcessShader.h"
#include "../lrGLViewportSizeQuad.h"

#include "../lrGLPingPongBuffer.h"

class lrGL3_3Bloom
{
private:
	lrGLPostProcessShader *BloomFilter;
	lrGLPostProcessShader *CopyShader;

	lrGL3_3Blur *Blur;
	lrGLViewportSizeQuad *ViewportSizeQuad;

	lrGLPingPongBuffer *BloomBuffer;

	void FillBloomBuffer(lrGLPingPongBuffer *target,const lmVector3D &min_intensity)
	{
		BloomBuffer->Enable();

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		glViewport(0,0,target->GetWidth(),target->GetHeight());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		BloomFilter->UseProgram();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,target->GetImg()->GetId());
		glUniform1i(glGetUniformLocation(BloomFilter->GetProgramId(),"lr_PrevRenderedImg"),0);
		glUniform3fv(glGetUniformLocation(BloomFilter->GetProgramId(),"lr_MinIntensity"),1,(const float*)&min_intensity);

		ViewportSizeQuad->Draw();

		BloomFilter->DisableProgram();

		BloomBuffer->Disable();
	}

	void DrawBloomBufferToTarget(lrGLPingPongBuffer *target)
	{
		//FÚÚÚÚÚÚÚÚÚÚÚÚÚÚJ de undorító dolgot csinálok. xPPPPPPPPPPPPP
		//Ezt kurvára javítani kell. xPPP
		//Vagy mégsem? Újra át kéne olvasni az egészet. -_-

		target->Enable();

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);

		glViewport(0,0,target->GetWidth(),target->GetHeight());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		CopyShader->UseProgram();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,BloomBuffer->GetImg()->GetId());
		glUniform1i(glGetUniformLocation(CopyShader->GetProgramId(),"lr_PrevRenderedImg"),0);

		ViewportSizeQuad->Draw();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,target->GetImg()->GetId());
		glUniform1i(glGetUniformLocation(CopyShader->GetProgramId(),"lr_PrevRenderedImg"),0);

		ViewportSizeQuad->Draw();

		CopyShader->DisableProgram();

		target->Disable();
	}

public:

	void Apply(lrGLPingPongBuffer *target,const lmVector3D &min_intensity,int blur_count)
	{
		BloomBuffer->Resize(target->GetWidth(),target->GetHeight());
		FillBloomBuffer(target,min_intensity);
		BloomBuffer->Flip();

		int IsVertical = 0;

		for(int i=0;i < blur_count*2;i++)
		{
			Blur->Apply(BloomBuffer,IsVertical);
			IsVertical = !IsVertical;

			BloomBuffer->Flip();
		}

		BloomBuffer->Flip();
		DrawBloomBufferToTarget(target);
	}

	lrGL3_3Bloom(lrGLPostProcessShader *bloom_filter,lrGLPostProcessShader *copy_shader,lrGL3_3Blur *blur,lrGLViewportSizeQuad *viewport_size_quad/*,unsigned int width,unsigned int height,const lmVector3D &min_intensity*/)
		:BloomFilter(bloom_filter),CopyShader(copy_shader),Blur(blur),ViewportSizeQuad(viewport_size_quad)//,Width(width),Height(height),MinIntensity(min_intensity)
	{
		BloomBuffer = new lrGLPingPongBuffer(0,0);
	}

	virtual ~lrGL3_3Bloom()
	{
		delete BloomBuffer;
	}
};

class lrGL3_3PostProcessor : public liPostProcessor
{
private:
    lrGLPostProcessShader		*CopyShader;

    lrGLPostProcessShader		*BlurShader;
    lrGLPostProcessShader		*BloomFilter;

    lrGL3_3Blur *Blur;
    lrGL3_3Bloom *Bloom;

    class lrGL3_3PostProcessCommand : public liPostProcessCommand
    {
	protected:
    	lrGLPingPongBuffer *Target;

	public:

		void SetTarget(lrGLPingPongBuffer *target)
		{
			Target = target;
		}

		lrGL3_3PostProcessCommand()
		{
			Target = nullptr;
		}

		virtual ~lrGL3_3PostProcessCommand() override
		{

		}
    };

    std::list<lrGL3_3PostProcessCommand *> PostProcessCommands;


	class lrGL3_3BlurCommand : public lrGL3_3PostProcessCommand
	{
	protected:
		lrGL3_3Blur *Blur;

		unsigned int BlurCount;

	public:

		virtual void Apply() override
		{
			int IsVertical = 0;

			for(int i=0;i < BlurCount*2;i++)
			{
				Blur->Apply(Target,IsVertical);
				IsVertical = !IsVertical;

				Target->Flip();
			}
		}

		lrGL3_3BlurCommand(lrGL3_3Blur *blur,unsigned int blur_count)
			:Blur(blur),BlurCount(blur_count)
		{}

		virtual ~lrGL3_3BlurCommand() override {}
	};

	class lrGL3_3BloomCommand : public lrGL3_3PostProcessCommand
	{
	protected:
		lrGL3_3Bloom	*Bloom;

		lmVector3D		MinIntensity;
		unsigned int	BlurCount;

	public:

		virtual void Apply() override
		{
			Bloom->Apply(Target,MinIntensity,BlurCount);

			Target->Flip();
		}

		lrGL3_3BloomCommand(lrGL3_3Bloom *bloom,const lmVector3D &min_intensity,unsigned int blur_count)
			:Bloom(bloom),MinIntensity(min_intensity),BlurCount(blur_count)
		{}

		virtual ~lrGL3_3BloomCommand() override{}
	};

    class lrGL3_3PostProcessCommandFactory : public liPostProcessCommandFactory
    {
	private:
    	lrGL3_3Blur *Blur;
		lrGL3_3Bloom *Bloom;

	public:
		virtual liPostProcessCommand *CreateBlurCommand(unsigned int blur_count) override
		{
			return new lrGL3_3BlurCommand(Blur,blur_count);
		}

		virtual liPostProcessCommand *CreateBloomCommand(const lmVector3D &min_intensity,unsigned int blur_count) override
		{
			return new lrGL3_3BloomCommand(Bloom,min_intensity,blur_count);
		}

		lrGL3_3PostProcessCommandFactory(lrGL3_3Blur *blur,lrGL3_3Bloom *bloom)
			:Blur(blur),Bloom(bloom)
		{}

		virtual ~lrGL3_3PostProcessCommandFactory() override
		{}
    };

    liPostProcessCommandFactory *PostProcessCommandFactory;

    lrGLPingPongBuffer *Target;

public:

	void SetTarget(lrGLPingPongBuffer *target)
	{
		Target = target;
	}

	liPostProcessCommandFactory *GetCommandFactory() override
	{
		return PostProcessCommandFactory;
	}

	void AddPostProcessCommand(liPostProcessCommand *command) override
	{
		PostProcessCommands.push_back( (lrGL3_3PostProcessCommand *) command);
	}

	void ClearPostProcessCommands() override
	{
		PostProcessCommands.clear();
	}

	void ApplyPostProcess() override
	{
		//GLfloat ClearColor[4];
		//glGetFloatv(GL_COLOR_CLEAR_VALUE,ClearColor);

		//glClearColor(0.0,0.0,0.0,1.0);

		for(auto Command : PostProcessCommands)
		{
			Command->SetTarget(Target);

			Command->Apply();
		}

		//glClearColor(ClearColor[0],ClearColor[1],ClearColor[2],ClearColor[3]);
	}

	lrGL3_3PostProcessor(lrGLViewportSizeQuad *viewport_size_quad,lrGLPostProcessShader *copy_shader)
	{
		CopyShader = copy_shader;

		BlurShader = new lrGLPostProcessShader("Content/Shaders/GL3/PostProcess/lrGL3_3PostProcess.vert" ,"Content/Shaders/GL3/PostProcess/lrGL3_3BlurShader.frag");
		BloomFilter = new lrGLPostProcessShader("Content/Shaders/GL3/PostProcess/lrGL3_3PostProcess.vert","Content/Shaders/GL3/PostProcess/lrGL3_3BloomFilter.frag");

		Blur = new lrGL3_3Blur(BlurShader,viewport_size_quad);
		Bloom = new lrGL3_3Bloom(BloomFilter,CopyShader,Blur,viewport_size_quad);

		PostProcessCommandFactory = new lrGL3_3PostProcessCommandFactory(Blur,Bloom);
	}

	virtual ~lrGL3_3PostProcessor() override
	{
		delete BlurShader;
		delete BloomFilter;

		delete Blur;
		delete Bloom;

		delete PostProcessCommandFactory;
	}
};

#endif // LR_GL_3_3_POST_PROCESSOR_H
