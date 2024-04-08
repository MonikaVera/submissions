import socket
import select
import sys
import struct
import hashlib

#python3 netcopy_cli.py localhost 10000 localhost 10001 1000 valami.txt
try :
    if len(sys.argv) != 7:
        print("Usage: python3 netcopy_cli.py <srv_ip> <srv_port> <chsum_srv_ip> <chsum_srv_port> <file id> <filepath>")
    netcopy_serv_addr = (sys.argv[1], int(sys.argv[2]))
    checksum_serv_addr = (sys.argv[3], int(sys.argv[4]))
    file_id=(sys.argv[5])
    file_path=sys.argv[6] 

    with socket.socket() as conn_netcopy:
        conn_netcopy.connect(netcopy_serv_addr)
        with open(file_path, "rb") as file:
            data = file.read(1024)
            while data:
                conn_netcopy.sendall(data)
                data = file.read(1024)
        conn_netcopy.close()

    test_string= file_id.encode('utf-8')
    m = hashlib.md5()
    m.update(test_string)
    checksum = m.hexdigest()[:12]
    print(checksum)

    with socket.socket() as conn_checksum:
        conn_checksum.connect(checksum_serv_addr)
        conn_checksum.sendall(bytes("BE|" + file_id + "|60|12|" + checksum, encoding="ascii"))
        data = conn_checksum.recv(1024)
        print(data)
        conn_checksum.close()     

except Exception as e:
    print(e)  

