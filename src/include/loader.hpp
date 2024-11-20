#pragma once
#include <cdefs.h>
#include <object.hpp>

namespace LOADER {
	Container *LoadContainer(Container *container, u8 *data, usize size);
}
