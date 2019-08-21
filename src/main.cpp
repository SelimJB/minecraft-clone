#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/noise.hpp>
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
#include <cstdlib>
#include <time.h>
using namespace std;

GLuint VAOs[2], VBOs[2];
GLuint vbo, vao;
const bool RAND = true;
const bool DEBUG = false;

const int MAX_UPDATE_NBR = 1;
int updatedNbr;
int frameNbr;

bool g_debug;

struct byte4
{
	uint8_t x, y, z, w;
	byte4() : x(0), y(0), z(0), w(0) {}
	byte4(uint8_t x, uint8_t y, uint8_t z, uint8_t w) : x(x), y(y), z(z), w(w) {}
};

const int CX = 16, CY = 16, CZ = 16;
const int WCX = 32, WCY = 1, WCZ = 32;

// uint8_t chunk[CX][CY][CZ];

// byte4 vertex[18 * CX * CY * CZ];

class Chunk
{
public:
	void Update(Shader &shader);
	void Noise(int seed);
	int vertexNbr;
	uint8_t blocks[CX][CY][CZ];
	int cpX, cpY, cpZ;
	GLuint vbo2, vao2;
	bool updated = false;

	Chunk(int x, int y, int z, int seed) : cpX(x), cpY(y), cpZ(z)
	{
		// cout << "INITIALISATION" << endl;
		memset(blocks, 0, sizeof blocks);
		// cout << vbo2 << endl;
	};

	void Configure()
	{
		glGenVertexArrays(1, &vao2);
		glBindVertexArray(vao2);
		glGenBuffers(1, &vbo2);
		cout << "Configure VBO : " << vbo2 << endl;
		glBindBuffer(GL_ARRAY_BUFFER, vbo2);
		// Pas utile apparemment
		glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 4, GL_BYTE, GL_FALSE, sizeof(byte4), (void *)0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
	}

	// TODO : Rename render
	void Draw(Shader &shader)
	{
		if (!updated)
		{
			if (updatedNbr >= MAX_UPDATE_NBR)
			{
				// cout << "Not Drawned\t";
				// cout << "\t" << cpX << "\t" << cpY << "\t" << cpZ << endl;
				return;
			}
			else
			{
				updatedNbr++;
				updated = true;
				Configure();
				// cout << "\nUPDATED\t\t";
				// cout << "\tVBO : " << vbo2 << "\t" << cpX << "\t" << cpY << "\t" << cpZ << endl;
				Noise(time(NULL));
				Update(shader);
				// cout << vertexNbr << vbo2 << endl;
			}
		}
		if (updated)
		{
			glm::mat4 model = model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			model = glm::translate(model, glm::vec3(cpX, cpY, cpZ));
			shader.setMat4("model", model);

			// cout << "Draw VBO : " << vbo2 << endl;
			// glBindBuffer(GL_ARRAY_BUFFER, vbo2);
			// glVertexAttribPointer(0, 4, GL_BYTE, GL_FALSE, sizeof(byte4), (void *)0);
			glBindVertexArray(vao2);

			if (g_debug)
			{
				glDrawArrays(GL_LINES, 0, vertexNbr);
			}
			else
			{
				glDrawArrays(GL_TRIANGLES, 0, vertexNbr);
			}
		}
	};
};

class World
{
public:
	Chunk *w[WCX][WCY][WCZ];
	Shader shader;
	World(Shader &s) : shader(s)
	{
		for (int x = 0; x < WCX; x++)
		{
			for (int y = 0; y < WCY; y++)
			{
				for (int z = 0; z < WCZ; z++)
				{
					w[x][y][z] = new Chunk(CX * x, -CY * y, CZ * z, time(NULL));
				}
			}
		}
	}
	void Render()
	{
		int aze = 0;
		if (frameNbr % 777 == 0)
		{
			aze = 8;
		}
		for (int x = 0; x < WCX; x++)
		{
			for (int y = 0; y < WCY; y++)
			{
				for (int z = 0; z < WCZ; z++)
				{
					w[x][y][z]->Draw(shader);
				}
			}
		}
		// if (frameNbr % 10 == 0)
		// {
		updatedNbr = 0;
		// }
		// if (updatedNbr != 0)

		// 	cout << "UPDATE NBR = " << updatedNbr << endl;
	}
};

