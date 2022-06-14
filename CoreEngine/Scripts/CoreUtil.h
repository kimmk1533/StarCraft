#pragma once

#define SAFE_DELETE(p)				{ if(p != nullptr) { delete (p);     (p) = nullptr; } }
#define SAFE_DELETE_ARRAY(p)		{ if(p != nullptr) { delete[] (p);   (p) = nullptr; } }
#define SAFE_RELEASE(p)				{ if(p != nullptr) { (p)->Release(); (p) = nullptr; } }

#define SAFE_CREATE(p)		{ if (FAILED((p)->Create())) return E_FAIL; }
#define SAFE_UPDATE(p)		{ if (FAILED((p)->Update(_deltaTime))) return E_FAIL; }
#define SAFE_RENDER(p)		{ if (FAILED((p)->Render())) return E_FAIL; }