#ifndef LR_GL3_3_FORWARD_RENDERER_H
#define LR_GL3_3_FORWARD_RENDERER_H

#include "../lrGLShader/lrGLPostProcessShader.h"

#include "lrGL3_3StmShader.h"
#include "lrGL3_3SkmShader.h"
#include "lrGL3_3FwdStmPrePassShader.h"
#include "lrGL3_3FwdSkmPrePassShader.h"

#include "../lrGLGPUFactory.h"
#include "../../lrCharacterAnim/lrCharAnimFactory.h"

#include "../../../lInterfaces/lRenderer/liRenderer.h"

#include "../lrGL3_3GuiRenderer/lrGL3_3GuiRenderer.h"

#include "../lrGLStaticMesh.h"
#include "../lrGLSkeletalMesh.h"
#include "../lrGLImg.h"
#include "../lrGLFramebuffer.h"

#include "../../lrPointLight.h"
#include "../../lrCamera.h"
#include "../../lrFrustum.h"



#include "lrGL3_3PostProcessor.h"

#include "../lrGLViewportSizeQuad.h"
#include "../lrGLSkybox.h"

/** \brief OpenGL 3.3-at használó forward renderer
 *
 * \todo A display listet picikét normálisabban meg kéne csinálni, mert túl sokszor kell képeket bindolni, és emiatt lassú, továbbá be kéne üzemelni az instanced renderinget. Be kéne üzemelni a directional lightot.
 *
 * \warning Nagyon work in progress!
 *
 * \author Seres Lóránt Gábor
 *
 */
class lrGL3_3ForwardRenderer : public liRenderer
{
protected:

	class lrGL_Screen : public liFramebuffer
	{
	public:

		virtual lrImg *GetColorBuffer() override
		{
			return nullptr;
		}

		virtual void Enable()
		{
			glBindFramebuffer(GL_FRAMEBUFFER,0);
		}

		virtual void Disable()
		{
			glBindFramebuffer(GL_FRAMEBUFFER,0);
		}

		lrGL_Screen() {}

		virtual ~lrGL_Screen() override {}
	};



	liResourceManager *RM;

	lrGLPingPongBuffer	*PingPongBuffer;
	//lrGL3_3PingPongBuffer	*BloomBuffer;

	lrGL_Screen Screen;

    bool Transparency;

    lrGLGPUFactory *GPUFactory;
    lrCharAnimFactory *CharAnimFactory;

    lrGL3_3FwdStmPrePassShader	*StmPrePassShader;
	lrGL3_3FwdSkmPrePassShader	*SkmPrePassShader;

	lrGLPostProcessShader		*SkyboxShader;
    lrGLPostProcessShader		*CopyShader;

    lrGLViewportSizeQuad	*ViewportSizeQuad;
    lrGLSkybox				*Skybox;

    lrGL3_3PostProcessor *PostProcessor;

    lrGL3_3StmShader *StmShader; /**< Static mesh shader, vele rajzoljuk ki a static mesheket. */
    lrGL3_3SkmShader *SkmShader; /**< Skeletal mesh shader, vele rajzoljuk ki a skeletal mesheket. */

    lrGL3_3GuiRenderer *GuiRenderer = nullptr;

    lmMatrix4x4 ViewMatrix;         /**< View matrix, ami a modelleket world space-ből view space-be transzformálja. */
    lmMatrix4x4 ProjectionMatrix;   /**< Projection matrix. \todo rendesen dokumentálni kéne. */

    struct lrDLNode
    {
        liMultiMaterial &MultiMaterial;
        liSceneNode    &SceneNode;

        lrDLNode(liMultiMaterial &multi_material,liSceneNode &scene_node)
			:MultiMaterial(multi_material),SceneNode(scene_node)
        {

        }

        virtual ~lrDLNode()
        {}
    };

    /** \brief Ideiglenes class.
     * \warning Át lesz szabva rendesen.
     */
    struct lrDLNode_StaticMesh : public lrDLNode
    {
        liStaticMesh    &Mesh;
        //lrMultiMaterial &MultiMaterial;
        //lrsSceneNode    &SceneNode;

        lrDLNode_StaticMesh(liStaticMesh &mesh,liMultiMaterial &multi_material,liSceneNode &scene_node)
            :lrDLNode(multi_material,scene_node),Mesh(mesh)//,MultiMaterial(multi_material),SceneNode(scene_node)
        {

        }

        virtual ~lrDLNode_StaticMesh() override
        {

        }
    };

