#pragma once
#include "Buffer.h"
class ConstBuffer :
    public Buffer
{
public:
    virtual void Init(ComPtr<ID3D11Device>& device, UINT elementSize
        , UINT numElement, void* pData) override;
};

