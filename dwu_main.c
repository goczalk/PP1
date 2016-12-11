#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
Dana jest dwukierunkowa lista wartoœci: 1,2,3,4,5,6,7,8,9,10.
Napisz program, który usunie 3 losowe wybrane elementy.
Wyœwietl listê przed usuniêciem, po usuniêciu oraz usuniête wêz³y.
*/

struct T{
    int value;
    struct T * prev;
    struct T * next;
};

//kwestia kompilatora - poprzedni sposob tez dziala
typedef struct T node_t;

//gdybym nie musiala przesuwa poczatku kolejki
//void add_sorted(node_t* root, node_t* item);
//zastosowanie dawne: add_sorted(rootA, temp);
void add_sorted(node_t** root, node_t* item);
void add_to_end(node_t ** root, node_t * item);
void delete_node(node_t ** root, int n, node_t ** deleted_list);
void free_list(node_t * root);
int get_sum_of_first_n_items(node_t * root, int n);
void print_list(const node_t * root);
void print_reversed_list(const node_t * root);

int main()
{
    node_t *a, *rootA=NULL;
    node_t *b, *rootD=NULL;

    //reczne tworzenie listy dwukierunkowej
    a = (node_t *)malloc(sizeof(node_t));
    a->value = 1;
    a->prev = NULL;
    a->next = NULL;
    rootA = a;

    int u;
    for(u = 2; u<=10; u++){
        b = (node_t *)malloc(sizeof(node_t));
        b->value = u;
        a->next = b;
        b->prev = a;
        a = b;
    }
    a->next = NULL;
    //koniec tworzenia rootA

    //test dwukierunkowosci
    /*
    printf("wart rootA: %d\n", *rootA);
    printf("wart rootA->next: %d\n", *(rootA->next));
    node_t * temp;
    temp = rootA->next;
    printf("teoretyczna wart rootA: %d\n", *(temp->prev));
    */

    printf("Lista A przed usunieciem: \n");
    print_list(rootA);

    //do losowania
    int zarodek;
    zarodek = time(NULL);
	srand(zarodek);

    rootD = NULL;

    //usun 3 losowe
    for(u=0; u<3; u++){
        delete_node(&rootA, rand()%11, &rootD);
    }

    printf("Lista A po usunieciu: \n");
    print_list(rootA);

    printf("Lista A po usunieciu odwrocona: \n");
    print_reversed_list(rootA);

    printf("Lista usunietych node'ow: \n");
    print_list(rootD);

    printf("Lista usunietych node'ow odwrocona: \n");
    print_reversed_list(rootD);


    free_list(rootA);
    free_list(rootD);

    return 0;
}

void add_sorted(node_t** root, node_t* item){
    node_t * o;

    //jawnie na poczatku
    //zwroc uwage: " (*root)->value "
    if(item->value < (*root)->value){
        item->next = *root;
        item->prev = NULL;
        *root = item;
        return;
    }

    //jesli w srodku
    //przechodzenie po liscie
    for(o = *root; o->next != NULL; o = o->next){
        if(item->value <= o->next->value){
            item->next = o->next;
            item->prev = o;
            o->next = item;
            return;
        }
    }
    //jesli na koncu
    item->prev = o;
    o->next = item;
    o->next->next = NULL;
}

void add_to_end(node_t ** root, node_t * item){
    node_t * o;
    item->next = NULL;

    //jesli lista jest 0 elementowa
    if(*root == NULL){
        item->prev = NULL;
        *root = item;
        return;
    }

    //przejdz do ostatniego elementu
    for(o = *root; o->next != NULL; o=o->next);
    item->prev = o;
    o->next = item;
}

//usun node'a numer n
void delete_node(node_t ** root, int n, node_t ** deleted_list){
    node_t *o = *root, *temp;
    int i;

    //przypadek usuwania pierwszego
    if (n == 1){
        o->next->prev = NULL;
        *root = o->next;
        add_to_end(deleted_list, o);
        return;
    }

    //przejdz do node'a o jeden wczesniej
    //przypadek usuwania ostatniego uwzgledniony
    for(i=2; i<n;i++){
        o = o->next;
    }
    temp = o->next;
    o->next = temp->next;
    //przypadek ostatniego node'a
    if(temp->next != NULL){
        temp->next->prev = o;
    }
    add_to_end(deleted_list, temp);
}

void free_list(node_t * root){
    node_t * o, *temp;
    for(o = root; o != NULL; o = temp){
        temp = o->next;
        free(o);
    }
}

int get_sum_of_first_n_items(node_t * root, int n){
    node_t * o = root;
    int i, sum = 0;
    for(i=0; i<n; i++){
        sum += o->value;
        o = o->next;
    }
    return sum;
}

void print_list(const node_t * root){
    node_t * o;
    for(o = root; o != NULL; o = o->next){
        printf("%d\n", o->value);
    }
}

//test czy lista jest napewno dwukierunkowa
void print_reversed_list(const node_t * root){
    node_t * o;
    for(o = root; o->next != NULL; o = o->next);
    for(o; o != root; o = o->prev){
        printf("%d\n", o->value);
    }
    //jeszcze root'a
    printf("%d\n", root->value);
}
