#include <cmath>
#include <vector>
#include "renderer.h"
#include "math_util.h"
#include "matrix4.h"

void CPURenderer::Renderer::render_clear() const
{
	int width = ViewPort::instance.width;
	int height = ViewPort::instance.height;
	memset(ViewPort::instance.frame_buffer, 0, width * height * sizeof(unsigned int));
}

void drawTri(const CPURenderer::Renderer &r, CPURenderer::Vector4 p0, CPURenderer::Vector4 p1, CPURenderer::Vector4 p2)
{
	using namespace CPURenderer;
	Vector4 t0 = r.mainCamera.ScrMappingMat() * r.mainCamera.ProjMat() * r.mainCamera.ViewMat() * p0;
	Vector4 t1 = r.mainCamera.ScrMappingMat() * r.mainCamera.ProjMat() * r.mainCamera.ViewMat() * p1;
	Vector4 t2 = r.mainCamera.ScrMappingMat() * r.mainCamera.ProjMat() * r.mainCamera.ViewMat() * p2;

	Vertex sv0 = { {t0.x / t0.w, t0.y / t0.w, t0.z / t0.w} };
	Vertex sv1 = { {t1.x / t1.w, t1.y / t1.w, t1.z / t1.w} };
	Vertex sv2 = { {t2.x / t2.w, t2.y / t2.w, t2.z / t2.w} };

	r.draw_wireframe_triangle(sv0, sv1, sv2, Color::black);
}

