# -*- coding: utf-8 -*-
"""
Created on Wed Apr 16 16:25:25 2025

@author: wdogw
(With some help from our robot friend ChatGPT.)
"""

import socket
from transformers import pipeline
import matplotlib.pyplot as plt
import PIL.Image as Image
import numpy as np

#Fix so torch and matplotlib don't **** each other.
import os
os.environ["KMP_DUPLICATE_LIB_OK"]="TRUE"

HOST = '0.0.0.0'  # The IP address of your computer
PORT = 33333           # Port to listen on

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
        while len(buffer) < 614400:
            data = conn.recv(614400)  # Receive up to 6 megabytes of data (a 1080p image)
            print("Recieved " + str(len(data)) + " bytes.\n")
        
            buffer += data
            if not data:
                break
        
        #print(data)    
        print("Recieved " + str(len(buffer)) + " bytes.\n")
            
        rgb565 = np.frombuffer(buffer, dtype=np.uint16)

        # Create empty array to store RGB888 result
        rgb888 = np.zeros((rgb565.size, 3), dtype=np.uint8)
        
        # Split the RGB565 value into R, G, B components
        rgb888[:, 0] = (rgb565 >> 11) & 0x1F  # Extract Red (5 bits)
        rgb888[:, 1] = (rgb565 >> 5) & 0x3F   # Extract Green (6 bits)
        rgb888[:, 2] = rgb565 & 0x1F          # Extract Blue (5 bits)
        
        # Scale to 8-bit (RGB888) by shifting and masking
        rgb888[:, 0] = (rgb888[:, 0] << 3) | (rgb888[:, 0] >> 2)  # Scale Red
        rgb888[:, 1] = (rgb888[:, 1] << 2) | (rgb888[:, 1] >> 4)  # Scale Green
        rgb888[:, 2] = (rgb888[:, 2] << 3) | (rgb888[:, 2] >> 2)  # Scale Blue
        
        rgb888.reshape(640, 480, 3)
        
        image = Image.fromarray(rgb888, "RGB")
        
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
        print("an error occored")
        print(e)
    #close the connection
    conn.close()