static float noise2d(float x, float y, int seed, int octaves, float persistence)
{
	float sum = 0;
	float strength = 1.0;
	float scale = 1.0;

	for (int i = 0; i < octaves; i++)
	{
		sum += strength * glm::simplex(glm::vec2(x, y) * scale);
		scale *= 2.0;
		strength *= persistence;
	}

	return sum;
}

static float noise3d_abs(float x, float y, float z, int seed, int octaves, float persistence)
{
	float sum = 0;
	float strength = 1.0;
	float scale = 1.0;

	for (int i = 0; i < octaves; i++)
	{
		sum += strength * fabs(glm::simplex(glm::vec3(x, y, z) * scale));
		scale *= 2.0;
		strength *= persistence;
	}

	return sum;
}

void noise(int seed, uint8_t (&chunk)[CX][CY][CZ])
{
	// TEMPS
	int ax = 0;
	int az = 0;
	int ay = 0;
	int SEALEVEL = 4;
	// if (noised)
	// 	return;
	// else
	// 	noised = true;
	for (int x = 0; x < CX; x++)
	{
		for (int z = 0; z < CZ; z++)
		{
			// Land height
			float n = noise2d((x + ax * CX) / 256.0, (z + az * CZ) / 256.0, seed, 5, 0.8) * 4;
			int h = n * 2;
			int y = 0;

			// Land blocks
			for (y = 0; y < CY; y++)
			{
				// Are we above "ground" level?
				if (y + ay * CY >= h)
				{
					// If we are not yet up to sea level, fill with water blocks
					if (y + ay * CY < SEALEVEL)
					{
						chunk[x][y][z] = 8;
						continue;
						// Otherwise, we are in the air
					}
					else
					{
						// A tree!
						if (chunk[x][y - 1][z] == 3 && (rand() & 0xff) == 0)
						// if (get(x, y - 1, z) == 3 && (rand() & 0xff) == 0)
						{
							// Trunk
							h = (rand() & 0x3) + 3;
							for (int i = 0; i < h; i++)
								chunk[x][y + i][z] = 5;
							// set(x, y + i, z, 5);

							// Leaves
							for (int ix = -3; ix <= 3; ix++)
							{
								for (int iy = -3; iy <= 3; iy++)
								{
									for (int iz = -3; iz <= 3; iz++)
									{
										if (ix * ix + iy * iy + iz * iz < 8 + (rand() & 1) && !chunk[x + ix][y + h + iy][z + iz])
											// if (ix * ix + iy * iy + iz * iz < 8 + (rand() & 1) && !get(x + ix, y + h + iy, z + iz))
											chunk[x + ix][y + h + iy][z + iz] = 4;
									}
								}
							}
						}
						break;
					}
				}

				// Random value used to determine land type
				float r = noise3d_abs((x + ax * CX) / 16.0, (y + ay * CY) / 16.0, (z + az * CZ) / 16.0, -seed, 2, 1);

				// Sand layer
				if (n + r * 5 < 4)
					chunk[x][y][z] = 7;
				// Dirt layer, but use grass blocks for the top
				else if (n + r * 5 < 8)
					chunk[x][y][z] = (h < SEALEVEL || y + ay * CY < h - 1) ? 1 : 3;
				// Rock layer
				else if (r < 1.25)
					chunk[x][y][z] = 6;
				// Sometimes, ores!
				else
					chunk[x][y][z] = 11;
			}
		}
	}
}

