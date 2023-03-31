#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

class ParticleMesh 
{
public:
	ParticleMesh();

	~ParticleMesh();

	GLuint getVAO() const;
	GLsizei getIndicesCount() const;

private:
	std::vector<GLfloat> m_vertices;
	std::vector<GLuint> m_indices;
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ebo;
};
