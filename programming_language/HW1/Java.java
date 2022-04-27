package com.yeop.pl;

import java.util.Collections;
import java.util.LinkedHashSet;
import java.util.Scanner;
import java.util.Vector;

public class Java {
    public static void main(String[] args) {
        long n, startTime, endTime;
        Vector<Long> numbers = new Vector<Long>();
        Scanner scanner = new Scanner(System.in);

        System.out.print("Input the number of numbers to process: ");
        n = scanner.nextInt();
        System.out.print("Input the numbers to be processed:\n");
        for (int i = 0; i < n; i++) {
            numbers.addElement(scanner.nextLong());
        }

        startTime = System.nanoTime();

        LinkedHashSet<Long> hashSet = new LinkedHashSet<Long>(numbers);
        numbers.clear();
        numbers.addAll(hashSet);
        Collections.sort(numbers);

        Solve s = new Solve(numbers);
        s.solver();
        endTime = System.nanoTime();

        System.out.printf("Total execution time using Python is %f Seconds!\n", (endTime - startTime) / (float) 1000000000);
        scanner.close();
    }

}

class Solve {
    private Vector<Long> numbers;

    public Solve(Vector<Long> numbers) {
        this.numbers = numbers;
    }

    long gcd(long a, long b) {
        long temp;
        if (a < b) {
            temp = b;
            b = a;
            a = temp;
        }

        while (b != 0) {
            temp = a % b;
            a = b;
            b = temp;
        }

        return a;
    }

    long gcdVector(Vector<Long> numbers) {
        int size = numbers.size();
        if (size == 1) {
            return numbers.get(0);
        }
        if (size == 2) {
            return gcd(numbers.get(0), numbers.get(1));
        }
        long temp = numbers.get(size - 1);
        numbers.remove(size - 1);
        return gcd(temp, gcdVector(numbers));
    }

    long prime_number_between(long a, long b) {
        long i = 0, res = 0;

        if (a > b) {
            i = b;
            b = a;
            a = i;
        }

        for (; a <= b; a++) {
            for (i = 2; i <= a; i++) {
                if (a % i == 0)
                    break;
            }
            if (a == i)
                res++;
        }
        return res;
    }

    void solver() {
        Vector<Long> temp_numbers = new Vector<Long>(numbers);
        System.out.printf("GCD of input numbers is %d\n", this.gcdVector(temp_numbers));
        long a, b;
        for (int i = 1; i < numbers.size(); i++) {
            a = numbers.get(i - 1);
            b = numbers.get(i);
            System.out.printf("Number of prime numbers between %d, %d : %d\n", a, b, this.prime_number_between(a, b));
        }
    }
}