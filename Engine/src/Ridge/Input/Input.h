#pragma once

#include "Key.h"
#include "Mouse.h"

namespace Ridge {
	extern bool InitPlatformInput();

	bool InitInput() {
		return InitPlatformInput();
	}
}