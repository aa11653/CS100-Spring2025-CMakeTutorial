#include "LorenzField.h"
#include <iostream>

int main() {
    LorenzField::LorenzField field{};
    LorenzField::Vector3 position{};

    std::cout << "Input Initial Position (x, y, z): \n";
    std::cin >> position.x() >> position.y() >> position.z();

    bool usePythonArrayFormat = false;
    std::cout << "Use Python array format? (0 for No, 1 for Yes): ";
    std::cin >> usePythonArrayFormat;
    

    size_t numSteps = 500;
    for(int i = 0; i < numSteps; i ++) {
        LorenzField::Vector3 value = field.valueAt(position);
        LorenzField::Vector3 curl = field.curlAt(position);

        if (usePythonArrayFormat) {
            if (i == 0) {
                std::cout << "[";
            }
            std::cout << "[" << value.x() << ", " << value.y() << ", " << value.z() << "]";
            if (i != numSteps - 1) {
                std::cout << ", ";
            } else {
                std::cout << "]\n";
            }
            
        } else {
            std::cout << "Value: (" << value.x() << ", " << value.y() << ", " << value.z() << ")\n";
        }
        position += value * 0.01f; // Update position with a small step
    }

    std::cout << "Final Position: (" << position.x() << ", " << position.y() << ", " << position.z() << ")\n";
    return 0;
}