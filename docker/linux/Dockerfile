FROM ubuntu:latest

WORKDIR /usr/src/app/ubuntuApp

ARG DEBIAN_FRONTEND=noninteractive

ENV LANG C.UTF-8
ENV LC_ALL C.UTF-8

RUN apt-get update && apt-get install -y \
  autoconf \
  automake \
  autopoint \
  bison \
  build-essential \
  ccache \
  clang \
  clang-format \
  clang-tidy \
  curl \
  doxygen \
  flex \
  g++-multilib \
  gcc-multilib \
  gettext \
  git \
  gperf \
  graphviz \
  lcov \
  libboost-all-dev \
  libsqlite3-dev \
  libssl-dev \
  ninja-build \
  pkg-config \
  python3 \
  python3-pip \
  valgrind \
  wget \
  && rm -rf /var/lib/apt/lists/* \
  && pip3 install --upgrade click conan conan_package_tools ConanTools pyelftools PyYAML \
  && update-alternatives --install "/usr/bin/ld" "ld" "/usr/bin/ld.gold" 10 \
  && update-alternatives --install "/usr/bin/ld" "ld" "/usr/bin/ld.bfd" 20

RUN apt-get update && apt-get install -y sudo software-properties-common

COPY ./plugin/ ./

RUN ./ci/linux/install-dependencies-ubuntu.sh

CMD ["./ci/linux/build-ubuntu.sh"]
