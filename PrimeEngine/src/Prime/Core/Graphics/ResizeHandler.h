#pragma once

namespace Prime
{
	class ResizeHandler
	{
	public:
		static void RegisterFunction(std::function<void(int, int)> func);
		static void Resize(int w, int h);

		inline static void ClearFunctions() { return s_resizeFunctions.clear(); }
		inline static int FunctionCount() { return (int)s_resizeFunctions.size(); }

	private:
		static std::vector<std::function<void(int, int)>> s_resizeFunctions;
	};
}

#define PRIME_BIND_RESIZE_FN(fn) std::bind(&fn, this, std::placeholders::_1, std::placeholders::_2)