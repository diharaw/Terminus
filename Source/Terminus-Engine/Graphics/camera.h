#pragma once

#include <Math/math_utility.h>

namespace terminus
{
	enum class ProjectionType
	{
		PERSPECTIVE = 0,
		ORTHOGRAPHIC
	};

	class Camera
	{
	private:

		// View
		Vector3 m_Position;
		Vector3 m_Forward;
		Vector3 m_Right;
		Vector3 m_Up;
		Matrix4 m_ViewProjection;
		Matrix4 m_Rotation;
		Matrix4 m_Translation;
		Quaternion m_Orientation;

		// Perspective Projection
		float m_FoV;
		float m_AspectRatio;
		float m_NearPlane;
		float m_FarPlane;

		// Orthographic Projection
		float m_LeftOrtho;
		float m_RightOrtho;
		float m_UpOrtho;
		float m_DownOrtho;

		// Per-Rotation Properties
		Quaternion m_qPitch;
		Quaternion m_qYaw;
		Quaternion m_qRoll;

		ProjectionType m_ProjectionType;

	public:
		Matrix4 m_ViewMatrix;
		Matrix4 m_ProjectionMatrix;

	public:
		Camera();
		Camera(Vector3 _Position, Vector3 _Forward, float _FoV, float _AspectRatio, float _NearPlane, float _FarPlane);
		~Camera();

		void Update();

		// Setters
		void SetPosition(Vector3 _Position);
		void SetForward(Vector3 _Forward);
		void SetPitch(float _Pitch);
		void SetYaw(float _Yaw);
		void SetRoll(float _Roll);
		void SetProjectionType(ProjectionType _Type);
		void SetCameraCorners(float _Left, float _Right, float _Up, float _Down);
		void SetFoV(float _fov);
		void SetAspectRatio(float _AspectRatio);
		void SetNearPlane(float _NearPlane);
		void SetFarPlane(float _FarPlane);

		// Getters
		inline Vector3 GetForward() { return m_Forward; }
		inline Vector3 GetPosition() { return m_Position; }
		inline Vector3 GetRight() { return m_Right; }

	private:
		void CreateProjectionMatrix();
	};
}
