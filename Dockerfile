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
RUN dnf install -y doxygen gzip
COPY . ~/workspace/
WORKDIR ~/workspace
RUN doxygen
RUN gzip --best -nvr ./doxygen_output/man

## Actual Lazuli image
FROM fedora:31

LABEL description="Lazuli development environment."
LABEL url="https://github.com/randruc/Lazuli"
LABEL maintainer="remi.andruccioli@gmail.com"

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
    && \
    dnf clean all

# Copy man pages from intermediate image
COPY --from=man_intermediate ~/workspace/doxygen_output/man/ /usr/share/man/

# Update man pages index cache
RUN mandb

WORKDIR ~/workspace

COPY ./VERSION /etc/lazuli-version

RUN echo " _                        _ _ " >> /etc/motd && \
    echo "| |                      | (_)" >> /etc/motd && \
    echo "| |      ____ _____ _   _| |_ " >> /etc/motd && \
    echo "| |     / _  (___  ) | | | | |" >> /etc/motd && \
    echo "| |____( ( | |/ __/| |_| | | |" >> /etc/motd && \
    echo "|_______)_||_(_____)\____|_|_| v$(cat /etc/lazuli-version)" >> /etc/motd && \
    echo "" >> /etc/motd && \
    echo "Welcome in the Lazuli development environment container." >> /etc/motd && \
    echo "For Lazuli version $(cat /etc/lazuli-version)." >> /etc/motd && \
    echo "This image was generated on $(date -u)." >> /etc/motd

    # Enable bash completion (for man pages)
RUN echo "source /etc/profile.d/bash_completion.sh" >> ~/.bashrc && \
    echo "cat /etc/motd" >> ~/.bashrc && \
    echo "cat /etc/fedora-release" >> ~/.bashrc && \
    echo "uname -a" >> ~/.bashrc && \
    echo "echo"  >> ~/.bashrc

CMD [ "/usr/bin/bash" ]
