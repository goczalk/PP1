#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
Dana jest lista wartoœci: 1,2,3,4,5,6,7,8,9,10.
Zapisz zawartoœæ listy do pliku. Nastêpnie wczytaj j¹ i porównaj (musz¹ byæ równe).
Wyœwietl obie listy.
*/

typedef struct {
    int value;
    char sign;
    char string[100];
    struct node_t * next;
} node_t;

node_t * _combine_lock_erase_iterate(node_t ** root, node_t * o, node_t ** result);
void add_to_end(node_t ** root, node_t * item);
void concatenate(node_t ** root, node_t ** result);
void combine_lock(node_t ** root, node_t ** root2, node_t ** result);
int compare_lists(node_t * list1, node_t * list2);
void divide_list(node_t ** root, node_t ** root2);
void erase_node_and_add(node_t ** root, node_t * o, node_t ** deleted_list);
void free_list(node_t * root);
void insert_string_to_list_chars(const char * string, node_t **root);
void insert_string_to_list_words(const char * string, node_t **root);
void print_list(const node_t * root);
void print_char_list(const node_t * root);
void print_string_list(const node_t * root);
void read_from_file(const char* file, node_t ** root);
void revert_list(node_t ** root, node_t ** result);
void write_to_file(const char* file, const node_t * root);

int main(){
    node_t *a, *ab, *rootA=NULL;
    //druga lista
    node_t *d, *da, *rootD=NULL;
    int u;

    /*poczatek rootA*/
    a = (node_t *)malloc(sizeof(node_t));
    a->value = 1;
    a->next = NULL;
    rootA = a;

    for(u = 2; u<=10; u++){
        ab = (node_t *)malloc(sizeof(node_t));
        a->next = ab;
        ab->value = u;
        a = ab;
    }
    a->next = NULL;
    /*koniec rootA*/

    printf("Lista A: \n");
    print_list(rootA);

    char plik[10] = "test.txt";
    write_to_file(plik, rootA);
    read_from_file(plik, &rootD);

    if(compare_lists(rootA, rootD) == 1) printf("Roznia sie");

    printf("Lista D: \n");
    print_list(rootD);

    free_list(rootA);
    free_list(rootD);
    return 0;
}

void add_to_end(node_t ** root, node_t * item){
    node_t * o;
    item->next = NULL;

    //jesli lista jest 0 elementowa
    if(*root == NULL){
        *root = item;
        return;
    }

    //przejdz do ostatniego elementu
    for(o = *root; o->next != NULL; o=o->next);
    o->next = item;
}

//do result dolacz liste root - usuwajac z root
void concatenate(node_t ** root, node_t ** result){
    node_t * o, *temp;
    for(o=*root; o != NULL;){
        temp = o->next;
        erase_node_and_add(root, o, result);
        o = temp;
    }
}

//wewn do combine_lock
node_t * _combine_lock_erase_iterate(node_t ** root, node_t * o, node_t ** result){
    node_t * temp;
    if (o != NULL){
            temp = o->next;
            erase_node_and_add(root, o, result);
            o = temp;
            return o;
    }
    return NULL;
}

//z root i root2 tworzy na zamek result
void combine_lock(node_t ** root, node_t ** root2, node_t ** result){
    node_t *oA = *root, *oD = *root2;
    for(; oA != NULL || oD != NULL;){
        //rozpatrzenie opcji nierownolicznych list!
        oA = _combine_lock_erase_iterate(root, oA, result);
        oD = _combine_lock_erase_iterate(root2, oD, result);
    }
}

//zwraca 1 gdy nie sa takie same; 0 gdy sa takie same
int compare_lists(node_t * list1, node_t * list2){
    node_t * o1=list1, * o2=list2;

    for(;o1->next != NULL && o2->next != NULL;){
        if (o1->value != o2->value){
            return 1;
        }
        o1=o1->next;
        o2=o2->next;
    }

    //jesli obydwa jednoczesnie sa NULL to sa takie same
    if(o1->next == NULL && o2->next == NULL){
        return 0;
    }
    return 1;
}

