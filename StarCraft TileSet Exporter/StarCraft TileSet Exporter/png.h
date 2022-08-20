#pragma once
#include <stdio.h>   // for FILE*
#include <string>
#include "zlib.h"
#pragma comment(lib, "zlib.lib")

#pragma region CRC

// CRC 참고:	https://joseoyeon.github.io/posts/png-%EA%B5%AC%EC%A1%B0/
//			https://www.w3.org/TR/PNG/#11PLTE

/* Table of CRCs of all 8-bit messages. */
unsigned long crc_table[256];

/* Flag: has the table been computed? Initially false. */
bool crc_table_computed = false;

/* Make the table for a fast CRC. */
void make_crc_table(void)
{
	unsigned long c;
	int n, k;

	for (n = 0; n < 256; n++) {
		c = (unsigned long)n;
		for (k = 0; k < 8; k++) {
			if (c & 1)
				c = 0xedb88320L ^ (c >> 1);
			else
				c = c >> 1;
		}
		crc_table[n] = c;
	}
	crc_table_computed = true;
}

/* Update a running CRC with the bytes buf[0..len-1]--the CRC
   should be initialized to all 1's, and the transmitted value
   is the 1's complement of the final running CRC (see the
   crc() routine below). */
unsigned long update_crc(unsigned long crc, unsigned char* buf, int len)
{
	unsigned long c = crc;
	int n;

	if (!crc_table_computed)
		make_crc_table();
	for (n = 0; n < len; n++) {
		c = crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
	}
	return c;
}

/* Return the CRC of the bytes buf[0..len-1]. */
unsigned long crc(unsigned char* buf, int len)
{
	return update_crc(0xffffffffL, buf, len) ^ 0xffffffffL;
}

#pragma endregion

enum class E_ColorType : uint8_t
{
	GrayScale = 0,
	TrueColor_RGB = 2,
	IndexedColor = 3,
	GrayScaleA = 4,
	TrueColor_RGBA = 6
};

typedef struct RGB
{
	uint8_t r;
	uint8_t g;
	uint8_t b;

	bool operator < (const RGB& _other) const
	{
		if (this->r == _other.r)
		{
			if (this->g == _other.g)
			{
				return this->b < _other.b;
			}

			return this->g < _other.b;
		}

		return this->r < _other.r;
	}
	bool operator == (const RGB& _other) const
	{
		return this->r == _other.r && this->g == _other.g && this->b == _other.b;
	}
} rgb;

namespace std
{
	template <>
	struct hash<RGB>
	{
		size_t operator()(const RGB& _rgb) const
		{
			return (_rgb.r << 8) ^ (_rgb.g << 4) ^ _rgb.b;
		}
	};
}

enum class E_FilterType : uint8_t
{
	None = 0,
	Sub = 1,
	Up = 2,
	Average = 3,
	Paeth = 4
};

typedef struct
{
	FILE* f;
	E_ColorType colorType;
	bool usePLTE; // ColorType이 2 or 6 인 경우에만 사용
	std::unordered_map<RGB, uint8_t> rgb_index;
} PngWriter;

uint8_t** PngMakeScanline(PngWriter* _writer, const uint8_t* _image, const uint32_t& _width, const uint32_t& _height)
{
	if (!_writer->f) return nullptr;

	uint8_t** scanline = new uint8_t * [_height];
	for (size_t i = 0; i < _height; ++i)
	{
		switch (_writer->colorType)
		{
		case E_ColorType::GrayScale:
			break;
		case E_ColorType::TrueColor_RGB:
			break;
		case E_ColorType::IndexedColor:
			scanline[i] = new uint8_t[_width + 1];
			scanline[i][0] = (uint8_t)(E_FilterType::None);
			break;
		case E_ColorType::GrayScaleA:
			break;
		case E_ColorType::TrueColor_RGBA:
			break;
		}
	}

	for (size_t y = 0; y < _height; ++y)
	{
		switch (_writer->colorType)
		{
		case E_ColorType::GrayScale:
			break;
		case E_ColorType::TrueColor_RGB:
			break;
		case E_ColorType::IndexedColor:
			for (size_t x = 0; x < _width; ++x)
			{
				size_t index = (y * _width + x) * 4;
				RGB rgb;

				rgb.r = _image[index + 0];
				rgb.g = _image[index + 1];
				rgb.b = _image[index + 2];

				scanline[y][x + 1] = _writer->rgb_index[rgb];
			}
			break;
		case E_ColorType::GrayScaleA:
			break;
		case E_ColorType::TrueColor_RGBA:
			break;
		}
	}

	return scanline;
}

