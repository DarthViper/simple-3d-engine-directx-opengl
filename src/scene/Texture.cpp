#include "Texture.h"

// 2D TEXTURE FROM IMAGE
Texture::Texture(wxImage* image, bool repeat, bool flipY, bool transparent, const glm::vec2 &scale)
{
	if (image != nullptr)
	{
		this->flipY       = flipY;
		this->repeat      = repeat;
		this->Scale       = scale;
		this->transparent = transparent;

		switch (RenderEngine::SelectedGraphicsAPI) {
		#if defined _WINDOWS
		case GRAPHICS_API_DIRECTX11:
		case GRAPHICS_API_DIRECTX12:
			this->loadTextureImagesDX({ image });
			break;
		#endif
		case GRAPHICS_API_OPENGL:
			this->type = GL_TEXTURE_2D;

			glCreateTextures(this->type, 1, &this->id);

			if (this->id > 0)
				this->loadTextureImageGL(image);

			break;
		case GRAPHICS_API_VULKAN:
			this->loadTextureImagesVK({ image });
			break;
		}
	}
	else
	{
		wxMessageBox(
			"ERROR: Failed to create a texture from image: wxImage is NULL",
			RenderEngine::Canvas.Window->GetTitle().c_str(), wxOK | wxICON_ERROR
		);
	}
}

// 2D TEXTURE FROM IMAGE FILE
Texture::Texture(const wxString &imageFile, bool repeat, bool flipY, bool transparent, const glm::vec2 &scale)
{
	wxImage* image = nullptr;

	if (!imageFile.empty())
		image = Utils::LoadImageFile(imageFile);

	if (image != nullptr)
	{
		this->flipY       = flipY;
		this->imageFiles  = { imageFile };
		this->repeat      = repeat;
		this->Scale       = scale;
		this->transparent = transparent;

		switch (RenderEngine::SelectedGraphicsAPI) {
		#if defined _WINDOWS
		case GRAPHICS_API_DIRECTX11:
		case GRAPHICS_API_DIRECTX12:
			this->loadTextureImagesDX({ image });
			break;
		#endif
		case GRAPHICS_API_OPENGL:
			this->type = GL_TEXTURE_2D;

			glCreateTextures(this->type, 1, &this->id);

			if (this->id > 0)
				this->loadTextureImageGL(image);

			break;
		case GRAPHICS_API_VULKAN:
			this->loadTextureImagesVK({ image });
			break;
		}

		image->Destroy();
	}
	else
	{
		wxMessageBox(
			("ERROR: Failed to create a texture from image file: " + imageFile),
			RenderEngine::Canvas.Window->GetTitle().c_str(), wxOK | wxICON_ERROR
		);
	}
}

// CUBEMAP TEXTURE FROM 6 IMAGE FILES
Texture::Texture(const std::vector<wxString> &imageFiles, bool repeat, bool flipY, bool transparent, const glm::vec2 &scale)
{
	wxImage*              image;
	std::vector<wxImage*> images;

	if ((int)imageFiles.size() == MAX_TEXTURES)
	{
		for (int i = 0; i < MAX_TEXTURES; i++) {
			if ((image = Utils::LoadImageFile(imageFiles[i])) != nullptr)
				images.push_back(image);
		}
	}

	if ((int)images.size() == MAX_TEXTURES)
	{
		this->flipY       = flipY;
		this->imageFiles  = imageFiles;
		this->Scale       = scale;
		this->transparent = transparent;

		switch (RenderEngine::SelectedGraphicsAPI) {
		#if defined _WINDOWS
		case GRAPHICS_API_DIRECTX11:
		case GRAPHICS_API_DIRECTX12:
			this->loadTextureImagesDX(images);
			break;
		#endif
		case GRAPHICS_API_OPENGL:
			this->type = GL_TEXTURE_CUBE_MAP;

			glCreateTextures(this->type, 1, &this->id);

			if (this->id > 0) {
				for (int i = 0; i < MAX_TEXTURES; i++)
					this->loadTextureImageGL(images[i], true, i);
			}

			break;
		case GRAPHICS_API_VULKAN:
			this->loadTextureImagesVK(images);
			break;
		}
	}

	for (auto img : images) {
		if (img != nullptr)
			img->Destroy();
	}
}

