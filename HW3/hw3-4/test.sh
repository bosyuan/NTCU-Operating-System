g++ hw3-4_serial.cpp -o s.out
time ./s.out < testcase/case3.txt
g++ hw3-4.cpp -o p.out -lpthread
time ./p.out -t 4 < testcase/case3.txt