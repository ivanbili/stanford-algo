#include<iostream>
#include<fstream>
#include<vector>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
using namespace std;

int partition_and_count_comparisons(vector<int>& numbers, int left, int right)
{
    // assume the pivot is the element at index left
    int pivot = numbers[left];
    // numbers at indices i-1 and below are smaller than i
    int i = left + 1;
    // numbers at indices j-1 and below are partitioned
    for (int j = i; j <= right; j++)
    {
        if (numbers[j] < pivot)
        {
            swap(numbers[j], numbers[i]);
            i++;
        }
    }
    if (left != (i - 1))
       swap(numbers[left], numbers[i-1]);
    return i-1;
}

int get_median_index(vector<int>& v, int a, int b, int c)
{
    if (v[a] > v[b])
       {
        if (v[c] > v[a])
            return a;
        else if (v[c] > v[b])
            return c;
        else
            return b;
       }
    else // b bigger than a
       {
       if (v[c] > v[b])
           return b;
       else if (v[c] > v[a])
           return c;
       else
           return a;
       }
}

uint64_t sort_and_count_comparisons(vector<int>& numbers, int start_ind, int end_ind, int pivot_mode)
{
    if (end_ind <= start_ind)
        return 0;
    int pivot_ind = start_ind;
    switch (pivot_mode)
    {
    case 1:
        pivot_ind = start_ind;
        break;
    case 2:
        pivot_ind = end_ind;
        break;
    case 3:
        pivot_ind = get_median_index(numbers, start_ind, (start_ind + end_ind) / 2, end_ind);
        break;
    case 4:
        pivot_ind = rand() % (end_ind - start_ind + 1) + start_ind;
        break;
    default:
        assert(("unexpected pivot mode! ", false));
        break;
    }
    if (pivot_ind != start_ind)
       swap(numbers[start_ind], numbers[pivot_ind]);
    int new_pivot_ind = partition_and_count_comparisons(numbers, start_ind, end_ind);
    uint64_t comparisons = (uint64_t) end_ind - (uint64_t) start_ind;
    comparisons += sort_and_count_comparisons(numbers, start_ind, new_pivot_ind -1, pivot_mode);
    comparisons += sort_and_count_comparisons(numbers, new_pivot_ind +1, end_ind, pivot_mode);
    return comparisons;
}

int main()
{
    std::cout << "How do you want to get the pivot? Enter number 1-4 \n";
    std::cout << "1 - first element \n";
    std::cout << "2 - last element \n";
    std::cout << "3 - median of three elements \n";
    std::cout << "4 - random (take average of 1000 executions) \n";
    int pivot_mode;
    std::cin >> pivot_mode;
    ifstream InFile;
    InFile.open("QuickSort.txt");
    vector<int>numbers;
    int number;
    while (InFile >> number)
        numbers.push_back(number);
    InFile.close();

    uint64_t inversions;
    if (pivot_mode == 4)
       {
       srand(time(NULL));
       assert(("RAND_MAX too small! ", RAND_MAX > numbers.size()));
       vector<int> original(numbers);
       inversions = sort_and_count_comparisons(numbers, 0, numbers.size() - 1, pivot_mode);
       for (int i = 0; i <= 998; i++)
          {
          copy(original.begin(), original.end(), numbers.begin());
          inversions += sort_and_count_comparisons(numbers, 0, numbers.size() - 1, pivot_mode);
          inversions /= 2;
          }
       }
    else
    {
        inversions = sort_and_count_comparisons(numbers, 0, numbers.size() - 1, pivot_mode);
    }
    cout << "There were " << inversions << " comparisons!\n";
    return 0;
}
