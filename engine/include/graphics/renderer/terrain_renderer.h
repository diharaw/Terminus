#pragma once 

#include <core/macro.h>

TERMINUS_BEGIN_NAMESPACE

struct FramePacket;

class TerrainRenderer
{
public:
	TerrainRenderer();
	~TerrainRenderer();
	void render(FramePacket* pkt);
};

TERMINUS_END_NAMESPACE