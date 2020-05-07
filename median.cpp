#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <time.h>
using namespace std;

int main()
{
    vector<int>input;
    ifstream inFile;
    int value;
    inFile.open("Median.txt");
    while (inFile >> value)
        input.push_back(value);
    inFile.close();
    const int total_size = input.size();

    clock_t start = clock();
    priority_queue<int, vector<int>> max_heap;
    priority_queue<int, vector<int>, greater<int>> min_heap;
    unsigned int sum = input[0];
    if (input[1] >= input[0])
    {
        min_heap.push(input[1]);
        max_heap.push(input[0]);
        sum += input[0];
    }
    else
    {
        max_heap.push(input[1]);
        min_heap.push(input[0]);
        sum += input[1];
    }
    for (int i = 2; i < total_size; i++)
    {
        int curr_int = input[i];
        if (max_heap.size() == min_heap.size())
        {
            if (max_heap.top() >= curr_int) // curr_int goes into max
            {
                max_heap.push(curr_int); // max 1 bigger than min
                sum += max_heap.top();
            }
            else // curr_int goes into min
            {
                min_heap.push(curr_int); // min 1 bigger than max
                sum += min_heap.top();
            }
        }
        else if (max_heap.size() > min_heap.size()) // max 1 bigger than min
        {
            if (max_heap.top() >= curr_int) // curr_int goes into max
            {
                max_heap.push(curr_int); // max 2 bigger than min
                min_heap.push(max_heap.top());
                max_heap.pop(); // equal sizes
                sum += max_heap.top();
            }
            else // curr_int goes into min
            {
                min_heap.push(curr_int); // now equal sizes
                sum += max_heap.top();
            }
        }
        else // min is 1 bigger than max
        {
            if (max_heap.top() >= curr_int) // curr_int goes into max
            {
                max_heap.push(curr_int); // now equal sizes
                sum += max_heap.top();
            }
            else // curr_int goes into min
            {
                min_heap.push(curr_int); // min 2 bigger than max
                max_heap.push(min_heap.top());
                min_heap.pop(); // equal sizes
                sum += max_heap.top();
            }
        }
    }
    clock_t end = clock();
    cout << "Sum of medians: " << sum << "\n";
    cout << "Heap algorithm took " << double(end - start) / CLOCKS_PER_SEC << " seconds.\n";
    start = clock();
    sum = 0;
    for (int i = 0; i < total_size; i++)
    {
        sort(input.begin(), input.begin() + i + 1);
        sum += input[(i + 2) / 2 - 1];
    }
    end = clock();
    cout << "Sum of medians: " << sum << "\n";
    cout << "Heap algorithm took " << double(end - start) / CLOCKS_PER_SEC << " seconds.\n";
    cout << "Done!\n";
    // correct answer is 46831213
}
