#pragma once

#include "Ridge/Common.h"

#ifdef RIDGE_PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#include "Windows.h"
	#undef WIN32_LEAN_AND_MEAN
#endif

#include <memory>
#include <functional>
#include <algorithm>
#include <chrono>
#include <cmath>

#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <array>
#include <string>

#include <fstream>
#include <iostream>
#include <sstream>

#include "Ridge/util/Log.h"
#include "Ridge/Events/Event.h"
#include "Ridge/Types.h"