// 2D FRAMEBUFFER TEXTURE (DIRECTX)
Texture::Texture(DXGI_FORMAT format, int width, int height)
{
	int                result;
	D3D11_SAMPLER_DESC samplerDesc11 = {};

	this->repeat = true;
	this->Scale  = glm::vec2(1.0f, 1.0f);

	switch (RenderEngine::SelectedGraphicsAPI) {
	#if defined _WINDOWS
	case GRAPHICS_API_DIRECTX11:
		this->colorBufferViewPort11.TopLeftX = 0.0f;
		this->colorBufferViewPort11.TopLeftY = 0.0f;
		this->colorBufferViewPort11.Width    = (FLOAT)width;
		this->colorBufferViewPort11.Height   = (FLOAT)height;
		this->colorBufferViewPort11.MinDepth = 0.0f;
		this->colorBufferViewPort11.MaxDepth = 1.0f;

		this->setFilteringDX11(samplerDesc11);
		this->setWrappingDX11(samplerDesc11);

		result = RenderEngine::Canvas.DX->CreateTextureBuffer11(format, samplerDesc11, this);

		if (result < 0)
			wxMessageBox("ERROR: Failed to create a texture frame buffer.", RenderEngine::Canvas.Window->GetTitle().c_str(), wxOK | wxICON_ERROR);

		break;
	case GRAPHICS_API_DIRECTX12:
		this->colorBufferViewPort12.TopLeftX = 0.0f;
		this->colorBufferViewPort12.TopLeftY = 0.0f;
		this->colorBufferViewPort12.Width    = (FLOAT)width;
		this->colorBufferViewPort12.Height   = (FLOAT)height;
		this->colorBufferViewPort12.MinDepth = 0.0f;
		this->colorBufferViewPort12.MaxDepth = 1.0f;

		this->setFilteringDX12(this->SamplerDesc12);
		this->setWrappingDX12(this->SamplerDesc12);

		result = RenderEngine::Canvas.DX->CreateTextureBuffer12(format, this);

		if (result < 0)
			wxMessageBox("ERROR: Failed to create a texture frame buffer.", RenderEngine::Canvas.Window->GetTitle().c_str(), wxOK | wxICON_ERROR);

		break;
	#endif
	}
}

// 2D FRAMEBUFFER TEXTURE (VULKAN)
Texture::Texture(VkFormat format, int width, int height)
{
	this->repeat = true;
	this->Scale  = glm::vec2(1.0f, 1.0f);
	this->size   = { width, height };

	this->colorBufferViewPort.x        = 0.0f;
	this->colorBufferViewPort.y        = 0.0f;
	this->colorBufferViewPort.width    = (float)width;
	this->colorBufferViewPort.height   = (float)height;
	this->colorBufferViewPort.minDepth = 0.0f;
	this->colorBufferViewPort.maxDepth = 1.0f;

	this->setFilteringVK(this->SamplerInfo);
	this->setWrappingVK(this->SamplerInfo);

	int result = RenderEngine::Canvas.VK->CreateTextureBuffer(format, this);

	if (result < 0)
		wxMessageBox("ERROR: Failed to create a texture frame buffer.", RenderEngine::Canvas.Window->GetTitle().c_str(), wxOK | wxICON_ERROR);
}

// 2D FRAMEBUFFER TEXTURE (OPENGL)
Texture::Texture(GLint formatIn, GLenum formatOut, GLenum dataType, GLenum attachment, int width, int height)
{
	this->repeat = true;
	this->Scale  = glm::vec2(1.0f, 1.0f);
	this->type   = GL_TEXTURE_2D;

	glCreateTextures(this->type, 1, &this->id);
	glBindTexture(this->type, this->id);

	this->setFilteringGL(false);
	this->setWrappingGL();

	glTexImage2D(this->type, 0, formatIn, width, height, 0, formatOut, dataType, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, this->type, this->id, 0);
	glBindTexture(this->type, 0);
}

