#include <iostream>

using namespace std;
void display(int arr[],int n)
{
    for (int i = 0; i <= n; i++)
    {
        cout<<arr[i];
    }
    cout<<"\n";
}
int insertion (int arr[],int size , int index, int element, int capacity)
{
    if(size >= capacity)
    {
        return -1;
    }
    for (int i = size-1; i >= index; i--)
    {
        arr[i+1] = arr[i];
    }
    arr[index] = element;
    return -1;
}
int deletion(int arr[], int size , int index)
{
    for (int i = index; i < size ; i++)
    {
        arr[i] = arr[i+1];
    }
}

int main()
{
    int size = 3;
    int arr[100] = {1,3,10,6};
    int index = 2;
    int element = 34;
    display(arr,size);
    insertion(arr,size,index,element,100);
    size = size + 1;
    display(arr,size);

    deletion(arr,size,index);
    size = size -1;
    display (arr,size);

    return 0;
}
