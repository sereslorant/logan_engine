#ifndef LR_GL3D_DUMB_LAYER_H
#define LR_GL3D_DUMB_LAYER_H

#include <lRenderer/lrRenderer/lrLayer.h>
#include <lRenderer/lrRenderer/lGLRenderer/lGLResources/lrGLResourceLoader.h>

#include <lRenderer/lrRenderer/lrUtils.h>
#include <lRenderer/lrRenderer/lGLRenderer/lGLUtils/lrGLRenderUtils.h>


#include <lRenderer/lrRenderer/lGLRenderer/lGLShaders/lrGLShader.h>
#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lGL3DShaders/lrGLStaticMeshShader.h>

#include <map>

class lrGLMeshVAORepository
{
private:
	lrGLShader &Shader;
	std::map<std::string,lrGL3StaticMeshVAO> MeshBindings;
	
public:
	
	lrGL3StaticMeshVAO &GetVAO(const std::string &model_name,lrGLStaticMaterialGroupView &static_mat_group)
	{
		std::string Key = model_name + "/" + static_mat_group.GetMaterial();
		
		lrGL3StaticMeshVAO &MeshBinding = MeshBindings[Key];
		
		lrGLStaticMeshShader static_mesh_shader(Shader);
		MeshBinding.Construct(static_mesh_shader,static_mat_group);
		
		return MeshBinding;
	}
	
	lrGLMeshVAORepository(lrGLShader &shader)
		:Shader(shader)
	{}
};

#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lrGL3DTextureBindingState.h>



class lrGL3DMeshRenderer : public li3DMeshVisitor
{
private:
	lrGLShader &StaticMeshShader;
	lrGLResourceLoader &ResourceLoader;
	
	lrGLMeshVAORepository &MeshBindings;
	
	lrGL3DTextureBindingState &TextureBindingState;
	
	lrGLLightArray &PointLights;
	
	const liMaterialLibrary &MaterialLibrary;
	
	bool BindAlbedo;
	bool BindEnvmap;
	
	void Draw(lrGLStaticMaterialGroupView &MtlGroup,lrGL3StaticMeshVAO &mesh_binding)
	{
		const liMaterial &Material = MaterialLibrary.GetMaterial(MtlGroup.GetMaterial());
		
		lrGLPhysicallyBasedShader PbShader(StaticMeshShader);
		lrGLRenderUtils::UploadUniformMaterial(PbShader,lrGLMaterialData(Material));
		
		
		lrGLTexture2DView AlbedoMap;
		lrGLTextureCubemapView EnvMap;
		
		if(BindAlbedo)
		{
			AlbedoMap = ResourceLoader.GetTexture(Material.GetAlbedoMap());
			
			TextureBindingState.BindAlbedoMap(AlbedoMap);
		}
		
		if(BindEnvmap)
		{
			EnvMap = ResourceLoader.GetCubemap("DUMMY");
			
			TextureBindingState.BindEnvironmentMap(EnvMap);
		}
		
		mesh_binding.Bind();
		if(PointLights.NumLights() > 0)
		{
			lrGLPointLightShader PointLightShader(StaticMeshShader);
			
			constexpr unsigned int MAX_LIGHT_COUNT = 256;
			
			unsigned int NumPasses = PointLights.NumLights() / MAX_LIGHT_COUNT;
			
			if(PointLights.NumLights() % MAX_LIGHT_COUNT > 0)
			{
				NumPasses += 1;
			}
			
			for(int i=0;i < NumPasses;i++)
			{
				unsigned int Size = std::min<std::size_t>(PointLights.NumLights() - i * MAX_LIGHT_COUNT,MAX_LIGHT_COUNT);
				
				glUniform1i(PointLightShader.GetLightCountLocation(),Size);
				lrGLRenderUtils::UploadUniformLightArray(PointLightShader,
														 PointLights.GetPositionBegin(i*MAX_LIGHT_COUNT),
														 PointLights.GetColorBegin(i*MAX_LIGHT_COUNT),
														 PointLights.GetIntensityBegin(i*MAX_LIGHT_COUNT),
														 Size
														);
				
				mesh_binding.Draw();
			}
		}
		else
		{
			mesh_binding.Draw();
		}
		
		glBindVertexArray(0);
	}
	
public:
	
