#include "text_handlers.hpp"

#include <vector>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <string>

using namespace std;

void handler(string buf)
{
		vector<int> nums;
		for (auto i : buf)
			if (i <= '9' && i >= '0')
				nums.push_back(i - '0');

		cout << "Sum of the digits: ";		
		cout << accumulate(nums.begin(),  nums.end(), 0) << endl;

		sort(nums.rbegin(), nums.rend());
		cout << "All digits in desc order: ";
		copy(nums.begin(), nums.end(), ostream_iterator<int>(cout));
		cout << endl;

		cout << "Min: ";
		auto min = min_element(nums.begin(), nums.end());
		if (min != nums.end()) 
			cout << *min_element(nums.begin(), nums.end()) << endl;
		else
			cout << "None" << endl;

		cout << "Max: ";
		auto max = max_element(nums.begin(), nums.end());
		if (max != nums.end())
			cout << *max_element(nums.begin(), nums.end()) << endl;
		else
			cout << "None" << endl;

}

