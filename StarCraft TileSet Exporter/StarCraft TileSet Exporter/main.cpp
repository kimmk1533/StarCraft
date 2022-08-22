#include "Global.h"
#include "Func.h"
#include "lodepng.h"

const uint32_t g_width = 32;
const uint32_t g_height = 32;
const uint32_t g_delay = 10;

int main()
{
	mkdir(FolderPath);
	mkdir(LoadPath);

	bool make_gif = true;
	bool make_png = true;

	export_megatiles("ashworld", make_gif, make_png);
	export_megatiles("badlands", make_gif, make_png);
	export_megatiles("install" , make_gif, make_png);
	export_megatiles("jungle"  , make_gif, make_png);
	export_megatiles("platform", make_gif, make_png);

	return 0;
}