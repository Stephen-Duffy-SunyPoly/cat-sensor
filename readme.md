# Cat Sensor
Final project for IoT
## Cloud setup
1. Install [docker](https://www.docker.com/) on your computer 
2. Make sure docker is running
3. Copy `.env.example` to `.env`
4. Edit `.env` putting your [huggingface](https://huggingface.co/) API key on the required line. (not you will have to accept Google's terms)
3. Run the dev script for your operating system
Windows:
```cmd
dev.cmd
```
Linux/MAc
```shell
./dev.sh
```
To stop the cloud services run
```shell
docker compose down
```

## Arduino setup
1. Install the Arduino R4 WIFI board in your arduino IDE
2. In `cloudConnection.h` enter the name of your WIFI network where it says `WIFI NAME HERE`, do the same thing for where it says `WIFI PASSWORD HERE`
3. In `cloudConnection.h` change the value of `CLOUD_ADDRESS` to be the IP address of your could device
4. Upload the code to the Arduino