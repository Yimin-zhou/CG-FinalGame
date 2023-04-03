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

glm::vec3 BezierCurve::bezierCurveDerivative(float t) const {
    glm::vec3 dP0 = 3.0f * (m_controlPoints[1] - m_controlPoints[0]);
    glm::vec3 dP1 = 3.0f * (m_controlPoints[2] - m_controlPoints[1]);
    glm::vec3 dP2 = 3.0f * (m_controlPoints[3] - m_controlPoints[2]);

    float u = 1.0f - t;

    glm::vec3 tangent = (u * u * dP0) + (2.0f * u * t * dP1) + (t * t * dP2);

    return tangent;
}

float BezierCurve::bezierCurveLength(int numSamples) const {
    float length = 0.0f;
    float dt = 1.0f / static_cast<float>(numSamples);

    for (int i = 0; i < numSamples; ++i) {
        float t0 = i * dt;
        float t1 = (i + 1) * dt;

        glm::vec3 tangent0 = bezierCurveDerivative(t0);
        glm::vec3 tangent1 = bezierCurveDerivative(t1);

        float segmentLength = (glm::length(tangent0) + glm::length(tangent1)) / 2.0f * dt;
        length += segmentLength;
    }

    return length;
}


std::vector<float> BezierCurve::generateArcLengthTable(int numSamples) const{
    std::vector<float> arcLengthTable(numSamples + 1);

    arcLengthTable[0] = 0.0f;
    float totalLength = bezierCurveLength(numSamples);
    float dt = 1.0f / static_cast<float>(numSamples);

    for (int i = 1; i <= numSamples; ++i) {
        float t = i * dt;
        glm::vec3 tangent = bezierCurveDerivative(t);
        float segmentLength = glm::length(tangent) * dt;
        arcLengthTable[i] = arcLengthTable[i - 1] + segmentLength / totalLength;
    }

    return arcLengthTable;

}


float BezierCurve::findTForArcLength(const std::vector<float>& arcLengthTable, float arcLength) const{
    auto it = std::lower_bound(arcLengthTable.begin(), arcLengthTable.end(), arcLength);

    if (it == arcLengthTable.end()) {
        return 1.0f;
    }

    int index = std::distance(arcLengthTable.begin(), it);
    float t0 = (index - 1) / static_cast<float>(arcLengthTable.size() - 1);
    float t1 = index / static_cast<float>(arcLengthTable.size() - 1);

    float l0 = arcLengthTable[index - 1];
    float l1 = arcLengthTable[index];

    return t0 + (arcLength - l0) / (l1 - l0) * (t1 - t0);
}
