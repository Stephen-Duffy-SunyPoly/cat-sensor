# -*- coding: utf-8 -*-
"""
Created on Wed Apr 16 16:25:25 2025

@author: wdogw
(With some help from our robot friend ChatGPT.)
additional modifications made by Stephen
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

#YES this is necessary for google AIs
access_token = os.getenv("HF_API_KEY") # get the API key from the environment variable
model="google/deeplabv3_mobilenet_v2_1.0_513" #specify what AI moble to sue

# Create the socket that will be used by the back end to connect to this server
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((HOST, PORT))
server_socket.listen(1)

print(f"Listening on {HOST}:{PORT}...")

#repeat this process forever
while True:
    #recieve an incoming connection
    conn, addr = server_socket.accept()
    try:
        # Accept a connection
        print(f"Connected by {addr}")

        buffer = b''
        #read data fron the input stream until 921600 bytes have been read or the stream closes
        while len(buffer) < 921600:
            data = conn.recv(921600)  # Receive up to 6 megabytes of data (a 1080p image)
            #print("Recieved " + str(len(data)) + " bytes.\n")

            buffer += data
            if not data: # if the stream closed
                break

        #print(data)
        #print("Recieved " + str(len(buffer)) + " bytes.\n")

        #convert the input bytes to an image
        image = Image.frombytes('RGB', (640, 480), buffer)

        #create the AI
        p = pipeline("image-segmentation", use_fast=True, model=model, token=access_token)

        #run the image through the AI
        segments = p(image)

        has_cat = False
        #check what is returned from the AI for containing a cat
        for s in segments:
            plt.figure()
            #things for saving images of what it detected
            plt.title(s['label'])
            plt.imshow(s['mask'])
            #check if this thing is a cat
            if s['label'] == 'cat':
                has_cat = True
        #print("Image " + ("does" if has_cat else "does not") +" contain a cat.")
        #print(has_cat)
        #print(str(has_cat).encode())

        #send the result back to the back end
        conn.send(str(has_cat).encode())
    except Exception as e:
        print("an error occored")
        print(e)
    #close the connection
    conn.close()