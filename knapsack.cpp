#include<iostream>
#include<fstream>
#include<vector>
#include <unordered_map>
#include <time.h>

using namespace std;

struct Item
{
    int value;
    int weight;
};

int knapsackRecursive(vector<unordered_map<int, int>> & lookup, vector<Item>& items, int knapsack_size, int max_item)
{
    if (max_item == 0)
        return 0;
    auto got = lookup[max_item].find(knapsack_size);
    if (got != lookup[max_item].end())
        return got->second;
    int result;
    int case1 = knapsackRecursive(lookup, items, knapsack_size, max_item - 1);
    int reduced_weight = knapsack_size - items[max_item - 1].weight;
    if (reduced_weight < 0)
    {
        result = case1;
    }
    else
    {
        int case2 = knapsackRecursive(lookup, items, reduced_weight, max_item - 1) + items[max_item - 1].value;
        result = case1 > case2 ? case1 : case2;
    }
    lookup[max_item][knapsack_size] = result;
    return result;
}

int knapsackLarge(vector<Item>& items, int knapsack_size)
{
    vector<unordered_map<int, int>> lookup(knapsack_size+1);
    return knapsackRecursive(lookup, items, knapsack_size, items.size());
}

int index(int sizeY, int x, int y)
{
    return x * sizeY + y;
}

int knapsackSimple(vector<Item>& items, int knapsack_size)
{
    int num_items = items.size();
    int width = (knapsack_size + 1);
    int * lookup = new int[(num_items + 1) * width];
    for (int x = 0; x <= knapsack_size; x++)
        lookup[index(width, 0, x)] = 0;

    for (int i = 1; i <= num_items; i++)
    {
        for (int x = 0; x <= knapsack_size; x++)
        {
            int case1 = lookup[index(width, i - 1, x)];
            int reduced_weight = x - items[i - 1].weight;
            if (reduced_weight < 0)
            {
                lookup[index(width, i, x)] = case1;
            }
            else
            {   
                int case2 = lookup[index(width, i - 1, reduced_weight)] + items[i-1].value;
                lookup[index(width, i, x)] = case1 > case2 ? case1 : case2;
            }
        }
    }
    int solution = lookup[index(width, num_items, knapsack_size)]; 
    delete[] lookup;
    return solution;
}

int main()
{
    vector<Item> items;
    ifstream InFile;
    //InFile.open("knapsack1.txt"); // 2493893, 0.03s, 0.15s
    InFile.open("knapsack_big.txt"); // 4243395, n/a, 2.29s
    int knapsack_size;
    InFile >> knapsack_size;
    int number_of_items;
    InFile >> number_of_items;
    for (int i = 0; i < number_of_items; i++)
    {
        Item curr_item;
        InFile >> curr_item.value;
        InFile >> curr_item.weight;
        items.push_back(curr_item);
    }
    InFile.close();
    clock_t start = clock();
    //std::cout << "Solution: " << knapsackSimple(items, knapsack_size) << "\n";
    std::cout << "Solution: " << knapsackLarge(items, knapsack_size) << "\n";
    clock_t end = clock();
    cout << "Time: " << double(end - start) / CLOCKS_PER_SEC << " seconds.\n";
}
