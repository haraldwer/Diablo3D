#pragma once
#include <DirectXMath.h>
class Light
{
public:
	Light();
	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);
	void SetSpecularColor(float, float, float, float);
	void SetSpecularPower(float);
	DirectX::XMFLOAT4 GetAmbientColor() const;
	DirectX::XMFLOAT4 GetDiffuseColor() const;
	DirectX::XMFLOAT3 GetDirection() const;
	DirectX::XMFLOAT4 GetSpecularColor() const;
	float GetSpecularPower() const;
private:
	DirectX::XMFLOAT4 m_ambientColor;
	DirectX::XMFLOAT4 m_diffuseColor;
	DirectX::XMFLOAT3 m_direction;
	DirectX::XMFLOAT4 m_specularColor;
	float m_specularPower;
};