void CPURenderer::Renderer::render_loop()
{	
	if (_frame_count % 480 < 240)
	{
		for (int i = 0; i < ViewPort::instance.width; ++i)
		{
			for (int j = 0; j < ViewPort::instance.height; ++j)
			{
				ViewPort::instance.SetPixel(i, j, Color::yellow);
			}
		}
	}
	else
	{
		for (int i = 0; i < ViewPort::instance.width; ++i)
		{
			for (int j = 0; j < ViewPort::instance.height; ++j)
			{
				ViewPort::instance.SetPixel(i, j, Color::sky_blue);
			}
		}
	}

	//render_clear();

	//Vertex x0{ {1000, 200, 0} }, x1{ {-400, 500, 0} }, x2{ {300, -100, 0} };
	//Vertex y0{ {20, 200, 0} }, y1{ {300, 500, 0} }, y2{ {1700, 100, 0} };
	//draw_wireframe_triangle(x0, x1, x2, Color::yellow);
	//draw_wireframe_triangle(y0, y1, y2, Color::red);

	drawTri(*this, { 1.0f, 1.0f, -1.0f, 1.0f }, { 1.0f, -1.0f, -1.0f, 1.0f }, { -1.0f, 1.0f, -1.0f, 1.0f });
	drawTri(*this, { -1.0f, -1.0f, -1.0f, 1.0f }, { 1.0f, -1.0f, -1.0f, 1.0f }, { -1.0f, 1.0f, -1.0f, 1.0f });
	drawTri(*this, { 1.0f, 1.0f, -1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, -1.0f, -1.0f, 1.0f });
	drawTri(*this, { 1.0f, -1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, -1.0f, -1.0f, 1.0f });
	drawTri(*this, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, -1.0f, 1.0f, 1.0f }, { -1.0f, 1.0f, 1.0f, 1.0f });
	drawTri(*this, { -1.0f, 1.0f, 1.0f, 1.0f }, { -1.0f, -1.0f, 1.0f, 1.0f }, { 1.0f, -1.0f, 1.0f, 1.0f });
	drawTri(*this, { -1.0f, 1.0f, -1.0f, 1.0f }, { -1.0f, -1.0f, -1.0f, 1.0f }, { -1.0f, 1.0f, 1.0f, 1.0f });
	drawTri(*this, { -1.0f, -1.0f, 1.0f, 1.0f }, { -1.0f, -1.0f, -1.0f, 1.0f }, { -1.0f, 1.0f, 1.0f, 1.0f });
	drawTri(*this, { -1.0f, -1.0f, -1.0f, 1.0f }, { 1.0f, -1.0f, -1.0f, 1.0f }, { 1.0f, -1.0f, 1.0f, 1.0f });
	drawTri(*this, { -1.0f, -1.0f, -1.0f, 1.0f }, { -1.0f, -1.0f, 1.0f, 1.0f }, { 1.0f, -1.0f, 1.0f, 1.0f });
	drawTri(*this, { 1.0f, 1.0f, -1.0f, 1.0f }, { -1.0f, 1.0f, -1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	drawTri(*this, { -1.0f, 1.0f, 1.0f, 1.0f }, { -1.0f, 1.0f, -1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	
	//////////////////////////////////////////////////////////////////////////
	// math test.
	Matrix4 m =
	{
		1.0f, 2.0f, 3.0f, 4.0f,
		5.0f, 8.0f, 7.0f, 6.0f,
		4.0f, 3.0f, 2.0f, 1.0f,
		5.0f, 6.0f, 8.0f, 7.0f
	};
	Matrix4 e = m.inverse() * m;
	//////////////////////////////////////////////////////////////////////////
	++_frame_count;
}

void CPURenderer::Renderer::draw_DDA_line(const Point2d &p0, const Point2d &p1, Color c) const
{
	float dx = p1.x - p0.x;
	float dy = p1.y - p0.y;

	int step = (int)round(abs(dx) > abs(dy) ? abs(dx) : abs(dy));

	float Xinc = dx / step;
	float Yinc = dy / step;

	float X = p0.x;
	float Y = p0.y;

	for (int i = 0; i < step; ++i)
	{
		ViewPort::instance.SetPixel((int)round(X), (int)round(Y), c);
		X += Xinc;
		Y += Yinc;
	}
}

void CPURenderer::Renderer::draw_wireframe_triangle(const Vertex & v0, const Vertex & v1, const Vertex & v2, Color c) const
{
	std::vector<Vertex> convex = { v0, v1, v2 };

	std::vector<Vertex> clipped = sutherland_hodgman_clipping(convex, { 0.0f, (float)ViewPort::instance.width - 1.0f }, { 0.0f, (float)ViewPort::instance.height - 1.0f }, { -1.0f, 1.0f });
	for (int i = 0; i < (int)clipped.size(); ++i)
	{
		if (i <= (int)clipped.size() - 2)
		{
			draw_DDA_line({ clipped[i].pos.x , clipped[i].pos.y }, { clipped[i + 1].pos.x , clipped[i + 1].pos.y }, c);
		}
		else
		{
			draw_DDA_line({ clipped[i].pos.x , clipped[i].pos.y }, { clipped[0].pos.x , clipped[0].pos.y }, c);
		}

		if (i != 0) draw_DDA_line({ clipped[i].pos.x , clipped[i].pos.y }, { clipped[0].pos.x , clipped[0].pos.y }, c);
	}
}

namespace
{
	enum ScissorAxis
	{
		X_neg, X_pos, Y_neg, Y_pos, Z_neg, Z_pos
	};

	bool vertexInsideClippingArea(const CPURenderer::Vertex &vert, ScissorAxis axis, float boundVal)
	{
		switch (axis)
		{
		case ScissorAxis::X_neg:
			return vert.pos.x >= boundVal;
		case ScissorAxis::X_pos:
			return vert.pos.x <= boundVal;
		case ScissorAxis::Y_neg:
			return vert.pos.y >= boundVal;
		case ScissorAxis::Y_pos:
			return vert.pos.y <= boundVal;
		case ScissorAxis::Z_neg:
			return vert.pos.z >= boundVal;
		case ScissorAxis::Z_pos:
			return vert.pos.z <= boundVal;
		default:
			return false;
		}
	}

	void clip_point_pair(std::vector<CPURenderer::Vertex> &vertsQueue, CPURenderer::Vertex first, CPURenderer::Vertex second, ScissorAxis axis, float boundVal)
	{
		using namespace CPURenderer;
		using namespace CPURenderer::Math;

		bool firstPointInsideBound = vertexInsideClippingArea(first, axis, boundVal);
		bool secondPointInsideBound = vertexInsideClippingArea(second, axis, boundVal);
		LerpAxis lerpAxis = axis <= 1 ? LerpAxis::X : (axis >= 4 ? LerpAxis::Z : LerpAxis::Y);

		if (firstPointInsideBound && secondPointInsideBound)
		{
			vertsQueue.push_back(std::move(second));
		}
		else if (!firstPointInsideBound && secondPointInsideBound)
		{
			Vertex intersect = VertexLerp(first, second, boundVal, lerpAxis);
			vertsQueue.push_back(std::move(intersect));
			vertsQueue.push_back(std::move(second));
		}
		else if (firstPointInsideBound && !secondPointInsideBound)
		{
			Vertex intersect = VertexLerp(first, second, boundVal, lerpAxis);
			vertsQueue.push_back(std::move(intersect));
		}
	}

	std::vector<CPURenderer::Vertex> sutherland_hodgman_clip_by_axis(std::vector<CPURenderer::Vertex> inputVerts, ScissorAxis axis, float boundVal)
	{
		using namespace CPURenderer;
		using namespace CPURenderer::Math;

		std::vector<Vertex> ret;

		for (size_t i = 0; i < inputVerts.size(); ++i)
		{
			if (i <= inputVerts.size() - 2)
			{
				clip_point_pair(ret, inputVerts[i], inputVerts[i + 1], axis, boundVal);
			}
			else
			{
				clip_point_pair(ret, inputVerts[i], inputVerts[0], axis, boundVal);
			}
		}

		return ret;
	}
}

std::vector<CPURenderer::Vertex> CPURenderer::Renderer::sutherland_hodgman_clipping(std::vector<Vertex> inputVerts, Point2d x_range, Point2d y_range, Point2d z_range) const
{
	inputVerts = sutherland_hodgman_clip_by_axis(inputVerts, ScissorAxis::X_neg, x_range.x);
	inputVerts = sutherland_hodgman_clip_by_axis(inputVerts, ScissorAxis::X_pos, x_range.y);
	inputVerts = sutherland_hodgman_clip_by_axis(inputVerts, ScissorAxis::Y_neg, y_range.x);
	inputVerts = sutherland_hodgman_clip_by_axis(inputVerts, ScissorAxis::Y_pos, y_range.y);
	inputVerts = sutherland_hodgman_clip_by_axis(inputVerts, ScissorAxis::Z_neg, z_range.x);
	inputVerts = sutherland_hodgman_clip_by_axis(inputVerts, ScissorAxis::Z_pos, z_range.y);

	return inputVerts;
}