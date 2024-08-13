#pragma once
#include "ElementGenerator.h"
class BoxElementGenerator
    : public IElementGenerator
{
public:
    BoxElementGenerator();
    BoxElementGenerator(const BoxElementGenerator&) = delete;
    BoxElementGenerator(BoxElementGenerator&&) = delete;
    virtual ~BoxElementGenerator() = default;

public:
    virtual void GenerateElement(ComPtr<ID3D11Device>& device
        , ComPtr<ID3D11DeviceContext>& context, vector<shared_ptr<Mesh>>& vec
        , int numOfElement) = 0;
    virtual void GlobalConstSetData(GlobalConst& globalConst) override;

    static MeshData GetBoxMeshData() { return m_box; }

protected:
    static MeshData m_box;
    float m_sumOfBoxWidth;
    float m_maxOfBoxHeight;
};

