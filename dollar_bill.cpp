/*
THE 100 DOLLAR BILL PROBLEM
*/
#include <iostream>
using namespace std;

int count_change(int amount, const int kinds[], int num_kinds);

int dollars[] = {5, 10, 20, 50, 100};
int dollars_size = 5;
int dollars_with_two[] = {2, 5, 10, 20, 50, 100};
int dollars_with_two_size = 6;
int num_ways = 1; //start num_ways at 1 to compensate for the extra case
                  //of one $50 bill and 3 $20 bills
int main() {
  cout << endl << "number of ways to solve the problem WITHOUT $2 bills: " 
  << num_ways << endl;
  cout << endl << "number of ways to solve the problem WITH $2 bills: " 
  << endl << endl;
  for(int i = 5; i < 100; i += 10) {
    num_ways += count_change(i, dollars, dollars_size);
    cout << "        number of ways to make $" << i << ": ";
    cout << count_change(i, dollars, dollars_size) << endl;
  }
  cout << endl << "TOTAL number of ways to solve the problem: " << num_ways << endl << endl;
  //cout << "Test: " << count_change(101, dollars_with_two, dollars_with_two_size) << endl;
}

int count_change(int amount, const int kinds[], int num_kinds) {
  if(amount == 0) {
    return 1;
  } else if(amount < 0 || num_kinds < 1) {
    return 0;
  } else {
    return
      count_change(amount - kinds[num_kinds - 1], kinds, num_kinds) + 
      count_change(amount, kinds, num_kinds - 1);
  } 
}
