
#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "camera.h"
#include <GLFW/glfw3.h>
#include <iostream>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Camera parameters
const bool CAMERA_CONTROL = true;
const float LASTX = SCR_WIDTH / 2.0f;
const float LASTY = SCR_HEIGHT / 2.0f;
const bool FIRSTMOUSE = true;

class WindowManager
{
public:
	WindowManager(Camera &camera,
				  bool cameraControl = CAMERA_CONTROL,
				  unsigned int scrWidth = SCR_WIDTH,
				  unsigned int scrHeight = SCR_HEIGHT,
				  bool lastX = LASTX,
				  bool lastY = LASTY,
				  bool firstMouse = FIRSTMOUSE);
	void ProcessInput();
	GLFWwindow *GetWindow() { return window; }
	void ProcessDeltaTime();
	float DeltaTime() { return deltaTime; }
	float AverageFrameRate() {return averageFrameRate; }
	static Camera *camera2;
	static float lastX;
	static float lastY;
	static bool firstMouse;
	// WindowManager(unsigned int scrWidth = SCR_WIDTH, unsigned int scrHeight = SCR_HEIGHT);
	// void Initialize(Camera &camera, bool cameraControl = CAMERA_CONTROL, bool lastX = LASTX, bool lastY = LASTY, bool firstMouse = FIRSTMOUSE);

private:
	GLFWwindow *window;
	float screenWidth;
	float screenHeight;
	bool cameraControl;
	float lastFrame;
	float deltaTime;
	float averageFrameRate;
};

#endif