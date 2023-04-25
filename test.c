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

price *insertPrice(price *p, int priceValue) {
    price *new = (price *)malloc(sizeof(price));
    new->prev = p->prev;
    new->next = p;
    if (p->prev != NULL) {
        p->prev->next = new;
    }
    p->prev = new;
    return new;
}

price *findPosition(price *root, int p) {
    if (root->price >= p) {
        return (insertPrice(root, p));
    }
    else if (root->next != 0) {
        findPosition(root->next, p);
    }
    else {
        price *new = (price *)malloc(sizeof(price));
        new->prev = root;
        root->next = new;
        return new;
    }
}

void ffff(price **p) {
    price *new = (price *)malloc(sizeof(price));
    new->price = 235423424;
    *p = new;
}

int main() {
    price *p1 = (price *)malloc(sizeof(price));
    p1->price = 324;
    price *p2 = (price *)malloc(sizeof(price));
    p2->price = 3234;
    price *p3 = (price *)malloc(sizeof(price));
    p3->price = 334;
    expire **exp = (expire **)calloc(3, sizeof(expire));
    ffff(&p1);
    return 0;
}