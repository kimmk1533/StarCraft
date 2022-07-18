#pragma once

#define SWAP(a, b) { decltype(a) temp = a; a = b; b = temp; }

#define SAFE_DELETE(p)				{ if(p != nullptr) { delete (p);     (p) = nullptr; } }
#define SAFE_DELETE_ARRAY(p)		{ if(p != nullptr) { delete[] (p);   (p) = nullptr; } }
#define SAFE_RELEASE(p)				{ if(p != nullptr) { (p)->Release(); (p) = nullptr; } }

#define SAFE_CREATE(p)		{ if (FAILED((p)->Create())) return E_FAIL; }
#define SAFE_UPDATE(p)		{ if (FAILED((p)->Update(_deltaTime))) return E_FAIL; }
#define SAFE_RENDER(p)		{ if (FAILED((p)->Render())) return E_FAIL; }

#define ErrorMessageBox(message) MessageBox(NULL, TEXT(message), TEXT("Error!"), MB_OK)

#define NULL_CHECK(p)	{ if (p == nullptr) { return E_FAIL; } }
#define NULL_CHECK_WITH_MSG(p, message)	{ if (p == nullptr) { ErrorMessageBox(message); return E_FAIL; } }

#define FAILED_CHECK(p) { if (FAILED(p)) return E_FAIL; }
#define FAILED_CHECK_WITH_MSG(p, message) { if (FAILED(p)) { ErrorMessageBox(message); return E_FAIL; } }

// 참고: https://gohen.tistory.com/31
#define PROPERTY(t,n)  __declspec( property (put = property__set_##n, get = property__get_##n)) t n; \
	typedef t property__tmp_type_##n
#define READONLY_PROPERTY(t,n) __declspec( property (get = property__get_##n) ) t n;\
	typedef t property__tmp_type_##n
#define WRITEONLY_PROPERTY(t,n) __declspec( property (put = property__set_##n) ) t n;\
	typedef t property__tmp_type_##n
#define GET(n) property__tmp_type_##n property__get_##n()
#define SET(n) void property__set_##n(const property__tmp_type_##n& _value)