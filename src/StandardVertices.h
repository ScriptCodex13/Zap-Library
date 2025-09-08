#pragma once

#ifndef STANDARD_VERTICES_H
#define STANDARD_VERTICES_H

#include <vector>
#include <array>

namespace zap
{
	namespace standard_cube
	{
		inline std::vector<float> standardcubevertices
		{
		   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		   0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		   0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		   0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		  -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		  -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		  -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		  -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		  -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		  -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		  -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		   0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		   0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		   0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		   0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		   0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		   0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		  -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		   0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		   0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		   0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		  -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		  -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		  -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		   0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		   0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		   0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		  -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		  -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
		};

		inline std::array<int, 4> vertex_attribute_position = { 0, 3, 6, 0 };
		inline std::array<int, 4> vertex_attribute_normals = { 1, 3, 6, 3 };
	}
}

#endif