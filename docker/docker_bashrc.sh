# SPDX-License-Identifier: GPL-3.0-only
# This file is part of Lazuli.
# Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

#
# This is the .bashrc file for the Lazuli container image.
#

# Enable bash completion (for man pages)
source /etc/profile.d/bash_completion.sh

cat > /dev/tty << EOF
 _                        _ _
| |                      | (_)
| |      ____ _____ _   _| |_
| |     / _  (___  ) | | | | |
| |____( ( | |/ __/| |_| | | |
|_______)_||_(_____)\____|_|_| $(cat /etc/lazuli-version)

Welcome in the Lazuli development environment container.
For Lazuli version $(cat /etc/lazuli-version).
This image was generated on $(cat /etc/lazuli-container-date).

$(cat /etc/fedora-release)
$(uname -a)

  Type 'man Lazuli' to read the documentation.

EOF

if [ "$(cat /~/workspace/VERSION)" != "$(cat /etc/lazuli-version)" ]; then
    printf "WARNING! You are using the container in version %s whereas your \
sources are in version %s\n" $(cat /etc/lazuli-version) \
$(cat /~/workspace/VERSION);

    printf "It is recommended that you pull and run the container in version \
%s\n\n" $(cat /~/workspace/VERSION);
fi
