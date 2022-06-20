# CPU-Direct-mapped-Cache-Simulator
A C++ program that intakes a .txt file that contains a list of hexadecimal values and produces the hit rate, miss rate, and AMAT as well as a visual representation 
of the cache. The program only simulates a direct-mapped cache system and the user can customize the block size and number of blocks. If the user decides not to input 
those command line values, then a 32B block of four sets is used by default. The program also shows the calculation process of the cache if the user chooses to do so.

The way to use this program is to type: ./cachesimulator [filename] [block size] [number of blocks]

I used two different test cases. The first test was a list of hexadecimal values all already the same size. There are only nine 12-bit addresses, and I simulate a 
128 B direct-mapped cache with a 32 B block. With the second test case, I wanted to test with lists of hexadecimal values of different byte sizes and if no offset bits 
were present. 
