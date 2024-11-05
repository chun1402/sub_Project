#ifndef AXISLINE_HPP
#define AXISLINE_HPP
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //value_ptr
#include <iostream>
#include <vector>

class AxisLine {
public:
	struct Line {
		glm::vec3 startPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		double length = 0.0f;
		double angle = 0.0f; 
		glm::vec3 endPosition = glm::vec3(0.0f, 0.0f, 0.0f);

		void calculateEndPosition() {
			// Convert angle from degrees to radians
			double angleRad = glm::radians(angle);
			// Calculate end position based on start position, length, and angle
			endPosition.x = startPosition.x + length * cos(angleRad);
			endPosition.y = startPosition.y + length * sin(angleRad);
			endPosition.z = startPosition.z; // Assuming the line is on the XY plane
		}
	};
	struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
	};

	Line line;
	glm::vec3 mLineColor = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 mPointColor = glm::vec3(0.0f, 0.0f, 0.0f);
	float mLineWidth = 1.0f;
	void draw();
private:
	void drawLine();
	void drawPoint();
};



#endif
