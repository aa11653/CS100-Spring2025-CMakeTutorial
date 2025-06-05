#include "LorenzField.h"

namespace LorenzField {
    Magnum::Vector3 LorenzField::valueAt(const Magnum::Vector3& position) {
        return Magnum::Vector3(
            _sigma * (position.y() - position.x()),
            position.x() * (_rho - position.z()) - position.y(),
            position.x() * position.y() - _beta * position.z()
        );
    }

    Magnum::Vector3 LorenzField::curlAt(const Magnum::Vector3& position) {
        return Magnum::Vector3(
            2.0f * position.x(),
            -position.y(),
            _rho - position.z() - _sigma
        );
    }
}