Texture::Texture()
{
	switch (RenderEngine::SelectedGraphicsAPI) {
	#if defined _WINDOWS
	case GRAPHICS_API_DIRECTX11:
		this->ColorBuffer11  = nullptr;
		this->Resource11     = nullptr;
		this->samplerState11 = nullptr;
		this->SRV11          = nullptr;

		break;
	case GRAPHICS_API_DIRECTX12:
		this->ColorBuffer12 = nullptr;
		this->Resource12    = nullptr;

		break;
	#endif
	case GRAPHICS_API_OPENGL:
		this->id   = 0;
		this->type = GL_TEXTURE_2D;

		break;
	case GRAPHICS_API_VULKAN:
		this->Image       = nullptr;
		this->ImageMemory = nullptr;
		this->ImageView   = nullptr;
		this->Sampler     = nullptr;
		this->SamplerInfo = {};

		break;
	}
}

Texture::~Texture()
{
	#if defined _WINDOWS
		_RELEASEP(this->ColorBuffer11);
		_RELEASEP(this->Resource11);
		_RELEASEP(this->samplerState11);
		_RELEASEP(this->SRV11);

		_RELEASEP(this->ColorBuffer12);
		_RELEASEP(this->Resource12);
	#endif

	if (this->id > 0)
	{
		glDeleteTextures(1, &this->id);

		this->id   = 0;
		this->type = GL_TEXTURE_2D;
	}

	RenderEngine::Canvas.VK->DestroyTexture(&this->Image, &this->ImageMemory, &this->ImageView, &this->Sampler);
	RenderEngine::Canvas.VK->DestroyFramebuffer(&this->ColorBufferVK);

	this->imageFiles.clear();
}

VkViewport Texture::ColorBufferViewPort()
{
	return this->colorBufferViewPort;
}

#if defined _WINDOWS
D3D11_VIEWPORT Texture::ColorBufferViewPort11()
{
	return this->colorBufferViewPort11;
}

D3D12_VIEWPORT Texture::ColorBufferViewPort12()
{
	return this->colorBufferViewPort12;
}
#endif

bool Texture::FlipY()
{
	return this->flipY;
}

GLuint Texture::ID()
{
	return this->id;
}

wxString Texture::ImageFile(int index)
{
	if ((this->imageFiles.size() == 1) && (this->imageFiles[0] != Utils::RESOURCE_IMAGES["emptyTexture"]))
		return this->imageFiles[0];
	else if ((this->imageFiles.size() > 1) && (index < (int)this->imageFiles.size()) && (this->imageFiles[index] != Utils::RESOURCE_IMAGES["emptyTexture"]))
		return this->imageFiles[index];

	return "";
}

bool Texture::IsOK()
{
	switch (RenderEngine::SelectedGraphicsAPI) {
	#if defined _WINDOWS
	case GRAPHICS_API_DIRECTX11:
		return (this->Resource11 != nullptr);
	case GRAPHICS_API_DIRECTX12:
		return (this->Resource12 != nullptr);
	#endif
	case GRAPHICS_API_OPENGL:
		return (this->id > 0);
	case GRAPHICS_API_VULKAN:
		return ((this->ImageView != nullptr) && (this->Sampler != nullptr));
	}

	return false;
}

#if defined _WINDOWS
void Texture::loadTextureImagesDX(const std::vector<wxImage*> &images)
{
	std::vector<wxImage>  images2;
	std::vector<uint8_t*> pixels2;

	for (auto image : images)
	{
		wxImage image2 = (this->flipY ? image->Mirror(false) : *image);
		images2.push_back(image2);
		pixels2.push_back(Utils::ToRGBA(&image2));
	}

	if (!images2.empty())
	{
		int                result;
		DXGI_FORMAT        format        = Utils::GetImageFormatDXGI(&images2[0]);
		D3D11_SAMPLER_DESC samplerDesc11 = {};

		this->size        = wxSize(images2[0].GetWidth(), images2[0].GetHeight());
		this->mipLevels   = ((uint32_t)(std::floor(std::log2(std::max(this->size.GetWidth(), this->size.GetHeight())))) + 1);
		this->transparent = (this->transparent && images2[0].HasAlpha());

		switch (RenderEngine::SelectedGraphicsAPI) {
		case GRAPHICS_API_DIRECTX11:
			this->setFilteringDX11(samplerDesc11);

			if (images.size() > 1)
				this->setWrappingCubemapDX11(samplerDesc11);
			else
				this->setWrappingDX11(samplerDesc11);

			result = RenderEngine::Canvas.DX->CreateTexture11(pixels2, format, samplerDesc11, this);

			if (result < 0)
				wxMessageBox(("ERROR: Texture::loadTextureImagesDX11: Failed to create a texture from image files."), RenderEngine::Canvas.Window->GetTitle().c_str(), wxOK | wxICON_ERROR);

			break;
		case GRAPHICS_API_DIRECTX12:
			this->setFilteringDX12(this->SamplerDesc12);

			if (images.size() > 1)
				this->setWrappingCubemapDX12(this->SamplerDesc12);
			else
				this->setWrappingDX12(this->SamplerDesc12);

			result = RenderEngine::Canvas.DX->CreateTexture12(pixels2, format, this);

			if (result < 0)
				wxMessageBox(("ERROR: Texture::loadTextureImagesDX12: Failed to create a texture from image files."), RenderEngine::Canvas.Window->GetTitle().c_str(), wxOK | wxICON_ERROR);

			break;
		}
	}

	for (auto pixels : pixels2)
		std::free(pixels);

	for (auto image : images2) {
		if (this->flipY)
			image.Destroy();
	}
}
#endif

