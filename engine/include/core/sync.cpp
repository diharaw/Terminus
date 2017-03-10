#include <core/sync.h>
#include <thread/semaphore.h>

namespace terminus
{
	namespace sync
	{
		Semaphore			_renderer_ready;
		Semaphore			_main_ready;
		Semaphore			_renderer_done;
		Semaphore			_swap_done;
		Semaphore			_renderer_exit;
		Semaphore			_loader_exit;
		Semaphore			_loader_wakeup;

		void wait_for_main_ready()
		{
			_main_ready.wait();
		}

		void notify_main_ready()
		{
			_main_ready.notify();
		}

		void wait_for_renderer_done()
		{
			_renderer_done.wait();
		}

		void notify_renderer_done()
		{
			_renderer_done.notify();
		}

		void wait_for_renderer_ready()
		{
			_renderer_ready.wait();
		}
		void notify_renderer_ready()
		{
			_renderer_ready.notify();
		}

		void wait_for_swap_done()
		{
			_swap_done.wait();
		}

		void notify_swap_done()
		{
			_swap_done.notify();
		}

		void wait_for_renderer_exit()
		{
			_renderer_exit.wait();
		}

		void notify_renderer_exit()
		{
			_renderer_exit.notify();
		}

		void wait_for_loader_wakeup()
		{
			_loader_wakeup.wait();
		}

		void notify_loader_wakeup()
		{
			_loader_wakeup.notify();
		}

		void wait_for_loader_exit()
		{
			_loader_exit.wait();
		}

		void notify_loader_exit()
		{
			_loader_exit.notify();
		}
	}
}
