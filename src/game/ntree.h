#pragma once

#include <vector>
#include <stack>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ObjectNode
{
public:
	glm::mat4 transform;
	std::vector<std::shared_ptr<ObjectNode>> children;

	ObjectNode(glm::mat4 x) : transform(x) {}
	void addChild(std::shared_ptr<ObjectNode> child)
	{
		children.push_back(child);
	}
};

inline void traverse(std::shared_ptr<ObjectNode> root)
{
	std::stack<std::shared_ptr<ObjectNode>> s;
	s.push(root);

	std::stack<glm::mat4> matrixStack;
	matrixStack.push(glm::mat4(1.0f)); // identity matrix

	while (!s.empty())
	{
		std::shared_ptr<ObjectNode> node = s.top();
		s.pop();

		glm::mat4 currentMatrix = matrixStack.top();
		matrixStack.pop();

		// Apply the parent's transform to the node's transform
		node->transform = currentMatrix * node->transform;

		// Push the children onto the stack, along with their transform matrix
		for (auto it = node->children.rbegin(); it != node->children.rend(); ++it)
		{
			s.push(*it);
			matrixStack.push(node->transform);
		}
	}
}