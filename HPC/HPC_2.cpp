#include <iostream>
#include <omp.h>
#include <bits/stdc++.h>
using namespace std;

void printArray(vector<int> &arr)
{
  for (int x : arr)
    cout << x << " ";
  cout << endl;
}

void sequential_bubble_sort(int arr[], int size)
{
  vector<int> array(arr, arr + size);

  double start = omp_get_wtime();
  for (int i = 0; i < size - 1; i++)
  {
    for (int j = 0; j < size - i - 1; j++)
    {
      if (array[j] > array[j + 1])
      {
        swap(array[j], array[j + 1]);
      }
    }
  }
  double end = omp_get_wtime();

  cout << "\nSequential Bubble Sort:\n";
  if (size <= 50)
    printArray(array);

  cout << "Time: " << end - start << endl;
}

void parallel_bubble_sort(int arr[], int size)
{
  vector<int> array(arr, arr + size);

  double start = omp_get_wtime();
  for (int k = 0; k < size; k++)
  {
    if (k % 2 == 0)
    {
#pragma omp parallel for
      for (int i = 0; i < size - 1; i += 2)
      {
        if (array[i] > array[i + 1])
        {
          swap(array[i], array[i + 1]);
        }
      }
    }
    else
    {
#pragma omp parallel for
      for (int i = 1; i < size - 1; i += 2)
      {
        if (array[i] > array[i + 1])
        {
          swap(array[i], array[i + 1]);
        }
      }
    }
  }
  double end = omp_get_wtime();

  cout << "\nParallel Bubble Sort:\n";
  if (size <= 50)
    printArray(array);
  cout << "Time: " << end - start << endl;
}

void merge(int arr[], int low, int mid, int high)
{
  int n = high - low + 1;
  vector<int> temp(n);

  int i = low, j = mid + 1, k = 0;

  while (i <= mid && j <= high)
  {
    if (arr[i] <= arr[j])
      temp[k++] = arr[i++];
    else
      temp[k++] = arr[j++];
  }

  while (i <= mid)
    temp[k++] = arr[i++];
  while (j <= high)
    temp[k++] = arr[j++];

  for (int i = 0; i < n; i++)
    arr[low + i] = temp[i];
}

void merge_sort(int arr[], int low, int high)
{
  if (low < high)
  {
    int mid = (low + high) / 2;
    merge_sort(arr, low, mid);
    merge_sort(arr, mid + 1, high);
    merge(arr, low, mid, high);
  }
}

void perform_merge_sort(int arr[], int size)
{
  vector<int> array(arr, arr + size);

  double start = omp_get_wtime();
  merge_sort(array.data(), 0, size - 1);
  double end = omp_get_wtime();

  cout << "\nSequential Merge Sort:\n";
  if (size <= 50)
    printArray(array);
  cout << "Time: " << end - start << endl;
}

void parallel_merge_sort(int arr[], int low, int high)
{
  if (low < high)
  {
    int mid = (low + high) / 2;

#pragma omp task
    parallel_merge_sort(arr, low, mid);

#pragma omp task
    parallel_merge_sort(arr, mid + 1, high);

#pragma omp taskwait
    merge(arr, low, mid, high);
  }
}

void perform_parallel_merge_sort(int arr[], int size)
{
  vector<int> array(arr, arr + size);

  double start = omp_get_wtime();

#pragma omp parallel
  {
#pragma omp single
    parallel_merge_sort(array.data(), 0, size - 1);
  }

  double end = omp_get_wtime();

  cout << "\nParallel Merge Sort:\n";
  if (size <= 50)
    printArray(array);
  cout << "Time: " << end - start << endl;
}

int main()
{
  int SIZE;
  cout << "Enter size of array: ";
  cin >> SIZE;

  vector<int> arr(SIZE);

  if (SIZE <= 50)
  {
    cout << "Enter elements:\n";
    for (int i = 0; i < SIZE; i++)
      cin >> arr[i];
  }
  else
  {

    cout << "Generating random array...\n";
    for (int i = 0; i < SIZE; i++)
      arr[i] = rand() % 10000;
  }

  if (SIZE <= 50)
  {
    cout << "\nOriginal Array:\n";
    printArray(arr);
  }

  // -------- BUBBLE SORT --------
  cout << "\n===== BUBBLE SORT =====\n";
  sequential_bubble_sort(arr.data(), SIZE);
  parallel_bubble_sort(arr.data(), SIZE);

  // -------- MERGE SORT --------
  cout << "\n===== MERGE SORT =====\n";
  perform_merge_sort(arr.data(), SIZE);
  perform_parallel_merge_sort(arr.data(), SIZE);

  return 0;
}
