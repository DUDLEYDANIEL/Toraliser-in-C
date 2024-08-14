/* Toralizer */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


/*

		+----+----+----+----+----+----+----+----+----+----+....+----+
		| VN | CD | DSTPORT |      DSTIP        | USERID       |NULL|
		+----+----+----+----+----+----+----+----+----+----+....+----+
 bytes     1    1      2              4           variable       1


*/

//Sock protocal is used for the TCP proxy connection 
#define PROXY      "127.0.0.1"
#define PROXYPORT   9080

typedef unsigned char int8;
typedef unsigned char int16;
typedef unsigned char int32;

struct proxy_request {
    int8 vn;
    int8 cd;
    int16 dstport;
    int32 dstip;
    unsigned char userid[8];
};

typedef struct proxy_request Req;

/*

		+----+----+----+----+----+----+----+----+----+----+....+----+
		| VN | CD | DSTPORT |      DSTIP        | USERID       |NULL|
		+----+----+----+----+----+----+----+----+----+----+....+----+
 # of byte  1    1      2              4           variable       1

*/

struct proxy_response{
    int8 vn;
    int8 cd;
    int16 _; // they will be ignore
    int32 __;
};

typedef struct  proxy_response Res;







