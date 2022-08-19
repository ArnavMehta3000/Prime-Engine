#include <pch.h>
#include "OrthographicCamera.h"

namespace Prime
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float nearPlane, float farPlane)
	{
		m_projectionMatrix = XMMatrixOrthographicOffCenterLH(left, right, bottom, top, nearPlane, farPlane);
		m_viewMatrix = XMMatrixIdentity();
		m_viewProjMatrix = m_projectionMatrix * m_viewMatrix;
	}
	void OrthographicCamera::UpdateMatrices()
	{
		Matrix transform = Matrix::CreateTranslation(m_position) * Matrix::CreateRotationZ(m_zRotation);
		
		Matrix view = transform.Invert();
		m_viewProjMatrix = m_projectionMatrix * m_viewMatrix;
	}
}