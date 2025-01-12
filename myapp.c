#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Nod
{
    int cheie;
    int balance;
    struct _Nod *st;
    struct _Nod *dr;
} Nod;

int _max(int a, int b) { return (a > b) ? a : b; }

int height(Nod *nodCurent)
{
    if (nodCurent == NULL)
        return 0;
    return 1 + _max(height(nodCurent->st), height(nodCurent->dr));
}

int balance(Nod *nodCurent)
{
    if (nodCurent == NULL)
        return 0;
    return height(nodCurent->st) - height(nodCurent->dr);
}

Nod *rotateLeft(Nod *parinte)
{
    Nod *fiudrept = parinte->dr;
    parinte->dr = fiudrept->st;
    fiudrept->st = parinte;
    parinte->balance = balance(parinte);
    fiudrept->balance = balance(fiudrept);
    return fiudrept;
}

Nod *rotateRight(Nod *parinte)
{
    Nod *fiustang = parinte->st;
    parinte->st = fiustang->dr;
    fiustang->dr = parinte;
    parinte->balance = balance(parinte);
    fiustang->balance = balance(fiustang);
    return fiustang;
}

int getBalance(Nod *nod)
{
    if (nod == NULL)
        return 0;

    return nod->balance;
}

Nod *balanceTree(Nod *nodCurent)
{
    nodCurent->balance = balance(nodCurent);

    if (nodCurent->balance > 1)
    {
        if (getBalance(nodCurent->st) < 0)
            nodCurent->st = rotateLeft(nodCurent->st);
        return rotateRight(nodCurent);
    }

    if (nodCurent->balance < -1)
    {
        if (getBalance(nodCurent->dr) > 0)
            nodCurent->dr = rotateRight(nodCurent->dr);
        return rotateLeft(nodCurent);
    }
    // verifica daca nodCurent nu este echilibrat la stanga:
    // - verifica daca e necesară o rotație dreapta => returneaza rezultatul
    // - altfel verifica dacă e necesară o rotație stânga dreapta => returneaza rezultatul

    // verifica daca nodCurent nu este echilibrat la dreapta:
    // - verifica daca e necesară o rotație stanga => returneaza rezultatul
    // - altfel verifica dacă e necesară o rotație dreapta stanga => returneaza rezultatul

    return nodCurent; // returneaza nodCurent
}

Nod *insert(Nod *nodCurent, int cheie)
{

    if (nodCurent == NULL)
    {
        Nod *n = (Nod *)malloc(sizeof(Nod));
        n->cheie = cheie;
        n->balance = 0;
        n->st = NULL;
        n->dr = NULL;
        return n;
    }
    else if (cheie > nodCurent->cheie)
    {
        nodCurent->dr = insert(nodCurent->dr, cheie);
    }
    else if (cheie < nodCurent->cheie)
    {
        nodCurent->st = insert(nodCurent->st, cheie);
    }
    else
    {
        return nodCurent;
    }

    return balanceTree(nodCurent);
}

Nod *getPred(Nod *n)
{
    if (n == NULL)
        return NULL;

    while (n->dr != NULL)
    {
        n = n->dr;
    }

    return n;
}

Nod *delete(Nod *nodCurent, int cheie)
{
    if (cheie > nodCurent->cheie)
    {
        nodCurent->dr = delete (nodCurent->dr, cheie);
    }
    else if (cheie < nodCurent->cheie)
    {
        nodCurent->st = delete (nodCurent->st, cheie);
    }
    else
    {
        if (nodCurent->st == NULL)
        {
            Nod *tmp = nodCurent->dr;
            free(nodCurent);
            return tmp;
        }

        if (nodCurent->dr == NULL)
        {
            Nod *tmp = nodCurent->st;
            free(nodCurent);
            return tmp;
        }

        Nod *pred = getPred(nodCurent->st);
        nodCurent->cheie = pred->cheie;
        nodCurent->st = delete (nodCurent->st, pred->cheie);
    }

    return balanceTree(nodCurent);
}

void preorder(Nod *nodCurent)
{

    if (nodCurent == NULL)
        return;

    printf("%d", nodCurent->cheie);
    preorder(nodCurent->st);
    preorder(nodCurent->dr);
}

void inorder(Nod *nodCurent)
{
    if (nodCurent == NULL)
        return;

    inorder(nodCurent->st);                                // parcurge subarb st
    printf("%d %d", nodCurent->cheie, nodCurent->balance); // nod curent
    inorder(nodCurent->dr);                                // parcuge subsrb dr
}

void postorder(Nod *nodCurent)
{
    if (nodCurent == NULL)
        return;

    postorder(nodCurent->st);       // parcurge arb st
    postorder(nodCurent->dr);       // parcurge arb dr
    printf("%d\n", nodCurent->cheie); // viziteaza nodul
}

void printarelevel(Nod *nodCurent, int level)
{
    if (nodCurent == NULL)
        return;
    if (level == 1)
        printf("%d", nodCurent->cheie);
    else if (level > 1)
    {
        printarelevel(nodCurent->st, level - 1);
        printarelevel(nodCurent->dr, level - 1);
    }
}

void _printare(char *prefix, Nod *node, int isLeft)
{
    if (node == NULL)
    {
        return;
    }

    printf("%s", prefix);
    printf("%s", isLeft ? "|--" : "|__");

    printf("%d %d\n", node->cheie, node->balance);

    char aux[100] = "";
    sprintf(aux, "%s%s\0", prefix, (isLeft ? "|   " : "    "));
    _printare(aux, node->st, 1);
    _printare(aux, node->dr, 0);
}

void printare(Nod *nod)
{
    _printare("", nod, 0);
    printf("--------------\n");
}


void level(Nod *r)
{
    int h = height(r);
    for (int i = 1; i <= h; i++)
    {
        printarelevel(r, i);
        printf("\n");
    }
}

int main()
{
    Nod *r = NULL;
    int v[] = {1, 2, 10, 5, 4, 3};
    int n=sizeof(v)/sizeof(v[0]);
    printf("-----INSERARE-----\n");
    for (int i = 0; i < n; i++)
    {
        r = insert(r, v[i]);
        printf("Dupa inserarea %d:\n", v[i]);
        printare(r);
    }
    printf("--Traversare in pre-ordine--:\n");
    preorder(r);
    printf("\n");

    printf("--Traversarea in inordine:\n");
    inorder(r);
    printf("\n");

    printf("--Traversarea in postordine:\n");
    postorder(r);
    printf("\n");

    printf("--Traversarea pe nivel:\n");
    inorder(r);
    printf("\n");

    printf("----STERGEREA----\n");
    for (int i = 0; i < 6; i++)
    {
        r = delete (r, v[i]);
        printf("Dupa stergere %d:\n", v[i]);
        printare(r);
    }
    return 0;
}