void Chunk::Noise(int seed)
{
	// TEMPS
	int ax = 0;
	int az = 0;
	int ay = 0;
	int SEALEVEL = 4;
	// if (noised)
	// 	return;
	// else
	// 	noised = true;
	for (int x = 0; x < CX; x++)
	{
		for (int z = 0; z < CZ; z++)
		{
			// Land height
			float n = noise2d((x + ax * CX) / 256.0, (z + az * CZ) / 256.0, seed, 5, 0.8) * 4;
			int h = n * 2;
			int y = 0;

			// Land blocks
			for (y = 0; y < CY; y++)
			{
				// Are we above "ground" level?
				if (y + ay * CY >= h)
				{
					// If we are not yet up to sea level, fill with water blocks
					if (y + ay * CY < SEALEVEL)
					{
						blocks[x][y][z] = 8;
						continue;
						// Otherwise, we are in the air
					}
					else
					{
						// // A tree!
						// if (blocks[x][y - 1][z] == 3 && (rand() & 0xff) == 0)
						// // if (get(x, y - 1, z) == 3 && (rand() & 0xff) == 0)
						// {
						// 	// Trunk
						// 	h = (rand() & 0x3) + 3;
						// 	for (int i = 0; i < h; i++)
						// 		blocks[x][y + i][z] = 5;
						// 	// set(x, y + i, z, 5);

						// 	// Leaves
						// 	for (int ix = -3; ix <= 3; ix++)
						// 	{
						// 		for (int iy = -3; iy <= 3; iy++)
						// 		{
						// 			for (int iz = -3; iz <= 3; iz++)
						// 			{
						// 				if (ix * ix + iy * iy + iz * iz < 8 + (rand() & 1) && !blocks[x + ix][y + h + iy][z + iz])
						// 					// if (ix * ix + iy * iy + iz * iz < 8 + (rand() & 1) && !get(x + ix, y + h + iy, z + iz))
						// 					blocks[x + ix][y + h + iy][z + iz] = 4;
						// 			}
						// 		}
						// 	}
						// }
						break;
					}
				}

				// Random value used to determine land type
				float r = noise3d_abs((x + ax * CX) / 16.0, (y + ay * CY) / 16.0, (z + az * CZ) / 16.0, -seed, 2, 1);

				// Sand layer
				if (n + r * 5 < 4)
					blocks[x][y][z] = 7;
				// Dirt layer, but use grass blocks for the top
				else if (n + r * 5 < 8)
					blocks[x][y][z] = (h < SEALEVEL || y + ay * CY < h - 1) ? 1 : 3;
				// Rock layer
				else if (r < 1.25)
					blocks[x][y][z] = 6;
				// Sometimes, ores!
				else
					blocks[x][y][z] = 11;
			}
		}
	}
}

void randomChunkInitialisation()
{
	for (int x = 0; x < CX; x++)
	{
		for (int y = 0; y < CY; y++)
		{
			for (int z = 0; z < CZ; z++)
			{
				// int empty = 1;
				int empty = rand() % 3000;
				// int water = 1;
				int water = rand() % 20;
				int rock = 1;
				// int rock = rand() % 100;
				// chunk[x][y][z] = 2;
				// chunk[x][y][z] = !empty ? 0 : !water ? 3 : !rock ? 4 : 2;
				// chunk[x][y][z] = !empty ? 0 : y == 0 ? 2 : y == 2 ? 4 : 3;
			}
		}
	}
	// chunk[3][3][3] = 0;
	// chunk[3][3][4] = 0;
	// chunk[3][3][5] = 0;
	// chunk[3][3][6] = 0;
}

