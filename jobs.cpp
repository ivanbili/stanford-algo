#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <time.h>

using namespace std;

bool compareDifference(pair <int,int> p1, pair <int, int> p2)
{
    int diff1 = p1.first - p1.second;
    int diff2 = p2.first - p2.second;
    if (diff1 == diff2)
        return p1.first > p2.first;
    else
        return diff1 > diff2;
}
bool compareRatio(pair <int, int> p1, pair <int, int> p2)
{
    return (float)p1.first/(float)p1.second > (float)p2.first / (float)p2.second; 
}

int main()
{
    vector<pair<int, int>> input;
    string line;
    ifstream myfile("jobs.txt");
    clock_t start = clock();
    if (myfile.is_open())
    {
        int first, second;
        getline(myfile, line);
        while (getline(myfile, line))
        {
            int i = 0;
            int current_int = 0;
            while (i < line.size())
            {
                if (line[i] == ' ')
                {
                    first = current_int;
                    current_int = 0;
                    i++;
                }
                current_int = current_int * 10 + (line[i] - '0');
                i++;
            }
            second = current_int;
            input.push_back(pair<int, int>(first, second));
        }
        myfile.close();
    }
    else
    {
        cout << "Unable to open file";
    }
    clock_t end = clock();
    cout << "File reading took " << double(end - start) / CLOCKS_PER_SEC << " seconds.\n";
    start = clock();
    sort(input.begin(), input.end(), compareDifference);
    unsigned long long sum = 0;
    unsigned long long time = 0;
    for (int i = 0; i < input.size(); i++)
    {
        time += (unsigned long long)input[i].second;
        sum += (unsigned long long)input[i].first * time;
    }
    end = clock();
    cout << "Weighted sum (greedy differences): " << sum <<"\n";
    cout << "That took " << double(end - start) / CLOCKS_PER_SEC << " seconds.\n";
    start = clock();
    sort(input.begin(), input.end(), compareRatio);
    sum = 0;
    time = 0;
    for (int i = 0; i < input.size(); i++)
    {
        time += (unsigned long long)input[i].second;
        sum += (unsigned long long)input[i].first * time;
    }
    end = clock();
    cout << "Weighted sum (greedy ratio): " << sum << "\n";
    cout << "That took " << double(end - start) / CLOCKS_PER_SEC << " seconds.\n";
    cout << "Hello World!\n";
}
