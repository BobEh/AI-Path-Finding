#pragma once

struct Vertex
{
	Vertex()
	{
	}

	Vertex(float iX, float iY, float iZ) :
		x(iX),
		y(iY),
		z(iZ)
	{
	}

	float x, y, z;
};
