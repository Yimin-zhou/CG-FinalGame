#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class PostProcessing
{
public:
	PostProcessing(const int width, const int height);
	~PostProcessing();

	void render(GLuint texture);

private:
	void initScreenQuad();
	void initFramebuffer();

	int m_width;
	int m_height;

	GLuint m_vao;
	GLuint m_vbo;

	GLuint m_fbo;
	GLuint m_rbo;

	GLuint m_screenShader;
};
