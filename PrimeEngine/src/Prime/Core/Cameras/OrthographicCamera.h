#pragma once

namespace Prime
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float width, float height, float top, float nearPlane, float farPlane);

		void SetPosition(const Vector3& position) { m_position = position; UpdateMatrices(); }
		void SetRotation(float zRotation) { m_zRotation = zRotation; UpdateMatrices(); }

		const Vector3& GetPosition() const { return m_position; }
		float GetRotation() const { return m_zRotation; }

		const Matrix& GetProjectionMatrix() const { return m_projectionMatrix; }
		const Matrix& GetViewMatrix()       const { return m_viewMatrix; }
		const Matrix& GetViewProjMatrix()   const { return m_viewProjMatrix; }

		void UpdateMatrices();
	private:
		Matrix m_projectionMatrix;
		Matrix m_viewMatrix;
		Matrix m_viewProjMatrix;

		Vector3 m_position = Vector3(5.0f, 3.0f, -1.0f);
		float m_zRotation = 0.0f;
	};
}