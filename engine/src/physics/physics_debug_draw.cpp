#include <physics/physics_debug_draw.h>
#include <graphics/render_device.h>

namespace terminus
{
	PhysicsDebugDraw::PhysicsDebugDraw()
	{
	}

	PhysicsDebugDraw::~PhysicsDebugDraw()
	{

	}

	void PhysicsDebugDraw::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color)
	{

	}

	void PhysicsDebugDraw::drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color)
	{

	}

	void PhysicsDebugDraw::draw3dText(const btVector3 &location, const char *textString)
	{

	}

	void PhysicsDebugDraw::reportErrorWarning(const char *warningString)
	{

	}

	void PhysicsDebugDraw::setDebugMode(int debugMode)
	{

	}

	int PhysicsDebugDraw::getDebugMode() const
	{

	}

	void PhysicsDebugDraw::render()
	{

	}
}