#include <iostream>
#include <omp.h>
#include <bits/stdc++.h>
using namespace std;

void printArray(const vector<int> &arr)
{
  for (int x : arr)
    cout << x << " ";
  cout << endl;
}

void minimum(const vector<int> &array)
{
  int n = array.size();

  int min_val = INT_MAX;
  double start = omp_get_wtime();
  for (int i = 0; i < n; i++)
    min_val = min(min_val, array[i]);
  double end = omp_get_wtime();

  cout << "\nMinimum (Sequential): " << min_val;
  cout << "\nTime: " << end - start << endl;

  int p_min = INT_MAX;
  start = omp_get_wtime();

#pragma omp parallel for reduction(min : p_min)
  for (int i = 0; i < n; i++)
    p_min = min(p_min, array[i]);

  end = omp_get_wtime();

  cout << "Minimum (Parallel): " << p_min;
  cout << "\nTime: " << end - start << endl;
}

void maximum(const vector<int> &array)
{
  int n = array.size();

  int max_val = INT_MIN;
  double start = omp_get_wtime();
  for (int i = 0; i < n; i++)
    max_val = max(max_val, array[i]);
  double end = omp_get_wtime();

  cout << "\nMaximum (Sequential): " << max_val;
  cout << "\nTime: " << end - start << endl;

  int p_max = INT_MIN;
  start = omp_get_wtime();

#pragma omp parallel for reduction(max : p_max)
  for (int i = 0; i < n; i++)
    p_max = max(p_max, array[i]);

  end = omp_get_wtime();

  cout << "Maximum (Parallel): " << p_max;
  cout << "\nTime: " << end - start << endl;
}

void sum_func(const vector<int> &array)
{
  int n = array.size();

  int sum = 0;
  double start = omp_get_wtime();
  for (int i = 0; i < n; i++)
    sum += array[i];
  double end = omp_get_wtime();

  cout << "\nSum (Sequential): " << sum;
  cout << "\nTime: " << end - start << endl;

  int p_sum = 0;
  start = omp_get_wtime();

#pragma omp parallel for reduction(+ : p_sum)
  for (int i = 0; i < n; i++)
    p_sum += array[i];

  end = omp_get_wtime();

  cout << "Sum (Parallel): " << p_sum;
  cout << "\nTime: " << end - start << endl;
}

void average(const vector<int> &array)
{
  int n = array.size();

  double sum = 0;
  double start = omp_get_wtime();
  for (int i = 0; i < n; i++)
    sum += array[i];
  double end = omp_get_wtime();

  cout << "\nAverage (Sequential): " << sum / n;
  cout << "\nTime: " << end - start << endl;

  double p_sum = 0;
  start = omp_get_wtime();

#pragma omp parallel for reduction(+ : p_sum)
  for (int i = 0; i < n; i++)
    p_sum += array[i];

  end = omp_get_wtime();

  cout << "Average (Parallel): " << p_sum / n;
  cout << "\nTime: " << end - start << endl;
}

int main()
{
  int N;
  cout << "Enter the number of elements: ";
  cin >> N;
  vector<int> arr(N);
  if (N < 50)
  {
    cout << "Enter the elements: " << endl;
    for (int i = 0; i < N; i++)
    {
      cin >> arr[i];
    }
  }
  else
  {
    for (int i = 0; i < N; i++)
    {
      arr[i] = rand() % 1000000;
    }
  }
  cout << "Max threads : " << omp_get_max_threads() << endl;
  if (N < 50)
    printArray(arr);
  minimum(arr);
  maximum(arr);
  sum_func(arr);
  average(arr);
  return 0;
}
