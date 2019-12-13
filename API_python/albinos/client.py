"""
Client connexion and action module
"""

from typing import List, Dict
import json
from albinos.controller import Controller
from albinos.config import Config
from albinos.error import check_response
import socket

class Client:

    def __init__(self, socket_path: str):
        self.controller = Controller(socket_path)
        self.loaded = {}

    def create(self, name: str) -> List[str]:
        self.controller.send(bytes(f'{{"REQUEST_NAME": "CONFIG_CREATE", "CONFIG_NAME": "{name}"}}', "utf-8"))
        response = self.controller.recv(4096)
        response = json.loads(response)
        check_response(response)
        return [response["CONFIG_KEY"], response["READONLY_CONFIG_KEY"]]

    def load(self, config_key: str) -> Config:
        self.controller.send(bytes(f'{{"REQUEST_NAME": "CONFIG_LOAD", "CONFIG_KEY": "{config_key}", "READONLY_CONFIG_KEY": "{config_key}"}}', "utf-8"))
        response = self.controller.recv(4096)
        response = json.loads(response)
        check_response(response)
        self.loaded[response["CONFIG_ID"]] = Config(response["CONFIG_ID"], response["CONFIG_NAME"], self.controller)
        return self.loaded[response["CONFIG_ID"]]
        
    def check_subscriptions(self):
        decoder = json.JSONDecoder()
        raw_event = ""
        self.controller.setblocking(0)
        tmp_event = self.controller.recv()
        while(1):
            raw_event += tmp_event.decode()
            try:
                tmp_event = self.controller.recv()
            except socket.error:
                break
        pos = 0
        while(1):
            try:
                event, pos = decoder.raw_decode(raw_event, pos)
            except json.JSONDecodeError:
                break
            self.loaded[event["CONFIG_ID"]]._callback(event["SETTING_NAME"], event["SUBSCRIPTION_EVENT_TYPE"])
        self.controller.setblocking(1)
            
    def get_loaded(self):
        return {id:conf.name for (id, conf) in self.loaded.items()}

    def get_conf(self, id: int) -> Config:
        return self.loaded[id]

    def unload(self, id):
        self.controller.send(bytes(f'{{"REQUEST_NAME": "CONFIG_UNLOAD", "CONFIG_ID": {self.configId}}}', "utf-8"))
        response = self.controller.recv(4096)
        response = json.loads(response)
        check_response(response)
        del self.loaded[id]

    def destroy(self, id):
        self.controller.send(bytes(f'{{"REQUEST_NAME": "CONFIG_DESTROY", "CONFIG_ID": {self.configId}}}', "utf-8"))
        response = self.controller.recv(4096)
        response = json.loads(response)
        check_response(response)
        del self.loaded[id]