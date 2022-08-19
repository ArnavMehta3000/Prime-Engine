#include <pch.h>
#include "OrthographicCamera.h"

namespace Prime
{
	OrthographicCamera::OrthographicCamera(float left, float width, float height, float top, float nearPlane, float farPlane)
	{
		m_projectionMatrix = XMMatrixOrthographicOffCenterLH(left, width, height, top, nearPlane, farPlane);
		m_viewMatrix = XMMatrixIdentity();
		m_viewProjMatrix = m_projectionMatrix * m_viewMatrix;
		UpdateMatrices();
	}
	
	void OrthographicCamera::UpdateMatrices()
	{
		Matrix translation = Matrix::CreateTranslation(m_position);
		Matrix rotation = Matrix::CreateRotationZ(m_zRotation);
		m_viewMatrix = rotation * translation;
		m_viewProjMatrix = m_projectionMatrix * m_viewMatrix;
	}
}