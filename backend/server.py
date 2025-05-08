import sys
import os

root_path = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
sys.path.append(root_path)

from fastapi import FastAPI, WebSocket
import json
import uuid
import uvicorn
import asyncio
import time
from datetime import datetime
import importlib

hanabi_gym = importlib.import_module("hanabi_gym")

def start_server(host="0.0.0.0", port=8000):
    hanabi_gym.start_server(host, port)

if __name__ == "__main__":
    start_server()