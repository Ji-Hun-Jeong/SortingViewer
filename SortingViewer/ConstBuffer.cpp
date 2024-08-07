#include "pch.h"
#include "ConstBuffer.h"
#include "D3DUtils.h"

void ConstBuffer::Init(ComPtr<ID3D11Device>& device, UINT elementSize, UINT numElement, void* pData)
{
	D3DUtils::CreateConstBuffer(device, elementSize, m_buffer);
	D3DUtils::CreateStagingBuffer(device, elementSize, numElement, pData, m_stagingBuffer);
}
