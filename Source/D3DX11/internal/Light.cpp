#include "Light.h"

Light::Light(): m_ambientColor(), m_diffuseColor(), m_direction(), m_specularColor(), m_specularPower(0)
{
}

void Light::SetAmbientColor(const float r, const float g, const float b, const float a)
{
	m_ambientColor = DirectX::XMFLOAT4(r, g, b, a);
}

void Light::SetDiffuseColor(const float r, const float g, const float b, const float a)
{
	m_diffuseColor = DirectX::XMFLOAT4(r, g, b, a);
}

void Light::SetDirection(const float x, const float y, const float z)
{
	m_direction = DirectX::XMFLOAT3(x, y, z);
}

void Light::SetSpecularColor(const float r, const float g, const float b, const float a)
{
	m_specularColor = DirectX::XMFLOAT4(r, g, b, a);
}

void Light::SetSpecularPower(const float power)
{
	m_specularPower = power;
}

DirectX::XMFLOAT4 Light::GetAmbientColor() const
{
	return m_ambientColor;
}

DirectX::XMFLOAT4 Light::GetDiffuseColor() const
{
	return m_diffuseColor;
}

DirectX::XMFLOAT3 Light::GetDirection() const
{
	return m_direction;
}

DirectX::XMFLOAT4 Light::GetSpecularColor() const
{
	return m_specularColor;
}

float Light::GetSpecularPower() const
{
	return m_specularPower;
}
