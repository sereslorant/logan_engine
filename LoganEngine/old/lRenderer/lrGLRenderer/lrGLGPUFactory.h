#ifndef LR_GL_GPU_FACTORY_H
#define LR_GL_GPU_FACTORY_H

#include <iostream>

#include "lrGLShader/lrGLStmShader.h"
#include "lrGLShader/lrGLSkmShader.h"

#include "lrGLStaticMesh.h"
#include "lrGLSkeletalMesh.h"
#include "lrGLImg.h"

#include "lGLFunctions.h"

class lrGLGPUFactory : public liGPUFactory
{
private:
	lrGLStmShader *StmShader;
	lrGLSkmShader *SkmShader;

public:

	/** \brief Ez a függvény feltölt egy bittérképet a videokártya memóriájába, és visszaad egy lrImg-et paraméterbe, amin keresztül el lehet érni.
	 *
	 * \todo A paraméterekhez fűzöttkomment nagyon parasztosan van megfogalmazva, át kell fogalmazni.
	 *
	 * \param Img lrImg **:  Arra az lrImg pointerre mutató pointer, amelyet rá akarunk állítani az újonnan létrehozott lrImg-re.
	 * \param Bmp BMP *:     A betöltött bittérkép adatai
	 * \return void
	 *
	 */
	virtual void UploadImage(liImg* &Img,liBitmap *Bitmap) override
	{
		GLuint Id;
		glGenTextures(1, &Id);
		glBindTexture(GL_TEXTURE_2D, Id);

		GLenum InternalFormat,Type;
		bool AlphaBlending;
		lGL_GetImageInfo(Bitmap,&InternalFormat,&Type,&AlphaBlending);

		#ifdef L_TESTING

			std::cout << "\nInternal format: " << InternalFormat << "\nType: " << Type << "\nAlphaBlending: " << AlphaBlending << std::endl;

		#endif

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,Bitmap->GetWidth(),Bitmap->GetHeight(), 0, InternalFormat, Type,Bitmap->GetPixelArray());

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_2D, 0);

		Img = new lrGLImg(Bitmap->GetWidth(),Bitmap->GetHeight(),AlphaBlending,Id);

		if(Img == nullptr)
		{
			glDeleteTextures(1,&Id);
		}
	}

	virtual void UploadCubemap(liImg* &Img,liBitmap *bitmaps[6]) override
	{
		GLuint Id;
		glGenTextures(1, &Id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, Id);

		for(int i=0;i <6;i++)
		{
			GLenum InternalFormat,Type;
			bool AlphaBlending;
			lGL_GetImageInfo(bitmaps[i],&InternalFormat,&Type,&AlphaBlending);

			#ifdef L_TESTING

				std::cout << "\nInternal format: " << InternalFormat << "\nType: " << Type << "\nAlphaBlending: " << AlphaBlending << std::endl;

			#endif

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA,bitmaps[i]->GetWidth(),bitmaps[i]->GetHeight(), 0, InternalFormat, Type,bitmaps[i]->GetPixelArray());

			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		Img = new lrGLImg(0,0,false,Id);

		if(Img == nullptr)
		{
			glDeleteTextures(1,&Id);
		}
	}

	/** \brief Ez a függvény feltölti a paraméterben kapott statikus mesht a videokártya memóriájába, és visszaad egy lrStaticMesht paraméterbe, amin keresztül el lehet érni.
	 *
	 * \todo A paraméterekhez fűzöttkomment nagyon parasztosan van megfogalmazva, át kell fogalmazni.
	 *
	 * \param Mesh lrStaticMesh **:  arra a static mesh pointerre mutató pointer, amelyet rá akarunk állítani az újonnan létrehozott statikus meshre.
	 * \param Src lrmStaticMesh *:   a fájlból betöltött csúcspontokat, háromszögeket és material groupokat tartalmazó adatstruktúra.
	 * \return void
	 *
	 */
	virtual void UploadStaticMesh(liStaticMesh* &Mesh,liStaticMeshResource *Src) override
	{
		Mesh = new lrGLStaticMesh(StmShader,Src);
	}

	/** \brief Ez a függvény feltölti a paraméterben kapott skeletal mesht a videokártya memóriájába, és visszaad egy lrSkeletalMesht paraméterbe, amin keresztül el lehet érni.
	 *
	 * \todo A paraméterekhez fűzöttkomment nagyon parasztosan van megfogalmazva, át kell fogalmazni.
	 *
	 * \param Mesh lrSkeletalMesh **:  arra a skeletal mesh pointerre mutató pointer, amelyet rá akarunk állítani az újonnan létrehozott skeletal meshre.
	 * \param Src lrmSkeletalMesh *:   a fájlból betöltött csúcspontokat, háromszögeket és stb.-ket tartalmazó adatstruktúra.
	 * \return void
	 *
	 */
	virtual void UploadSkeletalMesh(liSkeletalMesh* &Mesh,liSkeletalMeshResource *Src) override
	{
		Mesh = new lrGLSkeletalMesh(SkmShader,Src);
	}

	lrGLGPUFactory(lrGLStmShader *stm_shader,lrGLSkmShader *skm_shader)
		:StmShader(stm_shader),SkmShader(skm_shader)
	{
		//Üres függvénytörzs
	}

	virtual ~lrGLGPUFactory() override
	{
		//Üres függvénytörzs
	}
};

#endif // LR_GL_GPU_FACTORY_H
