#pragma once
class GraphicsPSO
{
public:
	GraphicsPSO(D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GraphicsPSO& operator = (const GraphicsPSO& other);

public:
	void SetVS(ComPtr<ID3D11VertexShader>& vs) { m_vs = vs; }
	void SetPS(ComPtr<ID3D11PixelShader>& ps) { m_ps = ps; }
	void SetCS(ComPtr<ID3D11ComputeShader>& cs) { m_cs = cs; }
	void SetInputLayout(ComPtr<ID3D11InputLayout>& inputLayout) { m_inputLayout = inputLayout; }
	void SetRS(ComPtr<ID3D11RasterizerState>& rs) { m_rs = rs; }
	void SetDSS(ComPtr<ID3D11DepthStencilState>& dss) { m_dss = dss; }
	void SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology) { m_primitiveTopology = topology; }

	ComPtr<ID3D11VertexShader>& GetVS() { return m_vs; }
	ComPtr<ID3D11PixelShader>& GetPS() { return m_ps; }
	ComPtr<ID3D11ComputeShader>& GetCS() { return m_cs; }
	ComPtr<ID3D11InputLayout>& GetInputLayout() { return m_inputLayout; }
	ComPtr<ID3D11RasterizerState>& GetRS() { return m_rs; }
	ComPtr<ID3D11DepthStencilState>& GetDSS() { return m_dss; }
	D3D11_PRIMITIVE_TOPOLOGY GetTopology() { return m_primitiveTopology; }

	static UINT m_stencilRef;

private:
	ComPtr<ID3D11InputLayout> m_inputLayout;
	ComPtr<ID3D11VertexShader> m_vs;
	ComPtr<ID3D11PixelShader> m_ps;
	ComPtr<ID3D11ComputeShader> m_cs;
	ComPtr<ID3D11RasterizerState> m_rs;
	ComPtr<ID3D11DepthStencilState> m_dss;

	D3D11_PRIMITIVE_TOPOLOGY m_primitiveTopology;
};