//z listy root wytnij co drugi element do listy root2
void divide_list(node_t ** root, node_t ** root2){
    node_t *o, *temp;
    int index = 1;
    for(o=*root; o != NULL;){
        //jesli niepatrzysta: zostaw
        //jesli patrzysta: dodaj do listy root2
        if(index%2 == 0){
            temp = o->next;
            erase_node_and_add(root, o, root2);
            o = temp;
            index++;
            continue;
        }
        index++;
        o = o->next;
    }
}

//usun node'a z listy i dodaj do listy deleted
void erase_node_and_add(node_t ** root, node_t * o, node_t ** deleted_list){
    node_t *temp = *root;

    //przypadek usuwania pierwszego
    if (o == *root){
        *root = o->next;
        add_to_end(deleted_list, o);
        return;
    }

    //przejdz do node'a o jeden wczesniej niz usuwany
    //przypadek ostatniego uwzgledniony
    for(temp; temp->next != o; temp = temp->next);

    temp->next = o->next;
    add_to_end(deleted_list, o);
}

void free_list(node_t * root){
    node_t * o, *temp;
    for(o = root; o != NULL; o = temp){
        temp = o->next;
        free(o);
    }
}

//wstaw kazdy znak ze stringa do listy root
void insert_string_to_list_chars(const char * string, node_t **root){
    node_t *a, *ab;
    char * temp=string;

    for(temp; *temp != '\0'; temp++){
        a = (node_t *)malloc(sizeof(node_t));
        a->sign = *temp;
        a->next = NULL;
        add_to_end(root, a);
    }
}

//wstaw kazde slowo ze stringa do listy root
void insert_string_to_list_words(const char * string, node_t **root){
    node_t *a;
    char * temp=string;
    char * pch;

    //strtok podzial na slowa
    pch = strtok (temp," ,.-");
    a = (node_t *)malloc(sizeof(node_t));
    strcpy(a->string, pch);
    a->next = NULL;
    add_to_end(root, a);

    while (pch != NULL){
        pch = strtok (NULL, " ,.-");
        if ( pch != NULL){
            a = (node_t *)malloc(sizeof(node_t));
            strcpy(a->string, pch);

            //printf("%s \t", a->string);

            a->next = NULL;
            add_to_end(root, a);
        }
        else {return;}
    }
}

void print_list(const node_t * root){
    node_t * o;
    for(o = root; o != NULL; o = o->next){
        printf("%d\n", o->value);
    }
}

void print_char_list(const node_t * root){
    node_t * o;
    for(o = root; o != NULL; o = o->next){
        printf("%c \t", o->sign);
    }
}

void print_string_list(const node_t * root){
    node_t * o;
    for(o = root; o != NULL; o = o->next){
        printf("%s ", o->string);
    }
}

//wczytaj do listy root
void read_from_file(const char* file, node_t ** root){
	FILE *g;
	g = fopen(file, "r");
	if(!g){printf("Blad otwierania pliku.");
                    return 0;}

    node_t * a;
    int temp;
    while(!feof(g)){
        fscanf(g, "%d", &temp);

        a = (node_t *)malloc(sizeof(node_t));
        a->value = temp;
        a->next = NULL;
        add_to_end(root, a);
    }
	fclose(g);
}

//odwraca element z listy root i dodaje do result; kasuje z root
void revert_list(node_t ** root, node_t ** result){
    node_t * o = *root, *temp;
    while(o->next != NULL){
        //przejdz na element ostatni
        temp = o;
        for(temp; temp->next != NULL; temp = temp->next);
        erase_node_and_add(root, temp, result);
    }
    erase_node_and_add(root, o, result);
    *root = NULL;
}

//zapisz cala liste
void write_to_file(const char* file, const node_t * root){
	FILE *g;
	g = fopen(file, "w");
	if(!g){printf("Blad otwierania pliku.");
                    return 0;}

    node_t * o=root;
    for(o;o != NULL; o=o->next){
        fprintf(g, " %d", o->value);
    }
	fclose(g);
}
