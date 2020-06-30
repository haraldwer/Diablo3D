#include "ModelList.h"

ModelList::ModelList(): m_modelCount(0), m_ModelInfoList(nullptr)
{
}

bool ModelList::Initialize(int numModels)
{
	m_modelCount = numModels;
	m_ModelInfoList = new ModelInfoType[m_modelCount];
	if (!m_ModelInfoList)
		return false;

	srand((unsigned int)time(NULL));
	for(int i = 0; i < m_modelCount; i++)
	{
		m_ModelInfoList[i].color = DirectX::XMFLOAT4((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, 1.0f);
		m_ModelInfoList[i].positionX = (((float)rand() - (float)rand()) / RAND_MAX) * 10.0f;
		m_ModelInfoList[i].positionY = (((float)rand() - (float)rand()) / RAND_MAX) * 10.0f;
		m_ModelInfoList[i].positionZ = ((((float)rand() - (float)rand()) / RAND_MAX) * 10.0f) + 5.0f;
	}

	return true;
}

void ModelList::Shutdown()
{
	if(m_ModelInfoList)
	{
		delete[] m_ModelInfoList;
		m_ModelInfoList = nullptr;
	}
}

int ModelList::GetModelCount()
{
	return m_modelCount;
}

void ModelList::GetData(int index, float& positionX, float& positionY, float& positionZ, DirectX::XMFLOAT4& color)
{
	positionX = m_ModelInfoList[index].positionX;
	positionY = m_ModelInfoList[index].positionY;
	positionZ = m_ModelInfoList[index].positionZ;
	color = m_ModelInfoList[index].color;
}
