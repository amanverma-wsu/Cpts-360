#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLINE 8192

void handle_client(int client_fd);

int main(int argc, char *argv[]) {
    int listen_fd, port;
    struct sockaddr_in server_addr;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    port = atoi(argv[1]);

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("socket");
        exit(1);
    }

    /* Allow port reuse to avoid "Address already in use" errors */
    int optval = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(listen_fd);
        exit(1);
    }

    if (listen(listen_fd, 10) < 0) {
        perror("listen");
        close(listen_fd);
        exit(1);
    }

    printf("Proxy listening on port %d\n", port);

    while (1) {
        int client_fd = accept(listen_fd, NULL, NULL);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }
        handle_client(client_fd);
        close(client_fd);
    }

    close(listen_fd);
    return 0;
}

void handle_client(int client_fd) {
    char buffer[MAXLINE];
    char method[16], uri[256], version[16];
    char hostname[256], path[256];
    int server_fd;
    struct hostent *server;

    /* ---- Read request from client ---- */
    ssize_t total_read = 0;
    ssize_t n;
    while (total_read < MAXLINE - 1) {
        n = read(client_fd, buffer + total_read, MAXLINE - 1 - total_read);
        if (n <= 0)
            break;
        total_read += n;
        /* Check if we've received the end of the HTTP headers (\r\n\r\n) */
        buffer[total_read] = '\0';
        if (strstr(buffer, "\r\n\r\n") != NULL)
            break;
    }

    if (total_read <= 0) {
        fprintf(stderr, "Error: failed to read from client\n");
        return;
    }
    buffer[total_read] = '\0';

    /* ---- Parse HTTP request line ---- */
    if (sscanf(buffer, "%15s %255s %15s", method, uri, version) != 3) {
        fprintf(stderr, "Error: malformed request line\n");
        return;
    }

    /* Only support GET requests */
    if (strcasecmp(method, "GET") != 0) {
        fprintf(stderr, "Error: only GET method is supported (got %s)\n", method);
        const char *msg = "HTTP/1.0 501 Not Implemented\r\n\r\n";
        write(client_fd, msg, strlen(msg));
        return;
    }

    printf("Received: %s %s %s\n", method, uri, version);

    /* ---- Extract hostname and path from URI ---- */
    /* Expected URI format: http://hostname/path or http://hostname:port/path */
    memset(hostname, 0, sizeof(hostname));
    memset(path, 0, sizeof(path));
    int remote_port = 80;

    char *host_start = strstr(uri, "://");
    if (host_start == NULL) {
        fprintf(stderr, "Error: invalid URI format (missing ://)\n");
        return;
    }
    host_start += 3; /* skip past "://" */

    /* Find the start of the path */
    char *path_start = strchr(host_start, '/');
    if (path_start != NULL) {
        strncpy(path, path_start, sizeof(path) - 1);
    } else {
        strcpy(path, "/");
    }

    /* Extract hostname (and optional port) */
    size_t host_len;
    if (path_start != NULL)
        host_len = path_start - host_start;
    else
        host_len = strlen(host_start);

    if (host_len >= sizeof(hostname)) {
        fprintf(stderr, "Error: hostname too long\n");
        return;
    }
    strncpy(hostname, host_start, host_len);
    hostname[host_len] = '\0';

    /* Check for a port number in the hostname (e.g., hostname:8080) */
    char *colon = strchr(hostname, ':');
    if (colon != NULL) {
        *colon = '\0';
        remote_port = atoi(colon + 1);
    }

    printf("  -> Host: %s, Port: %d, Path: %s\n", hostname, remote_port, path);

    /* ---- Connect to remote server ---- */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr, "Error: DNS lookup failed for %s\n", hostname);
        const char *msg = "HTTP/1.0 502 Bad Gateway\r\n\r\nDNS lookup failed\r\n";
        write(client_fd, msg, strlen(msg));
        return;
    }

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket (server)");
        return;
    }

    struct sockaddr_in remote_addr;
    memset(&remote_addr, 0, sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    memcpy(&remote_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    remote_addr.sin_port = htons(remote_port);

    if (connect(server_fd, (struct sockaddr *)&remote_addr, sizeof(remote_addr)) < 0) {
        perror("connect (server)");
        close(server_fd);
        const char *msg = "HTTP/1.0 502 Bad Gateway\r\n\r\nConnection failed\r\n";
        write(client_fd, msg, strlen(msg));
        return;
    }

    /* ---- Forward request to server ---- */
    /* Build a proper HTTP/1.0 request with just the path (not the full URI) */
    char request[MAXLINE];
    snprintf(request, sizeof(request),
             "GET %s HTTP/1.0\r\n"
             "Host: %s\r\n"
             "Connection: close\r\n"
             "\r\n",
             path, hostname);

    ssize_t bytes_written = 0;
    ssize_t req_len = strlen(request);
    while (bytes_written < req_len) {
        n = write(server_fd, request + bytes_written, req_len - bytes_written);
        if (n <= 0) {
            perror("write (to server)");
            close(server_fd);
            return;
        }
        bytes_written += n;
    }

    /* ---- Relay response back to client ---- */
    while ((n = read(server_fd, buffer, MAXLINE)) > 0) {
        ssize_t written = 0;
        while (written < n) {
            ssize_t w = write(client_fd, buffer + written, n - written);
            if (w <= 0) {
                perror("write (to client)");
                close(server_fd);
                return;
            }
            written += w;
        }
    }

    /* ---- Close server socket ---- */
    close(server_fd);
    printf("  -> Request completed for %s%s\n", hostname, path);
}