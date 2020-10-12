## Matrix Multiplication

* Task 1/2/3: Matrix multiplicaiton using standard algorithm 
  * Compile/execute matrixmult.cpp to multiply N * M matrices of your choice
  
* Task 4: Matrix multiplicaiton using SIMD
  * Compile/execute simd_big_matrixmult.cpp to multiply arbitrary N * N matrices where N range from 2 to 10^5. 

* Bonus: Matrix multiplicaiton using Devide and Conquer (DAC) strategy 
  * Compile/execute dac.cpp to multiply arbitrary N * N matrices where N is 2^n, n ranging from 4 to 16.
  * DAC is used to until the dimension of matrix is 16, after that Task4's implementation is used to calculate multiplication. 
  
* Result: 
  * The graph below shows the computation time (sec) on y axis and dimenstion of matrices N * N on x axis. 
    * Blue line represents the time using SIMD (simd_big_matrixmult.cpp)
    * Orange line reptresnts the time using DAC strategy (dac.cpp)
  * SIMD is much faster than DAC.
    * DAC has occupied about 100 times more memory than pure SIMD approach. 
    * This could be because “dividing” matrix to four matrices needs lot of memory copy and thus also slowing down the computation compared to pure SIMD approach. 
    
![Alt text](https://github.com/hannah220/mm/mm1.jpg?raw=true)
