#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct position position;

typedef struct bakery { // bakery有rank跟toast兩個性質
    int rating;
    int toast;
    int pos_i;
    int pos_j;
    position *pos;
} bakery;

typedef struct position {
    position *top;
    position *bot;
    position *left;
    position *right;
    bakery *pos;
} position;

bakery bakeryInit() {
    bakery b = {0, 0, 0, 0, NULL};
    return b;
}

position positionInit() {
    position p = {NULL, NULL, NULL, NULL, NULL};
    return p;
}

void close_bakery(position pos) {
    if (pos.left != NULL) {
        pos.left->right = pos.right;
    }
    if (pos.right != NULL) {
        pos.right->left = pos.left;
    }
    if (pos.top != NULL) {
        pos.top->bot = pos.bot;
    }
    if (pos.bot != NULL) {
        pos.bot->top = pos.top;
    }
}

void open_bakery(position pos) {
    if (pos.left != NULL) {
        pos.left->right = &pos;
    }
    if (pos.right != NULL) {
        pos.right->left = &pos;
    }
    if (pos.top != NULL) {
        pos.top->bot = &pos;
    }
    if (pos.bot != NULL) {
        pos.bot->top = &pos;
    }
}
int nearest(position pos1, position *nearby, int dir) { //檢查tosat夠不夠，如果不夠就關店
    while (nearby->pos->toast <= 0) {
        close_bakery(*nearby);
        if (dir == 0) {
            if (nearby->top != NULL) {
                nearby = nearby->top;
            }
            else {
                return 10000000;
            }
        }
        else if (dir == 1) {
            if (nearby->bot != NULL) {
                nearby = nearby->bot;
            }
            else {
                return 10000000;
            }
        }
        else if (dir == 2) {
            if (nearby->left != NULL) {
                nearby = nearby->left;
            }
            else {
                return 10000000;
            }
        }
        else if (dir == 3) {
            if (nearby->right != NULL) {
                nearby = nearby->right;
            }
            else {
                return 10000000;
            }
        }
    }
    return abs(nearby->pos->pos_i - pos1.pos->pos_i) + (nearby->pos->pos_j - pos1.pos->pos_j);
}

position *training(position pos) {
    bakery *distination = NULL;
    int minDistance = 10000000;
    int tempDistance = 0;
    int tempDir = 5;
    position *nearby[4] = {pos.top, pos.bot, pos.left, pos.right};
    for (size_t i = 0; i < 4; i++) {
        if (nearby[i] != NULL) {
            tempDistance = nearest(pos, nearby[i], i);
            if (minDistance > tempDistance) {
                minDistance = tempDistance;
                tempDir = i;
            }
            else if (minDistance == tempDistance) {
                if (nearby[i]->pos->rating < nearby[tempDir]->pos->rating) {
                    minDistance = tempDistance;
                    tempDir = i;
                }
            }
        }
    }
    if (minDistance == 10000000) {
        return NULL;
    }
    else {
        return nearby[tempDir];
    }
}

int main() {
    int m, n, T1, R, T2, rk1, rkr, rk2, lk1, lkr, lk2, sk1, sk2; // m, n為街道數量
    struct position **grid;                                      // grid是一個二維陣列，裡面的所有元素都是bakery的指標，以及上下左右的資訊
    scanf("%d %d", &n, &m);

    bakery *rank = malloc(m * n * sizeof(bakery)); // 分配一個以struct元素組成的array，由rank排序

    // 分配grid的空間
    grid = (struct position **)malloc(n * sizeof(struct position *));
    for (size_t i = 0; i < n; i++) {
        grid[i] = (struct position *)malloc(m * sizeof(position));
        for (size_t j = 0; j < m; j++) {
            grid[i][j] = positionInit();
        }
    }

    // 開始輸入rating, toast

    int tempRank;
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            scanf("%d", &tempRank);
            grid[i][j].pos = &rank[tempRank - 1]; //指派位置及上下左右
            rank[tempRank - 1].pos = &grid[i][j];
            if (i > 0) {
                grid[i - 1][j].bot = &grid[i][j];
            }
            if (i < n - 1) {
                grid[i + 1][j].top = &grid[i][j];
            }
            if (j > 0) {
                grid[i][j - 1].right = &grid[i][j];
            }
            if (j < m - 1) {
                grid[i][j + 1].left = &grid[i][j];
            }
            grid[i][j].pos->rating = tempRank;
            grid[i][j].pos->pos_i = i;
            grid[i][j].pos->pos_j = j;
        }
    }
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            scanf("%d", &grid[i][j].pos->toast);
        }
    }

    // 輸入日數
    scanf("%d %d %d", &T1, &R, &T2);
    scanf("%d %d %d", &rk1, &lk1, &sk1);
    scanf("%d %d", &rkr, &lkr);
    scanf("%d %d %d", &rk2, &lk2, &sk2);

    // training period 1
    struct position **visited = (struct position **)calloc(lk1, sizeof(struct position *));

    visited[0] = rank[rk1 - 1].pos;
    close_bakery(*visited[0]);
    struct position **closedBakery = (struct position **)calloc(n * m, sizeof(struct position *));
    int lk_real;
    int noToast = 0;      //每天開始時被吃垮的店數
    int NoToastIndex = 0; //即時記錄被吃垮的店數

    for (size_t day = 1; day <= T1; day++) {
        lk_real = lk1;
        for (size_t i = 1; i <= lk1; i++) {
            visited[i] = training(*visited[i - 1]);
            if (visited[i] != NULL) {
                if (visited[i]->pos->toast <= sk1) {
                    visited[i]->pos->toast = 0;
                    closedBakery[NoToastIndex] = visited[i];
                    NoToastIndex++; //吃垮一間+1
                }
                else {
                    visited[i]->pos->toast -= sk1;
                }
                close_bakery(*visited[i]);
            }
            else {
                lk_real = i;
                break;
            }
        }
        open_bakery(*visited[lk_real]);
        for (size_t i = 0; i < lk_real; i++) {
            if (visited[lk_real - 1 - i] != NULL) {
                open_bakery(*visited[lk_real - 1 - i]);
                visited[lk_real - 1 - i] = NULL;
            }
        }
        visited[0] = visited[lk_real];
        visited[lk_real] = NULL;
        lk_real = lk1;
        for (size_t i = noToast; i < NoToastIndex; i++) {
            close_bakery(*closedBakery[i]);
            closedBakery[i] = NULL;
        }
        noToast = NoToastIndex; //更新每天開始時被吃垮的店數
    }
}