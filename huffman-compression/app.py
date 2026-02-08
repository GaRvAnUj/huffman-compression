from flask import Flask, request, send_file, render_template
import subprocess
import os
import tempfile
import sys
from io import BytesIO

app = Flask(__name__)
BASE_DIR = os.path.dirname(os.path.abspath(__file__))


@app.route('/')
def index():
    return render_template('index.html')


@app.route('/compress', methods=['POST'])
def compress():
    f = request.files.get('file')
    if not f:
        return "No file", 400
    with tempfile.TemporaryDirectory() as td:
        in_path = os.path.join(td, 'input.txt')
        out_path = os.path.join(td, 'output.huff')
        f.save(in_path)
        script = os.path.join(BASE_DIR, 'src', 'huffman_py.py')
        try:
            proc = subprocess.run([sys.executable, script, 'encode', in_path, out_path], 
                                capture_output=True, text=True, timeout=30)
            if proc.returncode != 0:
                return f"Error: {proc.stderr}", 500
            if not os.path.exists(out_path):
                return "Compression failed: output file not created", 500
            # Read file into memory before temp dir is deleted
            with open(out_path, 'rb') as file:
                data = file.read()
            file_obj = BytesIO(data)
            return send_file(file_obj, as_attachment=True, download_name='compressed.txt', mimetype='text/plain')
        except subprocess.TimeoutExpired:
            return "Compression timed out", 500
        except Exception as e:
            return f"Error: {str(e)}", 500


@app.route('/decompress', methods=['POST'])
def decompress():
    f = request.files.get('file')
    if not f:
        return "No file", 400
    with tempfile.TemporaryDirectory() as td:
        in_path = os.path.join(td, 'input.txt')
        out_path = os.path.join(td, 'output.txt')
        f.save(in_path)
        script = os.path.join(BASE_DIR, 'src', 'huffman_py.py')
        try:
            proc = subprocess.run([sys.executable, script, 'decode', in_path, out_path], 
                                capture_output=True, text=True, timeout=30)
            if proc.returncode != 0:
                return f"Error: {proc.stderr}", 500
            if not os.path.exists(out_path):
                return "Decompression failed: output file not created", 500
            # Read file into memory before temp dir is deleted
            with open(out_path, 'rb') as file:
                data = file.read()
            file_obj = BytesIO(data)
            return send_file(file_obj, as_attachment=True, download_name='decoded.txt', mimetype='text/plain')
        except subprocess.TimeoutExpired:
            return "Decompression timed out", 500
        except Exception as e:
            return f"Error: {str(e)}", 500


if __name__ == '__main__':
    app.run(debug=True)
