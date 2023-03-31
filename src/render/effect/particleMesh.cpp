
#include "particleMesh.h"

ParticleMesh::ParticleMesh()
{
	// Create vertex and index data for a quad
	m_vertices = 
	{
		// Position (X, Y, Z),  Normal (NX, NY, NZ), Texture coordinates (U, V)
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f
	};

	m_indices = 
	{
		0, 1, 2,
		2, 3, 0
	};

	// Create VAO, VBO, and EBO for the mesh
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);

	// Normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	// Texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

	glBindVertexArray(0);
}

ParticleMesh::~ParticleMesh()
{
	glDeleteBuffers(1, &m_ebo);
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}

GLuint ParticleMesh::getVAO() const
{
	return m_vao;
}

GLsizei ParticleMesh::getIndicesCount() const
{
	return static_cast<GLsizei>(m_indices.size());
}

