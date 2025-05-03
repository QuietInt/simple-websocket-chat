/* Simple WebSocket Server */
/* by Baton Software → QuietInt, Baton Soft. CEO */
/* Created 3 May 2025 */

#include <iostream>
#include <ixwebsocket/IXWebSocketServer.h>
#include <thread>
#include <chrono>

int main()
{
    // Create server on port 8080, listening on all interfaces
    ix::WebSocketServer server(8080, "0.0.0.0");

    // Callback for each incoming message
    server.setOnClientMessageCallback([](std::shared_ptr<ix::ConnectionState> connectionState,
                                         ix::WebSocket& webSocket,
                                         const ix::WebSocketMessagePtr& msg)
    {
        if (msg->type == ix::WebSocketMessageType::Message)
        {
            std::cout << "\n[Server/Info] Received message from client: " << msg->str << std::endl;
            webSocket.send("\n[Server/Info] Server received: " + msg->str);
        }
    });

    auto res = server.listen();
    if (!res.first)
    {
        std::cerr << "[Server/Error] Failed to start: " << res.second << std::endl;
        return 1;
    }

    server.start();
    std::cout << "\n\t[Server/Info] Server is running on port 8080!" << std::endl;

    // Keep the server running
    while (true) std::this_thread::sleep_for(std::chrono::seconds(1));

    server.stop();
    return 0;
}
