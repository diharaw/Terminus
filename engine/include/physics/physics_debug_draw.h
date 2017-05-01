#pragma once

#include <LinearMath/btIDebugDraw.h>
#include <core/types.h>
#include <vector>

namespace terminus
{
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;

	struct PhysicsDebugVertex
	{
		Vector4 position;
		Vector4 color;
	};

	struct DebugDrawData
	{
		std::vector<Vector4> _lines;
		std::vector<Vector4> _line_colors;
		std::vector<PhysicsDebugVertex> _debug_vertices;
	};

	class PhysicsDebugDraw : public btIDebugDraw
	{
	public:
		PhysicsDebugDraw();
		~PhysicsDebugDraw();
		void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) override;
		void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color) override;
		void draw3dText(const btVector3 &location, const char *textString) override;
		void reportErrorWarning(const char *warningString) override;
		void setDebugMode(int debugMode) override;
		int getDebugMode() const override;
		void render();

	private:
		VertexArray*  _vertex_array;
		VertexBuffer* _vertex_buffer;
		IndexBuffer*  _index_buffer;
		DebugDrawData _draw_data[3];
	};
}