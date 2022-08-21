#pragma once

namespace Prime
{
	struct SimpleVertetx
	{
		struct
		{
			float x, y, z;
		}Position;
	};

	struct ColorVertex
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

	struct TexturedVertex
	{
		struct
		{
			float x, y, z;
		}Position;

		struct
		{
			float u, v;
		}Texture;
	};

	struct TexturedNormalVertex
	{
		struct
		{
			float x, y, z;
		}Position;

		struct
		{
			float xn, yn, zn;
		}Normal;

		struct
		{
			float u, v;
		}Texture;
	};
}