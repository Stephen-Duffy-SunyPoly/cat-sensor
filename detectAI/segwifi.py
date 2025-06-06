# -*- coding: utf-8 -*-
"""
Created on Wed Apr 16 16:25:25 2025

@author: wdogw
(With some(what less) help from our robot friend ChatGPT.)
"""

import socket
from transformers import pipeline
import matplotlib.pyplot as plt
import PIL.Image as Image
import numpy as np
import traceback

#Fix so torch and matplotlib don't **** each other.
import os
os.environ["KMP_DUPLICATE_LIB_OK"]="TRUE"

HOST = '0.0.0.0'  # The IP address of your computer
PORT = 33333           # Port to listen on

image_size = 160 * 120

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
        while len(buffer) < image_size:
            data = conn.recv(image_size)  # Receive up to 6 megabytes of data (a 1080p image)
            #print("Recieved " + str(len(data)) + " bytes.\n")
        
            buffer += data
            if not data:
                break
        
        #print(data)    
        print("Recieved " + str(len(buffer)) + " bytes.\n")
        
        image = Image.frombytes("L", (160, 120), buffer)
        
        image.save("capture.png")
        
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
        #print(has_cat)
        #print(str(has_cat).encode())

        #send the result back to the back end
        conn.send(str(has_cat).encode())
    except Exception as e:
        traceback.print_exc()
        print("an error occored")
        print(e)
    #close the connection
    conn.close()
