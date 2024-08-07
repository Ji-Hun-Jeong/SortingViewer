#include "pch.h"
#include "Buffer.h"
#include "D3DUtils.h"

void Buffer::Update(ComPtr<ID3D11DeviceContext>& context, UINT elementSize
	, UINT numElement, void* pData)
{
	D3DUtils::UpdateStagingBuffer(context, elementSize, numElement, pData, m_stagingBuffer);
	context->CopyResource(m_buffer.Get(), m_stagingBuffer.Get());
}
