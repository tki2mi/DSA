#include <math.h>
#include <stdio.h>

int plus(int lenA, int lenB, int a[], int b[], int output[]) {
    int digit = 0;
    for (int i = 0; i < lenA; i++) {
        digit = i + 1;
        output[i] += (a[i] + b[i]);
        if (output[i] >= 10) {
            output[i] -= 10;
            output[i + 1] += 1;
            digit++;
        }
    }
    return digit;
}
int subtract(int *lenA, int lenB, int a[], int b[]) {
    int output[256] = {0};
    int digit = 0;
    for (int i = 0; i < *lenA; i++) {
        digit = i + 1;
        output[i] += a[i] - b[i];
        if (output[i] < 0) {
            output[i] += 10;
            output[i + 1] -= 1;
        }
    }
    for (int i = 0; i < *lenA; i++) {
        if (output[*lenA - 1 - i] != 0) {
            break;
        }
        else {
            digit--;
        }
    }
    for (int i = 0; i < *lenA; i++) {
        a[i] = output[i];
    }
    *lenA = digit;
}
void multiplyBy2(int *lenA, int a[]) {
    int output[256] = {0};
    int digit = 0;
    for (int i = 0; i < *lenA; i++) {
        digit = i + 1;
        output[i] += a[i] * 2;
        if (output[i] >= 10) {
            output[i] -= 10;
            output[i + 1] += 1;
            digit++;
        }
    }
    for (int i = 0; i < digit; i++) {
        a[i] = output[i];
    }
    *lenA = digit;
}
void dividedBy2(int *lenA, int a[]) {
    int output[256] = {0};
    int digit = 0;
    for (int i = 0; i < *lenA; i++) {
        digit = i + 1;
        output[i] += a[i] / 2;
        if (a[i] % 2 != 0) {
            output[i - 1] += 5;
        }
    }
    if (a[*lenA - 1] == 1) {
        digit--;
        a[*lenA - 1] = 0;
    }
    for (int i = 0; i < digit; i++) {
        a[i] = output[i];
    }
    *lenA = digit;
}
void swap(int *lenA, int *lenB, int a[], int b[]) {
    int temp = 0;
    for (int i = 0; i < *lenB; i++) {
        temp = a[i];
        a[i] = b[i];
        b[i] = temp;
    }
    temp = *lenA;
    *lenA = *lenB;
    *lenB = temp;
}
void sort(int *lenA, int *lenB, int a[], int b[]) {
    if (*lenB > *lenA) {
        swap(lenA, lenB, a, b);
        return;
    }
    else if (*lenA == *lenB) {
        for (int i = 0; i < *lenA; i++) {
            if (a[*lenA - i - 1] > b[*lenB - i - 1]) {
                return;
            }
            else if (a[*lenA - i - 1] < b[*lenB - i - 1]) {
                swap(lenA, lenB, a, b);
                return;
            }
        }
    }
    return;
}
int input(int input[]) {
    while (1) {
        char x;
        int temp = 0;
        for (int i = 0; i < 256; i++) {
            x = getchar();
            if (x >= '0' && x <= '9') {
                input[i] = x - 48;
            }
            else {
                int temp;
                for (int j = 0; j < ((i + 1) / 2); j++) {
                    temp = input[j];
                    input[j] = input[i - 1 - j];
                    input[i - 1 - j] = temp;
                }
                return (i);
            }
            /*for (int j = 0; j < 8; j++) {
                x = getchar();
                if (x >= '0' && x <= '9') {
                    temp = x - 48;
                    for (int k = 0; k < (7 - j); k++) {
                        temp *= 10;
                    }
                    input[i] += temp;
                } else {
                    for (int k = 0; k < (8 - j); k++) {
                        input[i] /= 10;
                    }
                    return ((8 * i) + j);
                }
            }*/
        }
    }
}
void printOutPut(int len, int output[]) {
    for (int i = 0; i < len; i++) {
        printf("%d", output[len - i - 1]);
    }
}
int main() {
    int input1[256] = {0};
    int input2[256] = {0};
    int len1 = input(input1);
    int len2 = input(input2);
    sort(&len1, &len2, input1, input2);
    int k = 0;
    while ((len1 != 0) && (len2 != 0)) {
        if ((input1[0] % 2 == 0) && (input2[0] % 2 == 0)) {
            k++;
            dividedBy2(&len1, input1);
            dividedBy2(&len2, input2);
        }
        else if (input2[0] % 2 == 0) {
            dividedBy2(&len2, input2);
        }
        else if (input1[0] % 2 == 0) {
            dividedBy2(&len1, input1);
        }
        sort(&len1, &len2, input1, input2);
        subtract(&len1, len2, input1, input2);
    }
    for (int i = 0; i < k; i++) {
        multiplyBy2(&len2, input2);
    }
    printOutPut(len2, input2);
    return 0;
}
