#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <cmath>
#include <ctime>

using namespace std;

int n;
vector<long long> numbers;

long long gcd(long long a, long long b) {
    if (a < b) {
        swap(a, b);
    }
    long long temp = 0;
    while (b != 0) {
        temp = a % b;
        a = b;
        b = temp;
    }

    return a;
}

long long gcdVector(vector<long long> numbers) {
    if (numbers.size() == 1) {
        return numbers.back();
    } else if (numbers.size() == 2) {

        return gcd(numbers[0], numbers[1]);
    } else {
        long long temp = numbers.back();
        numbers.pop_back();
        return gcd(temp, gcdVector(numbers));
    }
}

long long prime_number_between(long long a, long long b) {
    int i = 0;
    long long result = 0;
    if (a > b)
        swap(a, b);

    for (; a <= b; a++) {
        for (i = 2; i <= a; i++)
            if (a % i == 0)
                break;
        if (a == i)
            result++;
    }
    return result;
}

void input();

void solve();

void myTimer(void (*fnc)()) {
    clock_t begin = clock();
    fnc();
    cout << "Total execution time using C++ is " << float(clock() - begin) / CLOCKS_PER_SEC << " seconds!\n";
}

int main() {
    input();

    myTimer(solve);

    return 0;
}

void input() {
    cout << "Input the number of numbers to process: ";
    cin >> n;
    cout << "Input the numbers to be processed:" << endl;
    for (int i = 0; i < n; ++i) {
        numbers.push_back(*istream_iterator<int>(cin));
    }
}

void solve() {
    numbers.erase(unique(numbers.begin(), numbers.end()), numbers.end());
    sort(numbers.begin(), numbers.end());

    long long a, b;

    cout << "GCD of input numbers is " << gcdVector(numbers) << '\n';

    for (int i = 1; i < numbers.size(); i++) {
        a = numbers[i - 1], b = numbers[i];
        cout << "Number of prime numbers between "
             << a << ", " << b << ": " << prime_number_between(a, b) << '\n';
    }
}
