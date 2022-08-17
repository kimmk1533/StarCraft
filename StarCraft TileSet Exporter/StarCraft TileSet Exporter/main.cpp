#include "Global.h"
#include "Func.h"

int main()
{
	mkdir(FolderPath);
	mkdir(LoadPath);

	/*export_megatiles("ashworld");
	export_megatiles("badlands");
	export_megatiles("install");
	export_megatiles("jungle");*/
	export_megatiles("space platform");

	return 0;
}