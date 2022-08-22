#include "Func.h"
#include "gif.h"
#include "lodepng.h"
#include <iomanip>

// 참고: https://blog.naver.com/PostView.naver?blogId=cjdeka3123&logNo=220845487707&parentCategoryNo=&categoryNo=1&viewDate=&isShowPopularPosts=false&from=postList
void export_megatiles(const std::string& _name, const bool& _make_gif, const bool& _make_png)
{
	if (_make_gif == false && _make_png == false)
		return;

	size_t total_megatiles = 0;
	int length = 0;
	std::vector<uint16_t> vx4_array;
#ifdef USE_1DIMENSION_VECTOR
	std::vector<uint8_t> vr4_array;
#else
	std::vector<std::vector<uint8_t>> vr4_array;
#endif // USE_1DIMENSION_VECTOR
	std::vector<uint8_t> wpe_array;

#pragma region VX4 (mega tile pool)
	// read VX4 file (maga tile pool)
	// size = 32 byte(unit struct size) * total mega tile number
	// each unit struct has 16 mini tile info(2 byte)

	std::cout << _name << ": [ Initialize VX4 ]\n";
	std::ifstream fin_vx4;

	std::cout << _name << ": " << _name << ".vx4 file Opening...\n";
	fin_vx4.open(LoadPath + _name + ".vx4", std::ifstream::binary);
	if (!fin_vx4)
	{
		std::cerr << _name << ".vx4 : " << "Opening file error\n";
		fin_vx4.close();
		return;
	}

	std::cout << _name << ": " << "Calculate " << _name << ".vx4 file Size\n";
	// 파일 크기 구하기
	fin_vx4.seekg(0, fin_vx4.end);
	length = (int)fin_vx4.tellg();
	fin_vx4.seekg(0, fin_vx4.beg);
	std::cout << _name << ": " << _name << ".vx4 file Size = " << length << " (bytes)\n";

	std::cout << _name << ": " << _name << ".vx4 file Reading...\n";
	// 전체 파일 읽기
	uint16_t* vx4_buffer = new uint16_t[length];
	fin_vx4.read((char*)vx4_buffer, length);

	std::cout << _name << ": " << "Data Initializing...\n";
	// mini tile info 하나가 2 byte 이므로 2 byte 씩 읽기
	size_t size = length * 0.5f;
	for (size_t i = 0; i < size; ++i)
	{
		vx4_array.push_back(vx4_buffer[i] /*vx4_buffer[i * 2 + 1] << 4 ^ vx4_buffer[i * 2]*/);
	}

	// 2 byte 씩 읽었으므로 / 32 가 아닌 / 16 임
	total_megatiles = size >> 4;
	std::cout << _name + ": Total " << total_megatiles << " megatiles\n";

	// 할당 해제
	delete[] vx4_buffer;
	fin_vx4.close();

	std::cout << _name << ": " << "[ VX4 Completed! ]\n\n";
#pragma endregion

#pragma region VR4 (mini tile pool)
	// read VR4 file (mini tile pool)
	// size = 64 byte(unit struct size) * total mini tile number
	// each unit struct has 64 wpe index(1 byte)

	std::cout << _name << ": [ Initialize VR4 ]\n";
	std::ifstream fin_vr4;

	std::cout << _name << ": " << _name << ".vr4 file Opening...\n";
	fin_vr4.open(LoadPath + _name + ".vr4", std::ifstream::binary);
	if (!fin_vr4)
	{
		std::cerr << _name << ".vr4 : " << "Opening file error\n";
		fin_vr4.close();
		return;
	}

	std::cout << _name << ": " << "Calculate " << _name << ".vr4 file Size\n";
	// 파일 크기 구하기
	fin_vr4.seekg(0, fin_vr4.end);
	length = (int)fin_vr4.tellg();
	fin_vr4.seekg(0, fin_vr4.beg);
	std::cout << _name << ": " << _name << ".vr4 file Size = " << length << " (bytes)\n";

	std::cout << _name << ": " << _name << ".vr4 file Reading...\n";
	// 전체 파일 읽기 ( wpe index 하나가 1 byte 이므로 1 byte 씩 읽음 -> 그냥 넣으면 됨 )
	char* vr4_tmparr = new char[length];
	fin_vr4.read(vr4_tmparr, length);
#ifdef USE_1DIMENSION_VECTOR
	for (int i = 0; i < length; ++i)
	{
		vr4_array.push_back(vr4_tmparr[i]);
	}
#else
	size = length >> 6; // / 64
	for (int i = 0; i < size; ++i)
	{
		for (int y = 0; y < 8; ++y)
		{
			vr4_array.push_back(std::vector<uint8_t>());

			int offset_y = (i * 8);
			int offset_x = (i * 64) + (y * 8);

			for (int x = 0; x < 7; ++x)
			{
				vr4_array[offset_y + y].push_back(vr4_tmparr[offset_x + x]);
#ifdef DEBUG
				std::cout << std::showpos << std::setw(4) << std::right << (vr4_tmparr[offset_x + x] == '\0' ? 0 : vr4_tmparr[offset_x + x]) << ", ";
#endif // DEBUG
			}
			vr4_array[offset_y + y].push_back(vr4_tmparr[offset_x + 7]);
#ifdef DEBUG
			std::cout << std::showpos << std::setw(4) << std::right << (vr4_tmparr[offset_x + 7] == '\0' ? 0 : vr4_tmparr[offset_x + 7]) << "\n";
#endif
		}
#ifdef DEBUG
		std::cout << "\n";
#endif
	}
#endif
	delete[] vr4_tmparr;

	fin_vr4.close();

	std::cout << _name << ": " << "[ VR4 Completed! ]\n\n";
#pragma endregion

#pragma region WPE (rgb color pool)
	// read WPE file (rgb color pool)
	// size = 4 byte(unit struct size) * total color number(=256)
	// each unit struct -> r, g, b, null (1 byte each)

	std::cout << _name << ": [ Initialize WPE ]\n";
	std::ifstream fin_wpe;

	std::cout << _name << ": " << _name << ".wpe file Opening...\n";
	fin_wpe.open(LoadPath + _name + ".wpe", std::ifstream::binary);
	if (!fin_wpe)
	{
		std::cerr << _name << ".wpe : " << "Opening file error\n";
		fin_wpe.close();
		return;
	}

	std::cout << _name << ": " << "Calculate " << _name << ".wpe file Size\n";
	// 파일 크기 구하기
	fin_wpe.seekg(0, fin_wpe.end);
	length = (int)fin_wpe.tellg();
	fin_wpe.seekg(0, fin_wpe.beg);
	std::cout << _name << ": " << _name << ".wpe file Size = " << length << " (bytes)\n";

	std::cout << _name << ": " << _name << ".wpe file Reading...\n";
	// 전체 파일 읽기 ( r, g, b 하나가 1 byte 이므로 1 byte 씩 읽음 -> 그냥 넣으면 됨 )
	char* wpe_tmparr = new char[length];
	fin_wpe.read(wpe_tmparr, length);
	for (int i = 0; i < length; ++i)
	{
		wpe_array.push_back(wpe_tmparr[i]);
	}
	delete[] wpe_tmparr;

	fin_wpe.close();

	std::cout << _name << ": " << "[ WPE Completed! ]\n\n";
#pragma endregion

	// 퍼센트
	int percentage = 0;
	int last_percentage = 0;

	std::vector<uint8_t> rgba;

	// mega tile 이미지 저장할 변수
	GifWriter g, g_all;

	const std::string savePath(SavePath + _name);

	mkdir(savePath);
	if (_make_gif == true)
	{
		mkdir(savePath + "/gif");

		GifBegin(&g_all, savePath + "/" + _name, g_width, g_height, g_delay);
	}
	if (_make_png == true)
	{
		mkdir(savePath + "/png");
	}

	std::cout << _name << ": " << "Exporting Images...\n\n";

	// 모든 mega tile을 추출
	for (size_t mega_tile_index = 0; mega_tile_index < total_megatiles; ++mega_tile_index)
	{
		// 퍼센트 계산
		percentage = (float)(mega_tile_index + 1) / total_megatiles * 20;
		if (percentage != last_percentage)
			std::cout << _name << ": " << percentage * 5 << "% completed\n";
		last_percentage = percentage;

		// image initialize
		for (size_t i = 0; i < 4096; ++i)
		{
			rgba.push_back(0);
		}

		// one mega tile -> 4 * 4 mini tiles
		for (size_t y = 0; y < 4; ++y)
		{
			for (size_t x = 0; x < 4; ++x)
			{
				uint32_t draw_offset_x = x * 8;
				uint32_t draw_offset_y = y * 8;

				uint16_t mini_tile_info = vx4_array[(mega_tile_index << 4) ^ (y << 2) ^ x];

				// 첫 1비트는 반전 여부, 나머지 15비트가 인덱스 이므로 첫 1비트는 버림
				uint16_t mini_tile_index = (mini_tile_info >> 1);
				// 첫 1비트(반전) 가져오기
				bool is_flipped = (int)(mini_tile_info & 1) == 1;

				// one mini tile -> 8 * 8 wpe indices
				for (size_t j = 0; j < 8; ++j)
				{
					for (size_t i = 0; i < 8; ++i)
					{
						uint32_t draw_x = draw_offset_x + (is_flipped ? 7 - i : i);
						uint32_t draw_y = draw_offset_y + j;

#ifdef USE_1DIMENSION_VECTOR
						uint8_t wpe_data = vr4_array[(mini_tile_index << 6) ^ (j << 3) ^ i];
#else
						uint8_t wpe_data = vr4_array[(mini_tile_index * 8) + j][i];
#endif

						// 실제 r, g, b 값
						uint8_t r = (uint8_t)(wpe_array[wpe_data << 2 ^ 0]);
						uint8_t g = (uint8_t)(wpe_array[wpe_data << 2 ^ 1]);
						uint8_t b = (uint8_t)(wpe_array[wpe_data << 2 ^ 2]);
						uint8_t a = 0;

						// 현재 mega tile 이미지에 한 픽셀 쓰기
						int rgb_index = (draw_y * 32 + draw_x) * 4;
						rgba[rgb_index + 0] = r;
						rgba[rgb_index + 1] = g;
						rgba[rgb_index + 2] = b;
						rgba[rgb_index + 3] = a;
					}
				}
			}
		}

		if (_make_gif == true)
		{
			GifBegin(&g, savePath + "/gif/" + _name + " (" + mega_tile_index + ")", g_width, g_height, g_delay);
			GifWriteFrame(&g, rgba.data(), g_width, g_height, g_delay);
			GifEnd(&g);

			GifWriteFrame(&g_all, rgba.data(), g_width, g_height, g_delay);
		}

		if (_make_png == true)
		{
			std::vector<uint8_t> ImageBuffer;
			lodepng::encode(ImageBuffer, rgba, g_width, g_height);
			lodepng::save_file(ImageBuffer, savePath + "/png/" + _name + " (" + mega_tile_index + ")");
			//PngWriter p;

			//PngBegin(&p, savePath + "/png/" + _name + " (" + mega_tile_index + ")", width, height, E_ColorType::IndexedColor, true);

			//// 다른 이름으로 저장
			//PngWrite(&p, rgba.data(), width, height);

			//PngEnd(&p);
		}

		rgba.clear();
	}

	if (_make_gif == true)
	{
		GifEnd(&g_all);
	}

	std::cout << "\n";
}

bool mkdir(const std::string& _path)
{
	filesystem::path p(_path);

	if (filesystem::is_directory(p))
		return false;

	return filesystem::create_directories(p);
}
std::string operator + (const std::string& _lhs, const size_t& _rhs)
{
	std::string str = _lhs;

	str += std::to_string(_rhs);

	return str;
}