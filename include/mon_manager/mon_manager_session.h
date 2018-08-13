#pragma once

#include <session/session.h>
#include <base/rpc.h>
#include <ram_session/ram_session.h>
//#include <string>
#include <trace_session/connection.h>

namespace Mon_manager {

	struct Session : Genode::Session
	{
		static const char *service_name() { return "mon_manager"; }

		virtual Genode::size_t update_info(Genode::Dataspace_capability ds_cap) = 0;

		virtual void update_rqs(Genode::Dataspace_capability ds_cap) = 0;

		virtual void update_dead(Genode::Dataspace_capability ds_cap) = 0;

		virtual int get_num_cores() = 0;

		virtual Genode::Trace::Execution_time get_idle_time(int core) = 0;

		virtual bool is_core_online(int core) = 0;

		virtual int get_util(int core) = 0;

		GENODE_RPC(Rpc_update_info, Genode::size_t, update_info, Genode::Dataspace_capability);

		GENODE_RPC(Rpc_update_rqs, void, update_rqs, Genode::Dataspace_capability);

		GENODE_RPC(Rpc_update_dead, void, update_dead, Genode::Dataspace_capability);

		GENODE_RPC(Rpc_get_num_cores, int, get_num_cores);

		GENODE_RPC(Rpc_get_idle_time, Genode::Trace::Execution_time, get_idle_time, int);

	 	GENODE_RPC(Rpc_is_core_online, bool, is_core_online, int);

		GENODE_RPC(Rpc_get_util, int, get_util, int);

		GENODE_RPC_INTERFACE(Rpc_update_info, Rpc_get_num_cores, Rpc_get_idle_time, Rpc_is_core_online, Rpc_update_rqs, Rpc_update_dead, Rpc_get_util);

	};

}