    /** \brief Ideiglenes class.
     * \warning Át lesz szabva rendesen.
     */
    struct lrDLNode_SkeletalMesh : public lrDLNode
    {
        liCharacter     &Mesh;
        liSkeleton      &Skeleton;
        //lrMultiMaterial &MultiMaterial;
        //lrsSceneNode    &SceneNode;

        lrDLNode_SkeletalMesh(liCharacter &mesh,liSkeleton &skeleton,liMultiMaterial &multi_material,liSceneNode &scene_node)
            :lrDLNode(multi_material,scene_node),Mesh(mesh),Skeleton(skeleton)//,MultiMaterial(multi_material),SceneNode(scene_node)
        {

        }

        virtual ~lrDLNode_SkeletalMesh() override
        {

        }
    };
    /*
    struct lrTreeNode
    {
        liMaterial		&Material;
        //lrsSceneNode    &SceneNode;

        lrTreeNode(liMaterial &material/*,lrsSceneNode &scene_node* /)
			:Material(material)/*,SceneNode(scene_node)* /
        {

        }

        virtual ~lrTreeNode()
        {}
    };

    struct lrTreeNode_StaticMesh : public lrTreeNode
    {
    	lrStaticMesh::lrMaterialGroup	&MaterialGroup;
    	//lrsStaticMeshNode				&StaticMeshNode;

    	lrTreeNode_StaticMesh(lrStaticMesh::lrMaterialGroup &material_group,liMaterial &material,lrsStaticMeshNode &static_mesh_node)
			:lrTreeNode(material),MaterialGroup(material_group)//,StaticMeshNode(static_mesh_node)
    	{

    	}

    	virtual ~lrTreeNode_StaticMesh() override {}
    };

    struct lrTreeNode_SkeletalMesh : public lrTreeNode
    {
        lrCharacter     	&Character;
    	lrSkeletalMesh		&SkeletalMesh;
        lrsSkeletalMeshNode &SkeletalMeshNode;

    	lrTreeNode_SkeletalMesh(lrCharacter &character,lrSkeletalMesh &skeletal_mesh,liMaterial &material,lrsSkeletalMeshNode &skeletal_mesh_node)
			:lrTreeNode(material),Character(character),SkeletalMesh(skeletal_mesh),SkeletalMeshNode(skeletal_mesh_node)
    	{

    	}

    	virtual ~lrTreeNode_SkeletalMesh() override {}
    };

    template <class T>
    class lrDisplayTreeNode
    {
    	std::string TextureName;
    	lrImg *Image = nullptr;

    	std::list<T> Elements;

    	lrDisplayTreeNode<T> *NoTextureChildren;
    	std::map<std::string,lrDisplayTreeNode<T> > TexturedChildren;

    	lrDisplayTreeNode(lrDisplayTreeNode *no_texture_children)
			:NoTextureChildren(no_texture_children)
    	{}

    	~lrDisplayTreeNode()
    	{
    		delete NoTextureChildren;
    	}
    };
    */

    std::list<lrDLNode_StaticMesh>   DL_StaticMesh;     /**< Display list, át lesz szabva rendesen. */
    std::list<lrDLNode_SkeletalMesh> DL_SkeletalMesh;   /**< Display list, át lesz szabva rendesen. */

    std::list<lrPointLight *> PointLights;

    const unsigned int N_POINT_LIGHTS = 32;

    /** \brief Ez a függvény tölti fel a model, view és a projection mátrixokat a videókártyára.
     *
     * \param Shader lrGLRendererShader*:               A shader, amelytől elkérjük a uniform locationöket.
     * \param ModelMatrix const lmMatrix4x4&    A mesh model mátrixa, ami body space-ből world space-be transzformálja.
     * \return void
     *
     */
    void UploadMatrices(lrGLRendererShader *Shader,const lmMatrix4x4 &ModelMatrix)
    {
		lmMatrix4x4 ModelViewMatrix = ViewMatrix*ModelMatrix;
		lmMatrix4x4 ModelViewProjectionMatrix = ProjectionMatrix*ModelViewMatrix;

		lmMatrix3x3 NormalMatrix;
		ModelMatrix.GetSubMatrix(3,3).Invert().Transpose(NormalMatrix);

		//Uploading matrices
		int ModelMatrixLocation = Shader->GetModelMatrixLocation();
		glUniformMatrix4fv(ModelMatrixLocation,1,GL_FALSE,&(ModelMatrix[0][0]));

		int ViewMatrixLocation = Shader->GetViewMatrixLocation();
		glUniformMatrix4fv(ViewMatrixLocation,1,GL_FALSE,&(ViewMatrix[0][0]));

		int ProjectionMatrixLocation = Shader->GetProjectionMatrixLocation();
		glUniformMatrix4fv(ProjectionMatrixLocation,1,GL_FALSE,&(ProjectionMatrix[0][0]));

		int ModelViewMatrixLocation = Shader->GetModelViewMatrixLocation();
		glUniformMatrix4fv(ModelViewMatrixLocation,1,GL_FALSE,&(ModelViewMatrix[0][0]));

		int ModelViewProjectionMatrixLocation = Shader->GetModelViewProjectionMatrixLocation();
		glUniformMatrix4fv(ModelViewProjectionMatrixLocation,1,GL_FALSE,&(ModelViewProjectionMatrix[0][0]));

		int NormalMatrixLocation = Shader->GetNormalMatrixLocation();
		glUniformMatrix3fv(NormalMatrixLocation,1,GL_FALSE,&(NormalMatrix[0][0]));
	}

