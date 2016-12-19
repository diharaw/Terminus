#pragma once

#ifndef MATHUTITLTY_H
#define MATHUTITLTY_H

#include <gtc/matrix_transform.hpp>
#include "../Core/Config.h"
#include "../Types.h"

namespace Terminus { namespace Math {

	inline Matrix4 Perspective(float fov, float aspectRatio, float nearPlane, float farPlane)
	{
		Matrix4 projection = glm::perspectiveRH(fov, aspectRatio, nearPlane, farPlane);

#if defined(TERMINUS_DIRECT3D11)
		glm::mat4 scale = glm::scale(glm::vec3(1.0f, 1.0f, 0.5f));
		glm::mat4 translate = glm::translate(glm::vec3(0.0f, 0.0f, 1.0f));
		projection = scale * translate * projection;
#endif

		return projection;
	}

	inline Matrix4 Ortho(float left, float right, float down, float up, float nearPlane, float farPlane)
	{
		Matrix4 projection = glm::ortho(left, right, down, up, nearPlane, farPlane);

#if defined(TERMINUS_DIRECT3D11)
		glm::mat4 scale = glm::scale(glm::vec3(1.0f, 1.0f, 0.5f));
		glm::mat4 translate = glm::translate(glm::vec3(0.0f, 0.0f, 1.0f));
		projection = scale * translate * projection;
#endif

		return projection;
	}

} }

#endif
