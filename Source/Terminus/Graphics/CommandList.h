#pragma once

#ifndef COMMANDLIST_H
#define COMMANDLIST_H

#include <stdint.h>
#include "RenderCommon.h"
#include "RenderDevice.h"
#include "../Types.h"

typedef void(*BackendFunction)(const void*);
typedef uint64_t SortKey;

// in a 64-bit key
// 5-bits | 4-bits | 10-bits | 10-bits | 1-bit | 1-bit   | 2-bits | 24-bits 
// Pass	  |	Subpass|  Mat Id | VAO	   | Z-Buf | Stencil | Cull   | Depth

struct DrawCommand
{
	SortKey			m_sort_key;
	BackendFunction m_function;
	void*			m_data;

	DrawCommand()
	{
		
	}

	inline void SetRenderPass(int _renderPass) { SortKey temp = _renderPass; m_sort_key |= temp << 59; }
	inline void SetRenderSubpass(int _renderSubPass) { SortKey temp = _renderSubPass; m_sort_key |= temp << 55; }
	inline void SetMaterial(int _material) { SortKey temp = _material; m_sort_key |= temp << 45; }
	inline void SetVertexArray(int _vertexArray) { SortKey temp = _vertexArray; m_sort_key |= temp << 35; }
	inline void SetDepthTest(int _depthTest) { SortKey temp = _depthTest; m_sort_key |= temp << 34; }
	inline void SetStencilTest(int _stencilTest) { SortKey temp = _stencilTest; m_sort_key |= temp << 33; }
	inline void SetFaceCulling(int _faceCull) { SortKey temp = _faceCull; m_sort_key |= temp << 31; }
	inline void SetDepth(float _depth) { m_sort_key |= (static_cast<SortKey>(_depth) & 0xffff); }

	inline int	GetRenderPass() { return (m_sort_key >> 59); }
	inline int	GetRenderSubpass() { return ((m_sort_key >> 55) & 15); }
	inline int	GetMaterial() { return ((m_sort_key >> 45) & 1023); }
	inline int	GetVertexArray() { return ((m_sort_key >> 35) & 1023); }
	inline int	GetDepthTest() { return ((m_sort_key >> 34) & 1); }
	inline int	GetStencilTest() { return ((m_sort_key >> 33) & 1); }
	inline int	GetFaceCulling() { return ((m_sort_key >> 31) & 3); }
};

namespace CommandData
{
	struct Draw
	{
		unsigned int FirstIndex;
		unsigned int Count;
	};

	struct DrawIndexed
	{
		unsigned int IndexCount;
	};

	struct DrawIndexedBaseVertex
	{
		unsigned int IndexCount;
		unsigned int BaseIndex;
		unsigned int BaseVertex;
	};

	struct UniformBufferCopy
	{
		ResourceHandle Buffer;
		void*		   Data;
		size_t		   Size;
		BufferMapType  MapType;
	};

	struct ClearRenderTarget
	{
		Vector4 ClearColor;
		FramebufferClearTarget Target;
	};
}

namespace GPUCommand
{
	void Draw(void* data);

	void DrawIndexed(void* data);

	void DrawIndexedBaseVertex(void* data);

	void UniformBufferCopy(void* data);

	void ClearRenderTarget(void* data);
}

#endif
