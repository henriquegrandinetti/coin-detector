#ifndef lista_h
#define lista_h

typedef struct ItemType{
    int points;
    float x0;
    float y0;
    float radius; //probable radius of the circle
}ItemTye;

typedef struct CelType *Pointer;

typedef struct CelType{
    ItemType Item;
    Pointer Next;
}CelType;

typedef struct{
    Pointer First, Last;
}ListType;

void MEList(ListType *List);
float Distance (float x0, float y0, float x, float y);
int ProbableRadius (ItemTye point, int minradius, int maxradius, int ***voting);
void Insert (ItemType x, ListType *List, float r, int ***voting, int minradius, int maxradius);
void Remove(ListType *List , ItemType *Item);
int Empty(ListType *List);
void Print (ListType *List);

#endif