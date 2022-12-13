#include <iostream>
#include <vector>
using namespace std;

int main()
{
    int current = 2;
    double proportion = 1;
    int maximum;
    bool isPrime = true;
    vector<int> primes;
    cout << "Please enter the maximum in your primes count\n";
    cin >> maximum;
    while (current <= maximum)
    {
        isPrime = true;
        for (int i = 2; i < primes.size(); i++)
        {
            if (current % i == 0)
            {
                isPrime = false;
                i = primes.size();
            }
        }
        if (isPrime == true)
        {
            primes.push_back(current);
            proportion -= proportion / current;
        }
        current ++;
    }
    cout << "The proportion of all numbers 1-" << maximum << " that are prime is \n"; 
    cout << proportion << endl;
    return 0;
}