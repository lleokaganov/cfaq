#!/bin/bash

name=${1}

# Убираем расширение из имени файла
base_name=$(basename "$name" | sed 's/\(.*\)\..*/\1/') #'

# Определяем размер изображения
size=$(identify -format "%w_%h" "${name}")

# Конвертируем в RAW RGB565
convert "${name}" -define bmp:subtype=RGB565 -flip -depth 8 -type TrueColor BMP3:- | tail -c +55 > "${base_name}_${size}.raw"

echo "✅ Готово! Файл сохранён как ${base_name}_${size}.raw"

exit


#!/bin/sh

name=${1}

size=`identify -format "%w_%h" ${name}`

convert "${name}" -define bmp:subtype=RGB565 -flip -depth 8 -type TrueColor BMP3:- | tail -c +55 > ${name}_${size}.raw

exit


convert 2.png -threshold 42% output.png
convert output.png -depth 1 gray:output.bin
xxd -i output.bin > image.h
