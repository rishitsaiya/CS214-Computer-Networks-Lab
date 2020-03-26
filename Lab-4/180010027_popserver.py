import socket
import pickle
import json
import sys
from datetime import datetime
import threading
from collections import defaultdict

# Class for email elements
class Email : 
    def __init__(self , email_sender , email_receiver , subject , stamp , body) : 
        self.email_sender = email_sender
        self.email_receiver = email_receiver
        self.subject = subject
        self.stamp = stamp
        self.body = body

# List of emails from the database
email_rows = defaultdict(lambda : list())

# Check the username and password
def checkuser(username , password) : 
    filename = "User.txt"
    fil = open(filename , "r")
    data = fil.readlines()
    fil.close()

# Check the username and password
    for buff in data :
        line = buff.split(' ')
        if username == line[0] and password == line[1][:-1] : 
            return True
    return False

# Extracting the exact email from the database
def email_list(user) : 
    filename = "users/" + user + "/MyMailBox.txt"
    mailbox = open(filename , "r")
    with open(filename, "r") as f:
        data = [json.loads(line) for line in f.readlines()]
    mailbox.close()
    dictn = dict()

#Bifurcating according to the fields.
    for buff in data :
        dictn = buff
        email_sender = dictn["From"]
        email_receiver = dictn["To"]
        subject = dictn["Subject"]
        revd = dictn["Received"].split('\n')
        stamp = revd[0]
        body = revd[1]
        mail = Email(email_sender , email_receiver , subject , stamp , body)
        email_rows[user].append(mail)


# to create mail list for user (returns final string having all mails representatives)
def create_email_rows(username) : 
    output = "" 
    ind = 1
    for mail in email_rows[username] : 
        output += str(ind)  + ' ' +  mail.email_sender + ' ' + mail.stamp + ' ' +  mail.subject + '\n'
        ind += 1
    return output

# This is a test function used to dubug
def test() :
    """
        This is a buffer function.
    """
    for i in range (0,100) :
        print("*")

# Function to delete from the database
def remove_mail(username , id) : 
    mailbox = "users/" + username + "/MyMailBox.txt"
    
    # clears file
    mail = open(mailbox, 'w')
    mail.close()
    for i in range(len(email_rows[username])) : 
        if i == id-1 : 
            continue
        else : 
            
            # function to print in ouput file
            dictn = {
                "From": email_rows[username][i].email_sender,
                "To": email_rows[username][i].email_receiver,
                "Subject": email_rows[username][i].subject,
                "Received": email_rows[username][i].stamp + '\n' + email_rows[username][i].body
            }
            
            # Appending in the database file.
            with open(mailbox, 'a+') as obj:
                obj.write(json.dumps(dictn))
                obj.write('\n')

def new_thread(conn, addr):
    global email_rows
    # take string having username and pass seperated by space
    user_pass = conn.recv(1024).decode()
    user_pass = user_pass.split(' ')
    username = user_pass[0]
    password = user_pass[1]

    # Connection establishment from sender and reciever using sockets
    if checkuser(username , password) :
        conn.send("+OK POP3 Server ready.".encode())

    # Boundary Condition
    else :
        conn.send("404".encode())
        conn.close()

    # exit(-1)
    # Connection establishment from sender and reciever using sockets
    a = 5
    b = 6
    c = "Check"

    # Calling the make list function
    email_list(username)

    while True : 
        
        id = conn.recv(1024)
        id = id.decode()

        other = id.split(' ')
        print(other)
        # print(other[1])

        if id == "-1" : 
            break
            # conn.close()
        elif id == "00" :  
            # send number of email in mailbox
            # email rows is the list of all emails
            conn.send(str(len(email_rows[username])).encode())
        elif id == "000" :
            # send mails list tokens to client
            mails = create_email_rows(username)
            conn.send(mails.encode())
        else : 
            if other[0] == "1" : 
                id = int(other[1])
                if id < 1 or id > len(email_rows[username]) : 
                    conn.send("-2".encode())
                else : 
                    mail = pickle.dumps(email_rows[username][id - 1])
                    conn.send(mail)
            else : 
                id = int(other[1])
                # print(id)
                if id < 1 or id > len(email_rows[username]) : 
                    conn.send("-2".encode())
                else :
                    conn.send("1".encode())
                    remove_mail(username , id)
                    email_rows[username] = []
                    email_list(username)

# Connection establishment from sender and reciever using sockets
s = socket.socket(socket.AF_INET , socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind(('localhost' , int(sys.argv[1])))
s.listen(1)
print('Server is good to go.')

while 1:
    # Connection establishment from sender and reciever using sockets
    conn , addr = s.accept()
    # For multiple client
    # For Concurrent server
    threading._start_new_thread(new_thread, (conn, addr))


# print(c)

conn.close()

'''
------------------------------DIRECTIONS------------------------------------------------
CLI:

python3 180010027_popserver.py 50027

These are ouputs corresponding to inputs in 180010027_mailclient.py

********************************
Server is good to go.
['00']
['000']
['1', '2']
['0', '4']
['-1']
*********************************


'''