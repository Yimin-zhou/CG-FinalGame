#include <glm/glm.hpp>



class BezierCurve {
public:
    BezierCurve(const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2, const glm::vec3& P3);

    glm::vec3 evaluate(float t) const;

private:
    glm::vec3 m_controlPoints[4];
};
