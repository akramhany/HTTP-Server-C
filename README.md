# HTTP-Server-C

A simple, modular HTTP server implemented in C. This project demonstrates basic HTTP request parsing, routing, and file serving, with support for gzip compression and concurrent client handling via process forking.

## Features
- Handles HTTP GET and POST requests
- Serves static files from a specified directory
- Echo and User-Agent endpoints
- Gzip compression support (if requested by client)
- Handles multiple clients concurrently using `fork()`
- Clear modular code structure


## Usage
Run the server with:

```sh
./program --directory <dir_path> <port>
```

- `<dir_path>`: Directory to serve files from
- `<port>`: Port number to listen on

Example:
```sh
./program --directory ./public 8080
```

## Endpoints
- `/echo/<message>`: Responds with `<message>`
- `/user-agent`: Responds with the client's User-Agent
- `/files/<filename>`: GET to download, POST to upload/overwrite
- `/`: Returns 200 OK
