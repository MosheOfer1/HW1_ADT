#include <stdlib.h>
#include <stdio.h>
#include "AdptArray.h"

typedef struct AdptArray_
{
    int size;
    void** elements; 
    COPY_FUNC cpy_fun;
    DEL_FUNC del_fun;
    PRINT_FUNC print_fun;
} AdptArray, *PAdptArray;

// Creating a new Adaptive Array
PAdptArray CreateAdptArray(COPY_FUNC cpy_fun,DEL_FUNC del_fun,PRINT_FUNC print_fun)
{
    PAdptArray ad = (PAdptArray)malloc(sizeof(AdptArray));
    if (!ad) return NULL;
    
    ad->size = 0;
    ad->elements = NULL;
    ad->cpy_fun = cpy_fun;
    ad->del_fun = del_fun;
    ad->print_fun = print_fun;
    return ad;
}
// Earse wisely the Array
void DeleteAdptArray(PAdptArray Pad)
{
    for (size_t i = 0; i < Pad->size; i++)
    {
        if (Pad->elements[i])
        {
            (Pad->del_fun)(Pad->elements[i]);
        }
    }
    free(Pad->elements);
    free(Pad);    
}
Result SetAdptArrayAt(PAdptArray Pad,int index, PElement element)
{
    // The index is in the range
    if (Pad->size > index)
    {
        if (Pad->elements[index])
        {
            (Pad->del_fun)(Pad->elements[index]);
        }
    }
    // The index is greater then the size so need to reallocate a new place in the memory, and to update the array size.
    else
    {
        if (!Pad->elements)
        {
            Pad->elements = malloc((index + 1) * sizeof(PElement));
            if (!Pad->elements)
            {
                return FAIL;
            }
        }
        else
        {
            PElement temp = realloc(Pad->elements, (index+1) * sizeof(PElement));
            if (!temp)
            {
                return FAIL;
            }
            Pad->elements = temp;
        }
        // Init the added cells in the array to NULL
        for (size_t i = Pad->size; i < index; i++)
        {
            Pad->elements[i] = NULL;
        }
        Pad->size = index + 1;     
    }
    // Copy the element to his place
    Pad->elements[index] = (Pad->cpy_fun)(element);
    return SUCCESS;
}
PElement GetAdptArrayAt(PAdptArray Pad,int index)
{
    if (!Pad->elements[index])
    {
        return NULL;
    }
    
    return (Pad->cpy_fun)(Pad->elements[index]);
}
int GetAdptArraySize(PAdptArray Pad)
{
    return Pad->size;
}
void PrintDB(PAdptArray Pad)
{
    for (size_t i = 0; i < Pad->size; i++)
    {
        if (Pad->elements[i])
        {
            (Pad->print_fun)(Pad->elements[i]);
        }
    }
}

