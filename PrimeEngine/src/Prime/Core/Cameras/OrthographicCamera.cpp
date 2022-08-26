#include <pch.h>
#include "OrthographicCamera.h"
#include "Prime/Core/Graphics/ResizeHandler.h"

namespace Prime
{
	OrthographicCamera::OrthographicCamera(float width, float height, float nearPlane, float farPlane)
		: m_position(Vector3(0.0f, 0.0f, -5.0f)), m_zRotation(0.0f), m_orthoScale(100.0f)
	{
		ResizeHandler::RegisterFunction(PRIME_BIND_RESIZE_FN(OrthographicCamera::OnResizeCallback));

		m_nearPlane = nearPlane;
		m_farPlane = farPlane;

		m_projectionMatrix = XMMatrixOrthographicLH(width, height, nearPlane, farPlane);
		UpdateMatrices();
	}
	
	void OrthographicCamera::UpdateMatrices()
	{
		// S*R*T format
		//m_viewMatrix = Matrix::CreateScale(m_orthoScale) * Matrix::CreateRotationZ(XMConvertToRadians(m_zRotation)) * XMMatrixLookToLH(m_position, Vector3(0.0f, 0.0f, 1.f), Vector3(0.0f, 1.0f, 0.0f));

		m_viewMatrix = XMMatrixLookToLH(m_position, Vector3(0.0f, 0.0f, 1.f), Vector3(0.0f, 1.0f, 0.0f));
		m_viewMatrix *= Matrix::CreateScale(m_orthoScale) * Matrix::CreateRotationZ(XMConvertToRadians(m_zRotation));

		m_viewProjMatrix = m_viewMatrix * m_projectionMatrix;
	}

	void OrthographicCamera::OnResizeCallback(int w, int h)
	{
		m_projectionMatrix = XMMatrixOrthographicLH((float)w, (float)h, m_nearPlane, m_farPlane);
	}
}