    /** \brief Ez a függvény tölti fel a materialt a videókártyára.
     *
     * \param Shader lrGLRendererShader*:            A shader, amelytől elkérjük a uniform locationöket.
     * \param Material const lrMaterial*:    A material, amit feltöltünk.
     * \return void
     *
     */
    void UploadMaterial(lrGLRendererShader *Shader,const liMaterial *Material,bool EmissiveEnabled)
    {
		int MaterialDiffuseLocation = Shader->GetMaterialDiffuseLocation();
		glUniform3fv(MaterialDiffuseLocation,1,Material->GetDiffuse());

		int MaterialSpecularLocation = Shader->GetMaterialSpecularLocation();
		glUniform3fv(MaterialSpecularLocation,1,Material->GetSpecular());

		int MaterialShininessLocation = Shader->GetMaterialShininessLocation();
		glUniform1f(MaterialShininessLocation,Material->GetShininess());

		/*
		lrGLImg *DiffuseMap		= (lrGLImg *) RM->LoadImg(Material->DiffuseMap);
		lrGLImg *NormalMap		= (lrGLImg *) RM->LoadImg(Material->NormalMap);
		lrGLImg *SpecularMap	= (lrGLImg *) RM->LoadImg(Material->SpecularMap);
		lrGLImg *EmissiveMap	= (lrGLImg *) RM->LoadImg(Material->EmissiveMap);

		BindTexture(DiffuseMap,0,Shader->GetDiffuseMapLocation());
		BindTexture(NormalMap,1,Shader->GetNormalMapLocation());
		BindTexture(SpecularMap,2,Shader->GetSpecularMapLocation());

		if(EmissiveEnabled)
		{
			BindTexture(EmissiveMap,3,Shader->GetEmissiveMapLocation());
		}
		else
		{
			GLint HasEmissiveMapLocation = Shader->GetEmissiveMapLocation().GetHasTextureLocation();
			glUniform1i(HasEmissiveMapLocation,0);
		}
		*/

		///MIELŐTT BE LEHETNE KAPCSOLNI AZ ÁTLÁTSZÓSÁGOT, KELL VALAMI, AMIVEL TÁVOLSÁG SZERINT RENDEZZÜK A HÁROMSZÖGEKET!!!;
		///(Ennek is csak a forward rendererben lesz haszna, de erre szükség lesz az átlátszó objektumokhoz.
		/// Jelen formájában a depth testing miatt egy teljesen átlátszó háromszög elfedi a mögötte lévő háromszögeket.
		/// Leginkább Bob lámpájánál lehet megfigyelni.)
		/*
		if(DiffuseMap->GetAlphaBlending() && Transparency)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		*/
	}

	void BindTexture(lrGLImg *image,GLuint active_texture,const lrGLRendererShader::lrGLTextureLocation &texture_location)
	{
		if(image != nullptr && image->CheckError())
		{
			glActiveTexture(GL_TEXTURE0 + active_texture);
			glBindTexture(GL_TEXTURE_2D,image->GetId());

			int HasDiffuseMapLocation = texture_location.GetHasTextureLocation();
			glUniform1i(HasDiffuseMapLocation,1);

			int DiffuseMapLocation = texture_location.GetTextureLocation();
			glUniform1i(DiffuseMapLocation,active_texture);
		}
		else
		{
			int HasDiffuseMapLocation = texture_location.GetHasTextureLocation();
			glUniform1i(HasDiffuseMapLocation,0);
		}
	}

