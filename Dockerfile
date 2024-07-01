FROM alpine:3.20
RUN apk update
RUN apk add --no-cache cmake make build-base
RUN apk add --no-cache python3 py3-pip



WORKDIR /HanaSim
COPY src ./
COPY CMakeLists.txt ./
COPY include ./
COPY bots ./

WORKDIR /HanaSim/build
COPY src ./
COPY CMakeLists.txt .
COPY include ./
COPY bots ./
RUN cmake .