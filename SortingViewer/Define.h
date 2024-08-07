#pragma once
#define CHECKRESULT(result) if(FAILED(result)) assert(0);

#define VIEW_SRV 1
#define VIEW_RTV 2 
#define VIEW_UAV 4
#define VIEW_DSV 8

#define KEYCHECK(type,state) KeyMgr::KeyCheck(KEY_TYPE::type, KEY_STATE::state) 
#define MOUSEPOS() KeyMgr::GetMousePos();