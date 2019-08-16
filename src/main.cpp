#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"
#include "shader.h"
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "textrenderer.h"
#include "windowmanager.h"
#include "vertices.h"
#include "scenes.h"
using namespace std;

GLuint VAOs[2], VBOs[2];


void DrawText(TextRenderer &textRenderer, WindowManager &window);


int main()
{
	// GLFW : Initialize and configure
	// -------------------------------
	Camera camera(glm::vec3(0.0f, 5.0f, 0.0f));
	WindowManager window(camera);

	// Load OpenGL function pointers
	// -----------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// OPENGL OPTIONS
	// --------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	// glCullFace(GL_FRONT);
	// glFrontFace(GL_CCW);


	// SHADERS
	// -------
	Shader ourShader("./shaders/textureShader.vert", "./shaders/textureShader.frag");
	Shader textShader("./shaders/textShader.vert", "./shaders/textShader.frag");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(SCR_WIDTH), 0.0f, static_cast<GLfloat>(SCR_HEIGHT));
	textShader.use();
	glUniformMatrix4fv(glGetUniformLocation(textShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	ourShader.use();
	glUniform1i(glGetUniformLocation(ourShader.Program, "texture1"), 0);
	ourShader.setInt("texture2", 1);

	// VBAs & VBOs
	// -----------
	glGenBuffers(2, VBOs);
	glGenVertexArrays(2, VAOs);
	// Cubes VAO & VBO
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 0, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// Text VAO & VBO
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// TEXT
	// ----
	TextRenderer textRenderer(textShader, VAOs[1], VBOs[1]);

	// TEXTURES
	// --------
	// texture 1
	unsigned int texture = buildTexture("textures/text1.jpg");
	unsigned texture2 = buildTexture2("textures/text2.png");

	//
	// ----
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// RENDER LOOP
	// -----------
	while (!glfwWindowShouldClose(window.GetWindow()))
	{
		window.ProcessDeltaTime();
		window.ProcessInput();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		draw3D(ourShader, camera, window, texture, texture2);
		drawText(textRenderer, window);
		// cout << window.AverageFrameRate() << endl;
		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}

	//
	// ----
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);

	glfwTerminate();
	return 0;
}
