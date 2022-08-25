#pragma once
#include "Global.h"

enum class Option : uint8_t
{
	None = 0,

	make_gif = 1 << 0,
	gif_one = 1 << 1,
	gif_all = 1 << 2,
	gif_sheet = 1 << 3,

	make_png = 1 << 4,
	png_one = 1 << 5,
	png_sheet = 1 << 6,

	All = -1
};

struct GifOption
{
	uint32_t delay;
};

struct PngOption
{

};

struct ImageOption
{
	uint8_t option;

	GifOption gifOption;
	PngOption pngOption;

	uint32_t width;
	uint32_t height;

	bool auto_sheet;
	uint32_t sheet_width;
	uint32_t sheet_height;
};

bool operator&(const uint8_t& _lhs, const Option& _rhs);
bool operator|=(uint8_t& _lhs, const Option& _rhs);

// 참고: https://blog.naver.com/PostView.naver?blogId=cjdeka3123&logNo=220845487707&parentCategoryNo=&categoryNo=1&viewDate=&isShowPopularPosts=false&from=postList
void export_megatiles(const std::string& _name, const ImageOption& _imageOption);

bool mkdir(const std::string& _path);
std::string operator + (const std::string& _lhs, const size_t& _rhs);