    template<class iterator>
    void UploadLights(lrGLRendererShader *shader,iterator &J)
    {
    	for(int i=0;i < N_POINT_LIGHTS;i++)
		{
			std::string Name = "lr_PointLights[";
			Name += std::to_string(i);
			Name += "]";

			if(J == PointLights.end())
			{
				glUniform1i(glGetUniformLocation(shader->GetProgramId(),(Name + ".Active").c_str()),0);

				continue;
			}

			glUniform3fv(glGetUniformLocation(shader->GetProgramId(),(Name + ".Ambient").c_str()),1, (*J)->GetAmbient());
			glUniform3fv(glGetUniformLocation(shader->GetProgramId(),(Name + ".Diffuse").c_str()),1, (*J)->GetDiffuse());
			glUniform3fv(glGetUniformLocation(shader->GetProgramId(),(Name + ".Specular").c_str()),1,(*J)->GetSpecular());
			glUniform3fv(glGetUniformLocation(shader->GetProgramId(),(Name + ".Attenuation").c_str()),1,(*J)->GetAttenuation());

			glUniform3fv(glGetUniformLocation(shader->GetProgramId(),(Name + ".Position").c_str()),1,(const float*)&((*J)->GetPosition()));
			glUniform1i (glGetUniformLocation(shader->GetProgramId(),(Name + ".Active").c_str()),(*J)->IsEnabled());

			J = std::next(J);
		}
    }

    /** \brief Skeletal mesh kirajzolásánál ezzel a függvénnyel töltjük fel a bone-ok transzformációit.
     *
     * \param Shader lrGLSkmShader*:    A shader, amitől elkérjük a uniform locationöket.
     * \param Skeleton lrSkeleton&:     A boneokat tartalmazó skeleton.
     * \return void
     *
     */
    void UploadBoneTransform(lrGLSkmShader *Shader,liSkeleton &bind_pose_skeleton,liSkeleton &Skeleton)
	{
		std::vector<lmMatrix4x4> BoneTransform;
		BoneTransform.resize(Skeleton.Size());

		for(unsigned int i=0;i < BoneTransform.size();i++)
		{
			BoneTransform[i] = Skeleton.GetTransformation(i) * bind_pose_skeleton.GetInverseTransformation(i);
			 /*
			std::cout << BoneTransform[i][0][0] << ";" << BoneTransform[i][1][0] << ";" << BoneTransform[i][2][0] << ";" << BoneTransform[i][3][0] << std::endl;
			std::cout << BoneTransform[i][0][1] << ";" << BoneTransform[i][1][1] << ";" << BoneTransform[i][2][1] << ";" << BoneTransform[i][3][1] << std::endl;
			std::cout << BoneTransform[i][0][2] << ";" << BoneTransform[i][1][2] << ";" << BoneTransform[i][2][2] << ";" << BoneTransform[i][3][2] << std::endl;
			std::cout << BoneTransform[i][0][3] << ";" << BoneTransform[i][1][3] << ";" << BoneTransform[i][2][3] << ";" << BoneTransform[i][3][3] << std::endl;
            // */
		}

		int BoneTransformLocation = Shader->GetBoneTransformLocation();
		glUniformMatrix4fv(BoneTransformLocation,BoneTransform.size(),GL_FALSE,&(BoneTransform[0][0][0]));
	}

    void UploadMatrices(lrGL3_3FwdStmPrePassShader *Shader,const lmMatrix4x4 &ModelMatrix)
	{
		lmMatrix4x4 ModelViewProjectionMatrix = ProjectionMatrix*(ViewMatrix*ModelMatrix);

		int ModelViewProjectionMatrixLocation = Shader->GetModelViewProjectionMatrixLocation();
		glUniformMatrix4fv(ModelViewProjectionMatrixLocation,1,GL_FALSE,&(ModelViewProjectionMatrix[0][0]));
	}

    void UploadMatrices(lrGL3_3FwdSkmPrePassShader *Shader,const lmMatrix4x4 &ModelMatrix)
	{
		lmMatrix4x4 ModelViewProjectionMatrix = ProjectionMatrix*(ViewMatrix*ModelMatrix);

		int ModelViewProjectionMatrixLocation = Shader->GetModelViewProjectionMatrixLocation();
		glUniformMatrix4fv(ModelViewProjectionMatrixLocation,1,GL_FALSE,&(ModelViewProjectionMatrix[0][0]));
	}

