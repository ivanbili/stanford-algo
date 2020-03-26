#include<iostream>
#include<fstream>
#include<string>
#include<vector>
using namespace std;

uint64_t merge_and_count_split_inv(vector<int>& numbers, int start_ind_left, int start_ind_right, int end_ind_right)
   {
   int end_ind_left = start_ind_right - 1;
   vector<int>output(end_ind_right - start_ind_left + 1);
   int left_ind = start_ind_left;
   int right_ind = start_ind_right;
   uint64_t inversions = 0;
   for (int i = 0; i < output.size(); i++)
      {
      if (left_ind > end_ind_left) // no more numbers in the left segment
         {
         output[i] = numbers[right_ind++];
         }
      else if (right_ind > end_ind_right) // no more numbers in the right segment
         {
         output[i] = numbers[left_ind++];
         }
      else if (numbers[left_ind] <= numbers[right_ind]) // the left number is smaller than (or equal to) the rigth
         {
         output[i] = numbers[left_ind++];
         }
      else // the right number is smaller than the right
         {
         inversions += (uint64_t) end_ind_left - (uint64_t) left_ind + 1;
         output[i] = numbers[right_ind++];
         }
      }
   for (int i = 0; i < output.size(); i++) // copy final numbers into the vector
      {
      numbers[start_ind_left + i] = output[i];
      }
   return inversions;
   }

uint64_t sort_and_count_inversions(vector<int>& numbers, int start_ind, int end_ind)
   {
   if (start_ind == end_ind)
      return 0;
   int mid_ind = (start_ind + end_ind) / 2;
   uint64_t left_inversions = sort_and_count_inversions(numbers, start_ind, mid_ind);
   uint64_t right_inversions = sort_and_count_inversions(numbers, mid_ind + 1, end_ind);
   uint64_t split_inversions = merge_and_count_split_inv(numbers, start_ind, mid_ind + 1, end_ind);
   return left_inversions + right_inversions + split_inversions;
   }

int main()
   {
   ifstream InFile;
   InFile.open("IntegerArray.txt");
   vector<int>numbers;
   int number;
   while (InFile >> number)
       numbers.push_back(number);
   InFile.close();
   uint64_t inversions = sort_and_count_inversions(numbers, 0, numbers.size() - 1);
   cout << "There were " << inversions << " inversions!\n";
   return 0;
   }
