
#include "lrGLResourceLoader.h"

lrGLTexture2DView lrGLResourceLoader::GetTexture(const std::string &resource_id)
{
	auto I = Textures.find(resource_id);
	if(I == Textures.end())
	{
		liBitmap2D *LoadedBitmap = LoadBitmap(resource_id);
		
		lrGLTextureResource	*TextureResource = &Textures[resource_id];
		lrGLTexture2DView	Texture2DView(TextureResource);
		
		if(LoadedBitmap != nullptr)
		{
			TextureResource->Initialize();
			Texture2DView.Fill(*LoadedBitmap);
		}
		
		return Texture2DView;
	}
	
	return lrGLTexture2DView(&I->second);
}

lrGLTextureCubemapView lrGLResourceLoader::GetCubemap(const std::string &resource_id)
{
	auto I = Cubemaps.find(resource_id);
	if(I == Cubemaps.end())
	{
		liCubemap *LoadedCubemap = LoadCubemap(resource_id);
		
		lrGLTextureResource	*TextureResource = &Cubemaps[resource_id];
		lrGLTextureCubemapView	CubemapView(TextureResource);
		
		if(LoadedCubemap != nullptr)
		{
			const void *CubemapData[6] = {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};
			
			CubemapData[GL_TEXTURE_CUBE_MAP_POSITIVE_Y - GL_TEXTURE_CUBE_MAP_POSITIVE_X] = (void *)LoadedCubemap->GetSide(liCubemap::LI_CUBEMAP_UP).GetPixelArray();
			CubemapData[GL_TEXTURE_CUBE_MAP_NEGATIVE_Y - GL_TEXTURE_CUBE_MAP_POSITIVE_X] = (void *)LoadedCubemap->GetSide(liCubemap::LI_CUBEMAP_DOWN).GetPixelArray();
			CubemapData[GL_TEXTURE_CUBE_MAP_NEGATIVE_X - GL_TEXTURE_CUBE_MAP_POSITIVE_X] = (void *)LoadedCubemap->GetSide(liCubemap::LI_CUBEMAP_LEFT).GetPixelArray();
			CubemapData[GL_TEXTURE_CUBE_MAP_POSITIVE_X - GL_TEXTURE_CUBE_MAP_POSITIVE_X] = (void *)LoadedCubemap->GetSide(liCubemap::LI_CUBEMAP_RIGHT).GetPixelArray();
			CubemapData[GL_TEXTURE_CUBE_MAP_NEGATIVE_Z - GL_TEXTURE_CUBE_MAP_POSITIVE_X] = (void *)LoadedCubemap->GetSide(liCubemap::LI_CUBEMAP_FRONT).GetPixelArray();
			CubemapData[GL_TEXTURE_CUBE_MAP_POSITIVE_Z - GL_TEXTURE_CUBE_MAP_POSITIVE_X] = (void *)LoadedCubemap->GetSide(liCubemap::LI_CUBEMAP_BACK).GetPixelArray();
			
			TextureResource->Initialize();
			CubemapView.Fill(LoadedCubemap->GetSize(),CubemapData);
		}
		
		return CubemapView;
	}
	
	return lrGLTextureCubemapView(&I->second);
}
