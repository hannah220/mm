#include <iostream>
using namespace std;

int main()
{
  int a1, a2, b1, b2, i, j, k;

  cout << "Enter rows / columns for the first matrix: ";
  cin >> a1 >> a2;
  cout << "Enter rows / columns for the second matrix: ";
  cin >> b1 >> b2;

  while (a2!=b1)
  {
    cout << "Error! column of first matrix not equal to row of second.";

    cout << "Enter rows and columns for first matrix: ";
    cin >> a1 >> a2;

    cout << "Enter rows and columns for second matrix: ";
    cin >> b1 >> b2;
  }

  float a[a1][a2], b[b1][b2], mult[a1][b2];

  // Storing elements of first matrix.
  cout << endl << "Enter elements of matrix 1:" << endl;
  for(i = 0; i < a1; ++i) {
    for(j = 0; j < a2; ++j) {
      cout << "Enter element a" << i + 1 << j + 1 << " : ";
      cin >> a[i][j];
    }
  }

  // Storing elements of second matrix.
  cout << endl << "Enter elements of matrix 2:" << endl;
  for(i = 0; i < b1; ++i) {
    for(j = 0; j < b2; ++j) {
      cout << "Enter element b" << i + 1 << j + 1 << " : ";
      cin >> b[i][j];
    }
  }

  // Initializing elements of matrix mult to 0.
  for(i = 0; i < a1; ++i) {
    for(j = 0; j < b2; ++j)
    {
      mult[i][j]=0;
    }
  }

  // Multiplying matrix a and b and storing in array mult.
  for(i = 0; i < a1; ++i) {
    for(j = 0; j < b2; ++j)
      for(k = 0; k < a2; ++k)
      {
        mult[i][j] += a[i][k] * b[k][j];
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
