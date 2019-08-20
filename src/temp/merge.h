void updateMerge(Shader &shader, int &i)
{
	// Vertex number
	i = 0;
	// -x
	bool vis = false;
	for (int x = 0; x < CX; x++)
	{
		for (int y = 0; y < CY; y++)
		{
			for (int z = 0; z < CZ; z++)
			{
				// Empty Block
				if (chunk[x][y][z] == 0 || (chunk[x - 1][y][z] && x != 0))
				{
					vis = false;
					continue;
				}

				int side = chunk[x][y][z];
				int top = side;
				if (side == 1)
				{
					top = 1;
					side = 2;
				}

				if (z != 0 && chunk[x][y][z] == chunk[x][y][z - 1] && vis)
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
				if (chunk[x][y][z] == 0 || (chunk[x + 1][y][z] && x != CX - 1))
				{
					vis = false;
					continue;
				}

				int side = chunk[x][y][z];
				int top = side;
				if (side == 1)
				{
					top = 1;
					side = 2;
				}

				if (z != 0 && chunk[x][y][z] == chunk[x][y][z - 1] && vis)
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
				if (chunk[x][y][z] == 0 || (chunk[x][y - 1][z] && y != 0))
				{
					vis = false;
					continue;
				}

				int side = chunk[x][y][z];
				int top = side;
				if (side == 1)
				{
					top = 1;
					side = 2;
				}

				if (z != 0 && chunk[x][y][z] == chunk[x][y][z - 1] && vis)
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
				if (chunk[x][y][z] == 0 || (chunk[x][y + 1][z] && y != CY - 1))
				{
					vis = false;
					continue;
				}

				int side = chunk[x][y][z];
				int top = side;
				if (side == 1)
				{
					top = 1;
					side = 2;
				}

				if (z != 0 && chunk[x][y][z] == chunk[x][y][z - 1] && vis)
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
				if (chunk[x][y][z] == 0 || (chunk[x][y][z - 1] && z != 0))
				{
					vis = false;
					continue;
				}

				int side = chunk[x][y][z];
				int top = side;
				if (side == 1)
				{
					top = 1;
					side = 2;
				}

				if (y != 0 && chunk[x][y][z] == chunk[x][y - 1][z] && vis)
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
				if (chunk[x][y][z] == 0 || (chunk[x][y][z + 1] && z != CZ - 1))
				{
					vis = false;
					continue;
				}

				int side = chunk[x][y][z];
				int top = side;
				if (side == 1)
				{
					top = 1;
					side = 2;
				}

				if (y != 0 && chunk[x][y][z] == chunk[x][y - 1][z] && vis)
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
}

void update(Shader &shader, int &i)
{
	// Vertex number
	i = 0;
	for (int x = 0; x < CX; x++)
	{
		for (int y = 0; y < CY; y++)
		{
			for (int z = 0; z < CZ; z++)
			{
				// Empty Block
				if (chunk[x][y][z] == 0)
					continue;

				int side = chunk[x][y][z];
				int top = side;
				if (side == 1)
				{
					top = 1;
					side = 2;
				}

				// eq else x != 0 && chunch[x - 1][y][z]
				if (!OCCLUSION_CULLING || x == 0 || chunk[x - 1][y][z] == 0)
				{
					// -x
					vertex[i++] = byte4(x, y, z, side);
					vertex[i++] = byte4(x, y, z + 1, side);
					vertex[i++] = byte4(x, y + 1, z, side);
					vertex[i++] = byte4(x, y + 1, z, side);
					vertex[i++] = byte4(x, y, z + 1, side);
					vertex[i++] = byte4(x, y + 1, z + 1, side);
				}

				if (!OCCLUSION_CULLING || x == CX - 1 || chunk[x + 1][y][z] == 0)
				{
					// +x
					vertex[i++] = byte4(x + 1, y, z, side);
					vertex[i++] = byte4(x + 1, y + 1, z, side);
					vertex[i++] = byte4(x + 1, y, z + 1, side);
					vertex[i++] = byte4(x + 1, y + 1, z, side);
					vertex[i++] = byte4(x + 1, y + 1, z + 1, side);
					vertex[i++] = byte4(x + 1, y, z + 1, side);
				}
				if (!OCCLUSION_CULLING || y == 0 || chunk[x][y - 1][z] == 0)
				{
					// -y
					vertex[i++] = byte4(x, y, z, side);
					vertex[i++] = byte4(x + 1, y, z, side);
					vertex[i++] = byte4(x, y, z + 1, side);
					vertex[i++] = byte4(x + 1, y, z, side);
					vertex[i++] = byte4(x + 1, y, z + 1, side);
					vertex[i++] = byte4(x, y, z + 1, side);
				}
				if (!OCCLUSION_CULLING || y == CY - 1 || chunk[x][y + 1][z] == 0)
				{
					// +y
					vertex[i++] = byte4(x, y + 1, z, top);
					vertex[i++] = byte4(x, y + 1, z + 1, top);
					vertex[i++] = byte4(x + 1, y + 1, z, top);
					vertex[i++] = byte4(x + 1, y + 1, z, top);
					vertex[i++] = byte4(x, y + 1, z + 1, top);
					vertex[i++] = byte4(x + 1, y + 1, z + 1, top);
				}
				if (!OCCLUSION_CULLING || z == 0 || chunk[x][y][z - 1] == 0)
				{
					// -z
					vertex[i++] = byte4(x, y, z, side);
					vertex[i++] = byte4(x, y + 1, z, side);
					vertex[i++] = byte4(x + 1, y, z, side);
					vertex[i++] = byte4(x, y + 1, z, side);
					vertex[i++] = byte4(x + 1, y + 1, z, side);
					vertex[i++] = byte4(x + 1, y, z, side);
				}
				if (!OCCLUSION_CULLING || z == CZ - 1 || chunk[x][y][z + 1] == 0)
				{
					// +z
					vertex[i++] = byte4(x, y, z + 1, side);
					vertex[i++] = byte4(x + 1, y, z + 1, side);
					vertex[i++] = byte4(x, y + 1, z + 1, side);
					vertex[i++] = byte4(x, y + 1, z + 1, side);
					vertex[i++] = byte4(x + 1, y, z + 1, side);
					vertex[i++] = byte4(x + 1, y + 1, z + 1, side);
				}
			}
		}
	}
	int num = 71;
}
