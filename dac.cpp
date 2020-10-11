#include <iostream>
#include <random>
#include <cmath>
#include "nmmintrin.h"
using namespace std;

void add(vector<vector<float> > &r, vector<vector<float> > &a, vector<vector<float> > &b, int rowR, int colR);

vector<vector<float> > mult(vector<vector<float> > a, vector<vector<float> > b) {
  int i, j, k, l;
  int n = a.size();
  vector<vector<float> > mult(n, vector<float>(n));
  float m4;
  int mod4 = n % 4;

  for(i = 0; i < n; ++i) {
    for(j = 0; j < n; ++j)
      for(k = 0; k < n; k += 4) {
        if (k + 3 < n) {
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

  return mult;
}

void split(vector<vector<float> > &x, vector<vector<float> > &x1, vector<vector<float> > &x2, vector<vector<float> > &x3, vector<vector<float> > &x4) {
  int i, j;
  int n = x.size();
    for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      if (i < n/2 && j < n/2) {
        x1[i][j] = x[i][j];
      } else if (i < n/2 && j >= n/2) {
        x2[i][j - n/2] = x[i][j];
      } else if (i >= n/2 && j < n/2) {
        x3[i - n/2][j] = x[i][j];
      } else if (j >= n/2 && j >= n/2) {
        x4[i - n/2][j - n/2] = x[i][j];
      }
    }
  }
}

vector<vector<float> > rec(vector<vector<float> > a, vector<vector<float> > b, int size) {
  int i, j;
  vector<vector<float> > r(size, vector<float>(size));

  if (size == 16) {
    return mult(a, b);
  } else {
    int new_size = size / 2;
    vector<vector<float> > a1(new_size, vector<float>(new_size));
    vector<vector<float> > a2(new_size, vector<float>(new_size));
    vector<vector<float> > a3(new_size, vector<float>(new_size));
    vector<vector<float> > a4(new_size, vector<float>(new_size));

    vector<vector<float> > b1(new_size, vector<float>(new_size));
    vector<vector<float> > b2(new_size, vector<float>(new_size));
    vector<vector<float> > b3(new_size, vector<float>(new_size));
    vector<vector<float> > b4(new_size, vector<float>(new_size));

    split(a, a1, a2, a3, a4);
    split(b, b1, b2, b3, b4);

    vector<vector<float> > c11(new_size, vector<float>(new_size));
    vector<vector<float> > c12(new_size, vector<float>(new_size));
    vector<vector<float> > c21(new_size, vector<float>(new_size));
    vector<vector<float> > c22(new_size, vector<float>(new_size));
    vector<vector<float> > temp1(new_size, vector<float>(new_size));
    vector<vector<float> > temp2(new_size, vector<float>(new_size));

    temp1 = rec(a1, b1, new_size);
    temp2 = rec(a2, b3, new_size);
    add(r, temp1, temp2, 0, 0); 
    temp1 = rec(a1, b2, new_size);
    temp2 = rec(a2, b4, new_size);
    add(r, temp1, temp2, 0, new_size);
    temp1 = rec(a3, b1, new_size);
    temp2 = rec(a4, b3, new_size);
    add(r, temp1, temp2, new_size, 0);
    temp1 = rec(a3, b2, new_size);
    temp2 = rec(a4, b4, new_size);
    add(r, temp1, temp2, new_size, new_size);

  }
  return r;
}

void add(vector<vector<float> > &r, vector<vector<float> > &a, vector<vector<float> > &b, int rowR, int colR) {
  int n = a.size();
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      r[i + rowR][j + colR] = a[i][j] + b[i][j];
}

int main()
{
  int a1, a2, b1, b2, i, j, k, l;

  std::random_device rd;  //Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<> distrib1(4, 16);
  std::uniform_real_distribution<> distrib2(0, 100);

  int n = distrib1(gen);

  // Number of row/column must be 2^n 
  n = a1 = a2 = b1 = b2 = std::pow(2, n); 
  cout << "Dimension of matrix: " << n << endl;
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

  /* Recursively compute multiplication by Divide and Conquer until the size of 16 and then use standard computation using SIMD */
  vector<vector<float> > r = rec(a, b, n);

  cout << endl << "Output Matrix: " << endl;
  for(i = 0; i < n; ++i) {
    for(j = 0; j < n; ++j) {
      cout << " " << r[i][j];
      if(j == n-1)
        cout << endl;
    }
  }

  return 0;
}
