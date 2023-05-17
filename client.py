import socket

# Define the target IP address and port number
target_ip = '127.0.0.1'  # Replace with the IP address of the target machine
target_port = 12345  # Replace with the port number you want to send data to

# Create a socket object
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect the socket to the target IP address and port number
client_socket.connect((target_ip, target_port))

# Send data to the server
message = 'Hello, server!'
client_socket.send(message.encode())

# Receive data from the server
response = client_socket.recv(1024)
print(response.decode())

# Close the socket
client_socket.close()
