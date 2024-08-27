#include "toralize.h"
Req *request(const char *dstip, const int dstport) {
    Req *req;
    req = malloc(reqsize);
    if (!req) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    req->vn = 4; // SOCKS4
    req->cd = 1; // CONNECT
    req->dstport = htons(dstport); // Convert to network byte order
    req->dstip = inet_addr(dstip); // Convert to network byte order
    strncpy((char *)req->username, USERNAME, 7);
    req->username[7] = '\0'; // Ensure null-termination

    return req;
}

int main(int argc, char *argv[]) {
    char *host;
    int port, s;
    struct sockaddr_in sock;
    Req *req;
    Res *res;
    char buf[ressize];
    int success;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
        return -1;
    }

    host = argv[1];
    port = atoi(argv[2]);

    // Create a socket
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("socket");
        return -1;
    }

    // Setup the target address structure
    sock.sin_family = AF_INET;
    sock.sin_port = htons(PROXYPORT);
    sock.sin_addr.s_addr = inet_addr(PROXY);

    // Connect to the proxy server
    if (connect(s, (struct sockaddr *)&sock, sizeof(sock)) < 0) {
        perror("connect");
        close(s);
        return -1;
    }

    // Create and send the request
    req = request(host, port);
    printf("Requesting IP: %s, Port: %d\n", host, port);
    printf("Request dstip: %s, dstport: %d\n", inet_ntoa(*(struct in_addr *)&req->dstip), ntohs(req->dstport));

    if (write(s, req, reqsize) < 0) {
        perror("write");
        free(req);
        close(s);
        return -1;
    }
    free(req);

    // Receive the response
    memset(buf, 0, ressize);
    if (read(s, buf, ressize) < 1) {
        perror("read");
        close(s);
        return -1;
    }

    // Interpret the response
    res = (Res *)buf;
    printf("Response Code: %d\n", res->cd);
    success = (res->cd == 90);
    if (!success) {
        fprintf(stderr, "Error unable to connect to proxy, error code: %d\n", res->cd);
        close(s);
        return -1;
    }

    printf("Successfully connected through the proxy to %s:%d\n", host, port);
    close(s);
    return 0;
}