uint8_t PngPaethPredictor(const uint8_t& _a, const uint8_t& _b, const uint8_t& _c)
{
	uint16_t Pr;

	uint16_t p = _a + _b - _c;
	uint16_t pa = abs(p - _a);
	uint16_t pb = abs(p - _b);
	uint16_t pc = abs(p - _c);

	if (pa <= pb && pa <= pc)
		Pr = _a;
	else if (pb <= pc)
		Pr = _b;
	else
		Pr = _c;

	return Pr;
}
uint8_t PngFiltering_Byte(const E_FilterType& _filterType, const uint8_t& _x, const uint8_t& _a, const uint8_t& _b, const uint8_t& _c)
{
	uint8_t result = 0;

	switch (_filterType)
	{
	case E_FilterType::None:
		result = _x;
		break;
	case E_FilterType::Sub:
		result = _x - _a;
		break;
	case E_FilterType::Up:
		result = _x - _b;
		break;
	case E_FilterType::Average:
		result = _x - (uint8_t)((uint16_t)floor(_a + _b) >> 1);
		break;
	case E_FilterType::Paeth:
		result = _x - PngPaethPredictor(_a, _b, _c);
		break;
	}

	return result;
}
bool PngFiltering_Scanline(PngWriter* _writer, uint8_t** _scanline, const uint32_t& _width, const uint32_t& _height)
{
	if (_writer->f == nullptr) return false;

	for (size_t y = 0; y < _height; ++y)
	{
		E_FilterType type = (E_FilterType)_scanline[y][0];

		switch (_writer->colorType)
		{
		case E_ColorType::GrayScale:
			break;
		case E_ColorType::TrueColor_RGB:
			break;
		case E_ColorType::IndexedColor:
			for (size_t x = 1; x < _width * 2 + 1; ++x)
			{
				size_t up = y - 1;
				size_t left = x - 1;

				uint8_t _x = _scanline[y][x];
				uint8_t _a = (x == 1) ? 0 : _scanline[y][left];
				uint8_t _b = (y == 0) ? 0 : _scanline[up][x];
				uint8_t _c = (x == 1 || y == 0) ? 0 : _scanline[up][left];

				_scanline[y][x] = PngFiltering_Byte(type, _x, _a, _b, _c);
			}
			break;
		case E_ColorType::GrayScaleA:
			break;
		case E_ColorType::TrueColor_RGBA:
			break;
		}

		return true;
	}
}

bool PngPLTEchunk(PngWriter* _writer, const uint8_t* _image, const uint32_t& _width, const uint32_t& _height)
{
	if (!_writer->f) return false;

	switch (_writer->colorType)
	{
	case E_ColorType::GrayScale:
	case E_ColorType::GrayScaleA:
		return false;
	}

	uint32_t size = 0;
	std::string length, data;

	data += "PLTE";		// Chunk Type

	uint16_t index = 0;
	uint32_t numPixels = _width * _height;

	for (size_t i = 0; i < numPixels; ++i)
	{
		RGB rgb;

		rgb.r = _image[i * 4 + 0];
		rgb.g = _image[i * 4 + 1];
		rgb.b = _image[i * 4 + 2];

		if (_writer->rgb_index.find(rgb) == _writer->rgb_index.end())
		{
			_writer->rgb_index[rgb] = index++;

			data += rgb.r;
			data += rgb.g;
			data += rgb.b;
		}
	}

	if (_writer->rgb_index.size() > 256)
		std::cerr << "PngPLTEchunk: 팔레트 항목이 256개를 넘습니다.\n";

	// PLTE Chunk
	size = 3 * _writer->rgb_index.size();	// length = 3 * 팔레트 개수 (4 bytes)
	length += size & 0xff000000 >> 24;
	length += size & 0x00ff0000 >> 16;
	length += size & 0x0000ff00 >> 8;
	length += size & 0x000000ff >> 0;

#pragma region CRC
	unsigned long crc_value = crc((unsigned char*)(data.c_str()), data.size());	// CRC
	data += (crc_value & 0xff000000) >> 24;
	data += (crc_value & 0x00ff0000) >> 16;
	data += (crc_value & 0x0000ff00) >> 8;
	data += (crc_value & 0x000000ff) >> 0;
#pragma endregion

	fputs((length + data).c_str(), _writer->f);

	return true;
}

