#pragma once

namespace Prime
{
	struct PrimitivesFrameBufferVS
	{
		SimpleMath::Matrix ViewMatrix;
		SimpleMath::Matrix ProjectionMatrix;
	};

	struct PrimitivesObjectBufferVS
	{
		SimpleMath::Matrix WorldMatrix;
		SimpleMath::Color Color;
	};

	struct WVPBuffer
	{
		SimpleMath::Matrix WorldMatrix;
		SimpleMath::Matrix ViewMatrix;
		SimpleMath::Matrix ProjectionMatrix;
	};

	struct ColorBuffer
	{
		float r;
		float g;
		float b;
		float a = 1.0f;
	};
}
