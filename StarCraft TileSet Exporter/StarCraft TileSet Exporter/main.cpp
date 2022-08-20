#include "Global.h"
//#include "Func.h"
#include "png.h"

const uint32_t width = 32;
const uint32_t height = 32;
const uint32_t delay = 10;

int main()
{
	PngWriter p;
	std::string rgba;
	rgba += 0xff;
	rgba += '\0';
	rgba += '\0';
	rgba += '\0';

	rgba += '\0';
	rgba += 0xff;
	rgba += '\0';
	rgba += '\0';

	rgba += '\0';
	rgba += '\0';
	rgba += 0xff;
	rgba += '\0';
	PngBegin(&p, "test", 3, 1, E_ColorType::IndexedColor, true);
	// 다른 이름으로 저장
	PngWrite(&p, (uint8_t*)rgba.data(), 3, 1);
	PngEnd(&p);

	/*mkdir(FolderPath);
	mkdir(LoadPath);

	export_megatiles("ashworld",		false,	true);
	export_megatiles("badlands",		false,	true);
	export_megatiles("install",			false,	true);
	export_megatiles("jungle",			false,	true);
	export_megatiles("platform",		false,	true);*/

	return 0;
}