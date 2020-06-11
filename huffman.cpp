#include<iostream>
#include<fstream>
#include <queue>

using namespace std;

struct TreeNode
{
    TreeNode* left;
    TreeNode* right;
    TreeNode(TreeNode* l, TreeNode* r)
        : left(l), right(r) {}
};

struct MinHeapElement
{
    int weight;
    TreeNode * letter;
    MinHeapElement(int weight, TreeNode* letter)
        : weight(weight), letter(letter) {}
};

bool operator<(const MinHeapElement& e1, const MinHeapElement& e2)
{
    return e1.weight > e2.weight;
}

bool ComparePairFirst(pair<int, int> a, pair<int, int> b)
{
    return true;
}

int findMaxDepth(TreeNode * root)
{
    if (root == NULL)
        return 0;
    int lDepth = findMaxDepth(root->left);
    int rDepth = findMaxDepth(root->right);
    return 1 + (lDepth > rDepth ? lDepth : rDepth);
}

int findMinDepth(TreeNode* root)
{
    if (root == NULL)
        return 0;
    int lDepth = findMinDepth(root->left);
    int rDepth = findMinDepth(root->right);
    return 1 + (lDepth < rDepth ? lDepth : rDepth);
}

TreeNode * buildHuffmanTree(priority_queue<MinHeapElement> & min_heap)
{
    int new_freq = min_heap.top().weight;
    TreeNode* leftNode = min_heap.top().letter;
    min_heap.pop();
    new_freq += min_heap.top().weight;
    TreeNode* rightNode = min_heap.top().letter;
    min_heap.pop();
    TreeNode* newNode = new TreeNode(leftNode, rightNode);
    if (min_heap.empty())
        return newNode;
    min_heap.push(MinHeapElement(new_freq, newNode));
    return buildHuffmanTree(min_heap);
}

int main()
{
    priority_queue<MinHeapElement> min_heap;
    ifstream InFile;
    InFile.open("huffman.txt");
    vector<int>numbers;
    int curr_int;
    InFile >> curr_int;
    const int num_letters = curr_int;
    for (int i = 0; i < num_letters; i++)
    {
        InFile >> curr_int;
        min_heap.push(MinHeapElement(curr_int, new TreeNode(NULL,NULL)));
    }
    InFile.close();
    TreeNode * rootNode = buildHuffmanTree(min_heap);
    int maxDepth = findMaxDepth(rootNode);
    int minDepth = findMinDepth(rootNode);
    std::cout << "Max length: " << maxDepth-1 << "\n"; // 19
    std::cout << "Min length: " << minDepth-1 << "\n"; // 9
    return 0;
}
