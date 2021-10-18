#include <iostream>

void merge(int arr[], int left, int mid, int right) {

    int const subArrOne = mid - left + 1,
              subArrTwo = right - mid;

    int *leftArr = new int[subArrOne],
        *rightArr = new int[subArrTwo];


    for (int i = 0; i < subArrOne; ++i) {
        leftArr[i] = arr[left + i];
    }

    for (int i = 0; i < subArrTwo; ++i) {
        rightArr[i] = arr[mid + 1 + i];
    }
    
    int indexRightArr = 0, indexLeftArr = 0;
    int indexOfMergedArray = left;

    while (indexLeftArr < subArrOne && indexRightArr < subArrTwo) {

        if (leftArr[indexLeftArr] <= rightArr[indexRightArr]) {

            arr[indexOfMergedArray] = leftArr[indexLeftArr++];
        }

        else {
            arr[indexOfMergedArray] = rightArr[indexRightArr++];
        }

        indexOfMergedArray++;
    }

    while(indexLeftArr < subArrOne) {
        arr[indexOfMergedArray++] = leftArr[indexLeftArr++];
    }

    while (indexRightArr < subArrTwo) {
        arr[indexOfMergedArray++] = rightArr[indexRightArr++];
    }

    delete[] leftArr;
    delete[] rightArr;
    
}

void mergeSort(int arr[], int left, int right) {

    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

int main() {

    int nums[] = {3, 19, 8, -2, 4, 0, 7};
    int len = sizeof(nums) / sizeof(nums[0]);

    mergeSort(nums, 0, len - 1);

    for (int i = 0; i < len; ++i) {
        std::cout << nums[i] << ' ';
    }


    return 0;
}