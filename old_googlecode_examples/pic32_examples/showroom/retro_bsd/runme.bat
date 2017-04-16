cd avrdude
avrdude -Cavrdude.conf -c stk500v2 -P COM5 -p pic32 -b 115200 -v -U flash:w:../unix.hex:i
