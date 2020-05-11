#
# Lazuli Dockerfile.
#
# This Dockerfile describes the container image that is used as the development
# environment for Lazuli. Hence this image is used by kernel and user application
# developpers, as well as for continuous integration.
#

FROM fedora:31

LABEL description="Lazuli development environment."
LABEL url="https://github.com/randruc/Lazuli"
LABEL maintainer="remi.andruccioli@gmail.com"

RUN dnf install -y sed && dnf clean all

# This will allow installation of man pages along with packages
RUN sed -i '/tsflags=nodocs/d' /etc/dnf/dnf.conf

# We reinstall sed to force the installation of its man page
RUN dnf reinstall -y sed && dnf clean all

RUN dnf install -y \
    man-db \
    bash \
    grep \
    less \
    findutils \
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

COPY VERSION /tmp

RUN echo Welcome in the Lazuli development environment container. > ~/.motd && \
    echo For Lazuli version $(cat /tmp/VERSION). >> ~/.motd && \
    echo This image was generated on $(date -u). >> ~/.motd

RUN echo 'cat ~/.motd' >> ~/.bashrc

RUN echo 'cat /etc/fedora-release' >> ~/.bashrc

RUN echo 'uname -a' >> ~/.bashrc

RUN echo 'echo' >> ~/.bashrc

CMD [ "/usr/bin/bash" ]
