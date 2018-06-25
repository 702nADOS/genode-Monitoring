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

namespace Mon_manager
{

	struct Session_component : Genode::Rpc_object<Session>
	{

		private:

			Mon_manager *_monmanager = nullptr;

		public:
			enum { CAP_QUOTA = 2 };
			Genode::size_t update_info(Genode::Dataspace_capability ds_cap)
			{
				return _monmanager->update_info(ds_cap);
			}

			void update_rqs(Genode::Dataspace_capability ds_cap)
			{
				_monmanager->update_rqs(ds_cap);
			}

			void update_dead(Genode::Dataspace_capability ds_cap)
			{
				_monmanager->update_dead(ds_cap);
			}

			int get_num_cores()
			{
				return _monmanager->get_num_cores();
			}

			Genode::Trace::Execution_time get_idle_time(int core)
			{
				return _monmanager->get_idle_time(core);
			}

			bool is_core_online(int core)
			{
				return _monmanager->is_core_online(core);
			}

			int get_util(int core)
			{
				return _monmanager->get_util(core);
			}

			Session_component(Mon_manager *monmanager)
			: Genode::Rpc_object<Session>()
			{
				_monmanager = monmanager;
			}
			Session_component(const Session_component&);
			Session_component& operator = (const Session_component&);

	};

	class Root_component : public Genode::Root_component<Session_component>
	{

		private:

			//Genode::Env &_env;
			Mon_manager *_monmanager = nullptr;
			

		protected:

			Session_component *_create_session(const char *)
			{
				return new (md_alloc()) Session_component(_monmanager);
			}

		public:

			Root_component(Genode::Entrypoint &ep,
			               Genode::Allocator &allocator,
			               Mon_manager *monmanager)
			: Genode::Root_component<Session_component>(ep, allocator)
			{
				_monmanager = monmanager;
			}
			Root_component(const Root_component&);
			Root_component& operator = (const Root_component&);
	};

}


/*
int main()
{
	Mon_manager::Mon_manager monmanager;

	Cap_connection cap;

	static Sliced_heap sliced_heap(env()->ram_session(),
	                               env()->rm_session());

	enum { STACK_SIZE = 4096 };
	static Rpc_entrypoint ep(&cap, STACK_SIZE, "mon_manager_ep");

	static Mon_manager::Root_component mon_manager_root(&ep, &sliced_heap, &monmanager);
	env()->parent()->announce(ep.manage(&mon_manager_root)); 

	sleep_forever();

	return 0;
}
*/

struct Main
{	
	Libc::Env &_env;
	Genode::Entrypoint &_ep;	
	Mon_manager::Mon_manager monmanager {_env};
	Genode::Sliced_heap sliced_heap{_env.ram(),
	                               _env.rm()};	
	Mon_manager::Root_component _mon_manager_root{_ep, sliced_heap, &monmanager};
	
	Main(Libc::Env &env) : _env(env), _ep(_env.ep())
	{
				_env.parent().announce(_ep.manage(_mon_manager_root));
	}	
	
};

//void Component::construct(Genode::Env &env) { static Main main(env); }
void Libc::Component::construct(Libc::Env &env)
{
	Libc::with_libc([&] () { static Main main(env); });
}
