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
	void RenderToScreen();

	void SetShader(Shader& shader);

	void resize(int width, int height);

private:
	void initScreenQuad();
	void initFramebuffer();

	int m_width;
	int m_height;

	Shader m_screenShader;
	GLuint m_quadVAO, m_quadVBO;
	GLuint m_framebuffer, m_rt;
	GLuint m_depthStencilRBO;
};
