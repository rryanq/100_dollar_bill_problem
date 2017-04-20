/*ONE HUNDRED DOLLAR BILL PROBLEM VERSION 2
Parameters to pass into main: 
  - Denominations of bills to try
  - Dollar amount to sum to

  - compile using g++ -std=c++11 dollar_bill_v2.cpp -o dollar_bill_v2.exe
*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
using namespace std;

struct Greater {
  template<class T>
  bool operator()(T const &a, T const &b) const { 
    return a > b; 
  }
};

//REQUIRES: you are using all possible combinations of
//1, 2, 5, 10, 20 and 50 dollar bills only. No 100 or anything else
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

//REQUIRES: you are using all possible combinations of
//1, 2, 5, 10, 20 and 50 dollar bills only. No 100 or anything else
//MODIFIES: change_bills
//EFFECTS: uses change to find the simplest combination of
//bills to return to the customer as change
void update_change_bills(int change, vector<int> &change_bills, 
                         const set<int, Greater> &dollars_in);

//REQUIRES: you are using all possible combinations of
//1, 2, 5, 10, 20 and 50 dollar bills only. No 100 or anything else
//MODIFIES: dollars, denomination
//EFFECTS: sets up dollars for next time and updates 
//the denomination to add
void update_dollars(vector<int> &dollars, int &denomination, 
                    const set<int, Greater> &dollars_in);

//EFFECTS: prints the entire sequence of dollars that make up each solution,
//followed by the dollar amount summed by the bills contained in the sequence
void print_solutions(vector<int> dollars, int max_bill, int dollar_amount,
		     const set<int, Greater> &dollars_in);

//in case I want to sort a vector from greatest to least


//main takes ints that represent dollar denominations as its first arguments,
//and the final argument is the dollar amount that should be summed to
int main(int argc, char * argv[]) {
  set<int, Greater> dollars_in;
  for(int i = 1; i < argc - 1; ++i) {
    dollars_in.insert(stoi(argv[i]));
  }
  int max_bill = *(dollars_in.begin());
  int dollar_amount = stoi(argv[argc - 1]);
  //sort(dollars_in.begin(), dollars_in.end(), Greater());

  /*cout << endl << "dollars_in: ";
  for(vector<int>::iterator it = dollars_in.begin(); 
      it != dollars_in.end(); ++it) {
    cout << *it << ' ';
    }
  cout << endl;*/
 
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
  /*
  if(denomination == 50) {
    dollars.pop_back();
    denomination = 20;
    return;
  }
  else if(denomination == 20) {
    dollars.pop_back();
    denomination = 10;
    return;
  }
  else if(denomination == 10) {
    dollars.pop_back();
    denomination = 5;
    return;
  }
  else if(denomination == 5) {
    dollars.pop_back();
    denomination = 2;
    return;
  }
  else if(denomination == 2) {
    dollars.pop_back();
    denomination = 1;
    return;
  }
  else { //denomination == 1
    for(unsigned i = 0; i < dollars.size(); ++i) {
      if(dollars[i] == 1) {
	dollars.erase(dollars.begin() + i, dollars.end());
	//erase all of the 1s
      }
    }

    /*cout << "dollars.back(): " << dollars.back() << endl;
    cout << "dollars after 1s were supposed to be deleted: ";
    for(unsigned i = 0; i < dollars.size(); ++i) {
      cout << dollars[i] << " ";
    }
    cout << endl;
   
    if(dollars.back() == 50) {
      dollars.pop_back();
      denomination = 20;
      return;
    }
    else if(dollars.back() == 20) {
      dollars.pop_back();
      denomination = 10;
      return;
    }
    else if(dollars.back() == 10) {
      dollars.pop_back();
      denomination = 5;
      return;
    }
    else if(dollars.back() == 5) {
      dollars.pop_back();
      denomination = 2;
      return;
    }
    else { //at this point, the end can only be a 2 since all 1s are gone
      dollars.pop_back();
      denomination = 1;
      return;
    }
  }*/
}

void print_solutions(vector<int> dollars, int max_bill, int dollar_amount, 
		     const set<int, Greater> &dollars_in) {
  int sum = 0;
  int total = 0;
  vector<int> change_bills;
  dollars.push_back(max_bill);
  int denomination_to_add = max_bill; //first, add $50 bills to the vector
  int change = 0;
  int test_count = 0;

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
      
      cout << "solution: ";
      for(unsigned i = 0; i < dollars.size(); ++i) {
        cout << dollars[i] << " ";
	sum += dollars[i];
      }
      cout << "sum: " << sum << endl;
      sum = 0; //reset sum 
      ++test_count;
      /*cout << "change_bills: ";
      for(unsigned i = 0; i < change_bills.size(); ++i) {
        cout << change_bills[i] << " ";
      }
      cout << endl;*/
    }
    update_dollars(dollars, denomination_to_add, dollars_in); 
    //pop the unwanted numbers off of the end
    change_bills.clear();
    //clear change_bills
  }
  //cout << "test_count: " << test_count << endl;
}
