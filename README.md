# Group Members:
### Shreya Chati, Samprita Chakraborty


# UDP Server (server_c_udp.c)

Listens on a specified port for incoming UDP messages.

Receives a string from a client.

If the string contains any non-numeric characters, it responds with an error message.

If the string contains only digits, the function addDigits() adds all digits together.

Then the server Sends the sum back to the client.

Repeats the process with the new sum until a single-digit result is reached.

The server runs forever, so it can continue to handle requests from the client.

# UDP Client (client_c_udp.c)

Connects to a server at a specified IP address and port, then prompts the user to enter a string.

Sends the string to the server via UDP.

Receives and prints the server’s response.

For numeric input, it prints each sum until the single-digit result.

For non-numeric input, it prints the error message.

# TCP Server (server_c_tcp)

Listens on a specified port for incoming TCP connections.

Accepts a client connection and reads a string of up to 128 characters.

If the string contains any non-numeric characters, responds with:

"Sorry, cannot compute!"

and closes the connection.

If the string contains only digits:

Adds all digits together.

Sends the sum back to the client.

Repeats this process until a single-digit result is reached.

Closes the client connection after processing, then waits for new connections.

# TCP Client (client_c_tcp)

Connects to a server at a specified hostname/IP address and port.

Prompts the user to enter a string.

Sends the string to the server over TCP.

Reads and prints the server’s response(s) until the connection closes:

For numeric input, prints each sum until the single-digit result.

For non-numeric input, prints the error message.