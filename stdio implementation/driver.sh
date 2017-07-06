#Jeremy Albert Driver Script
#Prog 3
#!/bin/sh
g++ driver.cpp -o driver

./driver hamlet.txt >myoutput_hamlet.txt
./driver othello.txt >myoutput_othello.txt

#the source output has extraneous writeBuffer message so ignore it
diff --ignore-matching-lines=^writeBuffer.* myoutput_hamlet.txt output_hamlet.txt
diff --ignore-matching-lines=^writeBuffer.* myoutput_othello.txt output_othello.txt

diff test1.txt ~css503/programming/prog3/test1.txt
diff test2.txt ~css503/programming/prog3/test2.txt
diff test3.txt ~css503/programming/prog3/test3.txt
