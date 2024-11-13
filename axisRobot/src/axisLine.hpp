#ifndef AXISLINE_HPP
#define AXISLINE_HPP
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <iostream>
#include <vector>
#include <cmath>

class AxisLine {
public:
    struct Line {
        glm::vec3 startPosition = glm::vec3(0.0f, 0.0f, 0.0f);
        double length = 0.0f;
        double angle = 0.0f; 
		  glm::vec3 endPosition = glm::vec3(0.0f, 0.0f, 0.0f);
        double cumulativeAngle = 0.0f; 
        void calculateEndPosition(double parentAngle) {
            // 부모의 절대각도와 현재각도를 더해서 절대각도 계산
            cumulativeAngle = parentAngle + angle;

            // 각도를 라디안으로 변환
            double angleRad = glm::radians(cumulativeAngle);

            // 종단 위치 계산
            endPosition.x = startPosition.x + length * cos(angleRad);
            endPosition.y = startPosition.y + length * sin(angleRad);
            endPosition.z = startPosition.z;
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
    float mPointSize = 10.0f;
    void draw();
	 AxisLine();
	 ~AxisLine();
private:
	 GLuint VAO, VBO;
    void drawLineAndPoint();
    void drawPoint();
};

#endif

