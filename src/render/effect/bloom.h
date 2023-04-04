#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <framework/shader.h>
#include <iostream>

struct BloomMip
{
	glm::vec2 size;
	glm::ivec2 intSize;
	GLuint texture;
};

class Bloom
{
public:
	Bloom(const uint32_t width, const uint32_t height, uint32_t mipChainLength);
	~Bloom();

	void BindDownUpFramebuffer();
	void BindHDRFramebuffer();

	void RenderDownsample();
	void RenderUpsample();

	void RenderHDR();

	void UnbindFramebuffer();

	void SetShader(Shader& down, Shader& up, Shader& result);

	void resize(int width, int height);

private:
	void initScreenQuad();
	void initHDRFramebuffer();
	void initDownUpFramebuffer(uint32_t mipChainLength);

	int m_width;
	int m_height;

	Shader m_downShader;
	Shader m_upShader;
	Shader m_resultShader;
	GLuint m_quadVAO, m_quadVBO;
	GLuint m_downUpFBO;

	GLuint m_hdrFBO;
	GLuint m_hdrRT[2];
	GLuint m_depthStencilRBO;

	std::vector<BloomMip> m_mipChain;
};