void Chunk::Update(Shader &shader)
{
	byte4 vertex[18 * CX * CY * CZ];
	memset(vertex, 0, sizeof vertex);
	// Vertex number
	int i = 0;

	// -x
	bool vis = false;

	for (int x = 0; x < CX; x++)
	{
		for (int y = 0; y < CY; y++)
		{
			for (int z = 0; z < CZ; z++)
			{
				// Empty Block
				if (blocks[x][y][z] == 0 || (x != 0 && blocks[x - 1][y][z] ))
				{
					vis = false;
					continue;
				}

				int side = blocks[x][y][z];
				int top = side;
				if (side == 1)
				{
					top = 1;
					side = 2;
				}

				if (z != 0 && blocks[x][y][z] == blocks[x][y][z - 1] && vis)
				{
					vertex[i - 5] = byte4(x, y, z + 1, side);
					vertex[i - 2] = byte4(x, y, z + 1, side);
					vertex[i - 1] = byte4(x, y + 1, z + 1, side);
				}
				else
				{
					vertex[i++] = byte4(x, y, z, side);
					vertex[i++] = byte4(x, y, z + 1, side);
					vertex[i++] = byte4(x, y + 1, z, side);
					vertex[i++] = byte4(x, y + 1, z, side);
					vertex[i++] = byte4(x, y, z + 1, side);
					vertex[i++] = byte4(x, y + 1, z + 1, side);
				}
				vis = true;
			}
		}
	}
	// +x
	for (int x = 0; x < CX; x++)
	{
		for (int y = 0; y < CY; y++)
		{
			for (int z = 0; z < CZ; z++)
			{
				if (blocks[x][y][z] == 0 || (x != CX - 1 && blocks[x + 1][y][z]))
				{
					vis = false;
					continue;
				}

				int side = blocks[x][y][z];
				int top = side;
				if (side == 1)
				{
					top = 1;
					side = 2;
				}

				if (z != 0 && blocks[x][y][z] == blocks[x][y][z - 1] && vis)
				{
					vertex[i - 4] = byte4(x + 1, y, z + 1, side);
					vertex[i - 2] = byte4(x + 1, y + 1, z + 1, side);
					vertex[i - 1] = byte4(x + 1, y, z + 1, side);
				}
				else
				{
					vertex[i++] = byte4(x + 1, y, z, side);
					vertex[i++] = byte4(x + 1, y + 1, z, side);
					vertex[i++] = byte4(x + 1, y, z + 1, side);
					vertex[i++] = byte4(x + 1, y + 1, z, side);
					vertex[i++] = byte4(x + 1, y + 1, z + 1, side);
					vertex[i++] = byte4(x + 1, y, z + 1, side);
					// }
				}
				vis = true;
			}
		}
	}
	// -y
	for (int x = 0; x < CX; x++)
	{
		for (int y = 0; y < CY; y++)
		{
			for (int z = 0; z < CZ; z++)
			{
				// Empty Block
				if (blocks[x][y][z] == 0 || (y != 0 && blocks[x][y - 1][z]))
				{
					vis = false;
					continue;
				}

				int side = blocks[x][y][z];
				int top = side;
				if (side == 1)
				{
					top = 1;
					side = 2;
				}

				if (z != 0 && blocks[x][y][z] == blocks[x][y][z - 1] && vis)
				{
					vertex[i - 4] = byte4(x, y, z + 1, side);
					vertex[i - 2] = byte4(x + 1, y, z + 1, side);
					vertex[i - 1] = byte4(x, y, z + 1, side);
				}
				else
				{
					vertex[i++] = byte4(x, y, z, side);
					vertex[i++] = byte4(x + 1, y, z, side);
					vertex[i++] = byte4(x, y, z + 1, side);
					vertex[i++] = byte4(x + 1, y, z, side);
					vertex[i++] = byte4(x + 1, y, z + 1, side);
					vertex[i++] = byte4(x, y, z + 1, side);
				}
				vis = true;
			}
		}
	}
	// +y
	for (int x = 0; x < CX; x++)
	{
		for (int y = 0; y < CY; y++)
		{
			for (int z = 0; z < CZ; z++)
			{
				// Empty Block
				if (blocks[x][y][z] == 0 || (y != CY - 1 && blocks[x][y + 1][z]))
				{
					vis = false;
					continue;
				}

				int side = blocks[x][y][z];
				int top = side;
				if (side == 1)
				{
					top = 1;
					side = 2;
				}

				if (z != 0 && blocks[x][y][z] == blocks[x][y][z - 1] && vis)
				{
					vertex[i - 5] = byte4(x, y + 1, z + 1, top);
					vertex[i - 2] = byte4(x, y + 1, z + 1, top);
					vertex[i - 1] = byte4(x + 1, y + 1, z + 1, top);
				}
				else
				{
					vertex[i++] = byte4(x, y + 1, z, top);
					vertex[i++] = byte4(x, y + 1, z + 1, top);
					vertex[i++] = byte4(x + 1, y + 1, z, top);
					vertex[i++] = byte4(x + 1, y + 1, z, top);
					vertex[i++] = byte4(x, y + 1, z + 1, top);
					vertex[i++] = byte4(x + 1, y + 1, z + 1, top);
					// }
				}
				vis = true;
			}
		}
	}
	// -z
	for (int x = 0; x < CX; x++)
	{
		for (int z = CZ - 1; z >= 0; z--)
		{
			for (int y = 0; y < CY; y++)
			{
				// Empty Block
				if (blocks[x][y][z] == 0 || (z != 0 && blocks[x][y][z - 1] ))
				{
					vis = false;
					continue;
				}

				int side = blocks[x][y][z];
				int top = side;
				if (side == 1)
				{
					top = 1;
					side = 2;
				}

				if (y != 0 && blocks[x][y][z] == blocks[x][y - 1][z] && vis)
				{
					vertex[i - 5] = byte4(x, y + 1, z, side);
					vertex[i - 3] = byte4(x, y + 1, z, side);
					vertex[i - 2] = byte4(x + 1, y + 1, z, side);
				}
				else
				{
					// -z
					vertex[i++] = byte4(x, y, z, side);
					vertex[i++] = byte4(x, y + 1, z, side);
					vertex[i++] = byte4(x + 1, y, z, side);
					vertex[i++] = byte4(x, y + 1, z, side);
					vertex[i++] = byte4(x + 1, y + 1, z, side);
					vertex[i++] = byte4(x + 1, y, z, side);
				}
				vis = true;
			}
		}
	}
	// + z
	for (int x = 0; x < CX; x++)
	{
		for (int z = 0; z < CZ; z++)
		{
			for (int y = 0; y < CY; y++)
			{
				if (blocks[x][y][z] == 0 || ( z != CZ - 1 && blocks[x][y][z + 1]))
				{
					vis = false;
					continue;
				}

				int side = blocks[x][y][z];
				int top = side;
				if (side == 1)
				{
					top = 1;
					side = 2;
				}

				if (y != 0 && blocks[x][y][z] == blocks[x][y - 1][z] && vis)
				{
					vertex[i - 4] = byte4(x, y + 1, z + 1, side);
					vertex[i - 3] = byte4(x, y + 1, z + 1, side);
					vertex[i - 1] = byte4(x + 1, y + 1, z + 1, side);
				}
				else
				{
					vertex[i++] = byte4(x, y, z + 1, side);
					vertex[i++] = byte4(x + 1, y, z + 1, side);
					vertex[i++] = byte4(x, y + 1, z + 1, side);
					vertex[i++] = byte4(x, y + 1, z + 1, side);
					vertex[i++] = byte4(x + 1, y, z + 1, side);
					vertex[i++] = byte4(x + 1, y + 1, z + 1, side);
				}
				vis = true;
			}
		}
	}
	int num = 71;
	vertexNbr = i;
	// cout << "Update VBO : " << vbo2 << endl;
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glBufferData(GL_ARRAY_BUFFER, i * sizeof *vertex, vertex, GL_STATIC_DRAW);
}

