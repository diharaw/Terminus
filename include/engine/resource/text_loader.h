#pragma once

#ifndef TEXTLOADER_H
#define TEXTLOADER_H

#include <Resource/asset_common.h>
#include <types.h>

namespace terminus
{
	namespace text_loader
	{
		extern asset_common::TextLoadData* load(String id);
	};
}

#endif
