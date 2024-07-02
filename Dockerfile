FROM alpine:3.20
RUN apk update
RUN apk add --no-cache cmake make build-base
RUN apk add --no-cache python3 py3-pip
RUN apk add --no-cache libgomp



WORKDIR /HanaSim
#COPY . /

COPY src/ src/
COPY CMakeLists.txt ./
COPY include/ include/
COPY bots/ bots/
#RUN ls -la /HanaSim/*

RUN cmake . && make
ENTRYPOINT [ "/HanaSim/HanaSim" ]
CMD [ "echo", "Default argument from CMD instruction" ]
#COPY src ./
#COPY CMakeLists.txt .
#COPY include ./
#COPY bots ./
