#include<iostream>
#include<fstream>
#include <unordered_set>
#include <unordered_map>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
using namespace std;

unordered_set<int>unsatisfied_clauses;
// clause_to_reference and set_references are to be able to select at random, erase and add to unsatisfied_clauses in O(1) time
unordered_map<int,int>clause_to_reference;
vector<unordered_set<int>::const_iterator>set_references;

vector<vector<int>>variable_reach; // clauses that involve a given variable that we need to check when changing it
vector<pair<int,int>>clauses;
vector<bool>assignment;

void randomizeAssignment()
{
    for (int i = 0; i < assignment.size(); i++)
        if (rand() % 2)
            assignment[i] = true;
        else
            assignment[i] = false;
}

bool isClauseSatisfied(int clause_index)
{
    int var1 = clauses[clause_index].first - assignment.size();
    int var2 = clauses[clause_index].second - assignment.size();
    bool cond1 = false;
    bool cond2 = false;
    if (var1 > 0)
    {
        if (assignment[var1 - 1])
            cond1 = true;
    }
    else
    {
        if (!assignment[-1 * var1 - 1])
            cond1 = true;
    }
    if (var2 > 0)
    {
        if (assignment[var2 - 1])
            cond2 = true;
    }
    else
    {
        if (!assignment[-1 * var2 - 1])
            cond2 = true;
    }
    return cond1 | cond2;
}

void addClauseToSet(int clause_index)
{
    if (unsatisfied_clauses.find(clause_index) == unsatisfied_clauses.end())
    {
        set_references.push_back(unsatisfied_clauses.insert(clause_index).first);
        clause_to_reference[clause_index] = set_references.size()-1;
    }
}

void removeClauseFromSet(int clause_index)
{
    // O(1) time deletion from set_references, clause_to_reference and unsatisfied_clauses
    auto it = unsatisfied_clauses.find(clause_index);
    if (it != unsatisfied_clauses.end())
    {
        //delete the element represending clause_index from set_references by swapping to the end and deleting the end
        int index_to_swap = clause_to_reference[clause_index];
        swap(set_references[index_to_swap], set_references[set_references.size()-1]);
        set_references.pop_back();
        // update the clause_to_reference vector to reflect the element now at index_to_swap
        clause_to_reference[*set_references[index_to_swap]] = clause_to_reference[clause_index];
        // now safely delete the clause from both clause_to_reference and unsatisfied_clauses
        clause_to_reference.erase(clause_index);
        unsatisfied_clauses.erase(it);
    }
}

int getRandomClauseFromSet()
{
    return *set_references[rand() % set_references.size()];
}

void updateChanges(int changed_var)
{
    for (int i = 0; i < variable_reach[changed_var].size(); i++)
    {
        int clause_index = variable_reach[changed_var][i];
        if (isClauseSatisfied(clause_index))
            removeClauseFromSet(clause_index);
        else
            addClauseToSet(clause_index);
    }
}

int main()
{
    clock_t start = clock();
    ifstream InFile;
    InFile.open("2sat6.txt");
    int total_vars;
    InFile >> total_vars;
    variable_reach.resize(total_vars);
    assignment.resize(total_vars);
    for (int i = 0; i < total_vars; i++)
    {
        int curr_int1, curr_int2;
        InFile >> curr_int1;
        InFile >> curr_int2;
        variable_reach[abs(curr_int1) - 1].push_back(clauses.size());
        variable_reach[abs(curr_int2) - 1].push_back(clauses.size());
        curr_int1 += total_vars;
        curr_int2 += total_vars;
        clauses.push_back(make_pair(curr_int1, curr_int2));
    }
    InFile.close();
    for (int i = 0; i < total_vars; i++) // remove duplicates
    {
        sort(variable_reach[i].begin(), variable_reach[i].end());
        variable_reach[i].erase(unique(variable_reach[i].begin(), variable_reach[i].end()), variable_reach[i].end());
    }
    bool foundAssignment = false;
    for (int i = (int)ceil(log2(total_vars)); i > 0; i--)
    {
        cout << "Countdown (outer loop): "<< i << "\n";
        if (foundAssignment)
            break;
        randomizeAssignment();
        for (int i = 0; i < total_vars; i++)
            updateChanges(i);
        for (uint64_t j = 0; j < 2 * (uint64_t) total_vars * (uint64_t) total_vars; j++)
        {
            if (unsatisfied_clauses.size() == 0)
            {
                foundAssignment = true;
                break;
            }
            int unsatisfied_clause = getRandomClauseFromSet();
            int var_to_flip = abs( (rand() % 2 ? clauses[unsatisfied_clause].first : clauses[unsatisfied_clause].second) - total_vars)  - 1;
            assignment[var_to_flip] = !assignment[var_to_flip];
            updateChanges(var_to_flip);
        }
    }
    if (foundAssignment)
        cout << "Found satisfiable assignment!\n";
    else
        cout << "Did NOT find satisfiable assignment!\n";
    clock_t end = clock();
    cout << "Algorithm took " << double(end - start) / CLOCKS_PER_SEC << " seconds.\n";
    return 0; //101100
}
