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

HOST = '192.168.254.106'  # The IP address of your computer
PORT = 33333           # Port to listen on

access_token = ""
model="google/deeplabv3_mobilenet_v2_1.0_513"

# Create the socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((HOST, PORT))
server_socket.listen(1)

print(f"Listening on {HOST}:{PORT}...")

while True:
    # Accept a connection
    conn, addr = server_socket.accept()
    print(f"Connected by {addr}")
    buffer = b''
    while len(buffer) < 921600:
        data = conn.recv(921600)  # Receive up to 6 megabytes of data (a 1080p image)
        print("Recieved " + str(len(data)) + " bytes.\n")
    
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
    print("Image " + ("does" if has_cat else "does not") +" contain a cat.")
    
    conn.send(has_cat.to_bytes())
    
    conn.close()
