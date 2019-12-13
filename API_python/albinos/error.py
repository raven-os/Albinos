"""
Error definitions module
"""

class APIError(Exception):
    pass

class UnknownMessage(Exception):
    pass

def check_response(response):
    if "REQUEST_STATE" not in response:
        raise UnknownMessage("No REQUEST_STATE found in response")
    if response["REQUEST_STATE"] != "SUCCESS":
        raise APIError(f"Unexpected Error status : {response['REQUEST_STATE']}")
