FROM ubuntu:latest
LABEL maintainer="benjamin.hoffman.dev@gmail.com"

USER root

RUN apt-get update -y
RUN apt-get install tar -y
RUN apt-get install wget -y
RUN apt-get install build-essential -y
RUN apt-get install clang gcc g++ git -y

# Install Cmake
RUN wget -qO- "https://cmake.org/files/v3.13/cmake-3.13.4-Linux-x86_64.tar.gz" | tar --strip-components=1 -xz -C /usr/local

#Install Boost
WORKDIR /home
RUN pwd && ls -la                                                                                       \
    && wget --quiet --max-redirect 3 https://dl.bintray.com/boostorg/release/1.69.0/source/boost_1_69_0.tar.gz  \
    && tar zxf boost_1_69_0.tar.gz -C /usr/include/                                                     \
    && rm -rf /home/boost_1_69_0.tar.gz

WORKDIR /usr/include/boost_1_69_0/
RUN ./bootstrap.sh
RUN ./b2 --with-system --with-thread --with-date_time --with-regex --with-serialization stage
ENV BOOST_LIBRARYDIR="/usr/include/boost_1_69_0/stage/lib/"
ENV BOOST_ROOT="/usr/include/boost_1_69_0"

# Build and clone the boost dir
