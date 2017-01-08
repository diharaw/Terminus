#pragma once

#include <stdint.h>

#ifndef COMPONENT_H
#define COMPONENT_H

namespace terminus
{
using ComponentID = uint16_t;

#define INVALID_COMPONENT 0;

	struct IComponent
	{
		static const ComponentID _id = INVALID_COMPONENT;
	};

}

#endif
