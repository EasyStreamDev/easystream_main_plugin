FROM ubuntu:latest

RUN apt-get update && sudo apt-get install -y \
            g++-multilib \
            gcc-multilib \
            git \
            wget

WORKDIR /usr/src/app

COPY *.sh ./

RUN ./ci/linux/install-dependencies-ubuntu.sh
