# HTTP Web Proxy

A basic single-threaded HTTP web proxy written in C using POSIX sockets.

## How to Compile

```bash
make
```

This produces the `proxy` executable. To clean build artifacts:

```bash
make clean
```

## How to Run

```bash
./proxy <port>
```

For example, to listen on port 8080:

```bash
./proxy 8080
```

## Testing

**Using curl:**

```bash
curl -x localhost:8080 http://example.com/
```

**Using a browser:**

Set your browser's HTTP proxy to `localhost` on port `8080`, then navigate to any `http://` URL.

## Example Input and Output

Terminal 1 (start the proxy):
```
$ make
gcc -Wall -Wextra -g -o proxy proxy.c
$ ./proxy 8080
Proxy listening on port 8080
Received: GET http://example.com/ HTTP/1.1
  -> Host: example.com, Port: 80, Path: /
  -> Request completed for example.com/
```

Terminal 2 (send a request):
```
$ curl -x localhost:8080 http://example.com/
<!doctype html>
<html>
...
</html>
```

## Assumptions

- Only HTTP GET requests are supported (not POST, PUT, etc.).
- Only plain HTTP (port 80) is supported; HTTPS/CONNECT is not handled.
- The proxy uses HTTP/1.0 when forwarding to the remote server so that the
  server closes the connection after the response, simplifying the relay loop.
- Single-threaded, iterative design: one client is served at a time.
- The `Host` and `Connection: close` headers are added to the forwarded request;
  other client headers (e.g., cookies, user-agent) are not forwarded.
- URIs with an explicit port (e.g., `http://host:8080/path`) are supported.

## Design Overview

1. **Listen** – The proxy binds to the user-specified port and accepts connections.
2. **Read & Parse** – Reads the client's HTTP request and parses the method, URI, and version.
3. **Extract Host/Path** – Splits the absolute URI into hostname, optional port, and path.
4. **Connect** – Resolves the hostname via DNS and opens a TCP connection to port 80 (or the specified port).
5. **Forward** – Sends a reformatted HTTP/1.0 GET request to the remote server.
6. **Relay** – Reads the server's response in a loop and writes it back to the client using robust I/O (handling partial writes).
7. **Cleanup** – Closes both the server and client sockets.