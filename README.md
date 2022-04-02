# Tiny Web Server


## Description
- **Tiny** is a simple, iterative `HTTP/1.0 Web` server that uses `GET` method to serve static and dynamic content.
- It combines many important ideas like process control, Unix I/O, socket interface and HTTP protocol.
- While it lacks many of real web servers features, it can serve serve both static and dynamic content to a real Web browser.

## How does Tiny works?
- The `main` routine opens a listen port to recieve the connection requests.
- The `server` function does its job through:
    1) parse the HTTP request.
    2) parse the uri.
    3) serve the content.

## Requirements
- Linux or any UNIX-like OS.
- git.
- gcc.
- make.

## How to test Tiny?
- Use the following commands to download and compile the source code:
```
    git clone https://github.com/IslamWalid/tiny_web_server
    cd tiny_web_server
    make
```
- Run Tiny:
```
./tiny <port>
```
- Send an HTTP request to it using:

    **telnet:**
    ```
    telnet localhost <port>
    ```
    **Web browser:**
    ```
    http://localhost:<port>
    ```

**Example using telnet:**

![picture alt](https://raw.githubusercontent.com/IslamWalid/tiny_web_server/master/screen-shots/telnet_example.png "telnet adder example")

**Example using browser:**

![picture alt](https://raw.githubusercontent.com/IslamWalid/tiny_web_server/master/screen-shots/browser_example(1).png "browser home page example")

**Another example using browser:**

![picture alt](https://raw.githubusercontent.com/IslamWalid/tiny_web_server/master/screen-shots/browser_example(2).png "browser adder example")
