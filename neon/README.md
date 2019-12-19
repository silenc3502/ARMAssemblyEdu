### How to Setting Program for ARM Assembly

sudo apt-get install qemu-user-static qemu-system  
sudo apt-get install gcc-arm-linux-gnueabi  

### Dynamic Version

arm-linux-gnueabi-gcc -S '*.c'  

## If you wanna debug it

qemu-arm-static -g 1234 -L /usr/arm-linux/gnueabi ./a.out  

gdb-multiarch  
file a.out  
target remote localhost:1234  
b main  
c  

## Just Execution

qeu-arm-static -L /usr/arm-linux/gnueabi ./a.out  

## When /lib/ld-linux.so.3 Problem

arm-linux-gnueabi-gcc second.S  

qemu-arm -L /usr/arm-linux-gnueabi a.out  


### Static Version

arm-linux-gnueabi-gcc -static '*.c'  

qemu-arm a.out  

### For NEON

arm-linux-gnueabi-gcc -static -mfloat-abi=softfp -mfpu=neon -mcpu=cortex-a15 '*.c'  

qemu-arm a.out  

### Additional Info

https://community.arm.com/developer/ip-products/processors/b/processors-ip-blog/posts/coding-for-neon---part-4-shifting-left-and-right  

### Detailed Info

1.c: vector load  
2.c: vector store  
3.c: vector add, sub, mul  
4.c: vector adjacent add  
5.c: vector min, max  
6.c: vector long add, wide add, saturation add, narrow add  
7.c: vector add average, add round average  
8.c: vector long sub, wide sub, saturation sub, narrow sub  
9.c: vector sub average  
10.c: vector multiplication  
11.c: Vector Adjacent Lane Add  
12.c: Vector Adjacent Min, Max  
13.c: General Sum vs NEON Sum  
14.c: Get & Set Vector Lane  
15.c: Vector Initialization  
16.c: Look Up Vector Lane  
17.c: Vector Sub Extraction  
18.c: Vector Lane Swap  
19.c: Vector Bit Operation  
20.c: Vector Bit Selection  
21.c: Vector Negative  
22.c: Vector Bit Count  
23.c: Vector Neon Shift  
24.c: Vector Shift Left, Right  
25.c: Vector Shirt and Insert  
26.c: Vector Division  
27.c: Vector Comparison  
28.c: Vector Test Operation  
29.c: General Select vs NEON Select  
30.c: Vector ABS Operation  
31.c: Vector ABS Difference  
32.c: Vector ABS Comparison  
33.c: General ABS vs NEON ABS  
34.c: General MAX vs NEON MAX  
35.c: Vector Combine & HI-LOW Division  
36.c: Vector Narrow & Long  
37.c: Vector Type Casting  
38.c: Vector Reinterpretation  
39.c: General Standard Deviation vs NEON Standard Deviation  
40.c: Vector Inverse & Expansion  
41.c: Vector Division  
42.c: Vector Interleave & De-Interleave  
43.c: Vector Scalar Multiplication  
44.c: 4x4 Matrix Multiplication General vs NEON  
44.c: 44.c upgrade version  
