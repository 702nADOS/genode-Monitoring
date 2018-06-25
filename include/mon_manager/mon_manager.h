#pragma once

#include <base/heap.h>
#include <base/signal.h>
#include <mon_manager/mon_manager_session.h>
#include <utilization/connection.h>
#include <utilization/utilization.h>
//#include <os/attached_ram_dataspace.h>
#include <base/attached_ram_dataspace.h>
//#include <os/server.h>
#include <root/component.h>
#include <timer_session/connection.h>
#include <util/string.h>
//#include <cap_session/connection.h>
#include <trace_session/connection.h>
#include <base/thread_state.h>

namespace Mon_manager{
struct Monitoring_object
{
	Genode::Session_label			session_label;
	Genode::Trace::Thread_name		thread_name;
	Genode::Trace::Policy_id		policy_id;
	Genode::Trace::Execution_time		execution_time;
	unsigned	   			prio;
	unsigned	   			id;
	int  	   			foc_id;
	Genode::size_t		   			ram_quota;
	Genode::size_t		   			ram_used;
	Genode::Trace::CPU_info::State 		state;
	Genode::Affinity::Location		affinity;
	unsigned long long			start_time;
	unsigned long long			arrival_time;
	unsigned long long			exit_time;
};

class Mon_manager
{
	public:
		Genode::Env &_env;
		Genode::size_t update_info(Genode::Dataspace_capability ds_cap);
		void update_rqs(Genode::Dataspace_capability rq_ds_cap);
		void update_dead(Genode::Dataspace_capability dead_ds_cap);
		Mon_manager(Genode::Env &_env);
		Mon_manager(const Mon_manager&);
		Mon_manager& operator = (const Mon_manager&);
		Genode::Dataspace_capability init_ds_cap(int num_threads);
		int get_num_cores();
		Genode::Trace::Execution_time get_idle_time(int core);
		bool is_core_online(int core);
		int get_util(int core);
		
	protected:
		Utilization::Connection util {};
		int num_subjects {};
		int* rq {};
		long long unsigned* dead{};
		Genode::Dataspace_capability rq_ds_cap{};
		bool core0_is_online{};
		bool core1_is_online{};
		bool core2_is_online{};
		bool core3_is_online{};
		int num_cores{};
		Genode::Trace::Execution_time idle0{};
		Genode::Trace::Execution_time idle1{};
		Genode::Trace::Execution_time idle2{};
		Genode::Trace::Execution_time idle3{};
		Genode::size_t ram_avail{};
		int num_threads{};
		double util0{};
		double util1{};
		double util2{};
		double util3{};
};
}
