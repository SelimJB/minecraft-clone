#include "windowmanager.h"

int i = 0;
extern bool g_debug;

Camera *WindowManager::camera2 = NULL;
float WindowManager::lastX = LASTX;
float WindowManager::lastY = LASTY;
bool WindowManager::firstMouse = FIRSTMOUSE;

void WindowManager::ProcessDeltaTime()
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	i++;
	averageFrameRate = 1 / (currentFrame / i);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	if (WindowManager::firstMouse)
	{
		WindowManager::lastX = xpos;
		WindowManager::lastY = ypos;
		WindowManager::firstMouse = false;
	}

	float xoffset = xpos - WindowManager::lastX;
	float yoffset = WindowManager::lastY - ypos;

	WindowManager::lastX = xpos;
	WindowManager::lastY = ypos;

	WindowManager::camera2->ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	WindowManager::camera2->ProcessMouseScroll(yoffset);
}

static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
	if (WindowManager::firstMouse)
	{
		WindowManager::lastX = xpos;
		WindowManager::lastY = ypos;
		WindowManager::firstMouse = false;
	}

	float xoffset = xpos - WindowManager::lastX;
	float yoffset = WindowManager::lastY - ypos;

	WindowManager::lastX = xpos;
	WindowManager::lastY = ypos;

	WindowManager::camera2->ProcessMouseMovement(xoffset, yoffset);
}

WindowManager::WindowManager(Camera &camera, bool aCameraControl, unsigned int scrWidth, unsigned int scrHeight, bool aLastX, bool aLastY, bool aFirstMouse)
	: screenHeight(scrHeight), screenWidth(scrWidth), cameraControl(aCameraControl)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
	WindowManager::window = window;
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	lastX = aLastX;
	lastY = aLastY;
	firstMouse = aFirstMouse;
	camera2 = &camera;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (cameraControl)
	{
		glfwSetScrollCallback(window, scroll_callback);
		// glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetCursorPosCallback(window, cursor_position_callback);
	}
}

// void WindowManager::Initialize(Camera &camera, bool aCameraControl, bool aLastX, bool aLastY, bool aFirstMouse)
// {
// 	cameraControl = aCameraControl;
// 	lastX = aLastX;
// 	lastY = aLastY;
// 	firstMouse = aFirstMouse;
// 	camera2 = &camera;
// 	glfwSetScrollCallback(window, scroll_callback);
// 	glfwSetCursorPosCallback(window, mouse_callback);
// }

// WindowManager::WindowManager(unsigned int scrWidth, unsigned int scrHeight) : screenHeight(scrHeight), screenWidth(scrWidth)
// {
// 	glfwInit();
// 	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
// 	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
// 	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

// 	GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
// 	WindowManager::window = window;
// 	if (window == NULL)
// 	{
// 		std::cout << "Failed to create GLFW window" << std::endl;
// 		glfwTerminate();
// 	}
// 	glfwMakeContextCurrent(window);
// 	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
// };

void WindowManager::ProcessInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (cameraControl)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			camera2->ProcessKeyboard(FORWARD, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera2->ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera2->ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera2->ProcessKeyboard(RIGHT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		{
			g_debug = !g_debug;
		}
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		{
		}
	}
}
