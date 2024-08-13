#include "pch.h"
#include "GraphicsPSO.h"
UINT GraphicsPSO::m_stencilRef = 0;
GraphicsPSO::GraphicsPSO(D3D11_PRIMITIVE_TOPOLOGY topology)
	: m_primitiveTopology(topology)
{
}

GraphicsPSO& GraphicsPSO::operator=(const GraphicsPSO& other)
{
	m_inputLayout = other.m_inputLayout;
	m_vs = other.m_vs;
	m_cs = other.m_cs;
	m_ps = other.m_ps;
	m_rs = other.m_rs;
	m_dss = other.m_dss;
	m_primitiveTopology = other.m_primitiveTopology;
	return *this;
}
