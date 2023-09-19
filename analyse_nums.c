#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>
#include <unistd.h>

void print();
int *randomNumbers = NULL;        // for storing numbers
int initialSize, size, totalSize; // initialSize stores initial size given in command line(n),size stores the size of total valid values inside array,totalSize is the actual size needed to store our numbers different for cases (n < range and n>=range)
int r2, r1, range;                // r1 and r2 are values given from command line and range(r2-r1+1) stores the total numbers between r1 to r2
int counts[7] = {0};              // stores total count of each 6 operation performed
double opTime[7] = {0};           // stores total time of each 6 operation performed

// function that generates a 32bit random number between r1 and r2 (inclusive)
int gen_rand()
{
    int random = RAND_MAX == 0x7fffffff ? rand() : rand() << 16 | rand() << 1 | rand() & 1;
    return r1 + random % range;
}
// function that check if size of array is smaller than range (r2-r1+1)
int size_in_range()
{
    return abs(initialSize) >= range;
}
// Comparison function for qsort
int cmp(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}
// Binary search function that find the target value from an array which includes -1 values denoting empty or invalid values ,if its found it returns the position and if its not return the negative of position where it would exist
int binary_search(int v)
{
    int left = 0;
    int right = totalSize - 1;
    int result = -1;
    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        while (randomNumbers[mid] == -1)
            mid--;

        if (randomNumbers[mid] == v)
        {
            while (randomNumbers[mid] == v)
                mid--;
            result = mid + 1; // return  first occurence of that number
            break;
        }
        else if (randomNumbers[mid] < v)
        {
            left = mid + 1; // Adjust the left boundary
            while (randomNumbers[left] == -1)
                left++;
        }
        else
        {
            right = mid - 1; // Adjust the right boundary
            while (randomNumbers[right] == -1)
                right--;
        }
    }
    if (result >= 0)
        return result;
    return -(left); // Target not found
}

// function that generates n random integers between r1 and r2
int *generateRandomIntegers()
{

    printf("Initializing array with random numbers....\n");
    totalSize = size + size * 0.10;
    int *numbers;
    if (size_in_range())
    {
        totalSize = range;
        numbers = (int *)calloc(range, sizeof(int)); // zero initialized array
    }
    else
        numbers = (int *)malloc(totalSize * sizeof(int));
    if (numbers == NULL)
    {
        printf("Memory allocation failed !!!\n");
        return NULL;
    }
    if (size_in_range())
    {
        for (int i = 0; i < size; i++)
        {
            numbers[gen_rand() - r1]++;
        }
        printf("Initialization successfull.\n");
    }
    else
    {
        for (int i = 0; i < totalSize; i++) // generating n*1.1 random integers in which we have to include -1 in between the numbers
            numbers[i] = gen_rand();
        printf("Initialization successfull.Now Sorting....\n");
        qsort(numbers, totalSize, sizeof(numbers[0]), cmp);
        printf("Sorted\n\n");
        printf("Adding -1 denoting invalid inputs...\n\n");
        int noOfNegOne = totalSize * 0.1;
        // uniformly distributing the -1 ,not putting it in between sequence of numbers
        for (int i = 1; i <= noOfNegOne; i++)
        {
            int j = (i * 10) - 1;
            int current = numbers[j];
            while (current == numbers[j])
            {
                j++;
                if (j == totalSize - 1)
                    break;
            }
            numbers[j] = -1;
            size--;
        }
    }

    return numbers;
}

// Function to add new number
int add(int v)
{
    if (size >= totalSize)
        return 0;
    // Count store approach
    if (size_in_range()) // If size is smaller than range we increase the count
    {
        randomNumbers[v - r1]++;
        size++;
        return 1;
    }
    else
    {
        // Number store approach
        int pos = abs(binary_search(v));

        if (randomNumbers[pos - 1] == -1)
            randomNumbers[pos - 1] = v;

        else if (randomNumbers[pos] == -1)
            randomNumbers[pos] = v;
        else
        {
            // we need to find nearest -1 and shift the elements accordingly
            int rightSearch = pos + 1;
            int leftSearch = pos - 1;
            while (randomNumbers[rightSearch] != -1 && randomNumbers[leftSearch] != -1) // finding nearest -1
            {
                if (rightSearch >= totalSize - 1 && leftSearch <= 0)
                    break;

                if (rightSearch < totalSize - 1)
                    rightSearch++;
                if (leftSearch > 0)
                    leftSearch--;
            }

            if (randomNumbers[rightSearch] == -1) // if -1 is found on right side of pos
            {
                for (int i = rightSearch; i > pos; i--)
                    randomNumbers[i] = randomNumbers[i - 1];
                randomNumbers[pos] = v;
            }
            else if (randomNumbers[leftSearch] == -1) // if -1 is found on left side of pos
            {
                for (int i = leftSearch; i < pos - 1; i++)
                    randomNumbers[i] = randomNumbers[i + 1];
                randomNumbers[pos - 1] = v;
            }
            else
                return 0;
        }
        size++;
        return 1;
    }
}

