#include "postprocessing.h"	

void PostProcessing::initScreenQuad()
{
	// Set up screen quad geometry
	float quadVertices[] = 
	{
		// Positions      // TexCoords
		-1.0f,  1.0f,     0.0f, 1.0f,
		-1.0f, -1.0f,     0.0f, 0.0f,
		 1.0f, -1.0f,     1.0f, 0.0f,

		-1.0f,  1.0f,     0.0f, 1.0f,
		 1.0f, -1.0f,     1.0f, 0.0f,
		 1.0f,  1.0f,     1.0f, 1.0f
	};

	glGenVertexArrays(1, &m_quadVAO);
	glGenBuffers(1, &m_quadVBO);
	glBindVertexArray(m_quadVAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindVertexArray(0);
}

PostProcessing::~PostProcessing()
{
    glDeleteVertexArrays(1, &m_quadVAO);
    glDeleteBuffers(1, &m_quadVBO);
    glDeleteFramebuffers(1, &m_framebuffer);
    glDeleteTextures(1, &m_rt);
	glDeleteRenderbuffers(1, &m_depthStencilRBO);
}

void PostProcessing::BindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
}

void PostProcessing::UnbindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessing::RenderToScreen()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	// disable blending
	glDisable(GL_BLEND);
	// disable face culling
	glDisable(GL_CULL_FACE);

	m_screenShader.bind();
	glBindVertexArray(m_quadVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_rt);
	glUniform1i(0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void PostProcessing::SetShader(Shader& shader)
{
	m_screenShader = shader;
}

void PostProcessing::resize(int width, int height)
{
	m_width = width;
	m_height = height;

	// Resize the framebuffer
}

void PostProcessing::initFramebuffer()
{
	// Generate framebuffer
	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

	// Generate texture to store color information with HDR support
	glGenTextures(1, &m_rt);
	glBindTexture(GL_TEXTURE_2D, m_rt);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_rt, 0);

	// Create and configure the depth-stencil renderbuffer
	glGenRenderbuffers(1, &m_depthStencilRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilRBO);


	// Check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "ERROR: Framebuffer is not complete!" << std::endl;
	}

	// Unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

PostProcessing::PostProcessing(const uint32_t width, const uint32_t height) : 
	m_width(width),
	m_height(height)
{
	initScreenQuad();
	initFramebuffer();
}
