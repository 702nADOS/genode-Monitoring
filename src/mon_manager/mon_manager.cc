#include "mon_manager/mon_manager.h"
#include <timer_session/connection.h>
#include <base/env.h>
#include <base/printf.h>
#include <util/xml_node.h>
#include <util/xml_generator.h>
#include <string>
#include <util/string.h>
#include <trace_session/connection.h>
//#include <os/config.h>
namespace Mon_manager{
	Mon_manager::Mon_manager(Genode::Env &env):_env(env) {}

	

	size_t Mon_manager::update_info(Genode::Dataspace_capability mon_ds_cap)
	{
		num_threads=100;
		Monitoring_object *threads = _env.rm().attach(mon_ds_cap);
		static Timer::Connection timer {_env};
		static Genode::Trace::Connection trace(_env, 1024*4096, 64*4096, 0);
		Genode::Trace::Subject_id subjects[num_threads];
		size_t num_subjects = trace.subjects(subjects, num_threads);
		Genode::Trace::RAM_info init;
		for (size_t i = 0; i < num_subjects; i++) {
			Genode::Trace::CPU_info cpu_info = trace.cpu_info(subjects[i]);
			Genode::Trace::RAM_info ram_info = trace.ram_info(subjects[i]);
			if(Genode::strcmp(ram_info.session_label().string(), "init")!=0) {
       				threads[i].session_label=ram_info.session_label();
				threads[i].thread_name=ram_info.thread_name();
				threads[i].prio=cpu_info.prio();
				threads[i].execution_time=cpu_info.execution_time();
				threads[i].id=cpu_info.id();
				threads[i].foc_id=cpu_info.foc_id();
				threads[i].ram_used=ram_info.ram_used();
				threads[i].ram_quota=ram_info.ram_quota();
				threads[i].arrival_time=cpu_info.arrival_time();
				threads[i].start_time=cpu_info.start_time();
				threads[i].exit_time=cpu_info.kill_time();
				threads[i].policy_id=cpu_info.policy_id();
				threads[i].state=cpu_info.state();
				threads[i].affinity=cpu_info.affinity();
			}else {
				init=ram_info;
			}
						
		}
		//Genode::Trace::SCHEDULER_info scheduler_info = trace.scheduler_info(subjects[0]);
		ram_avail=init.ram_quota();
		_env.rm().detach(threads);
		return num_subjects;
	}

	void Mon_manager::update_rqs(Genode::Dataspace_capability rq_ds_cap)
	{
		rq=_env.rm().attach(rq_ds_cap);
		_env.cpu().rq(rq_ds_cap);
		_env.rm().detach(rq);

	}

	void Mon_manager::update_dead(Genode::Dataspace_capability dead_ds_cap)
	{
		dead=_env.rm().attach(dead_ds_cap);
		_env.cpu().dead(dead_ds_cap);
		_env.rm().detach(dead);
	}


	int Mon_manager::get_num_cores() {
		num_threads=1;
		static Genode::Trace::Connection trace(_env, 1024*4096, 64*4096, 0);
		Genode::Trace::Subject_id subjects[num_threads];
		//size_t num_subjects = trace.subjects(subjects, num_threads);
		Genode::Trace::SCHEDULER_info scheduler_info = trace.scheduler_info(subjects[0]);
		num_cores=scheduler_info.num_cores();
		return num_cores;
	}
	
	Genode::Trace::Execution_time Mon_manager::get_idle_time(int core) {
		num_threads=1;
		static Genode::Trace::Connection trace(_env, 1024*4096, 64*4096, 0);
		Genode::Trace::Subject_id subjects[num_threads];
		//size_t num_subjects = trace.subjects(subjects, num_threads);
		Genode::Trace::SCHEDULER_info scheduler_info = trace.scheduler_info(subjects[0]);
		idle0=scheduler_info.idle0();
		idle1=scheduler_info.idle1();
		idle2=scheduler_info.idle2();
		idle3=scheduler_info.idle3();
		switch(core) {
			case 0:return idle0;
			case 1:return idle1;
			case 2:return idle2;
			case 3:return idle3;
			default:return -1;
		}	
	}

	bool Mon_manager::is_core_online(int core) {
		num_threads=1;
		static Genode::Trace::Connection trace(_env, 1024*4096, 64*4096, 0);
		Genode::Trace::Subject_id subjects[num_threads];
		//size_t num_subjects = trace.subjects(subjects, num_threads);
		Genode::Trace::SCHEDULER_info scheduler_info = trace.scheduler_info(subjects[0]);
		core0_is_online=scheduler_info.core0_is_online();
		core1_is_online=scheduler_info.core1_is_online();
		core2_is_online=scheduler_info.core2_is_online();
		core3_is_online=scheduler_info.core3_is_online();
		switch(core) {
			case 0:return core0_is_online;
			case 1:return core1_is_online;
			case 2:return core2_is_online;
			case 3:return core3_is_online;
			default:return -1;
		}
	}

	int Mon_manager::get_util(int core)
	{
		return util.utilization(core);
	}
}
