#include <stdio.h>
#include "calc.h"


int main(){
    int choice, a, b;
    printf("Simple Calculator\n");
    printf("1. Add\n2. Subtract\n3. Multiply\n4. Divide\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    printf("Enter two numbers: ");
    scanf("%d %d", &a, &b);

    switch (choice)
    {
    case 1:
        printf("Result: %d\n", add(a, b));
        break;
    case 2:
        printf("Result: %d\n", subtract(a, b));
        break;
    case 3:
        printf("Result: %d\n", multiply(a, b));
        break;
    case 4:    
        printf("Result: %.2f\n", divide(a, b));
        break;
    default:
        break;
    }
    return 0;
}