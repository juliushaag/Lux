#include "Camera2D.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/Application.h"

namespace Lux
{
Camera2D::Camera2D()
    : m_ProjMat(
        glm::ortho(
            -Application::AspectRatio(),
            Application::AspectRatio(),
            -1.0f,
            1.0f, 
            NEAR_VAL, 
            FAR_VAL
        )
    )
    , m_ViewMat(1)
    , m_Position({0.0f, 0.0f})
    , m_Rotation(0.0f)
{
    m_Width = Application::AspectRatio() * 2;
    m_Height = 2;
    recalculate();
}

void Camera2D::recalculate()
{
    mat4 transform = glm::translate(glm::mat4(1.0f), { m_Position.x, m_Position.y, 1.0f }) * 
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
		
	m_ViewMat = glm::inverse(transform);
	m_ViewProjMat = m_ProjMat * m_ViewMat;
}

void Camera2D::set_projection(float left, float right, float bottom, float top)
{
    m_ProjMat = glm::ortho(left, right, bottom, top, NEAR_VAL, FAR_VAL);
    recalculate();

    m_Width = abs(left) + abs(right);
    m_Height = abs(top) + abs(bottom);
}

}