bool PngIDATchunk(PngWriter* _writer, const uint8_t* _image, const uint32_t& _width, const uint32_t& _height)
{
	if (!_writer->f) return false;

	uint8_t** scanline = PngMakeScanline(_writer, _image, _width, _height);

	PngFiltering_Scanline(_writer, scanline, _width, _height);

	std::string length;
	std::vector<uint8_t> data;

	// IDAT Chunk
	data.push_back('I');
	data.push_back('D');
	data.push_back('A');
	data.push_back('T');		// Chunk Type

#pragma region zlib

	std::vector<uint8_t> zlib_buffer, zlib_data;

	zlib_data.push_back(0x00);
	zlib_data.push_back(0x08);		// Compression Method (2 bytes)

#pragma region deflate
	std::vector<uint8_t> deflate_data;

	deflate_data.push_back(0x01);	// Last Block Flag (1: last block) (1 byte)

	uLong size;
	switch (_writer->colorType)
	{
	case E_ColorType::IndexedColor:
		size = _writer->rgb_index.size();
		break;
	case E_ColorType::TrueColor_RGB:
		size = _height * _width;
		break;
	}

	deflate_data.push_back((size & 0x00ff) >> 0);
	deflate_data.push_back((size & 0xff00) >> 8);	// little-endian block size	(2 bytes)

	deflate_data.push_back((~size & 0x00ff) >> 0);
	deflate_data.push_back((~size & 0xff00) >> 8);	// little-endian block size NOT	(2 bytes)

	for (size_t y = 0; y < _height; ++y)
	{
		for (size_t x = 0; x < _width + 1; ++x)
		{
			deflate_data.push_back(scanline[y][x]);
		}
	}

	// deflate
	uLong uCompSize = deflate_data.size() + 1;
	uLong CompSize = compressBound(uCompSize);
	char* deflate_buffer = new char[CompSize];
	compress((Bytef*)deflate_buffer, &CompSize, (Bytef*)deflate_data.data(), uCompSize);

	zlib_data.insert(zlib_data.end(), deflate_buffer, deflate_buffer + CompSize);

	delete[] deflate_buffer;

#pragma endregion

#pragma region ZCRC
	unsigned long zcrc_value = crc((unsigned char*)(zlib_data.data()), zlib_data.size());	// CRC
	zlib_data.push_back((zcrc_value & 0xff000000) >> 24);
	zlib_data.push_back((zcrc_value & 0x00ff0000) >> 16);
	zlib_data.push_back((zcrc_value & 0x0000ff00) >> 8);
	zlib_data.push_back((zcrc_value & 0x000000ff) >> 0);

	auto adler = adler32(0, nullptr, 0);
	auto adler2 = adler32(adler, zlib_data.data(), zlib_data.size());
#pragma endregion

	/*CompSize = compressBound(zlib_data.size());

	compress((Bytef*)zlib_buffer.data(), &CompSize, (Byte*)zlib_data.data(), zlib_data.size() + 1);*/

	data.insert(data.end(), zlib_data.begin(), zlib_data.end());

#pragma endregion

#pragma region CRC
	unsigned long crc_value = crc((unsigned char*)(data.data()), data.size());	// CRC

	data.push_back((crc_value & 0xff000000) >> 24);
	data.push_back((crc_value & 0x00ff0000) >> 16);
	data.push_back((crc_value & 0x0000ff00) >> 8);
	data.push_back((crc_value & 0x000000ff) >> 0);
#pragma endregion

	size = zlib_buffer.size();

	length += (size & 0xff000000) >> 24;
	length += (size & 0x00ff0000) >> 16;
	length += (size & 0x0000ff00) >> 8;
	length += (size & 0x000000ff) >> 0;	// length (4 bytes)

	fputs((length + (char*)data.data()).c_str(), _writer->f);

	for (uint32_t i = 0; i < _height; ++i)
	{
		delete[] scanline[i];
	}
	delete[] scanline;

	return true;
}

