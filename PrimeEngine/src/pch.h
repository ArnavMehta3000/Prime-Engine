#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <memory>

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>

#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <d3d11.h>
#include <wrl.h>

#include "Prime/Logger.h"
#include "Prime/Core/Timer.h"
#include "Prime/Core/ServiceLocator/IService.h"
#include "Prime/Core/ServiceLocator/Locator.h"

#include "SimpleMath.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "D3DCompiler.lib")

using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;
