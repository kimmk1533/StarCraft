#include "stdafx_Core.h"

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