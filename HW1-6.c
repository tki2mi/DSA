#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct bakery { // bakery有rank跟toast兩個性質
    int rating;
    int toast;
    int visited;
} bakery;

bakery *training(bakery **grid, int i, int j, int m, int n, int sk, int day) {
    int distance = 1;
    int ratingTemp = 10000000;
    int isNull = 0; //紀錄null的數量
    bakery *visit = NULL;
    bakery *nearby[4];
    while (1) {
        if (i - distance >= 0) {
            nearby[0] = &grid[i - distance][j];
        }
        else {
            nearby[0] = NULL;
        }
        if (i + distance < n) {
            nearby[1] = &grid[i + distance][j];
        }
        else {
            nearby[1] = NULL;
        }
        if (j - distance >= 0) {
            nearby[2] = &grid[i][j - distance];
        }
        else {
            nearby[2] = NULL;
        }
        if (j + distance < m) {
            nearby[3] = &grid[i][j + distance];
        }
        else {
            nearby[3] = NULL;
        }
        for (size_t i = 0; i < 4; i++) {
            if (nearby[i] == NULL) {
                isNull++;
            }
        }
        if (isNull == 4) {
            return NULL;
        }
        for (size_t i = 0; i < 4; i++) { //決定要visit哪間，選rating最小的
            if (nearby[i] != NULL) {
                if (nearby[i]->visited == day) {
                    continue;
                }
                else if (nearby[i]->toast <= 0) {
                    continue;
                }
                else if ((visit == NULL) ||
                         (nearby[i]->rating < visit->rating)) {
                    visit = nearby[i];
                }
            }
        }
        if (visit == NULL) {
            distance++;
            continue;
        }
        else {
            visit->visited = day;
            visit->toast -= sk;
            if (visit->toast < 0) {
                visit->toast = 0;
            }
            return visit;
        }
    }
}

int main() {
    int m, n, T1, R, T2, rk1, rkr, rk2, lk1, lkr, lk2, sk1,
        sk2; // m, n為街道數量
    struct bakery **grid; // grid是一個二維陣列，裡面的所有元素都是bakery的指標
    scanf("%d %d", &n, &m);

    //分配grid的空間
    grid = (struct bakery **)malloc(n * sizeof(struct bakery *));
    for (size_t i = 0; i < n; i++) {
        grid[i] = (struct bakery *)malloc(m * sizeof(struct bakery));
        for (size_t j = 0; j < m; j++) {
            grid[i][j] = (struct bakery){0, 0, 0};
        }
    }

    //開始輸入rating, toast
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            scanf("%d", &grid[i][j].rating);
        }
    }
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            scanf("%d", &grid[i][j].toast);
        }
    }

    //輸入日數
    scanf("%d %d %d", &T1, &R, &T2);
    scanf("%d %d %d", &rk1, &lk1, &sk1);
    scanf("%d %d", &rkr, &lkr);
    scanf("%d %d %d", &rk2, &lk2, &sk2);

    // training period 1
    bakery *temp = NULL;
    int pos_i, pos_j;
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            if (grid[i][j].rating = rk1) {
                *temp = grid[i][j];
                pos_i = i;
                pos_j = j;
            }
        }
    }

    for (size_t day = 1; day <= T1; day++) {
        for (size_t i = 0; i < lk1; i++) {
            temp = training(grid, pos_i, pos_j, m, n, sk1, day);
            if (temp == NULL) {
                break;
            }
        }
    }

    // training period 2
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            if (grid[i][j].rating = rk2) {
                *temp = grid[i][j];
                pos_i = i;
                pos_j = j;
            }
        }
    }

    for (size_t day = T1 + R + 1; day <= T1 + R + T2; day++) {
        for (size_t i = 0; i < lk2; i++) {
            temp = training(grid, pos_i, pos_j, m, n, sk2, day);
            if (temp == NULL) {
                break;
            }
        }
    }

    // print output
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m - 1; j++) {
            printf("%d ", &grid[i][j]);
        }
        printf("%d\n", &grid[i][m - 1]);
    }

    //釋放記憶體
    for (size_t i = 0; i < n; i++) {
        free(grid[i]);
    }
    free(grid);
    return 0;
}