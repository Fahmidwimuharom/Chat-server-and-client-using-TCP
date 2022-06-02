from concurrent.futures import thread
import threading
import socket

#IP Localhost
host = '127.0.0.1'
#port
port = 12000 

#Create server
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
result = client.connect_ex((host, port))

#Check if Server is online or offline
if result == 0:
    nickname = input("Input your name: ")
else:
    print("Server offline...")
    client.close()
    quit()

#Receiving incoming messages from the server
def recieve():
    while True:
        try:
            message = client.recv(1024).decode('ascii')
            if message == 'nick': 
                client.send(nickname.encode('ascii'))
            else: #if not print the message
                print(message) 
        except:
            print("An Error have occured")
            client.close()
            breaks

#write message and send to server
def write():
    while True:
        message = f'{nickname}: {input("")}'
        client.send(message.encode('ascii'))

recieve_thread = threading.Thread(target = recieve)
recieve_thread.start()

write_thread = threading.Thread(target = write)
write_thread.start()