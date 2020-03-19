#include <iostream>
#include <vector>
#include <assert.h>
using namespace std;

// IMPORTANT: int vectors represent digits of large numbers, where the least
// significant digits are stored in smaller vector positions. i.e. for number
// 5678, we would store 8 at index 0, 7 at index 1, etc.

// for debug
void printVector(vector<int> num)
   {
   string numStr;
   for (int i = num.size() - 1; i >= 0; i--)
      numStr += '0' + num[i];
   cout << "The vector is: " << numStr << endl;
   }

// adds zeros to the shorter vector to make sizes equal
// and also returns the new size
int makeSizesEqual(vector<int> &v1, vector<int> &v2)
   {
   while(v1.size()>v2.size())
      v2.push_back(0);
   while(v2.size()>v1.size())
      v1.push_back(0);
   return v1.size();
   }

// 00123 -> 123
// 00000 -> 0
void removeLeadingZeros(vector<int> &v)
   {
   int extraZeros = 0;
   for (int i = v.size() - 1; i > 0; i--)
      {
      if (v[i] == 0)
         extraZeros++;
      else
         break;
      }
   v.resize(v.size()-extraZeros);
   }

vector<int> simpleAdd(vector<int> num1, vector<int> num2)
   {
   int digits = makeSizesEqual(num1, num2);
   vector<int> result;
   int carry = 0;
   for (int i = 0; i<digits; i++)
      {
      int digitSum = num1[i] + num2[i] + carry;
      carry = digitSum/10;
      result.push_back(digitSum%10);
      }
   if (carry)
      result.push_back(carry);
   return result;
   }

// result = num1 - num2;
// assume num1 is bigger!
vector<int> simpleSubtract(vector<int> num1, vector<int> num2)
   {
   vector<int> result;
   int digits = makeSizesEqual(num1, num2);
   int borrow = 0;
   for (int i = 0; i<digits; i++)
      {
      int digitDiff = num1[i] - num2[i] - borrow;
      borrow = 0;
      if (digitDiff<0)  // can this be done without branches? maybe using 10s complement
         {
         digitDiff+=10;
         borrow = 1;
         }
      result.push_back(digitDiff);
      }
   assert(("simpleSubtract only works if num1 is > or = num2 ", borrow==0));
   return result;
   }

vector<int> karatsubaMult(vector<int> num1, vector<int> num2)
   {
   int digits = makeSizesEqual(num1, num2);
   assert(("karatsubaMult needs to have at leaast one digit ", digits>0));
   if (digits == 1)
      {
      vector<int> result;
      int product = num1[0] * num2[0];
      result.push_back(product%10);
      if (product/10)
         result.push_back(product/10); //2nd digit if needed
      return result;
      }
   if (digits%2 != 0) // condition taken if the number of digits is odd
      {
      digits++;
      num1.push_back(0);
      num2.push_back(0);
      }
   // vectors are of the same size and sizes are divisible by 2
   vector<int> sectionB = vector<int>(num1.begin(), num1.begin() + digits/2);
   vector<int> sectionA = vector<int>(num1.begin() + digits/2, num1.begin () + digits);
   vector<int> sectionD = vector<int>(num2.begin(), num2.begin () + digits/2);
   vector<int> sectionC = vector<int>(num2.begin() + digits/2, num2.begin () + digits);
   vector<int> expr1 = karatsubaMult(sectionA, sectionC);
   vector<int> expr2 = karatsubaMult(sectionB, sectionD);
   vector<int> expr3 = karatsubaMult(simpleAdd(sectionA, sectionB), simpleAdd(sectionC, sectionD));
   vector<int> expr4 = simpleSubtract(expr3, simpleAdd(expr2, expr1));
   expr1.insert(expr1.begin(), digits, 0);
   expr4.insert(expr4.begin(), digits/2, 0);
   return simpleAdd(expr2, simpleAdd(expr1, expr4));
   }

int main()
   {
   string numStr1, numStr2, productStr;
   vector <int> numVec1, numVec2, prodVec;
   cout << "Please enter the first number:  ";
   getline (cin, numStr1);
   for (int i = numStr1.size() - 1; i >= 0; i--)
      numVec1.push_back(numStr1[i] - '0');
   cout << "Please enter the second number: ";
   getline (cin, numStr2);
   for (int i = numStr2.size() - 1; i >= 0; i--)
      numVec2.push_back(numStr2[i] - '0');

   prodVec = karatsubaMult(numVec1, numVec2);

   removeLeadingZeros(prodVec);
   for (int i = prodVec.size() - 1; i >= 0; i--)
      productStr += '0' + prodVec[i];

   cout << "The product is: " << productStr << endl;
   return 0;
   }
