#!/bin/sh

stty -F /dev/ttyUSB0 9600 raw
cat /dev/ttyUSB0 | xxd -p

exit

lleo@lleonuc3:/home/opt/Arduino/Устройства/RFID-2$ ./read.sh

02090200b2a00d140302090200b2a00d140302090200b2a00d1403020902
00b2a00d140302090200b2a00d140302090200b2a00d140302090200b2a0
0d140302090200b2a00d140302090200b2a00d140302090200b2a00d1403

0209025adc379d27030209025adc379d27030209025adc379d2703020902
5adc379d27030209025adc379d27030209025adc379d27030209025adc37
9d27030209025adc379d27030209025adc379d27030209025adc379d2703
^C

lleo@lleonuc3:/home/opt/Arduino/Устройства/RFID-2$ ./read.sh
020903941eaf426d03020903941eaf426d03020903941eaf426d03020903
941eaf426d03020903941eaf426d03020903941eaf426d03020903941eaf
^C

      64|00B2A00D|7B	Синий
02 09 02|00b2a00d|14 03
02 09 02|00b2a00d|14 03

      56|5ADC379D|7A	LLeo
02 09 02 5adc379d|27 03
02 09 02 5adc379d|27 03

      16|0066CAB5|0F	Белый брелок

02 09 03 941eaf42 6d 03
02 09 03 941eaf42 6d 03
02 09 03 941eaf42 6d 03

      16|0066CAB50F	Белый брелок
    02840069F22FB5	Спец NFC
------------------------------------------------------------------------------------------------------------
header | length | card type | the card data | BCCOSS checks the received data against        | end of data |
------------------------------------------------------------------------------------------------------------
  02   |   09   |   01      |  SN0 ~ SN3    | operations other than the data header and tail |     03      |

Uart interface output introduction:

baud rate: 9600

header | length | card type | the card data | BCCOSS checks the received data against       | end of data
    02			09				01					SN0 ~ SN3		|operations other than the data header and tail |  03


the card types are:
0x02 EM4100
0x01 M 1K

for example, the data received by the serial port tool is:
02 09 02 04 2E 53 82 F0 03
1. 02 indicates the start of the data.
2. 09 indicates that the length of the whole data is 9 bytes, including the start and end of the data.
3. 02 indicates that the card type is EM4100.
4. 4-7 byte (04 2E 53 82) these 4 bytes represent the read card number.
8. F0 represents the BCC verification from the 2-8 byte.
9. 0x03 indicates the end of the data
