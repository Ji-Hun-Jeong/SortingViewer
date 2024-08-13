#include "pch.h"
#include "BoxElementGenerator.h"
#include "GeometryGenerator.h"

MeshData BoxElementGenerator::m_box;
BoxElementGenerator::BoxElementGenerator()
	: IElementGenerator()
	, m_sumOfBoxWidth(0)
	, m_maxOfBoxHeight(0)
{
	m_box = GeometryGenerator::MakeBox();
}

void BoxElementGenerator::GlobalConstSetData(GlobalConst& globalConst)
{
	globalConst.maxOfBoxHeight = m_sumOfBoxWidth;
	globalConst.sumOfBoxWidth = m_maxOfBoxHeight;
}
