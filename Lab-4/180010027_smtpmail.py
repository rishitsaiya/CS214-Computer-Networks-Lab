import os
import socket
import pickle
import json
import sys
import threading

# Save email 
def save_email(fields):
    address = "users/" + fields["To"] + "/MyMailBox.txt"
    print(address)
    with open(address, 'a+') as write_obj:
        write_obj.write(json.dumps(fields))
        write_obj.write('\n')

# For Multiple Client
# For Concurrent Server
def new_thread(connectionSocket, addr): 
    # Connection establishment from client and server using sockets
    init = connectionSocket.recv(1024)
    print("Client: ", init.decode())
    init = init.decode()

# Connection establishment from client and server using sockets
    serverName = init[5:]
    server_ack = "250 OK Hello " + serverName
    connectionSocket.send(server_ack.encode())

# Connection establishment from client and server using sockets
    email_sender = connectionSocket.recv(1024)
    print("Client: ", email_sender.decode())
    email_sender = email_sender.decode()

# Connection establishment from client and server using sockets
    email_sender = email_sender[11:]
    sender_ok = "250 " + email_sender + "... Sender ok"
    connectionSocket.send(sender_ok.encode())

# Connection establishment from client and server using sockets
    email_receiver = connectionSocket.recv(1024)
    print("Client: ", email_receiver.decode())
    email_receiver = email_receiver.decode()

# Connection establishment from client and server using sockets
    email_receiver = email_receiver[9:]
    recipient_ok = "250 root... Recipient ok"
    connectionSocket.send(recipient_ok.encode())

# Connection establishment from client and server using sockets
    data = connectionSocket.recv(1024)
    print("Client: ", data.decode())
    subject = connectionSocket.recv(1024)
    received_at = connectionSocket.recv(1024)
    emailbody = connectionSocket.recv(1024)
    print(received_at.decode('utf-8'))
    # print(subject.decode())

    # print(emailbody.decode())
    buff = {
        "From": email_sender,
        "To": email_receiver,
        "Subject": subject.decode(),
        "Received": received_at.decode() + '\n' + emailbody.decode()
    }
    print(buff)
    save_email(buff)

    # Acceptance from socket    
    acceptance = "250 OK Message accepted for delivery"
    connectionSocket.send(acceptance.encode())
    quit = connectionSocket.recv(1024)
    print("Client: ", quit.decode())

# Connection establishment from client and server using sockets
serverPort = int(sys.argv[1])
serverSocket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
serverSocket.bind(("localhost",serverPort))
serverSocket.listen(1)

# Connection establishment from client and server using sockets

print ("The server is good to go!")

while 1:
    connectionSocket, addr = serverSocket.accept()
    
    # For Multiple Client
    # For Concurrent Server
    
    threading._start_new_thread(new_thread, (connectionSocket, addr))

connectionSocket.close()

'''
------------------------------DIRECTIONS------------------------------------------------
CLI:

python3 180010027_smtpmail.py 50027

******************************************************************

The server is good to go!
Client:  HELO 127.0.0.1
Client:  MAIL FROM: a@b
Client:  RCPT TO: c@d
Client:  DATA: hello
03/08/2020, 07:02:15
{'From': 'a@b', 'To': 'c@d', 'Subject': 'hi', 'Received': '03/08/2020, 07:02:15\nhello'}
users/c@d/MyMailBox.txt
Client:  QUIT

******************************************************************
'''