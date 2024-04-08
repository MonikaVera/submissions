import sys
import struct

try:
    formats_in = ['9sif', 'f?c', 'ci9s','f9s?']
    for i in range(0, len(formats_in), 1) :
        packer_in = (struct.Struct(formats_in[i]))
        with open(sys.argv[1+i], 'rb') as f:
            packed_db = f.read(struct.calcsize(formats_in[i]))
            print((packer_in).unpack(packed_db))
except:
    print("An exception occured.")

formats_out = ['14si?','f?c','i12sf','ci15s']
values = [(b'elso', 72, True), (75.5, False, b'X'), (63, b'masodik', 82.9), (b'Z', 94, b'harmadik')]
for i in range(0, len(formats_out), 1) :
    packer_out = (struct.Struct(formats_out[i]))
    packed_data = packer_out.pack(*(values[i])) 
    print(packed_data)