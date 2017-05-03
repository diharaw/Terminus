#pragma once

namespace terminus
{
	namespace sync
	{
		// main thread ready
		extern void wait_for_main_ready();
		extern void notify_main_ready();

		// renderer done
		extern void wait_for_renderer_done();
		extern void notify_renderer_done();

		// renderer ready
		extern void wait_for_renderer_ready();
		extern void notify_renderer_ready();

		// swap done
		extern void wait_for_swap_done();
		extern void notify_swap_done();

		// renderer exit
		extern void wait_for_renderer_exit();
		extern void notify_renderer_exit();

		// loader wakeup
		extern void wait_for_loader_wakeup();
		extern void notify_loader_wakeup();

		// loader exit
		extern void wait_for_loader_exit();
		extern void notify_loader_exit();

		// renderer begin
		extern void wait_for_renderer_begin();
		extern void notify_renderer_begin();
	}
}
