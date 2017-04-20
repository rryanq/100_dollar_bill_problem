/*ONE HUNDRED DOLLAR BILL PROBLEM VERSION 2
Parameters to pass into main: 
  - Denominations of bills to try
  - Dollar amount to sum to
  - compile using g++ -std=c++11 dollar_bill_v2.cpp -o dollar_bill_v2.exe
  - To run: ./dollar_bill_v2.exe [bill denominations] [amount to sum to]
  - Ex: ./dollar_bill_v2.exe 100 50 20 10 5 2 1 100
*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
using namespace std;

//custom comparator for dollars_in set
struct Greater {
  template<class T>
  bool operator()(T const &a, T const &b) const { 
    return a > b; 
  }
};

//REQUIRES: dollars_in is not empty
//MODIFIES: dollars
//EFFECTS: Counts the number of ways that dollar_amount can be
//exceeded without using a bill that must be immediately returned
//to customer in change
//returns a pair of ints that contains <number of all combinations tried,
//number of solutions to the problem>
pair<int, int> count_ways(vector<int> dollars, int min_bill, 
                          int max_bill, int dollar_amount, 
                          const set<int, Greater> &dollars_in);

//MODIFIES: nothing
//EFFECTS: checks to see if the proposed set of dollars is 
//a solution to the one hundred dollar bill problem. 
//returns true if a solution was found and false otherwise
bool check_for_solution(int change, const vector<int> &dollars, 
                        const vector<int> &change_bills);

//REQUIRES: dollars_in is not empty
//MODIFIES: change_bills
//EFFECTS: uses change to find the simplest combination of
//bills to return to the customer as change
void update_change_bills(int change, vector<int> &change_bills, 
                         const set<int, Greater> &dollars_in);

//REQUIRES: dollars_in is not empty
//MODIFIES: dollars, denomination
//EFFECTS: sets up dollars for next time and updates 
//the denomination to add
void update_dollars(vector<int> &dollars, int &denomination, 
                    const set<int, Greater> &dollars_in);

//EFFECTS: prints the entire sequence of dollars that make up each solution,
//followed by the dollar amount summed by the bills contained in the sequence
void print_solutions(vector<int> dollars, int max_bill, int dollar_amount,
		     const set<int, Greater> &dollars_in);


//main takes ints that represent dollar denominations as its first arguments,
//and the final argument is the dollar amount that should be summed to
int main(int argc, char * argv[]) {
  set<int, Greater> dollars_in;
  for(int i = 1; i < argc - 1; ++i) {
    dollars_in.insert(stoi(argv[i]));
  }

  int max_bill = *(dollars_in.begin());
  int dollar_amount = stoi(argv[argc - 1]);
 
  vector<int> dollars;
  set<int, Greater>::iterator it = dollars_in.end();
  --it;
  pair<int, int> combos_solutions = 
    count_ways(dollars, *it, *(dollars_in.begin()), 
    stoi(argv[argc - 1]), dollars_in);
  print_solutions(dollars, max_bill, dollar_amount, dollars_in);
  cout << endl << "TOTAL NUMBER OF WAYS TO SOLVE PROBLEM " <<
  "WITH DOLLAR AMOUNT = $" << dollar_amount << ": " 
  << combos_solutions.second << endl;
  cout << "TOTAL COMBINATIONS TRIED: " << 
    combos_solutions.first << endl;
  return 0;
}


pair<int, int> count_ways(vector<int> dollars, int min_bill, 
                          int max_bill, int dollar_amount, 
			  const set<int, Greater> &dollars_in) {
  int num_combinations_tried = 0;
  int total = 0;
  vector<int> change_bills;
  dollars.push_back(max_bill);
  int denomination_to_add = max_bill; //first, add $50 bills to the vector
  int change = 0;
  int solutions = 0;

  while(dollars[0] != min_bill) {
    total = 0; //reset total
    change = 0; //reset change, may not be necessary
    for(unsigned i = 0; i < dollars.size(); ++i) {
      total += dollars[i];
    } //sets total to the sum of the numbers that are in dollars
    while(total < dollar_amount) {
      dollars.push_back(denomination_to_add);
      total += denomination_to_add; //adds new bills to vector in search
                                    //of another solution
    }

    change = total - dollar_amount;
    update_change_bills(change, change_bills, dollars_in); //update change_bills

    if(check_for_solution(change, dollars, change_bills)) {
      ++solutions; //if we find a new solution, increment solutions
    }
    update_dollars(dollars, denomination_to_add, dollars_in); 
    //pop the unwanted numbers off of the end
    change_bills.clear();
    //clear change_bills
    ++num_combinations_tried;
  }
  pair<int, int> p(num_combinations_tried, solutions);
  return p;
}

bool check_for_solution(int change, const vector<int> &dollars, 
                        const vector<int> &change_bills) {
  if(change == 0) {
    return false;
  }
  for(unsigned j = 0; j < change_bills.size(); ++j) {
      for(unsigned k = 0; k < dollars.size(); ++k) {
	if(change_bills[j] == dollars[k]) {
	  return false;
	}
      }
    }
  return true;
}

//WARNING: The way change_bills is set up right now, change can
//only be made out of the denominations that we start with
//This is problematic: we want change bills to include both
//real-life denominations and whatever the user inputs
//OR should they only be real life denominations? If so,
//there has to be a limit on them. $1000 is probably a good limit
void update_change_bills(int change, vector<int> &change_bills, 
                         const set<int, Greater> &dollars_in) {
  while(change != 0) { //update the change_bills 

    for(auto &it : dollars_in) {
      while(change / it != 0) {
	change_bills.push_back(it);
	change -= it;
      }
    }
  }
}

void update_dollars(vector<int> &dollars, int &denomination, 
                    const set<int, Greater> &dollars_in) {
  
  set<int, Greater>::iterator it = dollars_in.find(denomination);
  set<int, Greater>::iterator last = dollars_in.end();
  --last; //now last is an iterator to the last element in the set,
          //which should be the smallest denomination
  if(it == last) {
    for(unsigned i = 0; i < dollars.size(); ++i) {
      if(dollars[i] == *last) {
	dollars.erase(dollars.begin() + i, dollars.end());
	//erase all of the bills with the smallest denomination
      }
    }
    it = dollars_in.find(dollars.back());
    dollars.pop_back();
    denomination = *(++it);
  }
  else {
    dollars.pop_back();
    denomination = *(++it);
  }
}

void print_solutions(vector<int> dollars, int max_bill, int dollar_amount, 
		     const set<int, Greater> &dollars_in) {
  int sum = 0;
  int total = 0;
  vector<int> change_bills;
  dollars.push_back(max_bill);
  int denomination_to_add = max_bill;
  int change = 0;
  int test_count = 0;
  cout << endl;
  while(dollars[0] != 1) {
    total = 0; //reset total
    change = 0; //reset change, may not be necessary
    for(unsigned i = 0; i < dollars.size(); ++i) {
      total += dollars[i];
    } //sets total to the sum of the numbers that are in dollars
    while(total < dollar_amount) {
      dollars.push_back(denomination_to_add);
      total += denomination_to_add; //adds new bills to vector in search
                                    //of another solution
    }

    change = total - dollar_amount;
    update_change_bills(change, change_bills, dollars_in); //update change_bills

    if(check_for_solution(change, dollars, change_bills)) {
      
      cout << "Solution: ";
      for(unsigned i = 0; i < dollars.size(); ++i) {
        cout << dollars[i] << " ";
	sum += dollars[i];
      }
      cout << endl << "Sum of bills: " << sum << endl;
      sum = 0; //reset sum 
      ++test_count;
      cout << "Change received: ";
      for(unsigned i = 0; i < change_bills.size(); ++i) {
        cout << change_bills[i] << " ";
      }
      cout << endl << endl;
    }
    update_dollars(dollars, denomination_to_add, dollars_in); 
    //pop the unwanted numbers off of the end
    change_bills.clear();
    //clear change_bills
  }
}
