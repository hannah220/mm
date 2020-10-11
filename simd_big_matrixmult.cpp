#include <iostream>
#include <random>
using namespace std;
#include "nmmintrin.h"

int main()
{
  int a1, a2, b1, b2, i, j, k, l;

  std::random_device rd;  //Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<> distrib1(2, 100000);
  std::uniform_real_distribution<> distrib2(0, 100);

  int n = distrib1(gen);
  cout << "Dimension of matrix: " << n << endl;
 
  a1 = a2 = b1 = b2 = n; 

  // Data structure for N*N matrix
  vector<vector<float> > a(n, vector<float>(n));
  vector<vector<float> > b(n, vector<float>(n));
  vector<vector<float> > mult(n, vector<float>(n));

  for(i = 0; i < a1; ++i) {
    for(j = 0; j < a2; ++j) {
      a[i][j] = distrib2(gen);
      b[i][j] = distrib2(gen);
    }
  }

  /* In case we want to display matrix a and b
  cout << endl << "Matrix a: " << endl;
  for(i = 0; i < a1; ++i) {
    for(j = 0; j < a2; ++j) {
      cout << " " << a[i][j];
      if(j == a2-1)
        cout << endl;
    }
  }

  cout << endl << "Matrix b: " << endl;
  for(i = 0; i < b1; ++i) {
    for(j = 0; j < b2; ++j) {
      cout << " " << b[i][j];
      if(j == b2-1)
        cout << endl;
    }
  } 
  */

  // Initialize elements of matrix mult to 0.
  for(i = 0; i < a1; ++i) {
    for(j = 0; j < b2; ++j)
    {
      mult[i][j]=0;
    }
  }

  float m4;
  int mod4 = a2 % 4;
  for(i = 0; i < a1; ++i) {
    for(j = 0; j < b2; ++j)
      for(k = 0; k < a2; k += 4) {
        if (k + 3 < a2) {
          // Distribute four elements to SSE registers
          __m128 a4 = _mm_set_ps( a[i][k], a[i][k+1], a[i][k+2], a[i][k+3] );
          __m128 b4 = _mm_set_ps( b[k][j], b[k+1][j], b[k+2][j], b[k+3][j] );
          // Multiply four elements together
          __m128 sum4 = _mm_mul_ps(a4, b4);
          // Sum up the elements of sum4 and store in m4
          sum4 = _mm_hadd_ps(sum4, sum4);
          sum4 = _mm_hadd_ps(sum4, sum4);
          _mm_store_ss(&m4, sum4);
          // Insert the result of mul and sum of a's ith row and b's jth column
          mult[i][j] += m4;

        } else { // There are left less than four elements
          __m128 a4, b4;
          switch (mod4) {
            case 1: 
              a4 = _mm_set_ps( a[i][k], 0, 0, 0);
              b4 = _mm_set_ps( b[k][j], 0, 0, 0);
              break;
            case 2:
              a4 = _mm_set_ps( a[i][k], a[i][k+1], 0, 0);
              b4 = _mm_set_ps( b[k][j], b[k+1][j], 0, 0);
              break;
            case 3:
              a4 = _mm_set_ps( a[i][k], a[i][k+1], a[i][k+2], 0);
              b4 = _mm_set_ps( b[k][j], b[k+1][j], b[k+2][j], 0);
              break;
          }
          // Multiply four elements together
          __m128 sum4 = _mm_mul_ps(a4, b4);
          // Sum up the elements of sum4 and store in m4
          sum4 = _mm_hadd_ps(sum4, sum4);
          sum4 = _mm_hadd_ps(sum4, sum4);
          _mm_store_ss(&m4, sum4);
          // Insert the result of mul and sum of a's ith row and b's jth column
          mult[i][j] += m4;
        }
      }
  }

  // Displaying the multiplication of two matrix.
  cout << endl << "Output Matrix: " << endl;
  for(i = 0; i < a1; ++i) {
    for(j = 0; j < b2; ++j) {
      cout << " " << mult[i][j];
      if(j == b2-1)
        cout << endl;
    }
  }

  return 0;
}
