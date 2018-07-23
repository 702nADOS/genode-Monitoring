#pragma once

#include <mon_manager/mon_manager_client.h>
#include <base/connection.h>

namespace Mon_manager {

	struct Connection : Genode::Connection<Session>, Session_client
	{
		Connection(Genode::Env &env) : Genode::Connection<Mon_manager::Session>(env, session(env.parent(),
		                                                "ram_quota=6K, cap_quota=4")),
		               Session_client(cap()) { }
	};


}
