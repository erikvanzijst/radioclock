FROM ubuntu:20.04
# Published as: ghcr.io/erikvanzijst/arm-none-eabi-gcc:latest

LABEL org.opencontainers.image.source="https://github.com/erikvanzijst/radioclock"
LABEL org.opencontainers.image.description="ARM compiler toolkit"
LABEL org.opencontainers.image.licenses=MIT

RUN apt-get update && apt-get install -y --no-install-recommends wget make cpio libncurses5 git build-essential

# get the toolchain
RUN wget https://developer.arm.com/-/media/Files/downloads/gnu-rm/10-2020q4/gcc-arm-none-eabi-10-2020-q4-major-x86_64-linux.tar.bz2 -O gcc-arm-none-eabi.tar.bz2 && \
    mkdir gcc-arm-none-eabi && \
    tar xjfv gcc-arm-none-eabi.tar.bz2 -C gcc-arm-none-eabi --strip-components 1 && \
    rm gcc-arm-none-eabi.tar.bz2

# add the tools to the path
ENV PATH="/gcc-arm-none-eabi/bin:${PATH}"
