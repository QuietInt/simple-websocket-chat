/* Simple WebSocket Client */
/* by Baton Software → QuietInt, Baton Soft. CEO */
/* Created 3 May 2025 */
/* To use this client, you need to run a WebSocket server (compiled server.cpp) */

#include <iostream>
#include <ixwebsocket/IXWebSocketServer.h>
#include <mutex>
#include <condition_variable>

using namespace std;
using namespace ix;

int main()
{
    string msg;
    WebSocket webSocket;
    mutex mutex;
    condition_variable cv;
    bool connected = false;
    string url = "ws://localhost:8080"; // default to localhost

    webSocket.setUrl(url);
    webSocket.setOnMessageCallback([&connected, &cv](const WebSocketMessagePtr& msg)
    {
        if (msg->type == WebSocketMessageType::Message)
        {
            cout << msg->str << endl;
        }
        else if (msg->type == WebSocketMessageType::Open)
        {
            connected = true;
            cv.notify_one();
            cout << "Connection established!" << endl;
        }
        else if (msg->type == WebSocketMessageType::Error)
        {
            cout << "Connection error: " << msg->errorInfo.reason << endl; 
        }
    });

    string input;
    cout << "Enter server IP (e.g., ws://*:8080), or 'y' if it's running locally: ";
    getline(cin, input);
    if (input != "y")
    {
        url = input;
        webSocket.setUrl(url);
    }

    webSocket.start();
    cout << "Connecting...";

    {
        unique_lock<std::mutex> lock(mutex);
        if (!connected)
        {
            cv.wait_for(lock, chrono::seconds(5), [&connected]{ return connected; });
        }
    }

    if (connected)
    {
        while (true)
        {
            cout << "[Client] Enter your message or type 'exit' to quit: ";
            getline(cin, msg);
            if (msg == "exit")
            {
                break;
            }
            else
            {
                webSocket.send(msg + "\n");
            }
        }
    }
    else
    {
        cout << "\nFailed to connect to the server.";
        cout << "\nShutting down client...";
        return 1;
    }

    webSocket.stop();
}
