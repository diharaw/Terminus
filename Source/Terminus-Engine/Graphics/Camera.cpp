#include "Camera.h"
#include "../Platform/PlatformBackend.h"

namespace Terminus { namespace Graphics {

	Camera::Camera()
	{
		m_Position = Vector3(0.0f, 0.0f, 0.0f);
		m_Forward = Vector3(0.0f, 0.0f, -1.0f);
		m_AspectRatio = static_cast<float>(PlatformBackend::GetWidth()) / static_cast<float>(PlatformBackend::GetHeight());
	}

	Camera::Camera(Vector3 _Position, Vector3 _Forward, float _FoV, float _AspectRatio, float _NearPlane, float _FarPlane)
	{
		m_Position = _Position;
		m_Forward = _Forward;
		m_Right = glm::normalize(glm::cross(m_Forward, Vector3(0.0f, 1.0f, 0.0f)));
		m_Up = glm::normalize(glm::cross(m_Right, m_Forward));
		m_AspectRatio = _AspectRatio;
		m_FoV = _FoV;
		m_NearPlane = _NearPlane;
		m_FarPlane = _FarPlane;
		m_ProjectionType = ProjectionType::PERSPECTIVE;

		CreateProjectionMatrix();
	}

	Camera::~Camera()
	{

	}

	void Camera::Update()
	{
		m_Orientation = m_qPitch * m_Orientation;
		m_Orientation = m_Orientation * m_qYaw;
		m_Orientation = m_qRoll * m_Orientation;
		m_Orientation = glm::normalize(m_Orientation);

		m_Rotation = glm::mat4_cast(m_Orientation);
		m_Forward = glm::conjugate(m_Orientation) * Vector3(0.0f, 0.0f, -1.0f);
		m_Right = glm::normalize(glm::cross(m_Forward, Vector3(0.0f, 1.0f, 0.0f)));
		m_Up = glm::normalize(glm::cross(m_Right, m_Forward));

		m_Translation = glm::translate(Matrix4(), -m_Position);
		m_ViewMatrix = m_Rotation * m_Translation;
		m_ViewProjection = m_ProjectionMatrix * m_ViewMatrix;

		m_qPitch = glm::angleAxis(0.0f, Vector3(1.0f, 0.0f, 0.0f));
		m_qYaw = glm::angleAxis(0.0f, Vector3(0.0f, 1.0f, 0.0f));
		m_qRoll = glm::angleAxis(0.0f, Vector3(0.0f, 0.0f, 1.0f));
	}

	// Setters
	void Camera::SetPosition(Vector3 _Position)
	{
		m_Position = _Position;
	}

	void Camera::SetForward(Vector3 _Forward)
	{
		m_Forward = _Forward;
	}

	void Camera::SetPitch(float _Pitch)
	{
		m_qPitch = glm::angleAxis(_Pitch, Vector3(1.0f, 0.0f, 0.0f));
	}

	void Camera::SetYaw(float _Yaw)
	{
		m_qYaw = glm::angleAxis(_Yaw, Vector3(0.0f, 1.0f, 0.0f));
	}

	void Camera::SetRoll(float _Roll)
	{
		m_qRoll = glm::angleAxis(_Roll, Vector3(0.0f, 0.0f, 1.0f));
	}

	void Camera::SetProjectionType(ProjectionType _Type)
	{
		m_ProjectionType = _Type;
		CreateProjectionMatrix();
	}

	void Camera::SetCameraCorners(float _Left, float _Right, float _Up, float _Down)
	{
		m_LeftOrtho = _Left;
		m_RightOrtho = _Right;
		m_UpOrtho = _Up;
		m_DownOrtho = _Down;
		CreateProjectionMatrix();
	}

	void Camera::SetFoV(float _fov)
	{
		m_FoV = _fov;
		CreateProjectionMatrix();
	}

	void Camera::SetAspectRatio(float _AspectRatio)
	{
		m_AspectRatio = _AspectRatio;
		CreateProjectionMatrix();
	}

	void Camera::SetNearPlane(float _NearPlane)
	{
		m_NearPlane = _NearPlane;
		CreateProjectionMatrix();
	}

	void Camera::SetFarPlane(float _FarPlane)
	{
		m_FarPlane = _FarPlane;
		CreateProjectionMatrix();
	}

	void Camera::CreateProjectionMatrix()
	{
		if (m_ProjectionType == ProjectionType::PERSPECTIVE)
		{
			m_ProjectionMatrix = Math::Perspective(m_FoV, m_AspectRatio, m_NearPlane, m_FarPlane);
		}
		else if (m_ProjectionType == ProjectionType::ORTHOGRAPHIC)
		{
			m_ProjectionMatrix = Math::Ortho(m_LeftOrtho, m_RightOrtho, m_DownOrtho, m_UpOrtho, m_NearPlane, m_FarPlane);
		}
	}

} }