bool PngBegin(PngWriter* _writer, const std::string& _filename, const uint32_t& _width, const uint32_t& _height, const E_ColorType& _colorType, const bool& _usePLTE, const int32_t& _bitDepth = 8, const bool& _dither = false)
{
	(void)_bitDepth; (void)_dither; // Mute "Unused argument" warnings

	std::string file = _filename;
	if (file.rfind(".png") == std::string::npos)
		file += ".png";

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	_writer->f = 0;
	fopen_s(&_writer->f, file.c_str(), "wb");
#else
	_writer->f = fopen(file.c_str(), "wb");
#endif
	if (!_writer->f) return false;

	// allocate
	//_writer->oldImage = (uint8_t*)GIF_MALLOC(width * height * 4);
	_writer->colorType = _colorType;
	_writer->usePLTE = _usePLTE;

	std::string signature, length, data;

	// file signature
	signature += 0x89;
	signature += 0x50;	// P
	signature += 0x4E;	// N
	signature += 0x47;	// G
	signature += 0x0D;
	signature += 0x0A;
	signature += 0x1A;
	signature += 0x0A;

	// IHDR Chunk
	length += '\0';
	length += '\0';
	length += '\0';
	length += 0x0D;		// length 13 (4 bytes)

	data += "IHDR";		// Chunk Type (4 bytes)

	data += (_width & 0xff000000) >> 24;
	data += (_width & 0x00ff0000) >> 16;
	data += (_width & 0x0000ff00) >> 8;
	data += (_width & 0x000000ff) >> 0;		// width  (4 bytes)

	data += (_height & 0xff000000) >> 24;
	data += (_height & 0x00ff0000) >> 16;
	data += (_height & 0x0000ff00) >> 8;
	data += (_height & 0x000000ff) >> 0;	// height (4 bytes)

	data += _bitDepth;				//	bit depth  (1 byte)
	data += (uint8_t)_colorType;	//	color type (1 byte)		( 0: GrayScale, 2: TrueColor, 3: Indexed-Color,
									//							  4: GrayScale with alpha,	  6: TrueColor with alpha )

	data += '\0';					//  compression method (1 byte)
	data += '\0';					//	filter		method (1 byte)
	data += '\0';					//	interlace	method (1 byte)

	unsigned long crc_value = crc((unsigned char*)(data.c_str()), data.size());	// CRC
	data += (crc_value & 0xff000000) >> 24;
	data += (crc_value & 0x00ff0000) >> 16;
	data += (crc_value & 0x0000ff00) >> 8;
	data += (crc_value & 0x000000ff) >> 0;

	fputs((signature + length + data).c_str(), _writer->f);

	return true;
}

bool PngWrite(PngWriter* _writer, const uint8_t* _image, uint32_t _width, uint32_t _height, int _bitDepth = 8, bool _dither = false)
{
	if (!_writer->f) return false;

	if (_writer->colorType == E_ColorType::IndexedColor)
		PngPLTEchunk(_writer, _image, _width, _height);

	PngIDATchunk(_writer, _image, _width, _height);

	return true;
}

bool PngEnd(PngWriter* _writer)
{
	if (!_writer->f) return false;

	std::string length, data;

	// IEND Chunk
	length += '\0';
	length += '\0';
	length += '\0';
	length += '\0';		// length 0 (4 bytes)

	data += "IEND";		// Chunk Type

	unsigned long crc_value = crc((unsigned char*)(data.c_str()), data.size());	// CRC
	data += (crc_value & 0xff000000) >> 24;
	data += (crc_value & 0x00ff0000) >> 16;
	data += (crc_value & 0x0000ff00) >> 8;
	data += (crc_value & 0x000000ff) >> 0;

	fputs((length + data).c_str(), _writer->f);

	fclose(_writer->f);

	_writer->f = NULL;

	return true;
}