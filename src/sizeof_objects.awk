#!/usr/bin/awk -f

$2 ~ /__sizeof/ { print $2; }
$4 ~ /word/ {
    converted_size = strtonum($5);
    printf("0x%x (%d)\n", converted_size, converted_size);
}
