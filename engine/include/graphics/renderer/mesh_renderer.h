#pragma once 

#include <core/macro.h>
#include <graphics/draw_item.h>
#include <graphics/render_pass.h>

#define MAX_STATIC_MESH_DRAW_ITEMS 1024

TERMINUS_BEGIN_NAMESPACE

struct FramePacket;

class MeshRenderer
{
public:
	MeshRenderer();
	~MeshRenderer();
	void render(RenderSubPass& sub_pass, FramePacket* pkt);

private:
	DrawItem m_draw_items[MAX_STATIC_MESH_DRAW_ITEMS];
	uint32_t m_num_draw_items;
};

TERMINUS_END_NAMESPACE