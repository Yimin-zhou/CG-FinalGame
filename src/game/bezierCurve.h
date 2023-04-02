#include <glm/glm.hpp>

#include <vector>

class BezierCurve {
public:
    BezierCurve(const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2, const glm::vec3& P3);

    glm::vec3 evaluate(float t) const;

    glm::vec3 bezierCurveDerivative(float t) const;
    float bezierCurveLength(int numSamples) const;
    std::vector<float> generateArcLengthTable(int numSamples) const;
    float findTForArcLength(const std::vector<float>& arcLengthTable, float arcLength) const;
private:
    glm::vec3 m_controlPoints[4];
};