	void UploadBoneTransform(lrGL3_3FwdSkmPrePassShader *Shader,liSkeleton &bind_pose_skeleton,liSkeleton &Skeleton)
	{
		std::vector<lmMatrix4x4> BoneTransform;
		BoneTransform.resize(Skeleton.Size());

		for(unsigned int i=0;i < BoneTransform.size();i++)
		{
			BoneTransform[i] = Skeleton.GetTransformation(i) * bind_pose_skeleton.GetInverseTransformation(i);
		}

		int BoneTransformLocation = Shader->GetBoneTransformLocation();
		glUniformMatrix4fv(BoneTransformLocation,BoneTransform.size(),GL_FALSE,&(BoneTransform[0][0][0]));
	}

	void Clear()
	{
		glDepthMask(GL_TRUE);

		glViewport(0,0,800,600);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void DrawSkybox()
	{
		glDisable(GL_DEPTH_TEST);

		SkyboxShader->UseProgram();

		lrGLImg *SkyboxImg = (lrGLImg *)RM->LoadCubemap("purplenebula");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP,SkyboxImg->GetId());

		int SkyboxImgLocation = glGetUniformLocation(SkyboxShader->GetProgramId(),"lr_SkyboxImg");
		glUniform1i(SkyboxImgLocation,0);

		glUniformMatrix4fv(glGetUniformLocation(SkyboxShader->GetProgramId(),"lr_ViewMatrix"),1,GL_FALSE,&(ViewMatrix[0][0]));
		glUniformMatrix4fv(glGetUniformLocation(SkyboxShader->GetProgramId(),"lr_ProjectionMatrix"),1,GL_FALSE,&(ProjectionMatrix[0][0]));

		Skybox->Draw();

		SkyboxShader->DisableProgram();
	}

    void PrePass()
    {
    	glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

    	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);

    	StmPrePassShader->UseProgram();

    	for(auto I = DL_StaticMesh.begin();I != DL_StaticMesh.end();I++)
		{
			lmMatrix4x4 ModelMatrix(lmMatrix4x4::IDENTITY);
			I->SceneNode.GetModelMatrix(ModelMatrix);

			UploadMatrices(StmPrePassShader,ModelMatrix);

			for(unsigned int i=0;i < I->Mesh.GetSize();i++)
			{
				lrGLStaticMesh::lrGLMaterialGroup *MatGroup = (lrGLStaticMesh::lrGLMaterialGroup *) I->Mesh[i];

				MatGroup->Enable();
				glDrawElements(GL_TRIANGLES,MatGroup->GetBufferLength(),GL_UNSIGNED_INT,nullptr);
				MatGroup->Disable();
			}
		}

		StmPrePassShader->DisableProgram();

		//Drawing skeletal meshes
		SkmPrePassShader->UseProgram();

		for(auto I = DL_SkeletalMesh.begin();I != DL_SkeletalMesh.end();I++)
		{
			lmMatrix4x4 ModelMatrix(lmMatrix4x4::IDENTITY);
			I->SceneNode.GetModelMatrix(ModelMatrix);

			UploadMatrices(SkmPrePassShader,ModelMatrix);
			UploadBoneTransform(SkmPrePassShader,I->Mesh.GetBindPoseSkeleton(),I->Skeleton);

			for(unsigned int i=0;i < I->Mesh.Size();i++)
			{
				lrGLSkeletalMesh *GLMesh = (lrGLSkeletalMesh *)I->Mesh[i];

				GLMesh->Enable();
				glDrawElements(GL_TRIANGLES,GLMesh->GetIndexBufferLength(),GL_UNSIGNED_INT,nullptr);
				GLMesh->Disable();
			}
		}

