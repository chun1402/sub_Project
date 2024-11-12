#include "axisLine.hpp"

void AxisLine::drawLine() {
	Vertex vertices[2] = {
		{line.startPosition, mLineColor}, 
		{line.endPosition, mLineColor}     
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 위치 속성
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	// 색상 속성
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(5);

	// 선 굵기 설정
	glLineWidth(mLineWidth); 

	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void AxisLine::drawPoint() {
	Vertex vertices[2] = {
		{line.startPosition, mPointColor}, 
		{line.endPosition, mPointColor}    
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 위치 속성
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	// 색상 속성
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(5);

	glPointSize(mPointSize);

	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, 2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void AxisLine::draw() {
	drawLine();
	drawPoint();
}

