/*
 * author: Max Kellermann <mk@cm4all.com>
 */

#ifndef BENG_PROXY_SPAWN_SERVER_HXX
#define BENG_PROXY_SPAWN_SERVER_HXX

struct SpawnConfig;
struct CgroupState;
class SpawnHook;

void
RunSpawnServer(const SpawnConfig &config, const CgroupState &cgroup_state,
               SpawnHook *hook,
               int fd);

#endif