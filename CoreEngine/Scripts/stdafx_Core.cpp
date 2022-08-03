#include "stdafx_Core.h"

D3DXVECTOR2 operator+(const D3DXVECTOR2& _lhs, const D3DXVECTOR3& _rhs)
{
	return D3DXVECTOR2(_lhs.x + _rhs.x, _lhs.y + _rhs.y);
}
D3DXVECTOR3 operator+(const D3DXVECTOR3& _lhs, const D3DXVECTOR2& _rhs)
{
	return D3DXVECTOR3(_lhs.x + _rhs.x, _lhs.y + _rhs.y, _lhs.z);
}

D3DXVECTOR2 operator-(const D3DXVECTOR2& _lhs, const D3DXVECTOR3& _rhs)
{
	return D3DXVECTOR2(_lhs.x - _rhs.x, _lhs.y - _rhs.y);
}
D3DXVECTOR3 operator-(const D3DXVECTOR3& _lhs, const D3DXVECTOR2& _rhs)
{
	return D3DXVECTOR3(_lhs.x - _rhs.x, _lhs.y - _rhs.y, _lhs.z);
}

D3DXVECTOR2& operator+=(D3DXVECTOR2& _lhs, const D3DXVECTOR3& _rhs)
{
	_lhs.x += _rhs.x;
	_lhs.y += _rhs.y;

	return _lhs;
}
D3DXVECTOR3& operator+=(D3DXVECTOR3& _lhs, const D3DXVECTOR2& _rhs)
{
	_lhs.x += _rhs.x;
	_lhs.y += _rhs.y;

	return _lhs;
}

D3DXVECTOR2& operator-=(D3DXVECTOR2& _lhs, const D3DXVECTOR3& _rhs)
{
	_lhs.x -= _rhs.x;
	_lhs.y -= _rhs.y;

	return _lhs;
}
D3DXVECTOR3& operator-=(D3DXVECTOR3& _lhs, const D3DXVECTOR2& _rhs)
{
	_lhs.x -= _rhs.x;
	_lhs.y -= _rhs.y;

	return _lhs;
}

std::ostream& operator<<(std::ostream& _out, const D3DXVECTOR2& _vector)
{
	_out << "(" << _vector.x << ", " << _vector.y << ")";

	return _out;
}
std::ostream& operator<<(std::ostream& _out, const D3DXVECTOR3& _vector)
{
	_out << "(" << _vector.x << ", " << _vector.y << ", " << _vector.z << ")";

	return _out;
}