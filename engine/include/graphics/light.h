#pragma once

#ifndef LIGHT_H
#define LIGHT_H

#include <core/types.h>

namespace terminus
{
	struct Light
	{
		Vector4 color;
		float intensity;
		bool casts_shadow;
	};

	struct PointLight : Light
	{
		Vector3 position;
	};

	struct SpotLight : Light
	{
		Vector3 position;
		Vector3 direction;
	};

	struct DirectionalLight : Light
	{
		Vector3 direction;
	};
}

#endif
