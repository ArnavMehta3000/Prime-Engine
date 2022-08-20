#pragma once

namespace Prime
{
	struct SimpleColorVertex
	{
		struct
		{
			float x, y, z;
		}Position;

		struct
		{
			float r, g, b, a;
		}Color;
	};
}