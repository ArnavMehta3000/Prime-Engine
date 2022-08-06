#pragma once

struct Vertex
{
	float x, y, z;

	Vertex()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	Vertex(float X, float Y, float Z)
	{
		x = X;
		y = Y;
		z = Z;
	}
};