// Function that perform delete operation
int delete(int v)
{
    // Count store approach
    if (size_in_range())
    {
        int ind = v - r1;
        if (!randomNumbers[ind]) // if count is 0;
            return 0;
        else
        {
            randomNumbers[ind]--; // decrease the count by 1
            size--;
            return 1;
        }
    }

    // Number store approach
    int pos = binary_search(v);
    if (pos < 0) // if element is not present
        return 0;
    else // deleting first occurence of number
    {
        randomNumbers[pos] = -1;
        size--;
        return 1;
    }
}

// Function for finding number of occurence of an element
int find(int v)
{
    // Count store approach
    if (size_in_range())
        return randomNumbers[v - r1]; // return the count

    // Number store approach
    int pos = binary_search(v); // returns index of first occurence of element,if exist
    if (pos < 0)
        return 0;
    int count = 0;
    while (randomNumbers[pos] == v)
    {
        count++;
        pos++;
        if (pos == size)
            break;
    }
    return count;
}

// Successor finding function
int succ(int v)
{
    // Count store appraoch
    if (size_in_range())
    {
        int ind = v - r1;

        if (ind == totalSize - 1) // If its last element return -1
            return -1;
        int i = ind + 1;
        while (randomNumbers[i] == 0) // find non zero element after the current element
        {
            i++;
            if (i == totalSize) // checking if i was last element before increement
                return -1;
        }
        return i + r1;
    }

    // Number store approach
    int pos = binary_search(v);
    if (pos < 0 || pos == totalSize - 1)
        return -1;
    while (randomNumbers[pos] == v || randomNumbers[pos] == -1)
    {
        pos++;
        if (pos == totalSize)
            return -1;
    }
    return randomNumbers[pos];
}

// Predecessor finding function
int pred(int v)
{
    if (size_in_range())
    {
        int ind = v - r1;
        if (ind == 0)
            return -1;
        int i = ind - 1;
        while (randomNumbers[i] == 0)
        {
            i--;
            if (i == -1)
                return -1;
        }
        return i + r1;
    }

    // Number store approach
    int pos = binary_search(v);
    if (pos <= 0)
        return -1;
    while (randomNumbers[pos] == v || randomNumbers[pos] == -1)
    {
        pos--;
        if (pos == -1)
            return -1;
    }
    return randomNumbers[pos];
}

// Functions for returning min
int min()
{
    if (size_in_range()) // find first non zero element
    {
        int i = 0;
        while (randomNumbers[i] == 0)
            i++;
        return i + r1; // returning index as number as array is holding count
    }
    int i = 0;
    while (randomNumbers[i] == -1)
    {
        i++;
        if (i == totalSize - 1) // edge case :assuming our array might contain no valid values
            return -1;
    }

    return randomNumbers[i];
}

// Functions for returning max
int max()
{

    if (size_in_range()) // find last non zero element
    {
        int i = totalSize - 1;
        while (randomNumbers[i] == 0)
            i--;
        return i + r1; // returning index as number as array is holding count
    }
    int i = totalSize - 1;
    while (randomNumbers[i] == -1)
    {
        i--;
        if (i == -1) // edge case :assuming our array might contain no valid values
            return -1;
    }
    return randomNumbers[i];
}

// Function to print array
void print()
{
    if (randomNumbers == NULL)
        return;
    printf("Numbers: ");
    if (size_in_range())
    {
        int i = 0;
        while (i < totalSize)
        {
            int count = randomNumbers[i];
            while (count > 0) // printing i+r1 which denotes number count times
            {
                printf("%d ", i + r1);
                count--;
            }

            i++;
        }
    }
    else
        for (int i = 0; i < totalSize; i++)
        {
            if (randomNumbers[i] != -1)
                printf("%d ", randomNumbers[i]);
        }
    printf(": min %d : max %d ", min(), max());
    printf("\n");
}

// function that print input data
void print_input_data()
{
    size_t usedMemoryBytes;
    if (size_in_range())
        usedMemoryBytes = range * sizeof(int);
    else
        usedMemoryBytes = initialSize * 1.1 * sizeof(int);
    double usedMemoryInMB = (double)usedMemoryBytes / 1000000;
    printf("n = %d, r1 = %d, r2 = %d, Memory used = %f Mbytes \n", initialSize, r1, r2, usedMemoryInMB);
}

// function for printing performance test data
void driver()
{
    char op[7][10] = {"find", "add", "delete", "succ", "pred", "min", "max"};
    double avgTime = 0;
    printf("%-10s %-20s %-20s %s\n", "", "Op counts", "Total time", "Avg. Time");
    for (int i = 0; i < 7; i++)
    {
        if (counts[i] != 0)
            avgTime = (double)(opTime[i] / counts[i]);
        printf("%-10s %-20d %-20.6f %.10e\n", op[i], counts[i], opTime[i], avgTime);
    }
}

