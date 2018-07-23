/* global includes */
#include <base/env.h>
#include <base/printf.h>
#include <base/rpc_server.h>
#include <base/sleep.h>
//#include <cap_session/connection.h>
#include <libc/component.h>
#include <root/component.h>
#include <base/component.h>
#include "mon_manager/mon_manager.h"

/* local includes */
#include "mon_manager/mon_manager_session.h"

namespace Mon_manager {
	struct Main;
	struct Session_component;
	struct Root_component;
}

struct Mon_manager::Session_component : Genode::Rpc_object<Session>
{
	private:
		Mon_manager* _monitor=nullptr;
	public:
		enum { CAP_QUOTA = 2 };
		Genode::size_t update_info(Genode::Dataspace_capability ds_cap)
		{
			return _monitor->update_info(ds_cap);
		}
		void update_rqs(Genode::Dataspace_capability ds_cap)
		{
			_monitor->update_rqs(ds_cap);
		}
		void update_dead(Genode::Dataspace_capability ds_cap)
		{
			_monitor->update_dead(ds_cap);
		}
		int get_num_cores()
		{
			return _monitor->get_num_cores();
		}
		Genode::Trace::Execution_time get_idle_time(int core)
		{
			return _monitor->get_idle_time(core);
		}
		bool is_core_online(int core)
		{
			return _monitor->is_core_online(core);
		}
		int get_util(int core)
		{
			return _monitor->get_util(core);
		}
		
		Session_component(Mon_manager *monitor)
		: Genode::Rpc_object<Session>()
		{
			_monitor = monitor;
		}
	Session_component(const Session_component&);
	Session_component& operator = (const Session_component&);	
};

class Mon_manager::Root_component : public Genode::Root_component<Session_component>
{
	private:
		Mon_manager* _monitor { };
	protected:

		Session_component *_create_session(const char*)
		{
			return new (md_alloc()) Session_component(_monitor);
		}

	public:

		Root_component(Genode::Entrypoint &ep,
		               Genode::Allocator &alloc,
		               Mon_manager *monitor)
		:
			Genode::Root_component<Session_component>(ep, alloc)
		{
			_monitor=monitor;
		}
	Root_component(const Root_component&);
	Root_component& operator = (const Root_component&);	
};

struct Mon_manager::Main
{
	enum { ROOT_STACK_SIZE = 16*1024 };
	Genode::Env	&_env;
	Genode::Heap	heap	{ _env.ram(), _env.rm() };
	Mon_manager monitor { _env };
	Root_component Mon_manager_root { _env.ep(), heap , &monitor};

	Main(Libc::Env &env_) : _env(env_)
	{
		_env.parent().announce(_env.ep().manage(Mon_manager_root));
	}
};

Genode::size_t Component::stack_size() { return 32*1024; }

void Libc::Component::construct(Libc::Env &env)
{
	Libc::with_libc([&] () { static Mon_manager::Main main(env); });
}
