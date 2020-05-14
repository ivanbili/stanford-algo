#include <iostream>
#include <fstream>
#include <unordered_set>
#include <time.h>
using namespace std;

int main()
{
    unordered_set<long long int>input_set;
    vector<long long int>input_vector;
    ifstream inFile;
    long long int value;
    inFile.open("2sum.txt");
    while (inFile >> value)
    {
        input_set.insert(value);
        input_vector.push_back(value);
    }
    
    inFile.close();
    const int total_size = input_vector.size();
    clock_t start = clock();
    int counter = 0;

    
    for (long long int  sum = -10000; sum <= 10000; sum++)
    {
        for (int i = 0; i < total_size; i++)
        {
            // sum = input_vector[i] + X
            // X = sum - input_vector[i]
            unordered_set<long long int>::const_iterator it = input_set.find(sum - input_vector[i]);
            if (it != input_set.end() && input_vector[i] != sum - input_vector[i])
            {
                counter++;
                break;
            }
        }
    }

    /*
    unordered_set<int>sum_set;
    for (int i = 0; i < total_size; i++)
    {

        for (long long int sum = -10000; sum <= 10000; sum++)
        {
            
            unordered_set<long long int>::const_iterator it = input_set.find(sum - input_vector[i]);
            if (it != input_set.end() && input_vector[i] != sum - input_vector[i])
            {
                sum_set.insert((int)sum);
                continue;
            }
        }
    }
    counter = sum_set.size();
    */
    clock_t end = clock();
    cout << "Algorithm took " << double(end - start) / CLOCKS_PER_SEC << " seconds.\n";
    std::cout << "counter: " << counter << "\n"; // 427, time: 13 minutes
    std::cout << "Done!\n";
}
