import socket
import pickle
from datetime import datetime
import time
import sys
# from assets.core_modules.smtp.custom_mail_template import Mail
# from assets.core_modules.databases.custom_database_handler import DatabaseHandler
# from assets.core_modules.exeptions.custom_exeptions import WrongEmailFormat, EmptySubject, UserDoesNotExists

# import Mail from custom_mail_template, it has method to accept mail
# remember the error raised, convert it to byte using pickle
# send to mailbox, which will update the database

# class to take all the details for an email
class Email : 
    def __init__(self , email_sender , email_receiver , subject , stamp , body) : 
        self.email_sender = email_sender
        self.email_receiver = email_receiver
        self.subject = subject
        self.stamp = stamp
        self.body = body

# Input taken from here
def print_mail(mail) : 
    print("\nFrom: ",mail.email_sender)
    print("To: ",mail.email_receiver)
    print("Subject: ",mail.subject)
    print("Received: ",mail.stamp)
    print(mail.body)
    print("\n")

# Extracting the data from user.txt to compare with the given inputs
def checkuser(username, password):
    filename = "User.txt"
    userfile = open(filename , "r")
    data = userfile.readlines()
    userfile.close()


# Checking the username and password from the user.txt
    for ep in data :
        line = ep.split(' ')
        if username == line[0] and password == line[1][:-1] : 
            return True
    return False

# Function for SMTP server
def smtp_server(username, ip, port):

# Taking input from user
    sender = input("From: ")
    while '@' not in sender or sender.endswith('@') or sender.startswith('@'):
        print("Invalid Format")
        sender = input("\nFrom: ")

    receiver = input("To: ")
    
# Taking input from user    
    while '@' not in receiver or receiver.endswith('@') or receiver.startswith('@'):
        print("Invalid Format")
        receiver = input("To: ")
    
    subject = input("Subject: ")

# Boundary Cases
    while len(subject) == 0:
        print("Invalid Subject")
        subject = input("Subject: ")
    
    body, message_block = '', ''

# Boundary Cases    
    print('Email body (end with "." on a new line): ', end='')
    while message_block != '.':
        body += message_block
        message_block = input()

# Socket specifications taken from internet
    serverPort = port
    serverName = ip
    clientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    clientSocket.connect((serverName,serverPort))

#  Initialisation
    init = "HELO " + serverName
    clientSocket.sendto(init.encode(), (serverName, serverPort))
    server_ack = clientSocket.recv(1024)

# Client socket linking
    print ("Server: ", server_ack.decode())
    mail_from = "MAIL FROM: " + sender
    clientSocket.sendto(mail_from.encode(), (serverName, serverPort))
    sender_ok = clientSocket.recv(1024)

# Server and client connection establishment using sockets
    print ("Server: ", sender_ok.decode())
    rcpt_to = "RCPT TO: " + receiver
    clientSocket.sendto(rcpt_to.encode(), (serverName, serverPort))
    recipient_ok = clientSocket.recv(1024)

# # Server and client connection establishment using sockets
    print ("Server: ", recipient_ok.decode())
    data = "DATA: " + body

    received_at = datetime.now()
    received_at = received_at.strftime("%m/%d/%Y, %H:%M:%S")

    clientSocket.sendto(data.encode(), (serverName, serverPort))
    time.sleep(1)

    clientSocket.sendto(subject.encode(), (serverName, serverPort))
    time.sleep(1)

# Server and client connection establishment using sockets
    # print(received_at)
    clientSocket.sendto(received_at.encode(), (serverName, serverPort))
    time.sleep(1)

# Server and client connection establishment using sockets

    clientSocket.sendto(body.encode(), (serverName, serverPort))
    acceptance = clientSocket.recv(1024)
    print ("Server: ", acceptance.decode())
    quit = "QUIT"
    clientSocket.sendto(quit.encode(), (serverName, serverPort))    
    clientSocket.close()

