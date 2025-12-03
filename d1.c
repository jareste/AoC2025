#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

int main()
{
    char line[256];
    char right;
    int number = 50;
    int sum = 50;
    int aux;
    int prev_sum;
    int zero_count = 0;
    FILE* file = fopen("input.txt", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%c%d", &right, &number);
        if (right == 'R')
        {
#ifdef LEVEL1
            sum += number;
            if (sum > 99)
            {
                sum %= 100;
            }

#else
            aux = (100 - sum) % 100;
            if (aux == 0)
            {
                aux = 100;
            }

            if (aux <= number)
            {
                zero_count += 1 + (number - aux) / 100;
            }
            
            sum = (sum + (number%100) + 100) % 100;
#endif

}
        else if (right == 'L')
        {
#ifdef LEVEL1
            sum -= number;
            if (sum < 0)
            {
                sum %= 100;
            }

#else
            aux = sum % 100;
            if (aux == 0)
            {
                aux = 100;
            }

            if (aux <= number)
            {
                zero_count += 1 + (number - aux) / 100;
            }

            sum = (sum - (number%100) + 100) % 100;
#endif
        }


#ifdef LEVEL1
        if (sum == 0)
        {
            zero_count++;
        }
#endif
    }
    printf("Final number: %d\n", sum);
    printf("Zero count: %x\n", zero_count);
    printf("Zero count: %d\n", zero_count);
    fclose(file);
    return 0;
}
