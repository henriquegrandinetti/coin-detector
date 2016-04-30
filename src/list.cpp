#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "list.h"
#include <iostream>

void MEList(ListType *List){
    List->First = (Pointer) malloc(sizeof(CelType));
    List->Last = List->First;
    List->First->Next = NULL;
};

float Distance (float x0, float y0, float x, float y){
    return sqrt(pow(x0 - x, 2) + pow(y0 - y, 2));
}

int ProbableRadius (ItemTye point, int minradius, int maxradius, int ***voting){
    int r, max = 0, radius = 0;
    for (r = minradius; r < maxradius; r++) {
        if (voting[(int)point.x0][(int)point.y0][r - minradius] > max){
            max = voting[(int)point.x0][(int)point.y0][r - minradius];
            radius = r;
        }
    }

    return radius;
}

int CheckCenter (ItemType point, ListType *List, int r, int ***voting, int minradius, int maxradius){
    Pointer q;
    q = List->First->Next;
    
    while (q != NULL) {
        if (Distance(q->Item.x0, q->Item.y0, point.x0, point.y0) <= r) {
            q->Item.x0 = (q->Item.points * q->Item.x0 + point.x0)/ (q->Item.points + 1);
            q->Item.y0 = (q->Item.points * q->Item.y0 + point.y0)/ (q->Item.points + 1);
            q->Item.radius = (q->Item.radius * q->Item.points + ProbableRadius(point, minradius, maxradius, voting))/ (q->Item.points + 1);
            q->Item.points++;
            return 1; //if the point is in a coin already detected
        }
        q = q->Next;
    }
    
    return 0;
}

void Insert (ItemType x, ListType *List, float r, int ***voting, int minradius, int maxradius){
    if (CheckCenter(x, List, r, voting, minradius, maxradius) == 0){
        List->Last->Next = (Pointer) malloc(sizeof(CelType));
        List->Last = List->Last->Next;
        List->Last->Item = x;
        List->Last->Item.points = 1;
        List->Last->Item.radius = ProbableRadius(x, minradius, maxradius, voting);
        List->Last->Next = NULL;
    }
}

void Remove(ListType *List , ItemType *Item){
    
    Pointer q;
    q = List->First->Next;
    
    Item->points = q->Item.points;
    Item->x0 = q->Item.x0;
    Item->y0 =  q->Item.y0;
    
    List->First->Next = q->Next;
    
    if(List->First->Next == NULL)
        List->Last = List->First;
    free(q);
}

int Empty(ListType *List){
    return (List->First == List->Last);
}

void Print (ListType *List){
    Pointer q;
    q = List->First->Next;
    
    while (q != NULL) {
        q = q->Next;
    }
}