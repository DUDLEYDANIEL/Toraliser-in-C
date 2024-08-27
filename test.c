#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    unsigned char request[8];
    unsigned char response[8];

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9050);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the proxy
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Prepare SOCKS4 connect request
    request[0] = 0x04;  // SOCKS version
    request[1] = 0x01;  // CONNECT command
    request[2] = 0x00;  // Reserved
    request[3] = 0x00;  // Reserved
    request[4] = 0x00;  // Destination port (high byte)
    request[5] = 0x50;  // Destination port (low byte)
    request[6] = 0x01;  // Destination IP (high byte)
    request[7] = 0x01;  // Destination IP (low byte)

    // Send request
    if (send(sockfd, request, sizeof(request), 0) < 0) {
        perror("Send failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Receive response
    if (recv(sockfd, response, sizeof(response), 0) < 0) {
        perror("Receive failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Received response: %02x %02x\n", response[0], response[1]);

    // Close the socket
    close(sockfd);

    return 0;
}
