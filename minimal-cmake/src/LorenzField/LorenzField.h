#pragma once

#include "LorenzField/MyVector.h"

namespace LorenzField {
    using Vector3 = MyVector::Vector3;

class LorenzField {
public:
    LorenzField(float rho = 28.0f, float sigma = 10.0f, float beta = 8.0f / 3.0f)
        : _rho(rho), _sigma(sigma), _beta(beta) {}

    Vector3 valueAt(const Vector3& position);
    Vector3 curlAt(const Vector3& position);

private:
    float _rho;
    float _sigma;
    float _beta;
};

} // namespace LorenzField