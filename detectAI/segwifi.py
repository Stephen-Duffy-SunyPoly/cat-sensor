# -*- coding: utf-8 -*-
"""
Created on Wed Apr 16 16:25:25 2025

@author: wdogw
(With some help from our robot friend ChatGPT.)
"""

import socket
import torch
from transformers import pipeline
import matplotlib.pyplot as plt
import sys
import PIL.Image as Image

#Fix so torch and matplotlib don't **** each other.
import os
os.environ["KMP_DUPLICATE_LIB_OK"]="TRUE"
#set host to 0.0.0.0 to bind to any/all interfaces
HOST = '0.0.0.0'  # The IP address of your computer
PORT = 33333           # Port to listen on

#YES this is nearness for google AIs
access_token = os.getenv("HF_API_KEY")
model="google/deeplabv3_mobilenet_v2_1.0_513"

# Create the socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((HOST, PORT))
server_socket.listen(1)

print(f"Listening on {HOST}:{PORT}...")

while True:
    conn, addr = server_socket.accept()
    try:
        # Accept a connection
        print(f"Connected by {addr}")
        buffer = b''
        while len(buffer) < 921600:
            data = conn.recv(921600)  # Receive up to 6 megabytes of data (a 1080p image)
            #print("Recieved " + str(len(data)) + " bytes.\n")

            buffer += data
            if not data:
                break

        #print(data)
        print("Recieved " + str(len(buffer)) + " bytes.\n")

        image = Image.frombytes('RGB', (640, 480), buffer)

        p = pipeline("image-segmentation", use_fast=True, model=model, token=access_token)

        segments = p(image)

        has_cat = False
        for s in segments:
            plt.figure()
            plt.title(s['label'])
            plt.imshow(s['mask'])
            if s['label'] == 'cat':
                has_cat = True
        #print("Image " + ("does" if has_cat else "does not") +" contain a cat.")
        #print(has_cat)
        #print(str(has_cat).encode())

        conn.send(str(has_cat).encode())
    except Exception as e:
        print("an error occored")
        print(e)
    conn.close()