#include <iostream>
using namespace std;

#define N 6

// function to print the matrix

void printMatrix(int matrix[N][N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
    cout << "--------------------------------" << endl;
}

void rotateMatrix(int matrix[N][N], int layer)
{
    int start = layer;       // top - left index of layer
    int end = N - 1 - layer; // bottom-right index of layer

    // Save the top left element of the layer
    int temp = matrix[layer][layer];

    // STEP 1: Move ele in left column UP
    //  (row+1, -> row)
    for (int i = start; i < end; i++)
    {
        matrix[i][start] = matrix[i + 1][start];
    }

    // STEP 2: Move ele in bottom column LEFT
    // (col+1 -> col)
    for (int i = start; i < end; i++)
    {
        matrix[end][i] = matrix[end][i + 1];
    }

    // STEP 3: Move ele in right column DOWM
    // (row-1 -> row)
    for (int i = end; i > start; i--)
    {
        matrix[i][end] = matrix[i - 1][end];
    }

    // STEP 4: Move ele in top row right
    // (col-1 -> col)
    for (int i = end; i > start + 1; i--)
    {
        matrix[start][i] = matrix[start][i - 1];
    }

    matrix[start][start + 1] = temp;
}

int main()
{
    int matrix[N][N];

    // initialize matrix with 1..64

    int val = 1;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            matrix[i][j] = val++;
        }
    }

    cout << "Original Matrix:" << endl;
    printMatrix(matrix);

    int layer = 1;

    // rotate matrix inner layer( layer = 1 )
    rotateMatrix(matrix, layer);

    cout << "Matrix after Rotating inner later(" << layer << "):" << endl;
    printMatrix(matrix);

    return 0;
}