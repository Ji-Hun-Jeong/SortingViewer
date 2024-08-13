#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxtk/SimpleMath.h>
#include <directxtk/DDSTextureLoader.h>
#include <wrl.h>
#include <iostream>
#include <string>
#include <windows.h>
#include <vector>
#include <memory>
#include <random>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include <set>
#include <array>
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
using std::unique_ptr;
using std::make_unique;
using DirectX::XM_PI;
using std::random_device;
using std::mt19937;
using std::uniform_real_distribution;
using std::uniform_int_distribution;
using std::thread;
using std::mutex;
using std::condition_variable;
using std::chrono::seconds;
using std::set;

#include "Struct.h"