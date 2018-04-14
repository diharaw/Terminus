#pragma once

#include "types.h"

struct AssimpImportData;

namespace tsm_exporter
{
    extern void export_mesh(AssimpImportData* data, const char* output);
	extern void export_mesh(const char* file, const char* output);
}
