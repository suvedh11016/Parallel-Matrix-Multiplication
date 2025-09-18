                                                                Operating Systems–2: Spring 2024
                                                                
	                                                    Programming Assignment 3: Dynamic Matrix Squaring
																																								
																    Name    : SUVEDH DEVA
																	Roll No : CS22BTECH11016

This programming assignment focuses on when there is a shared counter C(counter in mine) increased by each thread so to avoid synchronisation issues
when thread compete increament the counter C by a value of rowInc using different mutual exclusion algorithms as
(a) TAS (b) CAS (c) Bounded CAS (d) Atomic increament and time excecution for each algo.
									

INPUT FORMAT

The program reads input from a file named inp.txt. The format of inp.txt should be as follows:

N K rowInc
a11 a12 ... a1N
a21 a22 ... a2N
...
aN1 aN2 ... aNN

- N      : Size of the square matrix (N x N)
- K      : Number of threads
- rowInc : each time shared counter increased by rowInc



Compilation: Compile the code using a C++ compiler. For example:

	  g++ Assgn3_Src_METHODNAME-\<CS22BTECH110116\>.cpp

NOTE:
HERE METHOD NAME CHANGES AS TAS,CAS,BOUNDEDCAS,ATOMIC.

Execution: Run the compiled executable:

    	./a.out

Output: 
             TIME OF EXECUTION TO COMPLETE SQUARE MULTIPLICATION FOR RESPECTIVE ALGORITHM AND SQUARE MATRIX AFTER MULTIPLICATION.
             
EXPERIMENTS : 4 EXPERIMENTS 
              I)    TIME VS SIZE OF INPUT(N)
              II)   TIME VS rowInc
              III)  TIME VS NO OF THREADS(K)
              IV)   TIME VS ALGORITHMS
              THERE RESPECTIVE GRAPHS AND OBSERVATIONS
**I AM SUBMITTING FOUR SOURCE CODE FILE FOR EACH ALGORITHMS EACH ALGORITHM OUTPUT FILE FOR RESPECTIVE ALGO IS ITS NAME LIKE TAS ALGO AS TAS.CPP CAS AS CAS.CPP SIMILARLY FOR OTHER AND THE OUTPUT FILES WILL BE OUT_TAS.TXT ETC***