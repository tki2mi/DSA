#include <stdio.h>
#include <stdlib.h>

typedef struct price {
    struct price *prev;
    struct price *next;
    int price;
} price;

typedef struct expire {
    struct expire *next;
    struct price *delete;
} expire;

typedef struct company {
    struct company *top;
    struct company *bot;
    int price;
    int sub;
} company;

// 刪除price節點
void removePrice(price *p) {
    if (p->prev != NULL) {
        p->prev->next = p->next;
    }
    if (p->next != NULL) {
        p->next->prev = p->prev;
    }
}

// 如果引入的expire node為null，則取代它，否則把引入的expire node當作下家，自己取代他成為head
expire *insertExpire(price *p, expire *ex) {
    expire *new = (expire *)malloc(sizeof(expire));
    new->next = NULL;
    new->delete = p;
    if (ex == NULL) {
        ex = new;
    }
    else {
        new->next = ex;
    }
    return new;
}

// 在p節點的前面插入一個price節點
price *insertPrice(price *p, int priceValue, expire **ex) {
    price *new = (price *)malloc(sizeof(price));
    new->price = priceValue;
    new->prev = p->prev;
    new->next = p;
    if (p->prev != NULL) {
        p->prev->next = new;
    }
    p->prev = new;
    *ex = insertExpire(new, *ex);
    return new;
}

// 將自己的top、top的top...的sub全都+1
void subCal(company *c_bot) {
    if (c_bot->top != NULL) {
        c_bot->top->sub += 1;
        subCal(c_bot->top);
    }
}

// 建立 connectTop 方法
void connectTop(company *c_top, company *c_bot) {
    c_bot->top = c_top;
    subCal(c_bot);
    if (c_top->bot == NULL) {
        c_top->bot = c_bot;
    }
}

void merge(company **arr, int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // 建立暫存陣列
    company *L[n1];
    company *R[n2];

    // 複製數據到暫存陣列
    for (i = 0; i < n1; i++) {
        L[i] = arr[l + i];
    }
    for (j = 0; j < n2; j++) {
        R[j] = arr[m + 1 + j];
    }

    // 合併暫存陣列到原始陣列
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i]->sub >= R[j]->sub) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // 複製L[]中剩下的元素到arr[]
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // 複製R[]中剩下的元素到arr[]
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(company **arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        // 排序左半部分
        mergeSort(arr, l, m);

        // 排序右半部分
        mergeSort(arr, m + 1, r);

        // 合併已排序的兩個部分
        merge(arr, l, m, r);
    }
}

void updatePrice(price *L_new, price *L_old, company *c) {
    int old_price = L_old == NULL ? 0 : L_old->price;
    int new_price = L_new == NULL ? 0 : L_new->price;
    int delta = new_price - old_price;
    c->price += delta;

    if (c->top != NULL) {
        updatePrice(L_new, L_old, c->top);
    }
}

price *findPosition(price *root, int p, expire **ex) {
    if (root->price >= p) {
        return (insertPrice(root, p, ex));
    }
    price *tmp = root;
    while (root->price <= p) {
        if (root->next != NULL) {
            root = root->next;
        }
        else {
            price *new = (price *)malloc(sizeof(price));
            new->price = p;
            new->prev = root;
            root->next = new;
            *ex = insertExpire(new, *ex);
            return tmp;
        }
    }
    insertPrice(root, p, ex);
    return tmp;
}

int main() {
    int N, M, C;
    scanf("%d %d %d", &N, &M, &C);

    // 建立儲存company的array
    company *arr_C[N];

    // 建立第一個 company
    for (int i = 0; i < N; i++) {
        // 建立新的 company
        company *c = (company *)malloc(sizeof(company));
        c->top = NULL;
        c->bot = NULL;
        c->sub = 0;
        c->price = 0;
        arr_C[i] = c;
    }

    for (int i = 1; i < N; i++) {
        int x;
        scanf("%d", &x);
        // 將新的 company 連接到上層的 company
        connectTop(arr_C[x - 1], arr_C[i]);
    }

    mergeSort(arr_C, 0, N - 1);

    // 讀取第二部分輸入

    price *price_l[N];
    expire **expire_arr = (expire **)calloc(M, sizeof(expire *)); // 建立 expire array

    for (size_t j = 0; j < N; j++) {
        int c, d;
        scanf("%d %d", &c, &d);
        price_l[j] = (price *)malloc(sizeof(price));
        price_l[j]->price = c;
        expire_arr[d] = insertExpire(price_l[j], expire_arr[d]);
    }
    for (size_t i = 1; i < M; i++) {
        for (size_t j = 0; j < N; j++) {
            int c, d;
            scanf("%d %d", &c, &d);
            price_l[j] = findPosition(price_l[j], c, &expire_arr[i + d]);
            expire_arr[d] = insertExpire(price_l[j], expire_arr[i + d]);
        }
    }

    // 釋放記憶體
    for (int i = 0; i < N; i++) {
        free(arr_C[i]);
    }
    return 0;
}
