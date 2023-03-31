#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "bezierCurve.h"

class CompositeBezierCurve
{
public:
    std::vector<BezierCurve> curves;
    
    CompositeBezierCurve(const std::vector<BezierCurve>& curves);
    CompositeBezierCurve();

    glm::vec3 GetPoint(float t) const;

    
private:
    
};