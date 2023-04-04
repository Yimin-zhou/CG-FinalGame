#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <framework/shader.h>
#include <iostream>

class PostProcessing
{
public:
	PostProcessing(const uint32_t width, const uint32_t height);
	~PostProcessing();

	void BindFramebuffer();
	void UnbindFramebuffer();

	void BlurScreenTex();
	void RenderToScreen();

	void SetShader(Shader& screen, Shader& blur);

	void resize(int width, int height);

private:
	void initScreenQuad();
	void initFramebuffer();

	int m_width;
	int m_height;

	Shader m_screenShader;
	GLuint m_quadVAO, m_quadVBO;
	GLuint m_framebuffer, m_rt[2];
	GLuint m_depthStencilRBO;

	GLuint m_pingpongFBO[2];
	GLuint m_pingpongBuffer[2];
	Shader m_blurShader;

};