	virtual void VisitStaticMesh(const li3DStaticMesh &static_mesh) override
	{
		lrGLStaticMeshView StaticMesh = ResourceLoader.GetStaticMesh(static_mesh.GetModelName());
		
		for(unsigned int i=0;i < StaticMesh.NumMtlGroups();i++)
		{
			lrGLStaticMaterialGroupView MtlGroup = StaticMesh.GetMaterialGroup(i);
			
			lrGL3StaticMeshVAO &MeshBinding = MeshBindings.GetVAO(static_mesh.GetModelName(),MtlGroup);
			
			Draw(MtlGroup,MeshBinding);
		}
	}
	
	virtual void VisitSkeletalMesh(const li3DSkeletalMesh &skeletal_mesh) override
	{
		lrGLStaticMeshView StaticMesh = ResourceLoader.GetSkeletalMesh(skeletal_mesh.GetCharacterName());
		
		for(unsigned int i=0;i < StaticMesh.NumMtlGroups();i++)
		{
			lrGLStaticMaterialGroupView MtlGroup = StaticMesh.GetMaterialGroup(i);
			
			lrGL3StaticMeshVAO &MeshBinding = MeshBindings.GetVAO(skeletal_mesh.GetCharacterName(),MtlGroup);
			
			Draw(MtlGroup,MeshBinding);
		}
	}
	
	lrGL3DMeshRenderer(lrGLShader &static_mesh_shader,lrGLResourceLoader &resource_loader,lrGLMeshVAORepository &mesh_bindings,lrGL3DTextureBindingState &texture_binding_state,lrGLLightArray &point_lights,const liMaterialLibrary &material_library,bool bind_albedo,bool bind_envmap)
		:StaticMeshShader(static_mesh_shader),ResourceLoader(resource_loader),MeshBindings(mesh_bindings),TextureBindingState(texture_binding_state),PointLights(point_lights),MaterialLibrary(material_library),BindAlbedo(bind_albedo),BindEnvmap(bind_envmap)
	{}
	
	virtual ~lrGL3DMeshRenderer(){}
};

class lGL3DSceneRenderer : public li3DSceneDrawer, public li3DElementVisitor
{
private:
	lrGLShader &StaticMeshShader;
	lrGLResourceLoader &ResourceLoader;
	
	lrGLMeshVAORepository &MeshBindings;
	
	lrGL3DTextureBindingState &TextureBindingState;
	
	const lmMatrix4x4 &ProjectionMatrix;
	const lmMatrix4x4 &ViewMatrix;
	
	lrGLLightArray &PointLights;
	
	bool BindAlbedo;
	bool BindEnvmap;
	
public:
	
	virtual void VisitMesh(const li3DMesh &mesh) override
	{
		lmMatrix4x4 ModelMatrix(lmMatrix4x4::IDENTITY);
		lrUtils::GetModelMatrix(mesh,ModelMatrix);
		
		lmMatrix3x3 NormalMatrix;
		
		lmMatrix3x3 InvModelMatrix;
		lmInverse(ModelMatrix.GetSubMatrix(3,3),InvModelMatrix);
		InvModelMatrix.Transpose(NormalMatrix);
		
		lmMatrix4x4 MvpMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix;
		lrGLTransform CurrentTransform(ModelMatrix,NormalMatrix,MvpMatrix);
		
		lrGL3DShader MeshShader(StaticMeshShader);
		
		lrGLRenderUtils::UploadUniformTransform(MeshShader,CurrentTransform);
		
		lrGL3DMeshRenderer MeshRenderer(StaticMeshShader,ResourceLoader,MeshBindings,TextureBindingState,PointLights,mesh.GetMaterialLibrary(),BindAlbedo,BindEnvmap);
		
		mesh.Accept(MeshRenderer);
	}
	
