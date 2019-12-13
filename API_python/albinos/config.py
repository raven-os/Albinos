"""
Config manipulation module
"""

from typing import Union, List, Optional, Dict, Any, Callable
import json
from albinos.controller import Controller
from albinos.error import check_response

class Config:

    def __init__(self, configId: int, name: str, controller: Controller):
        self.configId = configId
        self.name = name
        self.controller = controller
        self.subscribed = {}

    def get_dependencies(self) -> List[str]:
        self.controller.send(bytes(f'{{"REQUEST_NAME": "CONFIG_GET_DEPS", "CONFIG_ID": {self.configId}}}', "utf-8"))
        response = self.controller.recv(4096)
        response = json.loads(response)
        check_response(response)
        return response["DEPS"]

    def get_settings_names(self):
        self.controller.send(bytes(f'{{"REQUEST_NAME": "CONFIG_GET_SETTINGS_NAMES", "CONFIG_ID": {self.configId}}}', "utf-8"))
        response = self.controller.recv(4096)
        response = json.loads(response)
        check_response(response)
        return response["SETTINGS_NAMES"]        

    def get_settings(self, settings_filter: Optional[Union[str, List[str]]]=None) -> Dict[str, str]:
        if settings_filter is None:
            self.controller.send(bytes(f'{{"REQUEST_NAME": "CONFIG_GET_SETTINGS", "CONFIG_ID": {self.configId}}}', "utf-8"))
            response = self.controller.recv(4096)
            response = json.loads(response)
            check_response(response)
            return response["SETTINGS"]
        else:
            if isinstance(settings_filter, str):
                settings_filter = [settings_filter]
            settings_map = {}
            for setting in settings_filter:
                self.controller.send(bytes(f'{{"REQUEST_NAME": "SETTING_GET", "CONFIG_ID": {self.configId}, "SETTING_NAME":"{setting}"}}', "utf-8"))
                response = self.controller.recv(4096)
                response = json.loads(response)
                check_response(response)
                settings_map[setting] = response["SETTING_VALUE"]
            return settings_map
                

    def update_settings(self, settings: Dict[str, Any]):
        self.controller.send(bytes(f'{{"REQUEST_NAME": "SETTING_UPDATE", "CONFIG_ID": {self.configId}, "SETTINGS_TO_UPDATE":{json.dumps(settings)} }}', "utf-8"))
        response = self.controller.recv(4096)
        response = json.loads(response)
        check_response(response)
        

    def remove_settings(self, settings: Union[str, List[str]]):
        if isinstance(settings, str):
            settings = [settings]
        for setting in settings:
            self.controller.send(bytes(f'{{"REQUEST_NAME": "SETTING_REMOVE", "CONFIG_ID": {self.configId}, "SETTING_NAME":"{setting}"}}', "utf-8"))
            response = self.controller.recv(4096)
            response = json.loads(response)
            check_response(response)

    def subscribe(self, settings: Union[str, List[str]], callback: Callable[[str, str], None]):
        if isinstance(settings, str):
            settings = [settings]
        for setting in settings:
            self.controller.send(bytes(f'{{"REQUEST_NAME": "SUBSCRIBE_SETTING", "CONFIG_ID": {self.configId}, "SETTING_NAME":"{setting}"}}', "utf-8"))
            response = self.controller.recv(4096)
            response = json.loads(response)
            check_response(response)
            self.subscribed[setting] = callback
            
    
    def unsubscribe(self, settings: Union[str, List[str]]):
        if isinstance(settings, str):
            settings = [settings]
        for setting in settings:
            self.controller.send(bytes(f'{{"REQUEST_NAME": "UNSUBSCRIBE_SETTING", "CONFIG_ID": {self.configId}, "SETTING_NAME":"{setting}"}}', "utf-8"))
            response = self.controller.recv(4096)
            response = json.loads(response)
            check_response(response)
            del self.subscribed[setting]

    def _callback(self, setting: str, event_type: str):
        self.subscribed[setting](setting, event_type)

    def include(self, configs: Union[int, List[int], Dict[int, int]]):
        """
        :param: configs    dict of configs and their position. If a single or list of configs is passed, position will be defaulted to `-1`
        """
        if isinstance(configs, dict):
            for config, pos in configs.items():
                self.controller.send(bytes(f'{{"REQUEST_NAME": "CONFIG_INCLUDE", "CONFIG_ID": {self.configId}, "SRC":{config}, "INCLUDE_POSITION":"{pos}"}}', "utf-8"))
                response = self.controller.recv(4096)
                response = json.loads(response)
                check_response(response)
            return
        if isinstance(configs, int):
            configs = [configs]
        for config in configs:
            self.controller.send(bytes(f'{{"REQUEST_NAME": "CONFIG_INCLUDE", "CONFIG_ID": {self.configId}, "SRC":{config}, "INCLUDE_POSITION":"-1"}}', "utf-8"))
            response = self.controller.recv(4096)
            response = json.loads(response)
            check_response(response)

    def uninclude(self, configs: Union[int, List[int]]):
        if isinstance(configs, int):
            configs = [configs]
        for config in configs:
            self.controller.send(bytes(f'{{"REQUEST_NAME": "CONFIG_UNINCLUDE", "CONFIG_ID": {self.configId}, "SRC":{config}}}', "utf-8"))
            response = self.controller.recv(4096)
            response = json.loads(response)
            check_response(response)

    def get_alias(self, alias_filter: Optional[Union[str, List[str]]]=None) -> Dict[str, str]:
        pass
    
    def set_alias(self, alias: Dict[str, str]):
        pass

    def unset_alias(self, alias: Union[str, List[str]]):
        pass

