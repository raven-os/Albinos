"""
Controller to send and receive messages
"""

import socket

class Controller:

    def __init__(self, socket_path: str):
        self.socket = socket.socket(socket.AF_UNIX)
        self.socket.connect(socket_path)

    def setblocking(self, block):
        self.socket.setblocking(block)

    def send(self, message: bytes):
        return self.socket.sendall(message)

    def recv(self, nb_bytes: int=4096) -> bytes:
        return self.socket.recv(nb_bytes)
