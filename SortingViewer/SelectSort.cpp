#include "pch.h"
#include "SelectSort.h"

void Print(vector<shared_ptr<Mesh>>& vec)
{
	for (int i = 0; i < vec.size(); ++i)
	{
		cout << "Pos, X : " << vec[i]->GetTrans().x << " Y : " <<
			vec[i]->GetTrans().y << " Z : " << vec[i]->GetTrans().z << endl;
		cout << "Scale, X : " << vec[i]->GetScale().x << " Y : " <<
			vec[i]->GetScale().y << " Z : " << vec[i]->GetScale().z << endl;
	}
}
void SelectSort::StartSort(vector<shared_ptr<Mesh>>& vec)
{
	for (int i = 0; i < vec.size() - 1; ++i)
	{
		int minIdx = i;
		for (int j = i + 1; j < vec.size(); ++j)
		{
			if (vec[j]->GetScale().y < vec[minIdx]->GetScale().y)
				minIdx = j;
		}
		const float tempY = vec[i]->GetTrans().y;
		const float tempHeight = vec[i]->GetScale().y;
		vec[i]->GetTrans().y = vec[minIdx]->GetTrans().y;
		vec[minIdx]->GetTrans().y = tempY;
		vec[i]->GetScale().y = vec[minIdx]->GetScale().y;
		vec[minIdx]->GetScale().y = tempHeight;
	}
}
