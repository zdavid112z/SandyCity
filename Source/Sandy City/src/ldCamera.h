#pragma once

#include "ducky.h"

using namespace ducky;
using namespace core;
using namespace graphics;

class LDCamera : public Component
{
public:
	LDCamera(Camera* c);
	~LDCamera();

	void Init() override;
	void CleanUp() override;

	void Update() override;

	string GetType() { return GetComponentType(); }
	static string GetComponentType() { return "LDCamera"; }
private:
	Camera* m_Camera;
	glm::vec3 m_Focus = glm::vec3(0);
	float m_Distance = 4;
	float m_MoveSpeed = 2;
	float m_RotateSpeed = 10;
	glm::vec2 m_MinAABB = glm::vec2(-12, -12);
	glm::vec2 m_MaxAABB = glm::vec2( 12,  12);
	float m_DownAngle = glm::radians(55.0f);
	float m_Angle = 0;
};