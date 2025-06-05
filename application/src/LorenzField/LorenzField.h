#pragma once
#include <Magnum/Math/Vector3.h>

namespace LorenzField {

class LorenzField {
public:
    LorenzField(float rho = 28.0f, float sigma = 10.0f, float beta = 8.0f / 3.0f)
        : _rho(rho), _sigma(sigma), _beta(beta) {}

    Magnum::Vector3 valueAt(const Magnum::Vector3& position);
    Magnum::Vector3 curlAt(const Magnum::Vector3& position);

private:
    float _rho;
    float _sigma;
    float _beta;
};

} // namespace LorenzField