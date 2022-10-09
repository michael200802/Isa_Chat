#include "socket_wrapper.h"

#if defined(__WIN32__)||defined(_WIN32)

//it's windows
#include <winsock2.h>

#define GetLastSocketError() WSAGetLastError()
#define SE_INTR WSAEINTR
#define SE_NOBUFS WSAENOBUFS

#else

        #if defined(__linux__)||defined(__linux)||defined(linux)

                //it's linux
                #include <sys/socket.h>
                #include <arpa/inet.h>

        #else

                //it's freertos
                #include <lwip/sockets.h>

        #endif

        #include <errno.h>
        #define GetLastSocketError() errno
        #define SE_INTR EINTR
        #define SE_NOBUFS ENOBUFS

#endif

inline bool fun_send(int sockfd, const byte_t * restrict buf, size_t size)
{
        while(size != 0)
        {
                ssize_t n_sent_bytes = send(sockfd, buf, size, 0);
                if(n_sent_bytes == -1)
                {
                        int cur_error = GetLastSocketError();
                        if(cur_error == SE_INTR || cur_error == SE_NOBUFS)
                        {
                                continue;
                        }
                        return false;
                }
                size -= n_sent_bytes;
                buf += n_sent_bytes;
        }
	return true;
}

inline bool fun_recv(int sockfd, byte_t * restrict buf, size_t size)
{
        while(size != 0)
        {
                ssize_t n_recv_bytes = recv(sockfd, buf, size, 0);
                if(n_recv_bytes == -1)
                {
                        int cur_error = GetLastSocketError();
                        if(cur_error == SE_INTR || cur_error == SE_NOBUFS)
                        {
                                continue;
                        }
                        return false;
                }
                size -= n_recv_bytes;
                buf += n_recv_bytes;
        }
	return true;
}

inline bool send_number(int sockfd, portable_integer_t num)
{
        num = htonl(num);
        return fun_send(sockfd,(void*)&num,sizeof(portable_integer_t));
}

inline bool recv_number(int sockfd, portable_integer_t * restrict const num)
{
        bool success = fun_recv(sockfd,(void*)num, sizeof(portable_integer_t));
        *num = ntohl(*num);
        return success;
}