		SkmPrePassShader->DisableProgram();
    }

    void Draw()
    {
    	glDepthFunc(GL_EQUAL);

		glDepthMask(GL_FALSE);
    	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);

		glDisable(GL_BLEND);

		bool First = true;
		bool EmissiveEnabled = true;

		std::list<lrPointLight *>::iterator K = PointLights.begin();
		while(K != PointLights.end())
		{
			/*
			 * Drawing static meshes
			 */

			StmShader->UseProgram();

			/*
			 * Upload lights
			 */
			std::list<lrPointLight *>::iterator J;

			J = K;
			UploadLights(StmShader,J);


			for(auto I = DL_StaticMesh.begin();I != DL_StaticMesh.end();I++)
			{
				lmMatrix4x4 ModelMatrix(lmMatrix4x4::IDENTITY);
				I->SceneNode.GetModelMatrix(ModelMatrix);

				UploadMatrices(StmShader,ModelMatrix);

				for(unsigned int i=0;i < I->Mesh.GetSize();i++)
				{
					lrGLStaticMesh::lrGLMaterialGroup *MatGroup = (lrGLStaticMesh::lrGLMaterialGroup *) I->Mesh[i];
					const liMaterial *Material = I->MultiMaterial.GetMaterial(MatGroup->GetMaterial());

					UploadMaterial(StmShader,Material,EmissiveEnabled);

					lrGLImg *DiffuseMap		= (lrGLImg *) RM->LoadImg(Material->GetDiffuseMap());
					lrGLImg *NormalMap		= (lrGLImg *) RM->LoadImg(Material->GetNormalMap());
					lrGLImg *SpecularMap	= (lrGLImg *) RM->LoadImg(Material->GetSpecularMap());
					lrGLImg *EmissiveMap	= (lrGLImg *) RM->LoadImg(Material->GetEmissiveMap());

					BindTexture(DiffuseMap,0,StmShader->GetDiffuseMapLocation());
					BindTexture(NormalMap,1,StmShader->GetNormalMapLocation());
					BindTexture(SpecularMap,2,StmShader->GetSpecularMapLocation());

					if(EmissiveEnabled)
					{
						BindTexture(EmissiveMap,3,StmShader->GetEmissiveMapLocation());
					}
					else
					{
						GLint HasEmissiveMapLocation = StmShader->GetEmissiveMapLocation().GetHasTextureLocation();
						glUniform1i(HasEmissiveMapLocation,0);
					}

					MatGroup->Enable();
					glDrawElements(GL_TRIANGLES,MatGroup->GetBufferLength(),GL_UNSIGNED_INT,nullptr);
					MatGroup->Disable();
				}
			}

			StmShader->DisableProgram();

			/*
			 *Drawing skeletal meshes
			 */

			SkmShader->UseProgram();

			/*
			 * Upload lights
			 */
			J = K;
			UploadLights(SkmShader,J);

			K = J;

			for(auto I = DL_SkeletalMesh.begin();I != DL_SkeletalMesh.end();I++)
			{
				lmMatrix4x4 ModelMatrix(lmMatrix4x4::IDENTITY);
				I->SceneNode.GetModelMatrix(ModelMatrix);

				UploadMatrices(SkmShader,ModelMatrix);
				UploadBoneTransform(SkmShader,I->Mesh.GetBindPoseSkeleton(),I->Skeleton);

				for(unsigned int i=0;i < I->Mesh.Size();i++)
				{
					lrGLSkeletalMesh *GLMesh = (lrGLSkeletalMesh *)I->Mesh[i];
					const liMaterial *Material = I->MultiMaterial.GetMaterial(GLMesh->GetMaterial());

					UploadMaterial(SkmShader,Material,EmissiveEnabled);

					lrGLImg *DiffuseMap		= (lrGLImg *) RM->LoadImg(Material->GetDiffuseMap());
					lrGLImg *NormalMap		= (lrGLImg *) RM->LoadImg(Material->GetNormalMap());
					lrGLImg *SpecularMap	= (lrGLImg *) RM->LoadImg(Material->GetSpecularMap());
					lrGLImg *EmissiveMap	= (lrGLImg *) RM->LoadImg(Material->GetEmissiveMap());

					BindTexture(DiffuseMap,0,SkmShader->GetDiffuseMapLocation());
					BindTexture(NormalMap,1,SkmShader->GetNormalMapLocation());
					BindTexture(SpecularMap,2,SkmShader->GetSpecularMapLocation());

					if(EmissiveEnabled)
					{
						BindTexture(EmissiveMap,3,SkmShader->GetEmissiveMapLocation());
					}
					else
					{
						GLint HasEmissiveMapLocation = SkmShader->GetEmissiveMapLocation().GetHasTextureLocation();
						glUniform1i(HasEmissiveMapLocation,0);
					}

					GLMesh->Enable();
					glDrawElements(GL_TRIANGLES,GLMesh->GetIndexBufferLength(),GL_UNSIGNED_INT,nullptr);
					GLMesh->Disable();
				}
			}

			SkmShader->DisableProgram();

			if(First)
			{
				glEnable(GL_BLEND);
				glBlendEquation(GL_FUNC_ADD);
				glBlendFunc(GL_ONE,GL_ONE);

				EmissiveEnabled = false;
				First = false;
			}
		}
    }

