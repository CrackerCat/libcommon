/*
 * author: Max Kellermann <mk@cm4all.com>
 */

#ifndef CONNECT_SOCKET_HXX
#define CONNECT_SOCKET_HXX

#include "net/UniqueSocketDescriptor.hxx"
#include "event/SocketEvent.hxx"
#include "util/Cancellable.hxx"

#include <exception>

class Error;
class SocketAddress;
class AddressInfo;

class ConnectSocketHandler {
public:
    virtual void OnSocketConnectSuccess(UniqueSocketDescriptor &&fd) = 0;
    virtual void OnSocketConnectTimeout();
    virtual void OnSocketConnectError(std::exception_ptr ep) = 0;
};

/**
 * A class that connects to a SocketAddress.
 */
class ConnectSocket final : public Cancellable {
    ConnectSocketHandler &handler;

    UniqueSocketDescriptor fd;

    SocketEvent event;

public:
    explicit ConnectSocket(EventLoop &_event_loop,
                           ConnectSocketHandler &_handler);
    ~ConnectSocket();

    bool IsPending() const {
        return fd.IsDefined();
    }

    bool Connect(SocketAddress address);

    bool Connect(const AddressInfo &address,
                 const struct timeval *timeout);

    bool Connect(const AddressInfo &address,
                 const struct timeval &timeout) {
        return Connect(address, &timeout);
    }

    /**
     * Wait until the given socket is connected (this method returns
     * immediately and invokes the #ConnectSocketHandler on completion
     * or error).
     */
    void WaitConnected(UniqueSocketDescriptor &&_fd,
                       const struct timeval *timeout);

    void WaitConnected(UniqueSocketDescriptor &&_fd,
                       const struct timeval &timeout) {
        WaitConnected(std::move(_fd), &timeout);
    }

    /* virtual methods from Cancellable */
    void Cancel() override;

private:
    void OnEvent(unsigned events);
};

#endif