	virtual void VisitLight(const li3DLight &light) override
	{
		
	}
	
	virtual void Draw(const li3DElement &element) override
	{
		//lrGLVertexArray VertexArray;
		
		//VertexArray.Create();
		//VertexArray.Bind();
		
		if(!element.IsHidden())
		{
			element.Accept(*this);
		}
	}
	
	lGL3DSceneRenderer(lrGLShader &static_mesh_shader,lrGLResourceLoader &resource_loader,lrGLMeshVAORepository &mesh_bindings,lrGL3DTextureBindingState &texture_binding_state,const lmMatrix4x4 &projection_matrix,const lmMatrix4x4 &view_matrix,lrGLLightArray &point_lights,bool bind_albedo,bool bind_envmap)
		:StaticMeshShader(static_mesh_shader),ResourceLoader(resource_loader),MeshBindings(mesh_bindings),TextureBindingState(texture_binding_state),ProjectionMatrix(projection_matrix),ViewMatrix(view_matrix),PointLights(point_lights),BindAlbedo(bind_albedo),BindEnvmap(bind_envmap)
	{}
	
	virtual ~lGL3DSceneRenderer() override
	{}
};



class lGL3DLightLoader : public li3DSceneDrawer, public li3DElementVisitor
{
public:
	lrGLLightArray PointLights;
	
	virtual void VisitMesh(const li3DMesh &mesh) override
	{
		
	}
	
	virtual void VisitLight(const li3DLight &light) override
	{
		//PointLights.emplace_back(light);
		unsigned int LastId = PointLights.NumLights();
		PointLights.Resize(PointLights.NumLights() + 1);
		PointLights.SetLight(light,LastId);
	}
	
	virtual void Draw(const li3DElement &element) override
	{
		if(!element.IsHidden())
		{
			element.Accept(*this);
		}
	}
	
	lGL3DLightLoader()
	{}
	
	virtual ~lGL3DLightLoader() override
	{}
};

class lGL3DDumbLayer : public lr3DLayer
{
private:
	lrGLShader &StaticPrePassShader;
	lrGLShader &StaticMeshShader;
	lrGLShader &StaticEnvMapShader;
	
	lrGLResourceLoader &ResourceLoader;
	
	lrGLMeshVAORepository MeshBindings;
	
	bool initialized = false;
	
	virtual void DrawScene() override
	{
		if(Frustum == nullptr || Camera == nullptr || Scene == nullptr)
		{return;}
		
		auto Before = std::chrono::steady_clock::now();
		lGL3DLightLoader LightLoader;
		Scene->Draw(LightLoader);
		auto After = std::chrono::steady_clock::now();
		
		std::chrono::nanoseconds Difference = After - Before;
		std::cout << "Light cache time = " << Difference.count() / 1000000.0 << "ms" << std::endl;
		
		lrGLLightArray EmptyPointLights;
		
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE); // Ez kell, hogy a clear hatással legyen a depth bufferre.
		glDepthFunc(GL_LEQUAL);
		
