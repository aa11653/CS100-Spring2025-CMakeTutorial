#include "LorenzField/LorenzField.h"

namespace LorenzField {
    Vector3 LorenzField::valueAt(const Vector3& position) {
        return Vector3(
            _sigma * (position.y() - position.x()),
            position.x() * (_rho - position.z()) - position.y(),
            position.x() * position.y() - _beta * position.z()
        );
    }

    Vector3 LorenzField::curlAt(const Vector3& position) {
        return Vector3(
            2.0f * position.x(),
            -position.y(),
            _rho - position.z() - _sigma
        );
    }
}