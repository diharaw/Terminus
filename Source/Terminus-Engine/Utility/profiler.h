#pragma once

#include <Core/config.h>

#if defined(TERMINUS_PROFILING)

#include <Utility/Remotery.h>

	#define TERMINUS_BEGIN_CPU_PROFILE(x) rmt_BeginCPUSample(x, 0);
	#define TERMINUS_END_CPU_PROFILE rmt_EndCPUSample();
	#define TERMINUS_PROFILER_INSTANCE Remotery* rmt;
	#define TERMINUS_CREATE_PROFILER rmt_CreateGlobalInstance(&rmt);
	#define TERMINUS_DESTROY_PROFILER rmt_DestroyGlobalInstance(rmt);

	#if defined(TERMINUS_OPENGL)
		#define TERMINUS_BEGIN_GPU_PROFILE(x) rmt_BeginOpenGLSample(x);
		#define TERMINUS_END_GPU_PROFILE rmt_EndOpenGLSample();
	#elif defined(TERMINUS_DIRECT3D11)
		#define TERMINUS_BEGIN_GPU_PROFILE(x) rmt_BeginD3D11Sample(x);
		#define TERMINUS_END_GPU_PROFILE rmt_EndD3D11Sample();
	#endif
#else
	#define TERMINUS_BEGIN_CPU_PROFILE(x)
	#define TERMINUS_END_CPU_PROFILE
	#define TERMINUS_PROFILER_INSTANCE
	#define TERMINUS_CREATE_PROFILER
	#define TERMINUS_DESTROY_PROFILER
	#define TERMINUS_BEGIN_GPU_PROFILE(x)
	#define TERMINUS_END_GPU_PROFILE
#endif