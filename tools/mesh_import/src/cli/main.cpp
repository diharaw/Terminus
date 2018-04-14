#include <tsm_exporter.h>
#include <filesystem.h>
#include <stdio.h>

int main(int argc, char * argv[])
{
	if (argc == 1)
	{
		printf("usage: TSM_CLI <filename>");
		return 1;
	}
	else
	{
		tsm_exporter::export_mesh(argv[1], "out");
	}

	return 0;
}
