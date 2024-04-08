import socket
import sys
import struct
#import time

class NumberGuessingTCPSelectClient:
    def __init__(self, serverAddr = 'localhost', serverPort=10001) :
        format_='ci'
        self.start = 1
        self.end = 100
        self.packer = (struct.Struct(format_))
        self.client = socket.socket()
        self.client.connect((serverAddr, serverPort))
        self.exit=False
    def handleIncomingMessageFromServer(self) :
        data = self.client.recv(4096)
        response = (self.packer.unpack(data))[0]
        print(response)
        if response == b'Y' or response == b'K' or response == b'V':
            self.exit=True
        elif response == b'I' :
            self.start=self.half+1
        elif response == b'N' :
            self.end=self.half
    def handleConnection(self) :
        while not self.exit:
            #time.sleep(2)
            if self.end==self.start :
                self.half = self.start
                op = "="
            else :
                self.half = round((self.end-self.start+1)/2+self.start-1)
                op = ">"
            values=(bytes(op[0], encoding='ascii'), int(self.half))
            self.client.sendall(self.packer.pack(*values))
            self.handleIncomingMessageFromServer()
try:
    if len(sys.argv) != 3:
        print("Usage: python your_script.py <server_address> <server_port>")
    simpleTCPSelectClient = NumberGuessingTCPSelectClient(sys.argv[1], int(sys.argv[2]))
    simpleTCPSelectClient.handleConnection()
except Exception as e:
    print(e)
