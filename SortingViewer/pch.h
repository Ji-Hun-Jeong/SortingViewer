#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxtk/SimpleMath.h>
#include <wrl.h>
#include <iostream>
#include <string>
#include <windows.h>
#include <vector>
#include <memory>
#include "Define.h"
#include "Enum.h"


using std::cout;
using std::endl;
using std::string;
using std::wstring;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using DirectX::XM_PI;
#include "Struct.h"