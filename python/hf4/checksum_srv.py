import socket
import select
import sys
import struct
import time

#python3 checksum_srv.py localhost 10001

class SimpleTCPSelectServer :
    def __init__(self, addr='localhost', port=10001, timeout=1) :
        self.server = socket.socket()
        self.server.settimeout(1.0)
        self.server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.server.bind((addr, port))
        self.server.listen(5)
        self.inputs = [self.server]
        self.timeout = timeout
        self.checksums = []
        self.start_time = time.time()
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

class ChecksumTCPSelectServer(SimpleTCPSelectServer) :
    def __init__(self, addr='localhost', port=10001, timeout=1) :
        super().__init__(addr, port, timeout)
        self.checksums = []
        self.start_time = time.time()
    def handleDataFromClient(self, sock) :
        for item in self.checksums: 
            if(item["start_time"]+item["time_limit"]<time.time()) :
                self.checksums.remove(item)
        data = sock.recv(1024)
        print(data.decode())
        parts = (data.decode()).split("|")
        if parts[0]=="BE":
            self.checksums.append({"file_id": parts[1], "time_limit": int(parts[2]), "start_time": time.time(), "checksum_length":int(parts[3]), "checksum_bytes":parts[4]})
            sock.sendall(b"OK") 
        if parts[0]=="KI":
            found=False
            for item in self.checksums :
                if(item["file_id"]==parts[1]) :
                    sock.sendall(bytes(str(item["checksum_length"]) + "|" + item["checksum_bytes"], encoding='ascii')) 
                    found=True
            if not found :
               sock.sendall(b"0|") 
        self.inputs.remove(sock)
    
try :
    if len(sys.argv) != 3:
        print("Usage: python your_script.py <server_address> <server_port>")
    checksumTCPSelectServer = ChecksumTCPSelectServer(sys.argv[1], int(sys.argv[2]))
    checksumTCPSelectServer.handleConnections()  
except Exception as e:
    print(e)  