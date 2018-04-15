#pragma once

#define TE_KILOBYTE(x) x * 1024
#define TE_MEGABYTE(x) x * TE_KILOBYTE(1024)
#define TE_GIGABYTE(x) x * TE_MEGABYTE(1024)
#define TE_NEW(allocator) new(allocator, __LINE__, __FILE__)
#define TE_DELETE(object, allocator) custom_delete(object, allocator, __LINE__, __FILE__)