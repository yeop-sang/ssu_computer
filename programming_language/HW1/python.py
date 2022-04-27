from time import time

def timing(f):
    def wrap(*args, **kwars):
        ts = time()
        result = f(*args, **kwars)
        te = time()
        print("Total execution time using Python is ", te - ts, "Seconds!")
        return result
    return wrap

def gcd(a,b):
    if a < b:
        temp = b
        b = a
        a = temp
    while b != 0:
        temp = a % b
        a = b
        b = temp
    return a

def gcdVector(numbers: list):
    if len(numbers) == 1:
        return numbers[0]
    if len(numbers) == 2:
        return gcd(numbers[0], numbers[1])
    return gcd(numbers.pop(0), gcdVector(numbers))


def prime_number_between(a, b):
    i = 0
    res = 0
    if a > b:
        i = b
        b = a
        a = i
    while a <= b:
        i = 2
        while i <= a:
            if a % i == 0:
                break
            i = i + 1
        if a == i:
            res = res + 1
        a = a + 1
    return res

@timing
def solve(numbers: list):
    numbers = list(set(numbers))
    numbers.sort()

    temp_numbers = numbers.copy()
    print("GCD of input numbers is", gcdVector(temp_numbers))

    for i in range(1, len(numbers)):
        a = numbers[i-1]
        b = numbers[i]
        print(f"Number of prime numbers between{a}, {b} : {prime_number_between(a,b)}")


n = int(input("Input the number of numbers to process: "))
numbers= [int(i) for i in input("Input the numbers to be processed:\n").split()]
solve(numbers)

