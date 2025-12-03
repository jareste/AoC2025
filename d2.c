#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int is_repeated_pattern(const char *s)
{
    size_t block;
    size_t i;
    size_t ok;
    size_t len;
    
    
    len = strlen(s);
    if (len < 2)
        return 0;

    for (block = 1; block <= len / 2; ++block)
    {
        if (len % block != 0) 
            continue;

        ok = 1;

        for (i = block; i < len; ++i)
        {
            printf("Comparing s[%zu] = %c with s[%zu] = %c\n", i, s[i], i % block, s[i % block]);
            if (s[i] != s[i % block])
            {
                ok = 0;
                break;
            }
        }

        if (ok)
        {
            return 1;
        }
    }

    return 0;
}

int main()
{
    FILE* file;
    size_t length;
    size_t file_length;
    char* buffer;
    char* token;
    char* comma;
    char* separator;
    uint64_t number;
    uint64_t number2;
    uint64_t result = 0;
    char first_half[40];
    char second_half[40];
    char numberStr[40];
    char numberStr2[40];
    int found = 0;

    file = fopen("input (2).txt", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    fseek(file, 0, SEEK_END);
    file_length = ftell(file);
    fseek(file, 0, SEEK_SET);
    buffer = (char*)malloc(file_length + 1);
    if (!buffer)
    {
        perror("Memory allocation failed");
        fclose(file);
        return EXIT_FAILURE;
    }
    fread(buffer, 1, file_length, file);
    buffer[file_length] = '\0';
    
    token = strtok(buffer, ",");
    while (token != NULL)
    {
        // printf("Processing token: %s\n", token);
        separator = strchr(token, '-');
        if (!separator)
            exit(1);

        *separator = '\0';
        snprintf(numberStr, sizeof(numberStr), "%s", token);
        snprintf(numberStr2, sizeof(numberStr2), "%s", separator + 1);

        number  = strtoull(numberStr,  NULL, 10);
        number2 = strtoull(numberStr2, NULL, 10);
        if (number > number2)
            exit(1);

        // printf("Range: %llu to %llu\n", number, number2);
        number--;
        while (number < number2)
        {
#ifdef LVEEL1
            number += 1;
            snprintf(numberStr, sizeof(numberStr), "%llu", (unsigned long long)number);

            length = strlen(numberStr);
            if ((length % 2) != 0)
                continue;

            snprintf(first_half, sizeof(first_half), "%.*s", (int)(length / 2), numberStr);
            snprintf(second_half, sizeof(second_half), "%s", numberStr + (length / 2));
            
            if (strcmp(first_half, second_half) == 0)
            {
                
                found++;
                result += number;
                printf("new result: %llu (number: %s)\n", (unsigned long long)result, numberStr);

            }
#else
            number += 1;
            snprintf(numberStr, sizeof(numberStr), "%llu", (unsigned long long)number);

            if (is_repeated_pattern(numberStr))
            {
                found++;
                result += number;
                printf("new result: %llu (number: %s)\n", (unsigned long long)result, numberStr);
            }
#endif
        }

        *separator = '-';

        token = strtok(NULL, ",");
    }

    printf("%s\n", buffer);
    printf("Found matching halves count: %d\n", found);
    printf("Result: '%llu'\n", result);
    
    free(buffer);
    fclose(file);
    return 0;
}
