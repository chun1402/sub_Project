#ifndef AXISLINE_HPP
#define AXISLINE_HPP

class AxisLine {
public:
struct Line {
    glm::vec3 startPosition;
    double length;
    double angle; // angle in degrees
    glm::vec3 endPosition;

    void calculateEndPosition() {
        // Convert angle from degrees to radians
        double angleRad = glm::radians(angle);
        // Calculate end position based on start position, length, and angle
        endPosition.x = startPosition.x + length * cos(angleRad);
        endPosition.y = startPosition.y + length * sin(angleRad);
        endPosition.z = startPosition.z; // Assuming the line is on the XY plane
    }
};


private:


};



#endif
