/* Simple WebSocket Server */
/* by Baton Software → QuietInt, Baton Soft. CEO */
/* Created 3 May 2025 */

#include <iostream>
#include <ixwebsocket/IXWebSocketServer.h>
#include <thread>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>

//function geting current time
std::string getCurrentUTCTime() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::gmtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

//function save message history
void saveMessage(const std::string& message) {
    std::ofstream file("chat_history.txt", std::ios::app);
    if (file.is_open()) {
        file << "[" << getCurrentUTCTime() << "] " << message << std::endl;
        file.close();
    }
}

//function read message history
std::string readChatHistory() {
    std::ifstream file("chat_history.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main()
{
    const std::string SERVER_OWNER = "Your_Name";  //name server owner, edit to your name
    const std::string SERVER_START_TIME = getCurrentUTCTime(); //server start time
    // Create server on port 8080, listening on all interfaces
    ix::WebSocketServer server(8080, "0.0.0.0");

    // Callback for each incoming message
    // Callback for each incoming message
server.setOnClientMessageCallback([&SERVER_OWNER, &SERVER_START_TIME](
    std::shared_ptr<ix::ConnectionState> connectionState,
    ix::WebSocket& webSocket,
    const ix::WebSocketMessagePtr& msg)
{
    if (msg->type == ix::WebSocketMessageType::Message)
    {
        std::string message = msg->str;
        std::cout << "\n[Server/Info] Received message from client: " << message << std::endl;
        
        // command callback
        if (message == "/history") {
            std::string history = readChatHistory();
            std::cout << "\n[Server/Info] Sending chat history to client" << std::endl;
            webSocket.send("\n=== Chat History ===\n" + history + "================\n");
        }
        else if (message == "/info") {
            std::stringstream info;
            info << "\n=== Server Info ===\n"
                 << "Server Owner: " << SERVER_OWNER << "\n"
                 << "Server Start Time (UTC): " << SERVER_START_TIME << "\n"
                 << "Current Time (UTC): " << getCurrentUTCTime() << "\n"
                 << "================\n";
            std::cout << "\n[Server/Info] Sending server info to client" << std::endl;
            webSocket.send(info.str());
        }
        else {
            // save message
            saveMessage(message);
            std::string timestampedMessage = "[" + getCurrentUTCTime() + "] " + message;
            std::cout << "[Server/Info] Message saved and timestamped: " << timestampedMessage << std::endl;
            webSocket.send(timestampedMessage);
        }
    }
    else if (msg->type == ix::WebSocketMessageType::Open)
    {
        std::cout << "\n[Server/Info] New client connected." << std::endl;
        webSocket.send("[Server] Welcome to the chat!");
    }
    else if (msg->type == ix::WebSocketMessageType::Close)
    {
        std::cout << "\n[Server/Info] Client disconnected." << std::endl;
    }
    else if (msg->type == ix::WebSocketMessageType::Error)
    {
        std::cout << "\n[Server/Error] Connection error: " << msg->errorInfo.reason << std::endl;
    }
});

    auto res = server.listen();
    if (!res.first)
    {
        std::cerr << "[Server/Error] Failed to start: " << res.second << std::endl;
        return 1;
    }

    server.start();
    std::cout << "\n=== Server Information ===\n"
                  << "Owner: " << SERVER_OWNER << "\n"
                  << "Start Time (UTC): " << SERVER_START_TIME << "\n"
                  << "Status: Running on port 8080\n"
                  << "======================\n" << std::endl;

    // Keep the server running
    while (true) std::this_thread::sleep_for(std::chrono::seconds(1));

    server.stop();
    return 0;
}
