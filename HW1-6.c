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
        pos.left->right = pos.pos->pos;
    }
    if (pos.right != NULL) {
        pos.right->left = pos.pos->pos;
    }
    if (pos.top != NULL) {
        pos.top->bot = pos.pos->pos;
    }
    if (pos.bot != NULL) {
        pos.bot->top = pos.pos->pos;
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
    return abs(nearby->pos->pos_i - pos1.pos->pos_i) + abs(nearby->pos->pos_j - pos1.pos->pos_j);
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
            else if ((tempDir != 5) && (minDistance == tempDistance)) {
                if (nearby[i]->pos->rating < nearby[tempDir]->pos->rating) {
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

position node_inverse(position pos2) {
    position tempPos;
    tempPos.bot = pos2.top;
    tempPos.top = pos2.bot;
    tempPos.right = pos2.left;
    tempPos.left = pos2.right;
    tempPos.pos = pos2.pos;
    return tempPos;
}

int *findByRank(position *root, bakery *rank, int rk, int n, int m) {
    position *findRoot = rank[rk - 1].pos;
    int *pos_tmp = calloc(2, sizeof(int));
    for (size_t i = 0; i < 10000; i++) {
        if (findRoot->left != NULL) {
            findRoot = findRoot->left;
        }
        else {
            pos_tmp[0] = i;
            break;
        }
    }
    for (size_t j = 0; j < 10000; j++) {
        if (findRoot->top != NULL) {
            findRoot = findRoot->top;
        }
        else {
            pos_tmp[1] = j;
            break;
        }
    }
    if (findRoot != root) {
        pos_tmp[0] = m - 1 - pos_tmp[0];
        pos_tmp[1] = n - 1 - pos_tmp[1];
    }
    return pos_tmp;
}

position *navigate(position *root, int x, int y) {
    position *dstn = root;
    for (size_t i = 0; i < x; i++) {
        dstn = dstn->right;
    }
    for (size_t j = 0; j < y; j++) {
        dstn = dstn->bot;
    }
    return dstn;
}

position *navigate_inv(position *root, int x, int y) {
    position *dstn = root;
    for (size_t i = 0; i < x; i++) {
        dstn = dstn->left;
    }
    for (size_t j = 0; j < y; j++) {
        dstn = dstn->top;
    }
    return dstn;
}

void rotate(position *root1, position *center, position *center2, int lk) {
    position *pos_tmp1 = center;
    position *pos_tmp3 = center;
    position *pos_tmp2 = center2;
    position *pos_tmp4 = center2;
    position *temp = NULL;
    for (size_t i = 0; i < (lk / 2); i++) { //找到旋轉範圍的左上角
        pos_tmp1 = pos_tmp1->left->top;
        pos_tmp2 = pos_tmp2->left->top;
        pos_tmp3 = pos_tmp1->right->bot;
        pos_tmp4 = pos_tmp2->right->bot;
    }

    if (pos_tmp1 == root1) {
        root1 = pos_tmp2;
    }
    pos_tmp1->left->right = pos_tmp2; // pos1的左邊的右邊是pos2
    pos_tmp2->left->right = pos_tmp1; // pos2的左邊的右邊是pos1
    temp = pos_tmp1->left;
    pos_tmp1->left = pos_tmp2->left; // pos1跟pos2的左邊對調
    pos_tmp2->left = temp;

    pos_tmp1->top->bot = pos_tmp2;
    pos_tmp2->top->bot = pos_tmp1;
    temp = pos_tmp1->top;
    pos_tmp1->top = pos_tmp2->top;
    pos_tmp2->top = temp;

    pos_tmp1 = pos_tmp1->bot;
    pos_tmp2 = pos_tmp2->bot;
    for (size_t i = 0; i < lk - 2; i++) {
        pos_tmp1->left->right = pos_tmp2; // pos1的左邊的右邊是pos2
        pos_tmp2->left->right = pos_tmp1; // pos2的左邊的右邊是pos1
        temp = pos_tmp1->left;
        pos_tmp1->left = pos_tmp2->left; // pos1跟pos2的左邊對調
        pos_tmp2->left = temp;
        pos_tmp1 = pos_tmp1->bot;
        pos_tmp2 = pos_tmp2->bot;
    }

    pos_tmp1->left->right = pos_tmp2; // pos1的左邊的右邊是pos2
    pos_tmp2->left->right = pos_tmp1; // pos2的左邊的右邊是pos1
    temp = pos_tmp1->left;
    pos_tmp1->left = pos_tmp2->left; // pos1跟pos2的左邊對調
    pos_tmp2->left = temp;

    pos_tmp1->bot->top = pos_tmp2;
    pos_tmp2->bot->top = pos_tmp1;
    temp = pos_tmp1->bot;
    pos_tmp1->bot = pos_tmp2->bot;
    pos_tmp2->bot = temp;

    pos_tmp1 = pos_tmp1->right;
    pos_tmp2 = pos_tmp2->right;

    for (size_t i = 0; i < lk - 2; i++) {
        pos_tmp1->bot->top = pos_tmp2;
        pos_tmp2->bot->top = pos_tmp1;
        temp = pos_tmp1->bot;
        pos_tmp1->bot = pos_tmp2->bot;
        pos_tmp2->bot = temp;

        pos_tmp1 = pos_tmp1->right;
        pos_tmp2 = pos_tmp2->right;
    }

    pos_tmp1->bot->top = pos_tmp2;
    pos_tmp2->bot->top = pos_tmp1;
    temp = pos_tmp1->bot;
    pos_tmp1->bot = pos_tmp2->bot;
    pos_tmp2->bot = temp;

    pos_tmp1->right->left = pos_tmp2;
    pos_tmp2->right->left = pos_tmp1;
    temp = pos_tmp1->right;
    pos_tmp1->right = pos_tmp2->right;
    pos_tmp2->right = temp;

    pos_tmp1 = pos_tmp1->top;
    pos_tmp2 = pos_tmp2->top;

    for (size_t i = 0; i < lk - 2; i++) {
        pos_tmp1->right->left = pos_tmp2;
        pos_tmp2->right->left = pos_tmp1;
        temp = pos_tmp1->right;
        pos_tmp1->right = pos_tmp2->right;
        pos_tmp2->right = temp;

        pos_tmp1 = pos_tmp1->top;
        pos_tmp2 = pos_tmp2->top;
    }

    pos_tmp1->right->left = pos_tmp2;
    pos_tmp2->right->left = pos_tmp1;
    temp = pos_tmp1->right;
    pos_tmp1->right = pos_tmp2->right;
    pos_tmp2->right = temp;

    pos_tmp1->top->bot = pos_tmp2;
    pos_tmp2->top->bot = pos_tmp1;
    temp = pos_tmp1->top;
    pos_tmp1->top = pos_tmp2->top;
    pos_tmp2->top = temp;

    pos_tmp1 = pos_tmp1->left;
    pos_tmp2 = pos_tmp2->left;

    for (size_t i = 0; i < lk - 2; i++) {
        pos_tmp1->top->bot = pos_tmp2;
        pos_tmp2->top->bot = pos_tmp1;
        temp = pos_tmp1->top;
        pos_tmp1->top = pos_tmp2->top;
        pos_tmp2->top = temp;

        pos_tmp1 = pos_tmp1->left;
        pos_tmp2 = pos_tmp2->left;
    }
}

int main() {
    int m, n, T1, R, T2, rk1, rk2, lk1, lk2, sk1, sk2; // m, n為街道數量
    struct position **grid;                            // grid是一個二維陣列，裡面的所有元素都是bakery的指標，以及上下左右的資訊
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

    //分配racing period的計畫空間
    int *rkr = (int *)malloc(R * sizeof(int));
    int *lkr = (int *)malloc(R * sizeof(int));
    for (size_t i = 0; i < R; i++) {
        scanf("%d %d", &rkr[i], &lkr[i]);
    }
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
        }
        noToast = NoToastIndex; //更新每天開始時被吃垮的店數
    }

    // Racing period
    // A huge credit to Boru Lin, He's a fking genius
    for (size_t i = 0; i < NoToastIndex; i++) {
        open_bakery(*closedBakery[NoToastIndex - 1 - i]);
    }
    struct position **grid_inv; //建立一個顛倒版的地圖
    grid_inv = (struct position **)calloc(n, sizeof(struct position *));

    grid_inv[0] = (struct position *)calloc(m, sizeof(position));
    for (size_t j = 1; j < m; j++) {
        grid_inv[0][j].pos = grid[n - 1][m - j - 1].pos;
        grid_inv[0][j - 1].right = &grid_inv[0][j];
        grid_inv[0][j].left = &grid_inv[0][j - 1];
    }
    for (size_t i = 1; i < n; i++) {
        grid_inv[i] = (struct position *)calloc(m, sizeof(position));
        grid_inv[i][0].pos = grid[n - i - 1][m - 1].pos;
        for (size_t j = 1; j < m; j++) {
            grid_inv[i][j].pos = grid[n - i - 1][m - j - 1].pos;
            grid_inv[i][j - 1].right = &grid_inv[i][j];
            grid_inv[i][j].left = &grid_inv[i][j - 1];
            grid_inv[i - 1][j].bot = &grid_inv[i][j];
            grid_inv[i][j].top = &grid_inv[i - 1][j];
        }
    }

    position *root = &grid[0][0]; //開始旋轉
    position *root_inv = &grid_inv[n - 1][m - 1];
    position *center = NULL;
    position *center2 = NULL;
    int *pos_rkr = malloc(2 * sizeof(int));
    for (size_t i = 0; i < R; i++) {
        pos_rkr = findByRank(root, rank, rkr[0], n, m);
        center = navigate(root, pos_rkr[0], pos_rkr[1]);
        center2 = navigate_inv(root_inv, pos_rkr[0], pos_rkr[1]);
        rotate(root, center, center2, lkr[i]);
    }

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            grid[i][j] = *root;
            grid[i][j].pos->pos_i = i;
            grid[i][j].pos->pos_j = j;
            grid[i][j].pos->pos = &grid[i][j];
            root = root->right;
        }
        root = grid[i][0].bot;
    }
    for (size_t i = noToast; i < NoToastIndex; i++) {
        close_bakery(*closedBakery[i]);
    }

    // Training Period 2

    visited[0] = rank[rk2 - 1].pos;
    close_bakery(*visited[0]);

    for (size_t day = 1; day <= T2; day++) {
        lk_real = lk2;
        for (size_t i = 1; i <= lk2; i++) {
            visited[i] = training(*visited[i - 1]);
            if (visited[i] != NULL) {
                if (visited[i]->pos->toast <= sk2) {
                    visited[i]->pos->toast = 0;
                    closedBakery[NoToastIndex] = visited[i];
                    NoToastIndex++; //吃垮一間+1
                }
                else {
                    visited[i]->pos->toast -= sk2;
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
        lk_real = lk2;
        for (size_t i = noToast; i < NoToastIndex; i++) {
            close_bakery(*closedBakery[i]);
        }
        noToast = NoToastIndex; //更新每天開始時被吃垮的店數
    }

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m - 1; j++) {
            printf("%d ", grid[i][j].pos->toast);
        }
        printf("%d\n", grid[i][m - 1].pos->toast);
    }
}