FROM pytorch/pytorch:latest

COPY . .

RUN python3 -m pip install matplotlib
#RUN python3 -m pip install torch
#RUN python3 -m pip install torchvision
#RUN python3 -m pip install torchaudio
RUN python3 -m pip install transformers

RUN chmod +x segwifi.py

# Define environment variable
#ENV NAME World

CMD [ "python","-u", "segwifi.py" ]