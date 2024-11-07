#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //value_ptr
#include <iostream>
#include "shader.hpp"
#include "axisLine.hpp"

//camera
double fov = 45.0f;

glm::vec3 vecDir = glm::vec3(80., 0., 0.);
glm::vec3 vecPos = glm::vec3(0, -2.5, 1.);

#define AxisX glm::vec3(1.,0.,0.)
#define AxisY glm::vec3(0.,1.,0.)
#define AxisZ glm::vec3(0.,0.,1.)

void mouse_callback(GLFWwindow* window, double, double);
void scroll_callback(GLFWwindow *, double, double yoffset);
void keyboard_camera(GLFWwindow* window, float);

static void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "Glfw Error %d: %s\n", error, description); }

void processInput(GLFWwindow *window){	
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

//glm::mat view
glm::mat4 makeViewMat(const glm::vec3 &pos, const glm::vec3 &dir) {
	glm::mat4 mView = glm::mat4(1.0f);
	mView = glm::rotate(mView, glm::radians(-dir.x), AxisX);
	mView = glm::rotate(mView, glm::radians(-dir.y), AxisY);
	mView = glm::rotate(mView, glm::radians(-dir.z), AxisZ);
	mView = glm::translate(mView, -pos);
	return mView;
}

int main(int argc, char** argv) {
	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit()) return 1;

	// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char* glsl_version = "#version 100";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	// Create window with graphics context
	GLFWwindow* window = glfwCreateWindow(1280, 720, "6 axis robot - AmberVine", NULL, NULL);
	if (window == NULL) return 1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	glewInit();	// Add for using shader. // Mouse & KeyBoard event functions.
	glfwSetCursorPosCallback(window, mouse_callback);  // Mouse click
	glfwSetScrollCallback(window, scroll_callback);    // Mouse wheel

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Our state
	bool show_demo_window = true;
	bool show_another_window = true;
	bool show_line_setting = true;
	bool show_color_setting = true;
	bool show_line_position = true;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE); //point size setting 하려면 필요함.

	//shader
	Shader ourShader("../shader.vs", "../shader.fs"); // you can name your shader files however
	ourShader.use(); // don't forget to activate/use the shader before setting uniforms!

	glm::mat4 matProj, matView;
	
	// 6axis class
	AxisLine axisLine1;
	AxisLine axisLine2;
	AxisLine axisLine3;
	AxisLine axisLine4;
	AxisLine axisLine5;
	AxisLine axisLine6;

	// Main loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		//input keyboard
		processInput(window);
		keyboard_camera(window, 0.01);

		// imgui window
		//if (show_demo_window)
		//	ImGui::ShowDemoWindow(&show_demo_window);
		if (show_another_window) {
			ImGui::Begin("Another Window");   			
			ImGui::Text("line, color open window!");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			if (ImGui::Button("Open line setting"))
				show_line_setting = true;
			if (ImGui::Button("Open color setting"))
				show_color_setting = true;
			if (ImGui::Button("Open line position"))
				show_line_position = true;
			ImGui::End();
		}

		//line start position, end position
		if(show_line_position) {
			ImGui::Begin("Line position", &show_line_position, ImGuiWindowFlags_AlwaysAutoResize);   			
			ImGui::Text("Line 1 Position    ");
			ImGui::Text("x:%.2f", axisLine1.line.startPosition.x);
			ImGui::Text("y:%.2f", axisLine1.line.startPosition.y);
			ImGui::Text("z:%.2f", axisLine1.line.startPosition.z);
			ImGui::Separator();

			ImGui::Text("Line 2 Position   ");
			ImGui::Text("x:%.2f", axisLine2.line.startPosition.x);
			ImGui::Text("y:%.2f", axisLine2.line.startPosition.y);
			ImGui::Text("z:%.2f", axisLine2.line.startPosition.z);
			ImGui::Separator();		
			
			ImGui::Text("Line 3 Position   ");
			ImGui::Text("x:%.2f", axisLine3.line.startPosition.x);
			ImGui::Text("y:%.2f", axisLine3.line.startPosition.y);
			ImGui::Text("z:%.2f", axisLine3.line.startPosition.z);
			ImGui::Separator();				

			ImGui::Text("Line 4 Position   ");
			ImGui::Text("x:%.2f", axisLine4.line.startPosition.x);
			ImGui::Text("y:%.2f", axisLine4.line.startPosition.y);
			ImGui::Text("z:%.2f", axisLine4.line.startPosition.z);
			ImGui::Separator();				

			ImGui::Text("Line 5 Position   ");
			ImGui::Text("x:%.2f", axisLine5.line.startPosition.x);
			ImGui::Text("y:%.2f", axisLine5.line.startPosition.y);
			ImGui::Text("z:%.2f", axisLine5.line.startPosition.z);
			ImGui::Separator();				

			ImGui::Text("Line 6 Position   ");
			ImGui::Text("x:%.2f", axisLine6.line.startPosition.x);
			ImGui::Text("y:%.2f", axisLine6.line.startPosition.y);
			ImGui::Text("z:%.2f", axisLine6.line.startPosition.z);
			ImGui::Separator();				
			ImGui::End();

		}

		if(show_line_setting) {
			ImGui::Begin("Line setting", &show_line_setting, ImGuiWindowFlags_AlwaysAutoResize);   			
			ImGui::Text("Line 1 setting");
			axisLine1.line.startPosition = glm::vec3(0.0f, 0.0f, 0.0f); // 시작점
			static float length1 = 1.0f;
			ImGui::InputFloat("Lenght1", &length1, 0.01f, 1.0f, "%.3f");
			static float angle1 = 1.0f;
			ImGui::InputFloat("Angle1", &angle1, 1.0f, 1.0f, "%.3f");
			axisLine1.line.length = length1; // 길이
			axisLine1.line.angle = angle1; // 각도
			axisLine1.line.calculateEndPosition();
			static float lineWidth1 = 1.0f;
			ImGui::SliderFloat("Line width1", &lineWidth1, 1, 20, "%.0f");			
			axisLine1.mLineWidth = lineWidth1;
			static float pointSize1 = 10.0f;
			ImGui::SliderFloat("Point size1", &pointSize1, 1, 100, "%.0f");			
			axisLine1.mPointSize = pointSize1;
			ImGui::Separator();

			ImGui::Text("Line 2 setting");
			axisLine2.line.startPosition = axisLine1.line.endPosition; // 시작점
			static float length2 = 1.0f;
			ImGui::InputFloat("Lenght2", &length2, 0.01f, 1.0f, "%.3f");
			static float angle2 = 1.0f;
			ImGui::InputFloat("Angle2", &angle2, 1.0f, 1.0f, "%.3f");
			axisLine2.line.length = length2; // 길이
			axisLine2.line.angle = angle2; // 각도
			axisLine2.line.calculateEndPosition();
			static float lineWidth2 = 1.0f;
			ImGui::SliderFloat("Line width2", &lineWidth2, 1, 20, "%.0f");			
			axisLine2.mLineWidth = lineWidth2;
			static float pointSize2 = 10.0f;
			ImGui::SliderFloat("Point size2", &pointSize2, 1, 100, "%.0f");			
			axisLine2.mPointSize = pointSize2;
			ImGui::Separator();

			ImGui::Text("Line 3 setting");
			axisLine3.line.startPosition = axisLine2.line.endPosition; // 시작점
			static float length3 = 1.0f;
			ImGui::InputFloat("Lenght3", &length3, 0.01f, 1.0f, "%.3f");
			static float angle3 = 1.0f;
			ImGui::InputFloat("Angle3", &angle3, 1.0f, 1.0f, "%.3f");
			axisLine3.line.length = length3; // 길이
			axisLine3.line.angle = angle3; // 각도
			axisLine3.line.calculateEndPosition();
			static float lineWidth3 = 1.0f;
			ImGui::SliderFloat("Line width3", &lineWidth3, 1, 20, "%.0f");			
			axisLine3.mLineWidth = lineWidth3;
			static float pointSize3 = 10.0f;
			ImGui::SliderFloat("Point size3", &pointSize3, 1, 100, "%.0f");			
			axisLine3.mPointSize = pointSize3;
			ImGui::Separator();

			ImGui::Text("Line 4 setting");
			axisLine4.line.startPosition = axisLine3.line.endPosition; // 시작점
			static float length4 = 1.0f;
			ImGui::InputFloat("Lenght4", &length4, 0.01f, 1.0f, "%.3f");
			static float angle4 = 1.0f;
			ImGui::InputFloat("Angle4", &angle4, 1.0f, 1.0f, "%.3f");
			axisLine4.line.length = length4; // 길이
			axisLine4.line.angle = angle4; // 각도
			axisLine4.line.calculateEndPosition();
			static float lineWidth4 = 1.0f;
			ImGui::SliderFloat("Line width4", &lineWidth4, 1, 20, "%.0f");			
			axisLine4.mLineWidth = lineWidth4;
			static float pointSize4 = 10.0f;
			ImGui::SliderFloat("Point size4", &pointSize4, 1, 100, "%.0f");			
			axisLine4.mPointSize = pointSize4;
			ImGui::Separator();

			ImGui::Text("Line 5 setting");
			axisLine5.line.startPosition = axisLine4.line.endPosition; // 시작점
			static float length5 = 1.0f;
			ImGui::InputFloat("Lenght5", &length5, 0.01f, 1.0f, "%.3f");
			static float angle5 = 1.0f;
			ImGui::InputFloat("Angle5", &angle5, 1.0f, 1.0f, "%.3f");
			axisLine5.line.length = length5; // 길이
			axisLine5.line.angle = angle5; // 각도
			axisLine5.line.calculateEndPosition();
			static float lineWidth5 = 1.0f;
			ImGui::SliderFloat("Line width5", &lineWidth5, 1, 20, "%.0f");			
			axisLine5.mLineWidth = lineWidth5;
			static float pointSize5 = 10.0f;
			ImGui::SliderFloat("Point size5", &pointSize5, 1, 100, "%.0f");			
			axisLine5.mPointSize = pointSize5;
			ImGui::Separator();

			ImGui::Text("Line 6 setting");
			axisLine6.line.startPosition = axisLine5.line.endPosition; // 시작점
			static float length6 = 1.0f;
			ImGui::InputFloat("Lenght6", &length6, 0.01f, 1.0f, "%.3f");
			static float angle6 = 1.0f;
			ImGui::InputFloat("Angle6", &angle6, 1.0f, 1.0f, "%.3f");
			axisLine6.line.length = length6; // 길이
			axisLine6.line.angle = angle6; // 각도
			axisLine6.line.calculateEndPosition();
			static float lineWidth6 = 1.0f;
			ImGui::SliderFloat("Line width6", &lineWidth6, 1, 20, "%.0f");			
			axisLine6.mLineWidth = lineWidth6;
			static float pointSize6 = 10.0f;
			ImGui::SliderFloat("Point size6", &pointSize6, 1, 100, "%.0f");			
			axisLine6.mPointSize = pointSize6;
			ImGui::Separator();
			ImGui::End();
		}

		if(show_color_setting) {
			ImGui::Begin("color setting", &show_color_setting, ImGuiWindowFlags_AlwaysAutoResize);   			
			ImGui::Text("Colr Setting window!");
			ImGui::Separator();
			ImGui::Text("Line color!");
			static float col1[3] = { 0.0f,0.0f,0.0f };
			static float col2[3] = { 1.0f,1.0f,1.0f };
			static float col3[3] = { 0.0f,0.0f,0.0f };
			static float col4[3] = { 1.0f,1.0f,1.0f };
			static float col5[3] = { 0.0f,0.0f,0.0f };
			static float col6[3] = { 1.0f,1.0f,1.0f };

			ImGui::ColorEdit3("color 1", col1);
			ImGui::ColorEdit3("color 2", col2);
			ImGui::ColorEdit3("color 3", col3);
			ImGui::ColorEdit3("color 4", col4);
			ImGui::ColorEdit3("color 5", col5);
			ImGui::ColorEdit3("color 6", col6);
			//line color
			axisLine1.mLineColor = glm::make_vec3(col1);
			axisLine2.mLineColor = glm::make_vec3(col2);
			axisLine3.mLineColor = glm::make_vec3(col3);
			axisLine4.mLineColor = glm::make_vec3(col4);
			axisLine5.mLineColor = glm::make_vec3(col5);
			axisLine6.mLineColor = glm::make_vec3(col6);
			//point color
			axisLine1.mPointColor = glm::make_vec3(col1); 
			axisLine2.mPointColor = glm::make_vec3(col2); 
			axisLine3.mPointColor = glm::make_vec3(col3); 
			axisLine4.mPointColor = glm::make_vec3(col4); 
			axisLine5.mPointColor = glm::make_vec3(col5); 
			axisLine6.mPointColor = glm::make_vec3(col6); 

			ImGui::Separator();
			ImGui::End();
		}


		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		float aspectRatio = static_cast<float>(display_w) / static_cast<float>(display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw
		axisLine1.draw(); 
		axisLine2.draw(); 
		axisLine3.draw(); 
		axisLine4.draw(); 
		axisLine5.draw(); 
		axisLine6.draw(); 

		// shader 
		glm::mat4 model = glm::mat4(1.0f);
		matProj = glm::perspective(glm::radians(fov), (display_w*1.)/(display_h*1.), 0.1, 10000.);
		matView = makeViewMat(vecPos, vecDir);
		glm::mat4 MVP = matProj * matView * model;
		GLuint MVPLoc = glGetUniformLocation(ourShader.ID, "MVP");
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(MVP));

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void keyboard_camera(GLFWwindow* window, float) {
	const float camSpeed = 0.01f; 	
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		vecPos.x += camSpeed * -sin(glm::radians(vecDir.z));
		vecPos.y += camSpeed * cos(glm::radians(vecDir.z));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		vecPos.x += camSpeed * sin(glm::radians(vecDir.z));
		vecPos.y += camSpeed * -cos(glm::radians(vecDir.z));
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		vecPos.x += camSpeed * cos(glm::radians(vecDir.z));
		vecPos.y += camSpeed * sin(glm::radians(vecDir.z));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		vecPos.x += camSpeed * -cos(glm::radians(vecDir.z));
		vecPos.y += camSpeed * -sin(glm::radians(vecDir.z));
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) vecPos += AxisZ*camSpeed;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) vecPos += -AxisZ*camSpeed;
}

void mouse_callback(GLFWwindow* window, double, double) {
	ImGuiIO& io = ImGui::GetIO();

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) return;
	if (io.WantCaptureMouse) return;
	if (io.WantCaptureKeyboard) return;

	double mouseSpeed  = 0.01f;
	double xoffset = io.MouseDelta.x;
	double yoffset = io.MouseDelta.y;

	vecDir.z += (float)(xoffset * mouseSpeed);
	vecDir.x += (float)(yoffset * mouseSpeed);

	if(vecDir.x < -180.0f) vecDir.x = -180.0f;
	if(vecDir.x >  180.0f) vecDir.x =  180.0f;

	if(vecDir.z < -360.0f) vecDir.z += 360.0f;
	if(vecDir.z >  360.0f) vecDir.z -= 360.0f;
}

void scroll_callback(GLFWwindow *, double, double yoffset) {
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse) return;

	fov -= yoffset;
	if (fov <  1.0f) fov =  1.0f;
	if (fov > 45.0f) fov = 45.0f;
}

