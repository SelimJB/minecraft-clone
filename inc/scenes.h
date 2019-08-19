#include "camera.h"
#include "shader.h"
#include "windowmanager.h"
using namespace std;

extern GLuint VAOs[], VBOs[];

extern int vertexNbr;

void drawText(TextRenderer &textRenderer, WindowManager &window)
{
	textRenderer.RenderText("Framerate : " + to_string(window.AverageFrameRate()), 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
	textRenderer.RenderText("Vrt nbr  :  " + to_string(vertexNbr), 35.0f, 70.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
	textRenderer.RenderText("No mipmap, 2 textures, no culling", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
}

void twoTextures_noMipMap_noCulling_rotation(Shader &ourShader, Camera &camera, WindowManager &window, unsigned int texture, unsigned int texture2)
{
	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	ourShader.use();

	// Mouvement Camera
	float currentFrame = glfwGetTime();
	camera.Position.x = 10.0f * cos(currentFrame / 2);
	camera.Position.z = 10.0f * sin(currentFrame / 2);
	camera.Position.y = 14;
	glm::mat4 view;
	view = glm::lookAt(glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z), glm::vec3(0.0, 8.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	ourShader.setMat4("projection", projection);
	// glm::mat4 view = camera.GetViewMatrix();
	ourShader.setMat4("view", view);
	ourShader.setVec3("ourColor2", glm::vec3(1, 1, 1));

	glBindVertexArray(VAOs[0]);
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			model = glm::translate(model, glm::vec3(i - 50, (i - 1) % 2, j - 50));
			model = glm::rotate(model, glm::radians(20.0f * (i % 10)) * (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.1f * (i % 10)));
			ourShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}
}

void oneTexture_noMipMap_noCulling_rotation(Shader &ourShader, Camera &camera, WindowManager &window, unsigned int texture, unsigned int texture2)
{
	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	ourShader.use();

	// Mouvement Camera
	float currentFrame = glfwGetTime();
	camera.Position.x = 10.0f * cos(currentFrame / 2 * 0);
	camera.Position.z = 10.0f * sin(currentFrame / 2 * 0);
	camera.Position.y = 14;
	glm::mat4 view;
	view = glm::lookAt(glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z), glm::vec3(0.0, 8.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	ourShader.setMat4("projection", projection);
	// glm::mat4 view = camera.GetViewMatrix();
	ourShader.setMat4("view", view);
	ourShader.setVec3("ourColor2", glm::vec3(1, 1, 1));

	glBindVertexArray(VAOs[0]);
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			model = glm::translate(model, glm::vec3(i - 50, (i - 1) % 2, j - 50));
			model = glm::rotate(model, glm::radians(20.0f * (i % 10)) * (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.1f * (i % 10)));
			ourShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}
}

void oneTexture_noMipMap_noCulling_noRotation(Shader &ourShader, Camera &camera, WindowManager &window, unsigned int texture, unsigned int texture2)
{
	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	ourShader.use();

	// Mouvement Camera
	float currentFrame = glfwGetTime();
	camera.Position.x = 10.0f * cos(currentFrame / 2 * 0);
	camera.Position.z = 10.0f * sin(currentFrame / 2 * 0);
	camera.Position.y = 14;
	glm::mat4 view;
	view = glm::lookAt(glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z), glm::vec3(0.0, 8.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	ourShader.setMat4("projection", projection);
	// glm::mat4 view = camera.GetViewMatrix();
	ourShader.setMat4("view", view);
	ourShader.setVec3("ourColor2", glm::vec3(1, 1, 1));

	glBindVertexArray(VAOs[0]);
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			model = glm::translate(model, glm::vec3(i - 50, (i - 1) % 2, j - 50));
			// model = glm::rotate(model, glm::radians(20.0f * (i % 10)) * (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.1f * (i % 10)));
			ourShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}
}

void testMipMap(Shader &ourShader, Camera &camera, WindowManager &window, unsigned int texture, unsigned int texture2)
{
	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	ourShader.use();

	// Mouvement Camera
	// float currentFrame = glfwGetTime();
	// camera.Position.x = 10.0f * cos(currentFrame / 2 * 0);
	// camera.Position.z = 10.0f * sin(currentFrame / 2 * 0);
	// camera.Position.y = 19;
	// glm::mat4 view;
	// view = glm::lookAt(glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z), glm::vec3(0.0, 8.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	// view = glm::lookAt(glm::vec3(0,3,0), glm::vec3(0.0, 2, 4.0), glm::vec3(0.0, 1.0, 0.0));

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	ourShader.setMat4("projection", projection);
	glm::mat4 view = camera.GetViewMatrix();
	ourShader.setMat4("view", view);
	glBindVertexArray(VAOs[0]);
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			model = glm::translate(model, glm::vec3(i, 0, j));
			// model = glm::translate(model, glm::vec3(i - 25, 0, j - 50));

			// model = glm::rotate(model, glm::radians(20.0f * (i % 10)) * (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.1f * (i % 10)));
			if ((i + j) % 2 == 0)
			{
				ourShader.setVec3("ourColor2", glm::vec3(1, 0.2f, 0.2f));
			}
			else
				ourShader.setVec3("ourColor2", glm::vec3(0.2f, 0.2f, 1));
			ourShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}
}

void draw3D(Shader &ourShader, Camera &camera, WindowManager &window, unsigned int texture, unsigned int texture2)
{
	// oneTexture_noMipMap_noCulling_noRotation(ourShader, camera, window, texture, texture2); 		// 90
	// oneTexture_noMipMap_noCulling_rotation(ourShader, camera, window, texture, texture2); 		// 49
	// twoTextures_noMipMap_noCulling_rotation(ourShader, camera, window, texture, texture2);		// 45
	testMipMap(ourShader, camera, window, texture, texture2); // 45
}

unsigned int buildTexture(const char *texturePath)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	stbi_set_flip_vertically_on_load(true);
	// Texture Wraping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Texture filtering
	std::cout << "toto" << std::endl;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return texture;
}

unsigned int buildTexture2(const char *texturePath)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	return texture;
}