import sys
import json
import heapq
from collections import Counter

class Node:
    def __init__(self, byte=None, freq=0, left=None, right=None):
        self.byte = byte
        self.freq = freq
        self.left = left
        self.right = right
    def __lt__(self, other):
        return self.freq < other.freq

def build_tree(data_bytes):
    freq = Counter(data_bytes)
    pq = [Node(byte=b, freq=f) for b, f in freq.items()]
    heapq.heapify(pq)
    if len(pq) == 0:
        return None
    if len(pq) == 1:
        only = pq[0]
        return Node(byte=None, freq=only.freq, left=only, right=None)
    while len(pq) > 1:
        a = heapq.heappop(pq)
        b = heapq.heappop(pq)
        parent = Node(byte=None, freq=a.freq + b.freq, left=a, right=b)
        heapq.heappush(pq, parent)
    return pq[0]

def generate_codes(root):
    codes = {}
    def _gen(node, prefix):
        if node is None:
            return
        if node.left is None and node.right is None and node.byte is not None:
            codes[node.byte] = prefix if prefix != '' else '0'
            return
        _gen(node.left, prefix + '0')
        _gen(node.right, prefix + '1')
    _gen(root, '')
    return codes

def encode_file(input_path, output_path):
    with open(input_path, 'rb') as f:
        data = f.read()
    if len(data) == 0:
        header = {'codes': {}}
        with open(output_path, 'w') as out:
            out.write(json.dumps(header) + '\n')
            out.write('END_HEADER\n')
            out.write('')
        return
    root = build_tree(data)
    codes = generate_codes(root)
    # build mapping using ints as keys
    mapping = {str(b): codes[b] for b in codes}
    # build bitstring
    bitstring_parts = [mapping[str(b)] for b in data]
    bitstring = ''.join(bitstring_parts)
    # Write as text
    header = {'codes': mapping}
    with open(output_path, 'w') as out:
        out.write(json.dumps(header) + '\n')
        out.write('END_HEADER\n')
        out.write(bitstring)

def decode_file(input_path, output_path):
    with open(input_path, 'r') as f:
        header_line = f.readline().strip()
        end_marker = f.readline().strip()
        bitstring = f.read()
    
    if not header_line or end_marker != 'END_HEADER':
        with open(output_path, 'wb') as out:
            out.write(b'')
        return
    
    header = json.loads(header_line)
    mapping = header.get('codes', {})
    
    if len(mapping) == 0:
        with open(output_path, 'wb') as out:
            out.write(b'')
        return
    
    # reverse mapping
    rev = {v: int(k) for k, v in mapping.items()}
    # decode
    out_bytes = bytearray()
    cur = ''
    for bit in bitstring:
        cur += bit
        if cur in rev:
            out_bytes.append(rev[cur])
            cur = ''
    
    with open(output_path, 'wb') as out:
        out.write(bytes(out_bytes))

if __name__ == '__main__':
    if len(sys.argv) != 4:
        print('Usage: python huffman_py.py encode|decode input output')
        sys.exit(1)
    cmd = sys.argv[1]
    if cmd == 'encode':
        encode_file(sys.argv[2], sys.argv[3])
    elif cmd == 'decode':
        decode_file(sys.argv[2], sys.argv[3])
    else:
        print('Unknown command')
        sys.exit(1)

