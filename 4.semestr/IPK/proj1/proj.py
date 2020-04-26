#toto bude projekt do IPK

#Projekt 1 - HTTP resolver doménových jmen
#Autor: Václav Doleček

#Datum začátku: 20.02.2020
#Motivace: Chuť udělat tento předmět

import socket
import re
#import json
#import sys
#import unicodedata
#from pathlib import Path

server_host = '127.0.0.1'

server_port = 5353

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s_s: #s_s znamená server_socket
    s_s.bind((server_host, server_port))
    s_s.listen()
    print('I am listenig and bnided\n')
    conn, addr = s_s.accept()
    print('conn,addr accepted\n')
    with conn:
        print('I am sever and I am alive\n')
        print('Connected by', addr)
        while True:
            s_data = conn.recv(1024)
            if not s_data:
                break
            print(s_data)
            print('\n')
            get_var = re.search(".+GET.+",s_data)
            if get_var == 'GET'
                print('I found GET\n')
            conn.sendall(s_data)













