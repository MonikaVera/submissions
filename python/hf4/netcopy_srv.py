import socket
import select
import sys
import struct
import hashlib

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

class NetCopyTCPSelectServer(SimpleTCPSelectServer) :
    def __init__(self, addr='localhost', port=10001, timeout=1, checksum_addr='localhost', checksum_port=10000, file_path="new.txt", file_id='1000') :
        super().__init__(addr, port, timeout)
        self.end=False
        self.file_path=file_path
        self.file_id=file_id
        self.checksum_serv_addr=(checksum_addr, checksum_port)
    def closingServer(self) :
        if(len(self.inputs)==1) and self.inputs == [self.server]:
            self.inputs = []
    def handleDataFromClient(self, sock) :
        is_data=False
        with open(self.file_path, "ab") as file:
            data = sock.recv(1024)
            while data:
                is_data=True
                file.write(data)
                #print(data)
                data = sock.recv(1024)
        
        if is_data:
            with socket.socket() as conn_checksum:
                conn_checksum.connect(self.checksum_serv_addr)
                conn_checksum.sendall(bytes("KI|" + self.file_id, encoding="ascii"))
                data = conn_checksum.recv(1024)
                print(data.decode())
                parts=data.decode().split('|')
                if parts[0]!='0' :
                    test_string= self.file_id.encode('utf-8')
                    m = hashlib.md5()
                    m.update(test_string)
                    checksum = m.hexdigest()[:12]
                    if(checksum==parts[1]) :
                        print("CSUM OK") 
                else :
                    print("CSUM CORRUPTED")
                conn_checksum.close()   
        
try :
    if len(sys.argv) != 7:
        print("Usage: python3 netcopy_srv.py <srv_ip> <srv_port> <chsum_srv_ip> <chsum_srv_port> <file id> <filepath>")
    netCopyTCPSelectServer = NetCopyTCPSelectServer(sys.argv[1], int(sys.argv[2]), 1, sys.argv[3], int(sys.argv[4]), sys.argv[6], sys.argv[5])
    netCopyTCPSelectServer.handleConnections()   
except Exception as e:
    print(e)  
