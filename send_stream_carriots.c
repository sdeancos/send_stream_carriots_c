#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define SA struct sockaddr
#define MAXLINE 4096
#define MAXSUB 200
#define LISTENQ 1024
extern int h_errno;

void send_stream_carriots(char *device, char *apikey, char *data) {
    int sockfd;
    struct sockaddr_in servaddr;
    char **pptr, poststr[MAXLINE + 1], sendline[MAXLINE + 1], recvline[MAXLINE + 1];
    char *hname = "51.137.97.16"; // api.carriots.com
    char *uri = "/streams";
    ssize_t int_return_read;
    char *stream = "POST %s HTTP/1.0\r\nHost: %s\r\nContent-type: application/json\r\nContent-length: %d\r\nCarriots.apikey: %s\r\n\r\n%s";

    sprintf(poststr, "{\"protocol\": \"v2\", \"device\":\"%s\", \"at\":\"now\", \"data\": %s}\n", device, data);

    char str[50];
    struct hostent *hptr;
    if ((hptr = gethostbyname(hname)) == NULL) {
        fprintf(stderr, " Server down error for host: %s: %s", hname, hstrerror(h_errno));
        exit(1);
    }
    printf("hostname: %s \n", hptr->h_name);
    if (hptr->h_addrtype == AF_INET && (pptr = hptr->h_addr_list) != NULL) {
        printf("address: %s\n", inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
    } else {
        fprintf(stderr, "Error call inet_ntop \n");
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET -> IPv4
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(80);
    inet_pton(AF_INET, str, &servaddr.sin_addr);

    connect(sockfd, (SA *) & servaddr, sizeof(servaddr));

    sprintf(sendline, stream, uri, hname, (int)strlen(poststr), apikey, poststr);

    write(sockfd, sendline, strlen(sendline));
    while ((int_return_read = read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[int_return_read] = '\0';
        printf("%s", recvline);
    }

    close(sockfd);
}

int main(void)
{
    char *device = "Your_device_id_developer_here";
    char *apikey = "Your_apikey_here";
    char *data = "{\"keyExameple\":\"valueExample\"}";

    send_stream_carriots(device, apikey, data);

    exit(0);
}
