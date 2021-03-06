#ifndef GE3D_GLOBALS_H
	#include "../globals.h"
#endif

#ifndef GE3D_SHADERPROGRAM_H
#define GE3D_SHADERPROGRAM_H

class ShaderProgram
{
public:
	ShaderProgram(const wxString &name);
	~ShaderProgram();

public:
	GLuint Attribs[NR_OF_ATTRIBS];
	GLint  Uniforms[NR_OF_UBOS_GL];
	GLuint UniformBuffers[NR_OF_UBOS_GL];

private:
	wxString       name;
	GLuint         program;
	VkShaderModule vulkanFS;
	VkShaderModule vulkanVS;

	#if defined _WINDOWS
		ID3D11PixelShader*  shaderFS;
		ID3D11VertexShader* shaderVS;
		ID3DBlob*           fs;
		ID3DBlob*           vs;
	#endif

public:
	bool           IsOK();
	int            Link();
	int            Load(const wxString &shaderFile);
	int            LoadAndLink(const wxString &vs, const wxString &fs);
	void           Log();
	void           Log(GLuint shader);
	wxString       Name();
	GLuint         Program();
	int            UpdateAttribsGL(Mesh* mesh);
	int            UpdateUniformsGL(Mesh* mesh, const DrawProperties &properties = {});
	int            UpdateUniformsVK(VkDevice deviceContext, Mesh* mesh, const VKUniform &uniform, const DrawProperties &properties = {});
	VkShaderModule VulkanFS();
	VkShaderModule VulkanVS();

	#if defined _WINDOWS
		ShaderID            ID();
		ID3DBlob*           FS();
		ID3DBlob*           VS();
		ID3D11PixelShader*  FragmentShader();
		ID3D11VertexShader* VertexShader();
		int                 UpdateUniformsDX11(ID3D11Buffer** constBuffer, const void** constBufferValues, Mesh* mesh, const DrawProperties &properties = {});
		int                 UpdateUniformsDX12(Mesh* mesh, const DrawProperties &properties = {});
	#endif

private:
	int  loadShaderGL(GLuint type, const wxString &sourceText);
	void setAttribsGL();
	void setUniformsGL();
	void updateUniformGL(GLint id, UniformBufferTypeGL buffer, void* values, size_t valuesSize);
	int  updateUniformsVK(UniformBufferTypeVK type, UniformBinding binding, const VKUniform &uniform, void* values, size_t valuesSize, VkDevice deviceContext, Mesh* mesh);
	int  updateUniformSamplersVK(VkDescriptorSet uniformSet, VkDevice deviceContext, Mesh* mesh);

	#if defined _WINDOWS
		DXLightBuffer  getBufferLight();
		DXMatrixBuffer getBufferMatrices(Mesh* mesh, bool removeTranslation = false);
		const void*    getBufferValues(const DXMatrixBuffer &matrices, const DXLightBuffer &sunLight, Mesh* mesh, const DrawProperties &properties = {});
	#endif
};

#endif
