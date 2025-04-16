FROM python:latest

COPY . .

RUN python3 -m pip install matplotlib torch torchvision torchaudio transformers

RUN chmod +x segwifi.py

# Define environment variable
#ENV NAME World

CMD [ "python", "segwifi.py" ]