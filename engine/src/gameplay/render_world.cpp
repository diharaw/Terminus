#include <gameplay/render_world.h>
#include <core/frame_packet.h>

namespace terminus
{
	RenderWorld::RenderWorld()
	{

	}

	RenderWorld::~RenderWorld()
	{

	}

	void RenderWorld::initialize(GameWorld* game_world)
	{
		_game_world = game_world;
	}

	void RenderWorld::simulate(FramePacket* pkt, double dt)
	{

	}

	void RenderWorld::shutdown()
	{

	}
}