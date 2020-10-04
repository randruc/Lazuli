# SPDX-License-Identifier: GPL-3.0-only
# This file is part of Lazuli.
# Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

#
# Lazuli Dockerfile.
#
# This Dockerfile describes the container image that is used as the development
# environment for Lazuli. This image is used by kernel and user application
# developpers, as well as for continuous integration.
#
# Man pages are first built in an intermediate image. This has the advantage of
# being platform-independent. The man pages are then copied into the final
# Lazuli image.
#

## Intermediate image used to build man pages
FROM fedora:31 AS man_intermediate
RUN dnf install -y doxygen gzip make python3-sphinx
COPY . ~/workspace/
WORKDIR ~/workspace
# Build Doxygen man pages
RUN doxygen && gzip --best -nvr ./doxygen_output/man
# Build Sphinx man pages
WORKDIR doc
RUN make man && gzip --best -nvr ./_build/man

## Actual Lazuli image
FROM fedora:31

LABEL description="Lazuli development environment."
LABEL url="https://github.com/randruc/Lazuli"
LABEL maintainer="Remi Andruccioli <remi.andruccioli@gmail.com>"

# This will allow installation of man pages along with packages
RUN sed -i '/tsflags=nodocs/d' /etc/dnf/dnf.conf

# Packages listed here are already installed in Fedora 31. We reinstall them to
# force the installation of their man pages.
# For now it seems that DNF does not provide a convenient way to perform
# a "reinstall or install" on a given package, or an "install doc" for a given
# package.
#
# Sort by alphabetical order
RUN dnf reinstall -y \
    bash \
    coreutils \
    gawk \
    grep \
    sed \
    && \
    dnf clean all

# Sort by alphabetical order
RUN dnf install -y \
    avr-gcc \
    avr-binutils \
    bash-completion \
    binutils\
    clang-tools-extra \
    cmake \
    findutils \
    gawk \
    gcc \
    less \
    make \
    man-db \
    tee \
    time \
    && \
    dnf clean all

# Copy man pages from intermediate image
COPY --from=man_intermediate ~/workspace/doxygen_output/man/ /usr/share/man/

COPY --from=man_intermediate ~/workspace/doc/_build/man/ /usr/share/man/man1/

# Update man pages index cache
RUN mandb

WORKDIR ~/workspace

COPY ./VERSION /etc/lazuli-version

RUN date -u > /etc/lazuli-container-date

COPY ./docker/docker_bashrc.sh /tmp/

RUN cat /tmp/docker_bashrc.sh >> ~/.bashrc && rm /tmp/docker_bashrc.sh

CMD [ "/usr/bin/bash" ]
