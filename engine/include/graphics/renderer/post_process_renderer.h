#pragma once 

#include <core/macro.h>

TERMINUS_BEGIN_NAMESPACE

struct FramePacket;

class PostProcessRenderer
{
public:
	PostProcessRenderer();
	~PostProcessRenderer();
	void render(FramePacket* pkt);
};

TERMINUS_END_NAMESPACE