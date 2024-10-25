#ifndef MONTECARLO_VERTEXATTRIB_HPP
#define MONTECARLO_VERTEXATTRIB_HPP

#include <glm/glm.hpp>

struct VertexAttrib {
	glm::vec4 pos, col;
	VertexAttrib(const glm::vec4& pos, const glm::vec4& col): pos(pos), col(col){}
};


#endif // !VERTEXATTRIB_HPP
