#Jeremy Albert Eval Script
#!/bin/sh
g++ stdeval.cpp -o stdeval
g++ eval.cpp -ofast -o myeval
echo -n "<stdio.h> "
./stdeval r f a $1
echo -n "\"stdio.h\" "
./myeval r f a $1
echo -n "Unix I/O  "
./myeval r u a $1
echo -n "<stdio.h> "
./stdeval r f b $1
echo -n "\"stdio.h\" "
./myeval r f b $1
echo -n "Unix I/O  "
./myeval r u b $1
echo -n "<stdio.h> "
./stdeval r f c $1
echo -n "\"stdio.h\" "
./myeval r f c $1
echo -n "Unix I/O  "
./myeval r u c $1
echo -n "<stdio.h> "
./stdeval r f r $1
echo -n "\"stdio.h\" "
./myeval r f r $1
echo -n "Unix I/O  "
./myeval r u r $1
echo -n "<stdio.h> "
./stdeval w f a test.txt
echo -n "\"stdio.h\" "
./myeval w f a test.txt
echo -n "Unix I/O  "
./stdeval w u a test.txt
echo -n "<stdio.h> "
./stdeval w f b test.txt
echo -n "\"stdio.h\" "
./myeval w f b test.txt
echo -n "Unix I/O  "
./stdeval w u b test.txt
echo -n "<stdio.h> "
./stdeval w f c test.txt
echo -n "\"stdio.h\" "
./myeval w f c test.txt
echo -n "Unix I/O  "
./stdeval w u c test.txt
echo -n "<stdio.h> "
./stdeval w f r test.txt
echo -n "\"stdio.h\" "
./myeval w f r test.txt
echo -n "Unix I/O  "
./stdeval w u r test.txt