#function to manage mails using pop3                    
def pop_func(username, password, ip, port):
    # pass
    # print("Entered into pop ")
    s = socket.socket(socket.AF_INET , socket.SOCK_STREAM)
    s.connect((ip , int(port)))

    user_pass = username + ' ' + password
    s.send(user_pass.encode())


    stat =  s.recv(1024).decode()
    if stat == "404" : 
        print("Username or password is incorrect !")
        exit(-1)
    else :
        print(stat)

    print("")
    while True :

        print("Enter command code : ")
        print("\nSTAT : Counts the number of emails.\nLIST : Lists all the email for the user.\nRETR : Retrieves email based on the serial number.\nDELE : Deletes the email based on the serial number.\nQUIT : Closes the connection and terminate the program.")

        command = input("Enter command code : ")
        multi = command
        multi = command.split(' ')
        data = ""
        if multi[0] == "RETR" : 
            data += "1 " + str(multi[1])
        elif multi[0] == "DELE" : 
            data += "0 " + str(multi[1])
        else : 
            pass


        if command == "QUIT" : 
            s.send("-1".encode())
            print("+OK\nGoodbye!\n")
            s.close()
            break
        elif command == "STAT" : 
            s.send("00".encode())
            no_mails = s.recv(1024)
            print(f'Total no of emails: {no_mails.decode()}')
        elif command == "LIST" : 
            s.send("000".encode())
            show = s.recv(1024)
            show = show.decode()
            print(show)
        else : 
            s.send(data.encode())
            time.sleep(1)
            if data[0] == "1" : 
                in_mail = s.recv(1024)
                try :
                    if in_mail.decode() == "-2" : 
                        print("index out of bounds!!")
                        continue
                except : 
                    in_mail = pickle.loads(in_mail)
                    print_mail(in_mail)
            elif data[0] == "0" : 
                status_of_deleted_email = s.recv(1024)
                # Boundary case
                if status_of_deleted_email.decode() == "-2" : 
                    print("Email at the index " , data[2] , "does not exist!")
                # Deleting the index
                else : 
                    print("Email deleted at index " , data[2], '\n')
    # try:
    #     user_inbox = DatabaseHandler().get_inbox_of(username)
    # except UserDoesNotExists:
    #     print('No Such User Exists.')
    #     return None
    # except Exception as e:
    #     print(f'Exception: {e}, while working with inbox of {username}')
    #     print(f'checking var {e.random_variable}, checking isnstance: {isinstance(e, UserDoesNotExists)}, type: {type(e)}, class type: {type(UserDoesNotExists)}')
    #     return None

    # for index, mail in enumerate(user_inbox):
    #     print(f"{index} {mail['senders_email']} {mail['receiving_date_time']} {mail['subject']}")

    # number_of_mails = len(user_inbox)

    # response = int(input())
    # if response == -1:
    #     return None
    # elif response >= number_of_mails:
    #     print('Mail no. out of range, give again')
    #     work_with_inbox(username, password, ip, port)
    #     return None
    # else:
    #     display_or_delete(username, user_inbox[response])


username = input('Enter Username: ')
password = input('Enter Password: ')

# Management for POP3 server
if(checkuser(username,password)):
    
    # For all pre-statements in POP3 server
    while True:
        print("\nEnter command code : ")
        print("\n1.Manage Mail: Shows the stored mails of logged in user only \n2.Send Mail: Allows the user to send a mail \n3.Quit: Quits the program\n")
        
        try:
            response = int(input())
            
            # Boundary condition
            if response > 3 or response < 1:
                raise ValueError
            
            # Passing arguments
            elif response == 1:
                pop_func(username, password, sys.argv[1], int(sys.argv[2]))

            # Passing the arguments
            elif response == 2:
                smtp_server(username, sys.argv[1], int(sys.argv[2]))
            
            # At the end of the program
            else:
                print('Thank you.')
                sys.exit(0)

        except KeyboardInterrupt:
            print('Use Given Options To Exit!')
        except ValueError:
            print('Enter valid values')
        except Exception as e:
            print(f'Error {e} occured')
            break

# Boundary cases
else:
    print('Wrong Credentials. Please try again.\n')
    interact_user(sys.argv[1],int(sys.argv[2]))

'''
------------------------------DIRECTIONS------------------------------------------------
CLI:

python3 180010027_mailclient.py 127.0.0.1 50027


GENERAL: -------------------------------------

Enter Username: a@b
Enter Password: pass

Enter command code : 

1.Manage Mail: Shows the stored mails of logged in user only 
2.Send Mail: Allows the user to send a mail 
3.Quit: Quits the program

**************************************************************
1
+OK POP3 Server ready.

Enter command code : 

STAT : Counts the number of emails.
LIST : Lists all the email for the user.
RETR : Retrieves email based on the serial number.
DELE : Deletes the email based on the serial number.
QUIT : Closes the connection and terminate the program.

STAT
Total no of emails: 4

LIST
1 d@e 03/07/2020, 19:11:08 hello
2 l@y 03/07/2020, 21:56:46 sub
3 c@d 03/08/2020, 04:51:25 pass
4 bb@cc 03/08/2020, 04:52:19 pass

RETR 2
From:  l@y
To:  a@b
Subject:  sub
Received:  03/07/2020, 21:56:46
new

DELE 4
Email deleted at index  4 

QUIT
+OK
Goodbye!

******************************************************************
2
From: a@b
To: c@d
Subject: hi
Email body (end with "." on a new line): hello
.
Server:  250 OK Hello 127.0.0.1
Server:  250 a@b... Sender ok
Server:  250 root... Recipient ok
Server:  250 OK Message accepted for delivery

******************************************************************
3
Thank you.

******************************************************************
'''