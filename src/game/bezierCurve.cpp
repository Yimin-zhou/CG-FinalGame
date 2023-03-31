#include "BezierCurve.h"

BezierCurve::BezierCurve(const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2, const glm::vec3& P3) {
    m_controlPoints[0] = P0;
    m_controlPoints[1] = P1;
    m_controlPoints[2] = P2;
    m_controlPoints[3] = P3;
}

glm::vec3 BezierCurve::evaluate(float t) const {
    float u = 1.0f - t;
    float uu = u * u;
    float uuu = uu * u;
    float tt = t * t;
    float ttt = tt * t;

    glm::vec3 point = (uuu * m_controlPoints[0]) + (3.0f * uu * t * m_controlPoints[1]) + (3.0f * u * tt * m_controlPoints[2]) + (ttt * m_controlPoints[3]);

    return point;
}
