import socket
import select
import sys
import random
import struct

# python3 netcopy_srv.py localhost 10000 localhost 10001 1000 valami2.txt
#4-5 alfeladat
#fel is kell tölteni zipelve a kódot meg be is kell másolni
#mindent lehet használni kivéve ChatGPT-t

class SimpleTCPSelectServer :
    def __init__(self, addr='localhost', port=10001, timeout=1) :
        self.server = socket.socket()
        self.server.settimeout(1.0)
        self.server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.server.bind((addr, port))
        self.server.listen(5)
        self.inputs = [self.server]
        self.timeout = timeout
    def handleNewConnection(self, sock) :
        conn, client_addr = sock.accept()
        conn.settimeout(1.0)
        self.inputs.append(conn)
    def handleDataFromClient(self, sock) :
        pass
    def handleInputs(self, readable) :
        for sock in readable:
            if sock is self.server:
                self.handleNewConnection(sock)
            else :
                self.handleDataFromClient(sock)
    def handleConnections(self) :
        while self.inputs :
            try:
                readable, writable, exceptional = select.select(self.inputs, [], [], self.timeout)
                if not (readable or writable or exceptional) :
                    continue
                self.handleInputs(readable)
            except KeyboardInterrupt:
                print('Close')
                for c in self.inputs:
                    c.close()
                self.inputs = []

class NumberGuessingTCPSelectServer(SimpleTCPSelectServer) :
    def __init__(self, addr='localhost', port=10001, timeout=1) :
        super().__init__(addr, port, timeout)
        self.num = random.randrange(1,100,1)
        self.packer=struct.Struct('ci')
        self.end=False
        print(self.num)
    def closingServer(self) :
        if(len(self.inputs)==1) and self.inputs == [self.server]:
            self.inputs = []
    def handleDataFromClient(self, sock) :
        data = sock.recv(1024)
        if data:
            data_un = (self.packer.unpack(data))
            print(data_un)
            if ((data_un[0]==b'<') or (data_un[0]==b'>')) and not(self.end) :
                if (data_un[0]==b'<' and self.num<data_un[1]) or (data_un[0]==b'>' and self.num>data_un[1]) :
                    values=(b'I',0)
                elif (data_un[0]==b'<' and not(self.num<data_un[1])) or (data_un[0]==b'>' and not(self.num>data_un[1])) :
                    values=(b'N',0)                    
                sock.sendall(self.packer.pack(*values))
            else :
                if self.end==True :
                    values=(b'V',0)
                elif data_un[0]==b'=' :
                    if data_un[1]==self.num :
                        self.end=True
                        values=(b'Y',0)
                    else :
                        values=(b'K',0)
                sock.sendall(self.packer.pack(*values))
                sock.close()
                self.inputs.remove(sock)
                self.closingServer()
try :
    if len(sys.argv) != 3:
        print("Usage: python your_script.py <server_address> <server_port>")
    simpleTCPSelectServer = NumberGuessingTCPSelectServer(sys.argv[1], int(sys.argv[2]))
    simpleTCPSelectServer.handleConnections()  
except Exception as e:
    print(e)  