FROM alpine:3.20
#FROM ubuntu:24.04
FROM nvidia/cuda:11.8.0-cudnn8-runtime-ubuntu22.04


#################### for ubuntu
RUN apt-get update && apt-get -y install cmake protobuf-compiler
RUN apt-get -y install cmake
RUN apt-get -y install build-essential
##RUN apt-get install python3
##RUN apt-get install python3-pip
RUN apt-get -y install libomp-dev
##RUN apt-get install libgomp
##RUN apk add --no-cache gcompat libstdc++
####################



#################### for alpine
#RUN apk update
#RUN apk add --no-cache cmake make build-base
#RUN apk add --no-cache python3 py3-pip
#RUN apk add --no-cache libgomp
#RUN apk add gcompat libstdc++
####################



WORKDIR /HanaSim
#RUN wget -q https://download.pytorch.org/libtorch/nightly/cpu/libtorch-shared-with-deps-latest.zip
#RUN unzip -q libtorch-shared-with-deps-latest.zip
#COPY . /

COPY src/ src/
COPY CMakeLists.txt ./
COPY include/ include/
COPY bots/ bots/

#RUN ls -la /HanaSim/*

RUN cmake . && make
ENTRYPOINT [ "/HanaSim/HanaSim" ]
#RUN chmod +x deployment-service
CMD [ "echo", "Default argument from CMD instruction" ]
#COPY src ./
#COPY CMakeLists.txt .
#COPY include ./
#COPY bots ./
