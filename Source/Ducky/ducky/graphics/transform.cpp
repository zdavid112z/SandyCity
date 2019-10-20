#include "ducky_pch.h"
#include "transform.h"

namespace ducky { namespace graphics {

	vector<glm::mat4> TransformStack::m_Stack = vector<glm::mat4>(1, glm::mat4(1));

} }