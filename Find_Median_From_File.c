/*
 * Author: PArthPAtel
 * Roll No : 111501019
 */

#include <stdio.h>
#include <stdlib.h>

int findKthPos(int*, int, int);
void insertionSort(int*, int);
void partition(int*, int, int, int, int);

int main(void)
{
    char file[50];
    printf("Enter the name of file : ");
    scanf("%[^\n]s", file);

    FILE* fp = fopen(file, "r");
    if (fp == NULL)
    {
        printf("Failed while opening file!\n");
        return 1;
    }

    int nInt;
    fscanf(fp, "%d", &nInt);

    int* data = (int*) malloc(sizeof (int)* nInt);
    if (data == NULL)
    {
        printf("Failed while allocating memory\n");
        fclose(fp);
        return 1;
    }

    int i = 0;
    while (i < nInt)
        fscanf(fp, "%d", &data[i++]);
    fclose(fp);

    //if nInt is even then cosidering first integer out of two middle integer as a median
    printf(" Median of these integers is ->>> %d\n", findKthPos(data, nInt, (nInt + 1) / 2));
    free(data);
    return 0;
}

//return kth smallest integer in given array
//nInt is number of integers in the array

int findKthPos(int* array, int nInt, int k)
{
    if (nInt < 6)
    {
        insertionSort(array, nInt);
        return array[k - 1];
    }
    else
    { //when nInt is not a multiple of 5 answer is rounded up
        //that is taken care in next steps
        int nGroups = nInt / 5;
        int tmp = nGroups;
        int i = 0;
        //sorting groups of 5 in increasing order
        while (tmp--)
        {
            insertionSort(&array[i], 5);
            i += 5;
        }

        if ((tmp = nInt % 5) != 0)
        {
            nGroups++;
            insertionSort(&array[i], tmp); //here i points to the last non-complete group of "tmp" integers
        }
        //array for storing medians of groups
        int* arrayOfMedians = (int*) malloc(nGroups * sizeof (int));

        int j = 0; //iterator for arrayOfMedians
        i = 2; //points to first median in "array"
        while (j < (nGroups - 1))
        {
            arrayOfMedians[j++] = array[i];
            i += 5;
        }

        if (tmp == 0) //if last group is of size 5
            arrayOfMedians[j] = array[i];
        else //if size of last group is < 5
        { //i is pointing to 3rd integer of last group
            i = i - 3 + (tmp + 1) / 2; //now i is pointing to median of last group
            arrayOfMedians[j] = array[i];
        }
        //finding mom = median of median
        int mom = findKthPos(arrayOfMedians, nGroups, (nGroups + 1) / 2);

        free(arrayOfMedians);

        int smaller = 0, equal = 0, larger = 0; //with respect to mom
        //computing value for larger, smaller, equal
        i = 0;
        while (i < nInt)
        {
            if (array[i] < mom)
                smaller++;
            else if (array[i] > mom)
                larger++;
            else
                equal++;
            i++;
        }
        //if mom's position is kth then returning mom
        if (k > smaller && k <= (smaller + equal))
            return mom;

        partition(array, mom, nInt, smaller, equal);

        if (k <= smaller)
            return (findKthPos(array, smaller, k));
        else
            return (findKthPos(&array[smaller + equal], larger, k - smaller - equal));
    }
}

void insertionSort(int* array, int nInt)
{
    int key;
    int i = 1;
    int j;
    while (i < nInt)
    {
        key = array[i];
        j = i - 1;
        while (j >= 0 && array[j] > key)
        {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = key;
        i++;
    }
}
//partition given array : First Smaller than mom / Equal to mom / Larger than mom

void partition(int* array, int mom, int nInt, int smaller, int equal)
{
    int s = 0; //iterator for smaller part
    int e = smaller; //iterator for equal part
    int l = equal + smaller; //iterator for larger part
    int tmp;
    //if in the smaller partition there is a integer >= mom it will be exchanged
    //with integer smaller than mom from equal or larger portion 
    while (s < smaller)
    {
        if (array[s] >= mom)
        { //searches for integer smaller than mom
            while (e < nInt)
            { //if found exhange it
                if (array[e] < mom)
                {
                    tmp = array[s];
                    array[s] = array[e];
                    array[e] = tmp;
                    e++;
                    break;
                }
                e++;
            }
        }
        s++;
    }
    //reseting pointer to larger and equal section
    e = smaller;
    l = smaller + equal;
    //if in the equal partition there is a integer > mom it will be exchanged
    //with integer equal to the mom from larger portion
    while (e < (smaller + equal))
    { //if integer larger than mom is found
        if (array[e] > mom)
        { //searching in the larger portion
            while (l < nInt)
            {
                if (array[l] == mom) //if mom is found it will exchange
                {
                    tmp = array[e];
                    array[e] = array[l];
                    array[l] = tmp;
                    l++;
                    break;
                }
                l++;
            }
        }
        e++;
    }
}