void Texture::loadTextureImageGL(wxImage* image, bool cubemap, int index)
{
	wxImage  image2 = (this->flipY ? image->Mirror(false) : *image);
	uint8_t* pixels = (image2.HasAlpha() ? Utils::ToRGBA(&image2) : image2.GetData());
	GLenum   format = Utils::GetImageFormat(&image2);

	glBindTexture(this->type, this->id);

	this->size        = wxSize(image2.GetWidth(), image2.GetHeight());
	this->mipLevels   = ((uint32_t)(std::floor(std::log2(std::max(this->size.GetWidth(), this->size.GetHeight())))) + 1);
	this->transparent = (this->transparent && image2.HasAlpha());

	if (this->transparent)
		this->setAlphaBlendingGL(true);

	if (cubemap)
	{
		glTexParameteri(this->type, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(this->type, GL_TEXTURE_MAX_LEVEL,  0);

		glTexImage2D(
			(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index), 0, GL_RGBA8,
			image2.GetWidth(), image2.GetHeight(), 0, format, GL_UNSIGNED_BYTE, pixels
		);

		this->setWrappingCubemapGL();
		this->setFilteringGL(false);
	}
	else
	{
		glTexParameteri(this->type, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(this->type, GL_TEXTURE_MAX_LEVEL,  this->mipLevels - 1);

		// https://www.khronos.org/opengl/wiki/Common_Mistakes#Automatic_mipmap_generation
		glTexStorage2D(this->type, this->mipLevels, GL_RGBA8, image2.GetWidth(), image2.GetHeight());
		glTexSubImage2D(this->type, 0, 0, 0, image2.GetWidth(), image2.GetHeight(), format, GL_UNSIGNED_BYTE, pixels);

		glGenerateMipmap(this->type);

		this->setWrappingGL();
		this->setFilteringGL(true);
	}
	
	if (this->transparent)
		this->setAlphaBlendingGL(false);

	glBindTexture(this->type, 0);

	if (image2.HasAlpha())
		std::free(pixels);

	if (this->flipY)
		image2.Destroy();
}

void Texture::loadTextureImagesVK(const std::vector<wxImage*> &images)
{
	std::vector<wxImage>  images2;
	std::vector<uint8_t*> pixels2;

	for (auto image : images)
	{
		wxImage image2 = (this->flipY ? image->Mirror(false) : *image);
		images2.push_back(image2);
		pixels2.push_back(Utils::ToRGBA(&image2));
	}

	if (!images2.empty())
	{
		this->size        = wxSize(images2[0].GetWidth(), images2[0].GetHeight());
		this->mipLevels   = ((uint32_t)(std::floor(std::log2(std::max(this->size.GetWidth(), this->size.GetHeight())))) + 1);
		this->transparent = (this->transparent && images2[0].HasAlpha());

		this->setFilteringVK(this->SamplerInfo);

		if (images.size() > 1)
			this->setWrappingCubemapVK(this->SamplerInfo);
		else
			this->setWrappingVK(this->SamplerInfo);

		int result = RenderEngine::Canvas.VK->CreateTexture(pixels2, this);

		if (result < 0)
			wxMessageBox(("ERROR: Texture::loadTextureImagesVK: Failed to create a texture from image files."), RenderEngine::Canvas.Window->GetTitle().c_str(), wxOK | wxICON_ERROR);
	}

	for (auto pixels : pixels2)
		std::free(pixels);

	for (auto image : images2) {
		if (this->flipY)
			image.Destroy();
	}
}

uint32_t Texture::MipLevels()
{
	return this->mipLevels;
}

void Texture::reload()
{
	wxImage* image = nullptr;

	if (!this->imageFiles.empty() && (this->id > 0))
		image = Utils::LoadImageFile(this->imageFiles[0]);

	if (image != nullptr) {
		this->loadTextureImageGL(image);
		image->Destroy();
	}
}

bool Texture::Repeat()
{
	return this->repeat;
}

void Texture::setAlphaBlendingGL(bool enable)
{
	if (enable) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	} else {
		glDisable(GL_BLEND);
	}
}

#if defined _WINDOWS
void Texture::setFilteringDX11(D3D11_SAMPLER_DESC &samplerDesc)
{
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
}

void Texture::setFilteringDX12(D3D12_SAMPLER_DESC &samplerDesc)
{
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
}
#endif

void Texture::setFilteringGL(bool mipmap)
{
	glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, (mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR));
	glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::setFilteringVK(VkSamplerCreateInfo &samplerInfo)
{
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
}

#if defined _WINDOWS
void Texture::setWrappingDX11(D3D11_SAMPLER_DESC &samplerDesc)
{
	samplerDesc.AddressU = (this->repeat && !this->transparent ? D3D11_TEXTURE_ADDRESS_WRAP : D3D11_TEXTURE_ADDRESS_CLAMP);
	samplerDesc.AddressV = samplerDesc.AddressW = samplerDesc.AddressU;
}

void Texture::setWrappingDX12(D3D12_SAMPLER_DESC &samplerDesc)
{
	samplerDesc.AddressU = (this->repeat && !this->transparent ? D3D12_TEXTURE_ADDRESS_MODE_WRAP : D3D12_TEXTURE_ADDRESS_MODE_CLAMP);
	samplerDesc.AddressV = samplerDesc.AddressW = samplerDesc.AddressU;
}
#endif

void Texture::setWrappingGL()
{
	glTexParameteri(this->type, GL_TEXTURE_WRAP_S, (this->repeat && !this->transparent ? GL_REPEAT : GL_CLAMP_TO_EDGE));
	glTexParameteri(this->type, GL_TEXTURE_WRAP_T, (this->repeat && !this->transparent ? GL_REPEAT : GL_CLAMP_TO_EDGE));
}

void Texture::setWrappingVK(VkSamplerCreateInfo &samplerInfo)
{
	samplerInfo.addressModeU = (this->repeat && !this->transparent ? VK_SAMPLER_ADDRESS_MODE_REPEAT : VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
	samplerInfo.addressModeV = samplerInfo.addressModeW = samplerInfo.addressModeU;
}

#if defined _WINDOWS
void Texture::setWrappingCubemapDX11(D3D11_SAMPLER_DESC &samplerDesc)
{
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = samplerDesc.AddressW = samplerDesc.AddressU;
}
#endif

void Texture::setWrappingCubemapDX12(D3D12_SAMPLER_DESC &samplerDesc)
{
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc.AddressV = samplerDesc.AddressW = samplerDesc.AddressU;
}

void Texture::setWrappingCubemapGL()
{
	glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(this->type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Texture::setWrappingCubemapVK(VkSamplerCreateInfo &samplerInfo)
{
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.addressModeV = samplerInfo.addressModeW = samplerInfo.addressModeU;
}

void Texture::SetFlipY(bool newFlipY)
{
	this->flipY = newFlipY;
	this->reload();
}

void Texture::SetRepeat(bool newRepeat)
{
	this->repeat = newRepeat;
	this->reload();
}

void Texture::SetTransparent(bool newTransparent)
{
	this->transparent = newTransparent;
	this->reload();
}

wxSize Texture::Size()
{
	return this->size;
}

bool Texture::Transparent()
{
	return this->transparent;
}

GLenum Texture::Type()
{
    return this->type;
}
