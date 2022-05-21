# Socket Chat Application in Linux

A basic chat application having client-server architecture, developed using multi-threaded socket programming.

<br/>

### **Overview**

- Single multi-threaded server to handle multiple clients.
- A client can talk to another client during a session if the requested client is free.
- All messages are sent through TCP socket.
- One mutex mapped to each client to avoid race condition while connecting two clients.

<br/>

### **Setup and use locally**

1. Clone the repo and change directory

```
cd socket-chat-in-linux
```

2. Compile the `server.cpp` and `client.cpp` files

```
g++ server.cpp -o server -lpthread
g++ client.cpp -o client -lpthread
```

4. Run the server

```
./server <port_no>
```

5. Run the clients

<details>
  <summary> For each client open 2 terminals . Why ? :confused:</summary>
    <br/>
  <p>Role of terminals</p>

  <ul>
    <li>Terminal 2 is used to compile, run and send command(or message) to server(or peer connected client).</li>
    <li>Terminal 1 displays the chat messages (both sent and recieved). Client need NOT to type anything in this terminal.</li>
    <li>The stdout of client program is redirected to terminal 2 using > operator. `tty` of terminal 1 is needed for this.</li>
  </ul>
  
  <p>If output is not redirected, then consider the following scenario:
  </p>
    
  <ul>
    <li>client x1 and client x2 are chatting with each other</li>
    <li>client x1 is typing some messages and in between client x2 sends some message.</li>
    <li>Now this recieved messaged will get printed on the same line on which client x1 has its partially typed message.
    And this is definitely not desired.</li>
  </ul>
  
  <p>Hence, for each client, the chat message(both sent and recieved) are directed to show in another terminal.</p>

</details>
<br/>

- Terminal 1

```
tty
```

- Terminal 2

```
./client <host_name> <port_no> > <output-of-tty-in-terminal-1>
```

<br/>

### **Commands**

1. Server side
<table>
  <tr>
    <td> <h4>Command</h4> </td>
    <td> <h4>Use</h4> </td>
  </tr>
  <tr>
    <td> get clients </td>
    <td> show status(FREE / BUSY with whom) of active clients </td>
  </tr>
  <tr>
    <td> get free_clients </td>
    <td> show list of free clients </td>
  </tr>
</table>

<br/>

2. Client side

<table>
  <tr>
    <td> <h4>Command</h4> </td>
    <td> <h4>Use</h4> </td>
  </tr>
  <tr>
    <td> get clients </td>
    <td> get status(FREE / BUSY) of active clients </td>
  </tr>
   <tr>
    <td> connect X </td>
    <td> connect with client X </td>
  </tr>
  <tr>
    <td> goodbye </td>
    <td> disconnect with currently connected client. </td>
  </tr>
  <tr>
    <td> close </td>
    <td> close the connection with server </td>
  </tr>
  </table>
