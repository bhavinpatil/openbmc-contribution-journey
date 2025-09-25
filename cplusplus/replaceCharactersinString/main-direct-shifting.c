#include <stdio.h>
#include <string.h>

int main()
{
    char str[100] = "xxxaxxxbxxxcxxxd"; // original array
    int len = strlen(str);              // current length of string
    printf("Before Ops: %s\n\n", str);

    // Define struct operations
    struct Operation
    {
        int position;
        char *new_str;
    } ops[] = {
        {3, "tiger"}, // replace 'a' at index 3
        {7, "lion"},  // replace 'b' at index 7
        {11, "cat"}   // replace 'c' at index 11
    };

    int num_ops = sizeof(ops) / sizeof(ops[0]);

    int total_shifts = 0; // keep track of how indices shift after insertions

    for (int i = 0; i < num_ops; i++)
    {
        int index = ops[i].position + total_shifts; // adjust the index for previous shifts

        char *s = ops[i].new_str;

        int s_len = strlen(s);

        // calculate how many positions we need to shift
        int shift_len = s_len - 1; //-1 bacause original char 'a'/'b'/'c' will be removed

        // shift everything to the right to make space
        for (int j = len - 1; j >= index; j--)
        {
            str[j + shift_len] = str[j];
        }

        // Insert new string at index
        for (int j = 0; j < s_len; j++)
        {
            str[index + j] = s[j];
        }

        // Update string length and total shifts

        len += shift_len;
        total_shifts += shift_len;

        str[len] = '\0'; // null terminate
        printf("After op %d (%s at %d): %s\n", i + 1, ops[i].new_str, ops[i].position, str);
    }

    return 0;
}