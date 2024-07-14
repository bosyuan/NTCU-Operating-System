g++ hw3-3_serial.cpp -o serial.out
time ./serial.out < testcase/case3.txt
g++ hw3-3.cpp -o parellel.out -lpthread
time ./parellel.out -t 4 < testcase/case3.txt