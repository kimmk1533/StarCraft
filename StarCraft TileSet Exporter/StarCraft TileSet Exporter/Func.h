#pragma once
#include "Global.h"

// 참고: https://blog.naver.com/PostView.naver?blogId=cjdeka3123&logNo=220845487707&parentCategoryNo=&categoryNo=1&viewDate=&isShowPopularPosts=false&from=postList
void export_megatiles(const std::string& _name, const bool& _make_gif, const bool& _make_png);

bool mkdir(const std::string& _path);
std::string operator + (const std::string& _lhs, const size_t& _rhs);

extern const uint32_t width;
extern const uint32_t height;
extern const uint32_t delay;
