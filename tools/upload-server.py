#!/usr/bin/env python3

from http.server import BaseHTTPRequestHandler, HTTPServer
import os

class FileUploadHandler(BaseHTTPRequestHandler):
    def do_POST(self):
        content_length = int(self.headers['Content-Length'])
        data = self.rfile.read(content_length)
        with open('uploaded_file.txt', 'wb') as f:
            f.write(data)
        self.send_response(200)
        self.end_headers()
        self.wfile.write(b'File uploaded successfully')

def run(server_class=HTTPServer, handler_class=FileUploadHandler, port=8000):
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    print(f'Starting server on port {port}')
    httpd.serve_forever()

if __name__ == '__main__':
    run()


# curl -X POST -H "Content-Type: application/octet-stream" --data-binary @local_file.txt http://localhost:8000
