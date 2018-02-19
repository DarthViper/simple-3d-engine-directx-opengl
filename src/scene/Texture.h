#ifndef GE3D_GLOBALS_H
	#include "../globals.h"
#endif

#ifndef GE3D_TEXTURE_H
#define GE3D_TEXTURE_H

class Texture
{
public:
	Texture(wxImage* image, bool repeat = false, bool flipY = false, bool transparent = false, const glm::vec2 &scale = glm::vec2(1.0f, 1.0f));
	Texture(const wxString &imageFile, bool repeat = false, bool flipY = false, bool transparent = false, const glm::vec2 &scale = glm::vec2(1.0f, 1.0f));
	Texture(const std::vector<wxString> &imageFiles, bool repeat = false, bool flipY = false, bool transparent = false, const glm::vec2 &scale = glm::vec2(1.0f, 1.0f));
	Texture(GLint filter, GLint formatIn, GLenum formatOut, GLenum dataType, GLenum attachment, int width = 100, int height = 100);
	Texture();
	~Texture();

	#ifdef _WINDOWS
	Texture(D3D11_FILTER filter, DXGI_FORMAT format, int width = 100, int height = 100);
	Texture(D3D12_FILTER filter, DXGI_FORMAT format, int width = 100, int height = 100);
#endif

public:
	glm::vec2 Scale;

private:
	bool                  flipY;
	GLuint                id;
	std::vector<wxString> imageFiles;
	bool                  repeat;
	bool                  transparent;
	GLenum                type;

	#ifdef _WINDOWS
		ID3D11RenderTargetView*         colorBuffer11;
		D3D11_VIEWPORT                  colorBufferViewPort11;
		ID3D11Texture2D*                resource11;
		ID3D11SamplerState*             samplerState11;
		ID3D11ShaderResourceView*       srv11;
		ID3D12DescriptorHeap*           colorBuffer12;
		D3D12_VIEWPORT                  colorBufferViewPort12;
		ID3D12Resource*                 resource12;
		D3D12_SAMPLER_DESC              samplerDesc12;
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc12;
	#endif

public:
	bool     FlipY();
	bool     Repeat();
	bool     Transparent();
	GLuint   ID();
	wxString ImageFile(int index = 0);
	bool     IsOK();
	void     SetFlipY(bool newFlipY);
	void     SetRepeat(bool newRepeat);
	void     SetTransparent(bool newTransparent);
	GLenum   Type();

	#ifdef _WINDOWS
		ID3D11RenderTargetView*                ColorBuffer11();
		ID3D12DescriptorHeap*                  ColorBuffer12();
		D3D11_VIEWPORT*                        ColorBufferViewPort11();
		D3D12_VIEWPORT*                        ColorBufferViewPort12();
		ID3D11SamplerState*                    SamplerState11();
		ID3D11ShaderResourceView*              SRV11();
		ID3D12Resource*                        Resource12();
		const D3D12_SAMPLER_DESC*              SamplerDesc12();
		const D3D12_SHADER_RESOURCE_VIEW_DESC* SRVDesc12();
	#endif

private:
	void loadTextureImageGL(wxImage* image, bool cubemap = false, int index = 0);
	void reload();
	void setAlphaBlending(bool enable);
	void setFilteringGL(bool mipmap = true);
	void setWrappingGL();
	void setWrappingCubemapGL();

	#ifdef _WINDOWS
		void loadTextureImagesDX(const std::vector<wxImage*> &images);
		void setFilteringDX11(D3D11_SAMPLER_DESC &samplerDesc);
		void setFilteringDX12(D3D12_SAMPLER_DESC &samplerDesc);
		void setWrappingDX11(D3D11_SAMPLER_DESC &samplerDesc);
		void setWrappingDX12(D3D12_SAMPLER_DESC &samplerDesc);
		void setWrappingCubemapDX11(D3D11_SAMPLER_DESC &samplerDesc);
		void setWrappingCubemapDX12(D3D12_SAMPLER_DESC &samplerDesc);
	#endif

};

#endif