// function that perform random operations and calculate Performance test data
void perform_random_op()
{
    const int nops = 7;
    int tops = nops * 0.1 * initialSize;
    int count = 0;
    double start, end;
    double secs;
    printf("Now Performing random operations...\n");
    while (count < tops)
    {
        secs = 0;
        int r = gen_rand();
        int op = rand() % 7;
        start = clock();
        switch (op)
        {
        case 0:
            find(r);
            break;
        case 1:
            add(r);
            break;
        case 2:
            delete (r);
            break;
        case 3:
            succ(r);
            break;
        case 4:
            pred(r);
            break;
        case 5:
            min();
            break;
        case 6:
            max();
            break;
        default:
            break;
        }
        end = clock();
        secs = (end - start) / CLOCKS_PER_SEC;
        counts[op]++;
        opTime[op] = opTime[op] + secs;
        count++;
    }
    printf("Operations Performed.\n");
}

// function for printing functionality test sample
void test_performOp(int opSeq[], int sizeOfOpSeq, int num)
{
    for (int j = 0; j < sizeOfOpSeq; j++)
    {
        switch (opSeq[j])
        {
        case 0:
            printf("find %d %d ", num, find(num));
            break;
        case 1:
            printf("add %d %d ", num, add(num));
            break;
        case 2:
            printf("delete %d %d ", num, delete (num));
            break;
        case 3:
            printf("succ %d %d ", num, succ(num));
            break;
        case 4:
            printf("pred %d %d ", num, pred(num));
        }
        if (j < 7)
            printf(":");
    }
}

// function that restore the array after performing test operations in functionality test
void restore(int numbers[])
{
    if (size_in_range())
    {
        for (int i = 0; i < totalSize; i++)
        {
            if (numbers[i] != randomNumbers[i])
            {
                int count = abs(randomNumbers[i] - numbers[i]);
                if (numbers[i] > randomNumbers[i]) // numbers occurence is reduced
                    while (count)
                    {
                        printf("find %d %d :", i + r1, find(i + r1));
                        printf("add %d %d\n", i + r1, add(i + r1));
                        count--;
                    }
                else // number occurence is increased
                {
                    while (count)
                    {
                        printf("find %d %d :", i + r1, find(i + r1));
                        printf("delete %d %d\n", i + r1, delete (i + r1));
                        count--;
                    }
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < totalSize; i++)
        {
            if (numbers[i] == -1)
            {
                i++;
                continue;
            }
            int prevCount = 0, cur = numbers[i];
            while (cur == numbers[i] && i < totalSize)
            {
                prevCount++;
                i++;
            }
            i--;
            int curCount = find(cur);
            if (curCount != prevCount)
            {
                if (curCount < prevCount) // numbers occurence is reduced
                {
                    for (int j = curCount; j < prevCount; j++)
                    {
                        printf("find %d %d :", cur, find(cur));
                        printf("add %d %d\n", cur, add(cur));
                    }
                }
                else // number occurence is increased
                {
                    for (int j = curCount; j > prevCount; j--)
                    {
                        printf("find %d %d :", cur, find(cur));
                        printf("delete %d %d\n", cur, delete (cur));
                    }
                }
            }
        }
    }
    free(numbers);
}

// functionality test program
void test()
{
    print();
    const int nops = 7;
    int i = 0;
    int opSeq[] = {0, 2, 0, 2, 1, 0, 3, 4}; // sequence of operation performed in functionality test, its mutable doesnt affect our code in any way
    int sizeOfOpSeq = sizeof(opSeq) / sizeof(opSeq[0]);

    int numbers[totalSize];
    for (int i = 0; i < totalSize; i++)
        numbers[i] = randomNumbers[i];

    if (size_in_range())
    {
        while (i < totalSize) // that sequence is same and performed on each element
        {
            if (randomNumbers[i] == 0)
            {
                i++;
                continue;
            }
            test_performOp(opSeq, sizeOfOpSeq, i + r1);
            printf("\n");
            i++;
        }
    }
    else
    {
        while (i < totalSize)
        {
            if (randomNumbers[i] == -1)
            {
                i++;
                continue;
            }
            int current = randomNumbers[i];
            test_performOp(opSeq, sizeOfOpSeq, current);
            while (randomNumbers[i] == current && randomNumbers[i] == -1 && i < totalSize)
                i++;
            printf("\n");
            i++;
        }
    }
    print();
    // Now some data might removed or added after performing the sequences of operations we need to add them back
    // restoring the array
    restore(numbers); // passing original array with size before performing operation
    print();
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Invalid arguments numbers!!!\n");
        return 1;
    }
    srand(time(NULL));
    size = abs(atoi(argv[1]));
    initialSize = size;
    r1 = atoi(argv[2]);
    r2 = atoi(argv[3]);
    range = (r2 - r1 + 1);

    if (r1 > r2 || r1 <= 0 || size <= 0)
    {
        printf("Invalid input data!!!\n");
        return 1;
    }

    print_input_data();
    // Initializing random n integers
    randomNumbers = generateRandomIntegers();
    if (randomNumbers == NULL) // memory allocation failed
        return 0;
    // performing random operation
    perform_random_op();

    if (atoi(argv[1]) > 0)
    {
        printf("Showing Performance testing results...\n");
        sleep(2);
        system("cls");
        print_input_data();
        driver();
    }
    else
    {
        printf("Performing functional testing now....\n");
        sleep(2);
        system("cls");
        print_input_data();
        test();
    }

    free(randomNumbers);
    return 0;
}