int main()
{
	//
	updatedNbr = 0;
	frameNbr = 0;
	g_debug = DEBUG;
	if (RAND)
	{
		srand(time(NULL));
	}
	else
		srand(1);
	// GLFW : Initialize and configure
	// -------------------------------
	Camera camera(glm::vec3(0.0f, 12.0f, -5.0f));
	WindowManager window(camera);

	// Load OpenGL function pointers
	// -----------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// OPENGL occlusionCullingIONS
	// --------------
	// Pour le text
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// SHADERS
	// -------
	Shader ourShader("./shaders/textureShader.vert", "./shaders/textureShader.frag");
	Shader textShader("./shaders/textShader.vert", "./shaders/textShader.frag");
	Shader blockShader("./shaders/block.vert", "./shaders/block.frag");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(SCR_WIDTH), 0.0f, static_cast<GLfloat>(SCR_HEIGHT));
	textShader.use();
	glUniformMatrix4fv(glGetUniformLocation(textShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	ourShader.use();
	glUniform1i(glGetUniformLocation(ourShader.Program, "texture1"), 0);
	ourShader.setInt("texture2", 1);

	// // VBAs & VBOs
	// // -----------
	// //BOX
	// // vertexNbr = 0;
	// // randomChunkInitialisation();
	// // updateMergeEachFrameVAO(blockShader, vbo, vertexNbr);
	// glGenBuffers(1, &vbo);
	// glGenVertexArrays(1, &vao);
	// glBindVertexArray(vao);
	// glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW);
	// // glBufferData(GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);
	// glVertexAttribPointer(0, 4, GL_BYTE, GL_FALSE, sizeof(byte4), (void *)0);
	// // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void *)0);
	// glEnableVertexAttribArray(0);
	// // glDrawArrays(GL_TRIANGLES, 0, 36);
	// glBindBuffer(GL_ARRAY_BUFFER, 0);
	// glGenBuffers(2, VBOs);
	// glGenVertexArrays(2, VAOs);
	// // Cubes VAO & VBO
	// glBindVertexArray(VAOs[0]);
	// glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	// glEnableVertexAttribArray(0);
	// glVertexAttribPointer(1, 0, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	// glEnableVertexAttribArray(1);
	// glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	// glEnableVertexAttribArray(2);
	// // Text VAO & VBO
	// glBindVertexArray(VAOs[1]);
	// glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	// glEnableVertexAttribArray(0);
	// glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	// glBindBuffer(GL_ARRAY_BUFFER, 0);
	// glBindVertexArray(0);

	// // TEXT
	// // ----
	// TextRenderer textRenderer(textShader, VAOs[1], VBOs[1]);

	// // TEXTURES
	// // --------
	// // texture 1
	// unsigned int texture = buildTexture("textures/text1.jpg");
	// unsigned texture2 = buildTexture2("textures/text2.png");

	// //
	// // ----
	// glBindBuffer(GL_ARRAY_BUFFER, 0);
	// glBindVertexArray(0);

	// SCENE
	// Chunk toto(0, 0, 0, 2);
	// Chunk toto2(CX + 1, 0, 0, 1);
	// Chunk toto1(CX + 2, 0, 0, 1);
	// Chunk toto3(0, 0, CZ, time(NULL));
	// Chunk toto4(0, 0, CZ+1, time(NULL));
	// Chunk toto5(0, 0, CZ+2, time(NULL));
	// Chunk toto6(0, 0, CZ+3, time(NULL));
	// Chunk toto7(0, 0, 0, time(NULL));
	// Chunk *tots[36];
	// for (int i=0;i<36;i++){
	// 	tots[i] = new Chunk(CX*(i%6),0,CZ*(i%6),1);
	// }

	// Chunk toto4(0, 0, CZ, time(NULL));
	World w(blockShader);

	double delta = 0;
	double last = 0;
	double drawTime = 0;
	bool fixFrameRate = false;
	// RENDER LOOP
	// -----------
	while (!glfwWindowShouldClose(window.GetWindow()))
	{
		delta = glfwGetTime() - last;
		if (!fixFrameRate || delta > double(1.0f / 200.0f))
		{
			drawTime = glfwGetTime();
			// cout << glfwGetTime() << "   " << last << "   " << delta << "   " <<  frameNbr << endl;
			window.ProcessDeltaTime();
			window.ProcessInput();

			glClearColor(125.0f / 255.0f, 190.0f / 255.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// TEMP
			blockShader.use();
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			blockShader.setMat4("projection", projection);
			glm::mat4 view = camera.GetViewMatrix();
			blockShader.setMat4("view", view);
			blockShader.setVec3("myColor", glm::vec3(1, 1, 0));

			// toto.Draw(blockShader);
			// toto1.Draw(blockShader);
			// toto2.Draw(blockShader);
			// toto3.Draw(blockShader);
			// toto4.Draw(blockShader);
			// toto5.Draw(blockShader);
			// toto6.Draw(blockShader);
			// toto7.Draw(blockShader);
			// for (int i=0;i<36;i++){
			// 	tots[i]->Draw(blockShader);
			// }
			// updatedNbr = 0;
			// toto4.Draw(blockShader);

			w.Render();

			// textRenderer.RenderText("Framerate : " + to_string(window.AverageFrameRate()), 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
			// textRenderer.RenderText("Vrt nbr  :  " + to_string(vertexNbr), 35.0f, 70.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));

			glfwSwapBuffers(window.GetWindow());
			glfwPollEvents();
			frameNbr++;
			delta = 0;
			drawTime = glfwGetTime() - drawTime;
			last = glfwGetTime();
			// cout << " DRAWTIME : " << drawTime << endl;
		}
	}
	//
	// ----
	// glDeleteVertexArrays(2, VAOs);
	// glDeleteVertexArrays(1, &vao);
	// glDeleteBuffers(2, VBOs);
	// glDeleteBuffers(1, &vbo);

	glfwTerminate();
	return 0;
}
