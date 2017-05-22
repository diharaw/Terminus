#pragma once 

#include <core/macro.h>

TERMINUS_BEGIN_NAMESPACE

struct FramePacket;

class MeshRenderer
{
public:
	MeshRenderer();
	~MeshRenderer();
	void render(FramePacket* pkt);
};

TERMINUS_END_NAMESPACE