		glClearColor(0.0,0.0,0.0,1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		lmMatrix4x4 ProjectionMatrix(lmMatrix4x4::IDENTITY);
		lrUtils::GetProjectionMatrix(*Frustum,ProjectionMatrix);
		
		lmMatrix4x4 ViewMatrix(lmMatrix4x4::IDENTITY);
		lrUtils::GetViewMatrix(*Camera,ViewMatrix);
		
		lmVector3D CameraPosition = Camera->GetPosition();
		
		lrGL3DTextureBindingState TextureBindingState;
		
		glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
		
		glDisable(GL_BLEND);
		
		StaticPrePassShader.UseProgram();
		
		Before = std::chrono::steady_clock::now();
		lGL3DSceneRenderer PrePassSceneRenderer(StaticPrePassShader,ResourceLoader,MeshBindings,TextureBindingState,ProjectionMatrix,ViewMatrix,EmptyPointLights,false,false);
		Scene->Draw(PrePassSceneRenderer);
		After = std::chrono::steady_clock::now();
		
		Difference = After - Before;
		std::cout << "Pre pass time = " << Difference.count() / 1000000.0 << "ms" << std::endl;
		
		StaticPrePassShader.DisableProgram();
		
		glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
		
		StaticEnvMapShader.UseProgram();
		
		lrGLPhysicallyBasedShader EnvPbShader(StaticEnvMapShader);
		TextureBindingState.SetShader(&EnvPbShader);
		
		lrGL3DShader Shader1(StaticEnvMapShader);
		glUniform3fv(Shader1.GetCameraPositionLocation(),1,&CameraPosition[0]);
		
		Before = std::chrono::steady_clock::now();
		lGL3DSceneRenderer EnvMapSceneRenderer(StaticEnvMapShader,ResourceLoader,MeshBindings,TextureBindingState,ProjectionMatrix,ViewMatrix,EmptyPointLights,true,true);
		Scene->Draw(EnvMapSceneRenderer);
		After = std::chrono::steady_clock::now();
		
		Difference = After - Before;
		std::cout << "EnvMap time = " << Difference.count() / 1000000.0 << "ms" << std::endl;
		
		StaticEnvMapShader.DisableProgram();
		
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE,GL_ONE);
		
		//bool first = true;
		//bool no_need_for_more = false;
		
		Before = std::chrono::steady_clock::now();
		
		StaticMeshShader.UseProgram();
		
		lrGLPhysicallyBasedShader PointLightPbShader(StaticEnvMapShader);
		TextureBindingState.SetShader(&PointLightPbShader);
		
		lrGLPointLightShader PointLightShader(StaticMeshShader);
		
		lrGL3DShader Shader2(StaticEnvMapShader);
		glUniform3fv(Shader2.GetCameraPositionLocation(),1,&CameraPosition[0]);
		lGL3DSceneRenderer SceneRenderer(StaticMeshShader,ResourceLoader,MeshBindings,TextureBindingState,ProjectionMatrix,ViewMatrix,LightLoader.PointLights,true,false);
		Scene->Draw(SceneRenderer);
		After = std::chrono::steady_clock::now();
		
		Difference = After - Before;
		std::cout << "Point light time = " << Difference.count() / 1000000.0 << "ms" << std::endl;
		
		StaticMeshShader.DisableProgram();
	}
	
public:
	
	lGL3DDumbLayer(lrGLShader &static_prepass_shader,lrGLShader &static_mesh_shader,lrGLShader &static_env_map_shader,lrGLResourceLoader &resource_loader)
		:StaticPrePassShader(static_prepass_shader),StaticMeshShader(static_mesh_shader),StaticEnvMapShader(static_env_map_shader),ResourceLoader(resource_loader),MeshBindings(static_mesh_shader)
	{}
	
	virtual ~lGL3DDumbLayer() override
	{}
};

#include <lRenderer/lrRenderer/lGLRenderer/lrGLViewport.h>

class lrGL3DDumbLayerFactory : public liLayerFactory
{
private:
	lrGL3RenderState3D &RenderState3D;
	
	lrGLResourceLoader &ResourceLoader;
	
public:
	
	virtual lrLayer *CreateLayer() override
	{
		return new lGL3DDumbLayer(RenderState3D.GetStaticMeshPrePassProgram(),RenderState3D.GetStaticMeshPointLightProgram(),RenderState3D.GetStaticMeshEnvMapProgram(),ResourceLoader);
	}
	
	lrGL3DDumbLayerFactory(lrGL3RenderState3D &render_state_3D,lrGLResourceLoader &resource_loader)
		:RenderState3D(render_state_3D),ResourceLoader(resource_loader)
	{}
	virtual ~lrGL3DDumbLayerFactory()
	{}
};

#endif // LR_GL3D_DUMB_LAYER_H
