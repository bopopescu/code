#ifndef _SOCKET_ADDRESS_H
#define _SOCKET_ADDRESS_H

class SocketAddress
{
public:
    SocketAddress(uint32_t inAddress, uint16_t inPort)
    {
        GetAsSockAddrIn()->sin_family = AF_INET;
        GetAsSockAddrIn()->sin_addr.s_addr = htonl(inAddress);
        GetAsSockAddrIn()->sin_port = htons(inPort);
    }
    SocketAddress(const sockaddr& inSockAddr)
    {
        memcpy(&mSockAddr, &inSockAddr, sizeof(sockaddr));
    }
    size_t GetSize() const { return sizeof(sockaddr); }
private:
    sockaddr mSockAddr;
    sockaddr_in* GetAsSockAddrIn()
    { return reinterpret_cast<sockaddr_in*>( &mSockAddr ); }
};

typedef std::shared_ptr<SocketAddress> SocketAddressPtr;

#endif
