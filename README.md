# Simple WebSocket Client and Server

This project consists of a simple WebSocket client and server, designed for communication over WebSockets. The client can connect to a WebSocket server and send/receive messages in real-time. The server listens for incoming connections and responds with the received message.

## Requirements

- C++ compiler (supporting C++11 or higher)
- [ixWebSocket](https://github.com/machinezone/IXWebSocket) library
- CMake or another build system

## Setup

### Dependencies

Before building the client and server, you need to install the `ixWebSocket` library. Follow the instructions in the [ixWebSocket repository](https://github.com/machinezone/IXWebSocket) to set up the library.

### Building

1. Clone the repository:
    ```bash
    git clone https://github.com/Hideaku/simple-websocket-chat.git
    cd simple-websocket-chat
    ```

2. Ensure `ixWebSocket` is installed and linked properly.

3. Compile the server and client:
    ```bash
    g++ -std=c++11 main.cpp -o client -lixwebsocket
    g++ -std=c++11 servergit.cpp -o server -lixwebsocket
    ```

4. Run the server:
    ```bash
    ./server
    ```

5. Run the client:
    ```bash
    ./client
    ```

## Usage

### WebSocket Server (`servergit.cpp`)

- The WebSocket server listens on port 8080 (`ws://0.0.0.0:8080`).
- The server will print any received message from the client and send a response back.
- Keep the server running for client connections.

### WebSocket Client (`main.cpp`)

- The client connects to a WebSocket server.
- By default, it will connect to `ws://localhost:8080`. You can enter a different server address when prompted.
- The client sends messages to the server and displays server responses.
- Type `exit` to close the client connection.

### Example

1. Run the server:
    ```bash
    ./server
    ```
    Output:
    ```
    [Server/Info] Server is running on port 8080!
    ```

2. Run the client:
    ```bash
    ./client
    ```
    Output:
    ```
    Enter server IP (e.g., ws://*:8080), or 'y' if it's running locally: y
    Connecting...Connection established!
    [Client] Enter your message or type 'exit' to quit: Hello, Server!
    ```

3. The server will respond with:
    ```
    [Server/Info] Received message from client: Hello, Server!
    ```

## Notes

- The client waits for the server to respond before allowing further input.
- Both the client and server handle basic WebSocket messages (connect, message, error).
- The server can handle multiple clients, but the example here only uses a single client.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

