#!/bin/bash

clear

exit

TOOL=$(find ~/.platformio -name xtensa-esp32-elf-size | head -n1)

echo "Using: $TOOL"
echo

TOTAL=0

for f in $(find .pio/build/esp32 -name "*.o"); do
  SIZE=$(
    $TOOL -A "$f" \
    | awk '
        $1 ~ /^\.text/   {t += $2}
        $1 ~ /^\.rodata/ {r += $2}
        $1 ~ /^\.data/   {d += $2}
        $1 ~ /^\.bss/    {b += $2}
        END {print t+r+d}
      '
  )
  SIZE=${SIZE:-0}
  TOTAL=$((TOTAL + SIZE))
  printf "%8d  %s\n" "$SIZE" "$f"
done

echo
echo "======================"
echo " TOTAL FLASH SIZE: $TOTAL bytes"
echo "======================"




exit

TOOL=$(find ~/.platformio -name xtensa-esp32-elf-size | head -n1)

if [ -z "$TOOL" ]; then
  echo "xtensa-esp32-elf-size not found"
  exit 1
fi

echo "Using tool: $TOOL"
echo ""

TOTAL_TEXT=0
TOTAL_RODATA=0
TOTAL_DATA=0
TOTAL_BSS=0

for f in $(find .pio/build/esp32 -name "*.o"); do
  TEXT=$( $TOOL -A "$f" | awk '/\.text[ \t]/ {print $2}' )
  RODATA=$( $TOOL -A "$f" | awk '/\.rodata[ \t]/ {print $2}' )
  DATA=$( $TOOL -A "$f" | awk '/\.data[ \t]/ {print $2}' )
  BSS=$( $TOOL -A "$f" | awk '/\.bss[ \t]/ {print $2}' )

  # пустые хочу нулями
  TEXT=${TEXT:-0}
  RODATA=${RODATA:-0}
  DATA=${DATA:-0}
  BSS=${BSS:-0}

  SIZE=$((TEXT + RODATA + DATA + BSS))

  printf "%6d  %-60s\n" "$SIZE" "$f"

  TOTAL_TEXT=$((TOTAL_TEXT + TEXT))
  TOTAL_RODATA=$((TOTAL_RODATA + RODATA))
  TOTAL_DATA=$((TOTAL_DATA + DATA))
  TOTAL_BSS=$((TOTAL_BSS + BSS))
done

echo ""
echo "==================== TOTAL ===================="
echo "  TEXT:   $TOTAL_TEXT"
echo "  RODATA: $TOTAL_RODATA"
echo "  DATA:   $TOTAL_DATA"
echo "  BSS:    $TOTAL_BSS"
echo "-----------------------------------------------"
echo " REAL FLASH SIZE: $((TOTAL_TEXT + TOTAL_RODATA + TOTAL_DATA)) bytes"
echo "==============================================="
