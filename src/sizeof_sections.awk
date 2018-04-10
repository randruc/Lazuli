#!/usr/bin/awk -f
#
# This AWK script is used to retrieve the sizes of each useful sections in order
# to print the total bytes used by the kernel + used tasks in RAM and ROM.
#

$1 ~ /^\.text$/ { text_size = $2; }
$1 ~ /^\.progmem$/ { progmem_size = $2; }
$1 ~ /^\.data$/ { data_size = $2; }
$1 ~ /^\.rodata$/ { rodata_size = $2; }
$1 ~ /^\.bss$/ { bss_size = $2; }

END {
    total_rom_size = text_size + progmem_size + data_size + rodata_size;
    total_ram_size = data_size + rodata_size + bss_size;
    
    printf("%-10s %8d\n", ".text:", text_size);
    printf("%-10s %8d\n", ".progmem:", progmem_size);
    printf("%-10s %8d\n", ".data:", data_size);
    printf("%-10s %8d\n", ".rodata:", rodata_size);
    printf("%-10s %8d\n", ".bss:", bss_size);
    print "-------------------";
    printf("%-10s %8d\n", "Total ROM:", total_rom_size);
    printf("%-10s %8d\n", "Total RAM:", total_ram_size);
}
