#ifndef SP_ENGINE_H
#define SP_ENGINE_H

#define __FILENAME__                    (strrchr(__FILE__, '/') + 1)

#include <iostream>
#include <cassert>
#include <memory>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <set>
#include <unordered_map>
#include <unordered_set>

#define ARRAY_SIZE(r)   (sizeof(r) / sizeof(r[0]))

#ifdef SP_ENGINE_PLATFORM_WIN32
    //windows
#define VK_USE_PLATFORM_WIN32_KHR
#elif SP_ENGINE_PLATFORM_MACOS
    //mac
#define VK_USE_PLATFORM_MACOS_MVK
#elif SP_ENGINE_PLATFORM_LINUX
    //linux
#define VK_USE_PLATFORM_XCB_KHR
#endif

#define SP_ENGINE_GRAPHIC_API_VULKAN

#endif