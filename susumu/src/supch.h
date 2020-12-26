#pragma once

#include "engine/core/platform_detection.h"

#ifdef SU_PLATFORM_WINDOWS
	#ifndef NOMINMAX
		// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
		#define NOMINMAX
	#endif
#endif

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <cstdint>
#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "engine/core/base.h"
#include "engine/core/log.h"
#include "engine/debug/instrumentor.h"

#ifdef SU_PLATFORM_WINDOWS
    #include <Windows.h>
#endif
