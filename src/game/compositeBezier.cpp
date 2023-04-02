#include "compositeBezier.h"


CompositeBezierCurve::CompositeBezierCurve(const std::vector<BezierCurve>& curves) :
    curves(curves) {}

CompositeBezierCurve::CompositeBezierCurve() {};

glm::vec3 CompositeBezierCurve::GetPoint(float t) const
{
    int segmentCount = curves.size();
    float segmentT = t * segmentCount;
    int segmentIndex = static_cast<int>(segmentT);
    segmentIndex = std::min(segmentIndex, segmentCount - 1);
    float localT = segmentT - segmentIndex;

    return curves[segmentIndex].evaluate(localT);
}
