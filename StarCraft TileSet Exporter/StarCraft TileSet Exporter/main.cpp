#include "Global.h"
#include "Func.h"
#include "lodepng.h"

int main()
{
	mkdir(FolderPath);
	mkdir(LoadPath);

	ImageOption imageOption;

	imageOption.option = 0;
	//imageOption.option |= Option::make_gif;
	imageOption.option |= Option::make_png;
	imageOption.option |= Option::gif_sheet;
	imageOption.option |= Option::png_sheet;

	imageOption.width = 32;
	imageOption.height = 32;

	imageOption.auto_sheet = true;
	imageOption.sheet_width = 128;
	imageOption.sheet_height = 128;

	imageOption.gifOption.delay = 25;

	export_megatiles("ashworld", imageOption);
	export_megatiles("badlands", imageOption);
	export_megatiles("install" , imageOption);
	export_megatiles("jungle"  , imageOption);
	export_megatiles("platform", imageOption);

	return 0;
}