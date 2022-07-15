#pragma once

#ifndef ButiEngineHeader_H

#define ButiEngineHeader_H




#define WIN32_LEAN_AND_MEAN           
#pragma once
#include<cstdint>
const std::int32_t splitNum = 8;
#include<Windows.h>

#include <memory.h>
#include <tchar.h>
#include <wrl.h>

#include <assert.h>
#include <wchar.h>
#include <mmsystem.h>
#include <atlbase.h>
#include <math.h>
#include<direct.h>



#include<ctime>
#include<chrono>

// STL
#include <vector>
#include <list>
#include <map>
#include<unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <exception>
#include <stdexcept>




#pragma comment( lib, "d3d12.lib" )
#pragma comment( lib, "DXGI.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "comctl32.lib" )





#pragma warning(disable:4192)




#pragma comment(lib,"windowscodecs.lib")
#include"ButiRendering_Dx12/ButiRendering/Common.h"

#include"ButiMemorySystem/ButiMemorySystem/ButiList.h"
#include"Header/Common.h"
#include"Header/GameObjects/DefaultGameComponent/DefaultGameComponent.h"
#pragma warning(default:4192)


#endif // !ButiEngineHeader_H