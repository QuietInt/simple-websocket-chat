/* Simple WebSocket Client */
/* by Baton Software → QuietInt, Baton Soft. CEO */
/* Created 3 May 2025 */
/* To use this client, you need to run a WebSocket server (compiled servergit.cpp) */

#include <iostream>
#include <ixwebsocket/IXWebSocketServer.h>
#include <mutex>
#include <condition_variable>
#include <iomanip>
#include <sstream>

//function for neat formatting
void printFormattedMessage(const std::string& message) {
    if (message.substr(0, 3) == "===") {
        std::cout << "\n" << message << std::endl;
    }
    else {
        std::cout << message << std::endl;
    }
}

int main()
{
    std::string msg;
    ix::WebSocket webSocket;
    std::mutex mutex;
    std::condition_variable cv;
    bool connected = false;
    std::string url = "ws://localhost:8080"; // default to localhost

    webSocket.setUrl(url);
    webSocket.setOnMessageCallback([&connected, &cv](const ix::WebSocketMessagePtr& msg)
    {
        if (msg->type == ix::WebSocketMessageType::Message)
        {
            printFormattedMessage(msg->str);
        }
        else if (msg->type == ix::WebSocketMessageType::Open)
        {
            connected = true;
            cv.notify_one();
            std::cout << "Connection established!" << std::endl;
            std::cout << "\nAvailable commands:" << std::endl;
            std::cout << "/history - show chat history" << std::endl;
            std::cout << "/info    - show server information" << std::endl;
            std::cout << "exit     - close connection" << std::endl;
        }
        else if (msg->type == ix::WebSocketMessageType::Error)
        {
            std::cout << "Connection error: " << msg->errorInfo.reason << std::endl; 
        }
    });
    std::string input;
    std::cout << "Enter server IP (e.g., ws://*:8080), or 'y' if it's running locally: ";
    std::getline(std::cin, input);
    if (input != "y")
    {
        url = input;
        webSocket.setUrl(url);
    }

    webSocket.start();
    std::cout << "Connecting...";

    {
        std::unique_lock<std::mutex> lock(mutex);
        if (!connected)
        {
            cv.wait_for(lock, std::chrono::seconds(5), [&connected]{ return connected; });
        }
    }

    if (connected)
    {
        while (true)
        {
        std::cout << "\n[Client] Enter your message or command: ";  
        std::getline(std::cin, msg);
            if (msg == "exit")
            {
                break;
            }
            else
            {
                webSocket.send(msg);  
            }
        }
    }
    else
    {
        std::cout << "\nFailed to connect to the server.";
        std::cout << "\nShutting down client...";
        return 1;
    }

    webSocket.stop();
}
