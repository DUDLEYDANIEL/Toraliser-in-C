#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PROXY "127.0.0.1"
#define PROXYPORT 9050
#define reqsize sizeof(struct proxy_request)
#define ressize sizeof(struct proxy_response)
#define USERNAME "toraliz"

typedef unsigned char int8;
typedef unsigned int  int16;
typedef unsigned int  int32;

struct proxy_request {
    int8 vn;            // SOCKS protocol version
    int8 cd;            // Command code (1 = CONNECT)
    int16 dstport;      // Destination port (network byte order)
    int32 dstip;        // Destination IP address (network byte order)
    unsigned char username[8]; // Username (for authentication, if needed)
};

typedef struct proxy_request Req;

struct proxy_response {
    int8 vn;            // SOCKS protocol version
    int8 cd;            // Reply code (90 = request granted)
    int16 _;            // Reserved (ignore)
    int32 __;           // Reserved (ignore)
};

typedef struct proxy_response Res;