#
# Lazuli Docker file.
#

FROM fedora:30

LABEL description="Lazuli development environment."
LABEL url="https://github.com/randruc/Lazuli"
LABEL maintainer="remi.andruccioli@gmail.com"

RUN dnf install -y \
    bash \
    grep \
    less \
    sed \
    gawk \
    make \
    cmake \
    gcc \
    && \
    dnf clean all

RUN dnf install -y \
    avr-gcc \
    avr-binutils \
    && \
    dnf clean all

RUN dnf install -y \
    clang-tools-extra \
    && \
    dnf clean all

WORKDIR ~/workspace

RUN echo Welcome in the Lazuli development environment container. > ~/.motd && \
    echo This image was generated on $(date -u). >> ~/.motd && \
    echo >> ~/.motd

RUN echo 'cat ~/.motd' >> ~/.bashrc

CMD [ "/usr/bin/bash" ]
