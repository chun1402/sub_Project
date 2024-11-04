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

// Vertex array for a simple line
float lineVertices[] = {
    0.0f, 0.0f, 0.0f, // Start point
    0.5f, 0.5f, 0.0f  // End point
};

void drawLine() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, lineVertices);
    glDrawArrays(GL_LINES, 0, 2);
    glDisableClientState(GL_VERTEX_ARRAY);
}
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
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void processInput(GLFWwindow *window){	
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
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

	glewInit();	// Add for using shader.
					// Mouse & KeyBoard event functions.
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
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	glm::mat4 matProj, matView;

	glEnable(GL_DEPTH_TEST);
	//shader
	Shader ourShader("../shader.vs", "../shader.fs"); // you can name your shader files however
	ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
						  // either set it manually like so:
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
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);
		if (show_another_window) {
			ImGui::Begin("Another Window", &show_another_window);   			
			ImGui::Text("Hello from another window!");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			if (ImGui::Button("Close Me"))
				show_another_window = false;
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

		// Draw line using OpenGL
		drawLine();
		//update & draw entities.
		
		//here draw!!!!/////////////////////////////////////////////////////////////////////



		///////////////////////////////////////////////////////////////////////////////////

		// render container
		ourShader.use();
		glm::mat4 model = glm::mat4(1.0f);
		//draw entities on background.
		matProj = glm::perspective(glm::radians(fov), (display_w*1.)/(display_h*1.), 0.1, 10000.);
		matView = makeViewMat(vecPos, vecDir);
		glm::mat4 MVP = matProj * matView * model;
		ourShader.use(); 		
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

