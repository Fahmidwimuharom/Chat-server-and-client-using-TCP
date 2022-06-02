import threading
import socket

#IP Localhost
host = '127.0.0.1'
#port
port = 12000 

#Create server
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#Bind server to IP
server.bind((host, port)) 
#Server Listening
server.listen() 

#Client & Name
clients = []
nicknames = []

#Broadcast messages from client to client
def broadcast(message):
    for client in clients:
        client.send(message)

#handle clients when sending messages or receiving messages
def handle(client):
    while True:
        try:
            message = client.recv(1024)
            broadcast(message)
        except:
            index = clients.index(client)
            clients.remove(client)
            client.close()
            nickname = nicknames[index]
            broadcast(f'{nickname} LEFT THE CHAT'.encode('ascii'))
            print(f'{nickname} DISCONNETCTED')
            nicknames.remove(nickname)
            break

#The server accepts when a new client connects
def recieve():
    while True:
        client, address = server.accept()
        print(f"Connected with {str(address)}")
        
      
        client.send('nick'.encode('ascii'))
        nickname = client.recv(1024).decode('ascii')
        nicknames.append(nickname)
        clients.append(client)
        
        print(f'Client Name is {nickname}...')
        broadcast(f'{nickname} joined the chat...'.encode('ascii'))
        client.send('Connected to the server...'.encode('ascii'))

        thread = threading.Thread(target = handle, args = (client,))
        thread.start()

print("Server is listening...")
recieve() 