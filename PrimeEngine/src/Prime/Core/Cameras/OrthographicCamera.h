#pragma once

namespace Prime
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float width, float height, float nearPlane = 1.0f, float farPlane = 1000.0f);

		void SetPosition(const Vector3& position) { m_position   = position; UpdateMatrices(); }
		void SetRotation(float degrees)           { m_zRotation  = degrees;  UpdateMatrices(); }
		void SetOrthoScale(float scale = 100.0f)  { m_orthoScale = scale;    UpdateMatrices(); }

		const Vector3& GetPosition() const { return m_position; }
		float GetRotation()          const { return m_zRotation; }
		float GetOrthoScale()        const { return m_orthoScale; }

		const Matrix& GetProjectionMatrix() const { return m_projectionMatrix; }
		const Matrix& GetViewMatrix()       const { return m_viewMatrix; }
		const Matrix& GetViewProjMatrix()   const { return m_viewProjMatrix; }

		void OnResizeCallback(int w, int h);

	private:
		void UpdateMatrices();

		Matrix m_projectionMatrix;
		Matrix m_viewMatrix;
		Matrix m_viewProjMatrix;

		Vector3 m_position;
		float m_zRotation;
		float m_orthoScale = 100.0f;
		float m_nearPlane, m_farPlane;
	};
}