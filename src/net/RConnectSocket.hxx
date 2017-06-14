/*
 * author: Max Kellermann <mk@cm4all.com>
 */

#ifndef RESOLVE_CONNECT_SOCKET_HXX
#define RESOLVE_CONNECT_SOCKET_HXX

struct addrinfo;
class UniqueSocketDescriptor;

UniqueSocketDescriptor
ResolveConnectSocket(const char *host_and_port, int default_port,
                     const struct addrinfo &hints);

#endif