public:

    virtual void SetRM(liResourceManager *rm)override
    {
    	RM = rm;
    	RM->SetGPUFactory(GPUFactory);
    	RM->SetCharAnimFactory(CharAnimFactory);
    }

    virtual liFramebuffer *GetScreen() override
    {
    	return &Screen;
    }

    virtual liFramebuffer *CreateFramebuffer(unsigned int width,unsigned int height)
    {
    	liFramebuffer *NewFramebuffer = new lrGLFramebuffer(width,height);
    	return NewFramebuffer;
    }

    virtual liCamera *CreateCamera(const lmVector3D &position,float yaw,float pitch) override
    {
    	return new lrCamera(position,yaw,pitch);
    }

    virtual liFrustum *CreateFrustum(float top,float bottom,float left,float right,float far,float near) override
    {
    	return new lrFrustum(top,bottom,left,right,far,near);
    }

    virtual liPointLight *CreatePointLight(float ambient[],float diffuse[],float specular[],float attenuation[],const lmVector3D &position) override
    {
    	lrPointLight *NewLight = new lrPointLight(ambient,diffuse,specular,attenuation,position);
    	PointLights.push_back(NewLight);

    	return NewLight;
    }

    virtual void FreePointLights()
    {
    	for(lrPointLight *Light : PointLights)
		{
			delete Light;
		}

		PointLights.clear();
    }

    virtual bool CreateGuiRenderer(liGuiRenderer **gui_renderer) override
    {
        if(RM != nullptr)
        {
            if(GuiRenderer == nullptr)
            {
                GuiRenderer = new lrGL3_3GuiRenderer(RM);
            }

            *gui_renderer = GuiRenderer;

            return true;
        }
        else
        {
            return false;
        }
    }

    virtual liPostProcessor *GetPostProcessor() override
    {
    	return PostProcessor;
    }

    /** \brief Ez a függvény a pillanatnyilag használt framebuffert letisztítja, és beállítja a projection és view mátrixokat a paraméterben kapott kamera és frustum alapján.
     *
     * \param Frustum const lrFrustum &: A látható térrészt leíró csonkagúla
     * \param Camera const lrCamera &:   A kamera
     * \param BgColor const lrColor &:   A háttér új színe
     * \return void
     *
     */
    virtual void SetScene(const liFrustum *Frustum,const liCamera *Camera,const lrColor &BgColor)override
    {
		ViewMatrix		 = IDENTITY_MATRIX_4X4;
		ProjectionMatrix = IDENTITY_MATRIX_4X4;

		Camera->GetViewMatrix(ViewMatrix);
		Frustum->GetProjectionMatrix(ProjectionMatrix);

		glClearColor(BgColor.Red,BgColor.Green,BgColor.Blue,BgColor.Alpha);
	}

    /** \brief Ez a függvény letakarítja a display listet.
     *
     * \return void
     *
     */
    virtual void ClearDL() override
    {
		DL_StaticMesh.clear();
		DL_SkeletalMesh.clear();
	}

    /** \brief Ez a függvény hozzáad egy új statikus mesht a display listhez.
     *
     * \param Mesh lrStaticMesh&:               A display listhez hozzáadandó static mesh
     * \param MultiMaterial lrMultiMaterial&    A static mesh által használt multi material
     * \param SceneNode lrsSceneNode&           A static mesht tartalmazó scene node
     * \return void
     *
     */
    virtual void AddStaticMesh(liStaticMesh &Mesh,liMultiMaterial &MultiMaterial,liSceneNode &SceneNode,liStaticMeshNode &StaticMeshNode) override
    {
		DL_StaticMesh.push_back(lrDLNode_StaticMesh(Mesh,MultiMaterial,SceneNode));
	}

    /** \brief Ez a függvény hozzáad egy új skeletal mesht a színtér gráfhoz.
     *
     * \param Mesh lrCharacter&                 A skeletal mesheket tartalmazó karakter
     * \param Skeleton lrSkeleton&              A skeletal mesh testtartását megadó skeleton
     * \param MultiMaterial lrMultiMaterial&    A skeletal mesh által használt multi material
     * \param SceneNode lrsSceneNode&           A skeletal mesht tartalmazó scene node
     * \return void
     *
     */
    virtual void AddSkeletalMesh(liCharacter &Mesh,liSkeleton &Skeleton,liMultiMaterial &MultiMaterial,liSceneNode &SceneNode,liSkeletalMeshNode &SkeletalMeshNode) override
    {
		DL_SkeletalMesh.push_back(lrDLNode_SkeletalMesh(Mesh,Skeleton,MultiMaterial,SceneNode));
	}

    /** \brief Ez a függvény kirajzolja a jelenlegi színteret a jelenleg beállított framebufferbe.
     *
     * \return void
     *
     */
    virtual void Draw(liFramebuffer *framebuffer) override
    {
		PingPongBuffer->Enable();

		/*
		 * Letakarítjuk a ping pong buffer előző tartalmát
		 */
		Clear();

		/*
		 * Kirajzoljuk a skyboxot.
		 */
		DrawSkybox();

		/*
		 * Egy pre pass renderinggel feltöltjük a depth buffert, hogy a rajzolásnál a
		 * takarásban lévő felületek megvilágítását ne számoljuk.
		 */
		PrePass();

		/*
		 * Most kirajzoljuk a statikus és a skeletal mesheket fényekkel.
		 */
		Draw();

		PingPongBuffer->Disable();

		/*
		 * Flipeljük a ping pong buffert a post processing és a framebufferre való kirajzolás miatt.
		 */
		PingPongBuffer->Flip();

		/*
		 * PostProcess
		 */

		PostProcessor->SetTarget(PingPongBuffer);

		glClearColor(0.0,0.0,0.0,1.0);
		PostProcessor->ApplyPostProcess();

		/*
		 * Itt vágjuk ki a végeredményt a framebufferre.
		 */

		framebuffer->Enable();

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		glViewport(0,0,800,600);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		CopyShader->UseProgram();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,PingPongBuffer->GetImg()->GetId());
		glUniform1i(glGetUniformLocation(CopyShader->GetProgramId(),"lr_PrevRenderedImg"),0);

		ViewportSizeQuad->Draw();

		CopyShader->DisableProgram();

		framebuffer->Disable();
	}

    /** \brief Konstruktor
     *
     */
    lrGL3_3ForwardRenderer()
    {
		PingPongBuffer	= new lrGLPingPongBuffer(800,600);

		SkyboxShader = new lrGLPostProcessShader("Content/Shaders/GL3/lrGL3_3Skybox.vert" ,"Content/Shaders/GL3/lrGL3_3Skybox.frag");
		CopyShader = new lrGLPostProcessShader("Content/Shaders/GL3/PostProcess/lrGL3_3PostProcess.vert" ,"Content/Shaders/GL3/PostProcess/lrGL3_3CopyShader.frag");

		ViewportSizeQuad = new lrGLViewportSizeQuad(CopyShader);
		Skybox = new lrGLSkybox(SkyboxShader);

		PostProcessor = new lrGL3_3PostProcessor(ViewportSizeQuad,CopyShader);

		bool BloomEnabled = true;
		if(BloomEnabled)
		{
			lmVector3D MinIntensity = {0.9,0.9,0.9};
			unsigned int BlurCount = 1;

			liPostProcessCommand *Cmd = PostProcessor->GetCommandFactory()->CreateBloomCommand(MinIntensity,BlurCount);
			PostProcessor->AddPostProcessCommand(Cmd);
		}

		bool MegAkarokVakulni = false;
		if(MegAkarokVakulni)
		{
			liPostProcessCommand *Cmd = PostProcessor->GetCommandFactory()->CreateBlurCommand(1);
			PostProcessor->AddPostProcessCommand(Cmd);
		}

		StmPrePassShader = new lrGL3_3FwdStmPrePassShader("Content/Shaders/GL3/ForwardRenderer/lrGL3_3FwdStmPrePass.vert");
		SkmPrePassShader = new lrGL3_3FwdSkmPrePassShader("Content/Shaders/GL3/ForwardRenderer/lrGL3_3FwdSkmPrePass.vert");

		StmShader = new lrGL3_3StmShader("Content/Shaders/GL3/lrGL3_3StmShader.vert","Content/Shaders/GL3/ForwardRenderer/lrGL3_3FwdShader.frag");
		SkmShader = new lrGL3_3SkmShader("Content/Shaders/GL3/lrGL3_3SkmShader.vert","Content/Shaders/GL3/ForwardRenderer/lrGL3_3FwdShader.frag");

		GPUFactory = new lrGLGPUFactory(StmShader,SkmShader);
		CharAnimFactory = new lrCharAnimFactory;
	}

    /** \brief Virtuális destruktor
     *
     *
     */
    virtual ~lrGL3_3ForwardRenderer()
    {
		delete PingPongBuffer;

		delete SkyboxShader;
		delete CopyShader;

		delete ViewportSizeQuad;
		delete Skybox;

		delete PostProcessor;

		delete StmPrePassShader;
		delete SkmPrePassShader;

		delete StmShader;
		delete SkmShader;

		for(lrPointLight *I : PointLights)
		{
			delete I;
		}

		delete GuiRenderer;

		delete GPUFactory;
	}
};

#endif
