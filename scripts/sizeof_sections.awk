#!/usr/bin/awk -f

# SPDX-License-Identifier: GPL-3.0-only
# This file is part of Lazuli.
# Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

#
# This AWK script is used to retrieve the sizes of each useful sections in order
# to print the total bytes used by the kernel + user tasks in RAM and ROM.
#

/^\.text/ {
    text_size = $2;
}

/^\.progmem/ {
    progmem_size = $2;
}

/^\.data/ {
    data_size = $2;
}

/^\.rodata/ {
    rodata_size = $2;
}

/^\.bss/ {
    bss_size = $2;
}

/^\.noinit/ {
    noinit_size = $2;
}

function printSection(sectionName, memories, size) {
    if (0 != size) {
        printf("%-10s %13s %8d\n", sectionName, memories, size);
    }
}

END {
    total_rom_size = text_size + progmem_size + data_size + rodata_size;
    total_ram_size = data_size + rodata_size + bss_size + noinit_size;

    printSection(".text", "(ROM only):", text_size);
    printSection(".progmem", "(ROM only):", progmem_size);
    printSection(".data", "(ROM + RAM):", data_size);
    printSection(".rodata", "(ROM + RAM):", rodata_size);
    printSection(".bss", "(RAM only):", bss_size);
    printSection(".noinit", "(RAM only):", noinit_size);

    print "---------------------------------";
    printf("%-24s %8d\n", "Total ROM:", total_rom_size);
    printf("%-24s %8d\n", "Total RAM (static):", total_ram_size);
}
