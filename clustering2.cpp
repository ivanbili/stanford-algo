#include<iostream>
#include<fstream>
#include <unordered_set>
#include <unordered_map> 
using namespace std;

unordered_set<int>numbers;
unordered_map<int, int>leaders;

void flattenChain(int start, int leader)
{
    auto currIt = leaders.find(start);
    while (currIt != leaders.end())
    {
        if ((*currIt).second == leader)
            break;
        int nextNum = (*currIt).second;
        (*currIt).second = leader;
        currIt = leaders.find(nextNum);
    }
}

int getLeader(int start)
{
    auto currIt = leaders.find(start);
    bool flatten = false;
    while (currIt != leaders.end())
    {
        if ((*currIt).first == (*currIt).second)
            break;
        currIt = leaders.find((*currIt).second);
        flatten = true;
    }
    int leader = (*currIt).second;
    if (flatten)
        flattenChain(start, leader);
    return leader;
}

void unionLeaders(int dom, int sub)
{
    auto currIt = leaders.find(sub);
    if (currIt != leaders.end())
        (*currIt).second = dom;
}

int main()
{
    ifstream InFile;
    InFile.open("clustering_big.txt");
    int total_ints;
    InFile >> total_ints;
    int num_bits;
    InFile >> num_bits;
    for (int i = 0; i < total_ints; i++)
    {
        int curr_int = 0;
        for (int j = 0; j < num_bits; j++)
        {
            int bit;
            InFile >> bit;
            curr_int = (curr_int << 1) + bit;
        }
        numbers.insert(curr_int);
        leaders.insert(make_pair(curr_int, curr_int));        
    }
    InFile.close();
    for (auto i = numbers.begin(); i != numbers.end(); i++)
    {
        int curr_int = *i;
        int curr_int_leader = getLeader(curr_int);
        for (int mask1 = 1; mask1 < (1 << 24); mask1 = mask1 << 1)
        {
            bool bit1set = curr_int & mask1;
            int match1 = bit1set ? curr_int - mask1 : curr_int + mask1;
            if (numbers.find(match1) != numbers.end())
            {
                int match1_leader = getLeader(match1);
                if (match1_leader != curr_int_leader)
                {
                    unionLeaders(curr_int_leader, match1_leader);
                }
            }
            for (int mask2 = mask1 << 1; mask2 < (1 << 24); mask2 = mask2 << 1)
            {
                bool bit2set = curr_int & mask2;
                int match2 = bit2set ? match1 - mask2 : match1 + mask2;
                if (numbers.find(match2) != numbers.end())
                {
                    int match2_leader = getLeader(match2);
                    if (match2_leader != curr_int_leader)
                    {
                        unionLeaders(curr_int_leader, match2_leader);
                    }
                }
            }
        }
    }
    unordered_set<int>leader_count;
    for (auto i = numbers.begin(); i != numbers.end(); i++)
        leader_count.insert(getLeader(*i));
    std::cout << "Clusters: " << leader_count.size() << "\n";
    system("pause");
}
