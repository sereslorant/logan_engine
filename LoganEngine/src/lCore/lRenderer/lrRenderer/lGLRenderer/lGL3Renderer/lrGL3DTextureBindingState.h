#ifndef LR_GL_3D_TEXTURE_BINDING_STATE_H
#define LR_GL_3D_TEXTURE_BINDING_STATE_H


class lrGL3DTextureBindingState
{
private:
	liGLPbMatShader *Shader = nullptr;
	
	static constexpr unsigned int NUM_ACTIVE_TEXTURES = 2;
	
	GLuint BoundTextureId[NUM_ACTIVE_TEXTURES];
	
	template<class lrGLTextureView_T>
	void BindTexture(GLint texture_location,GLuint active_texture,lrGLTextureView_T &texture)
	{
		if(BoundTextureId[active_texture] != texture.GetTextureId())
		{
			glActiveTexture(GL_TEXTURE0 + active_texture);
			texture.Bind();
			
			BoundTextureId[active_texture] = texture.GetTextureId();
		}
		
		glUniform1i(texture_location,active_texture);
	}
	
	static constexpr GLuint ALBEDO_MAP_ACTIVE_TEXTURE = 0;
	static constexpr GLuint ENVIRONMENT_MAP_ACTIVE_TEXTURE = 1;
	
public:
	
	void SetShader(liGLPbMatShader *shader)
	{
		Shader = shader;
	}
	
	void Reset()
	{
		for(unsigned int i=0;i < NUM_ACTIVE_TEXTURES;i++)
		{
			BoundTextureId[i] = 0;
		}
	}
	
	void BindAlbedoMap(lrGLTexture2DView &albedo_map)
	{
		if(Shader != nullptr)
		{
			BindTexture(Shader->GetAlbedoMapLocation(),ALBEDO_MAP_ACTIVE_TEXTURE,albedo_map);
		}
	}
	
	void BindEnvironmentMap(lrGLTextureCubemapView &environment_map)
	{
		if(Shader != nullptr)
		{
			BindTexture(Shader->GetEnvironmentMapLocation(),ENVIRONMENT_MAP_ACTIVE_TEXTURE,environment_map);
		}
	}
	
	lrGL3DTextureBindingState()
	{
		Reset();
	}
	
	~lrGL3DTextureBindingState()
	{
		//
	}
	/*
	 * End of class
	 */
};

#endif // LR_GL_3D_TEXTURE_BINDING_STATE_H
