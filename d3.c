#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifdef LEVEL1
int main()
{
    char line[1024];
    int i;
    char* biggest_char;
    char* aux;
    char second_char;
    int biggest_number;
    char num[3];
    uint64_t result = 0;
    FILE* file = fopen("input.txt", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    while (fgets(line, sizeof(line), file))
    {     
        i = 0;
        biggest_char = NULL;
        while (line[i] != '\0' && line[i] != '\n')
        {
            
            if ((line[i] > ('9')) || (line[i] < '0'))
            {
                fprintf(stderr, "Invalid character detected: '%c'\n", line[i]);
                exit(1);
            }

            if (!biggest_char || ((line[i] > *biggest_char) && (line[i+1] != '\0') && (line[i+1] != '\n')))
                biggest_char = &line[i];

            i++;
        }

        if (biggest_char == NULL)
        {
            fprintf(stderr, "No valid characters found in line\n");
            exit(1);
        }

        i = 0;
        aux = biggest_char+1;
        second_char = '\0';
        while (aux[i] != '\0' && aux[i] != '\n')
        {
            if (aux[i] > second_char)
                second_char = aux[i];

            i++;
        }

        num[0] = *biggest_char;
        num[1] = second_char;
        num[2] = '\0';
        
        biggest_number = atoi(num);
        result += biggest_number;
        printf("Biggest number formed: %d\n", biggest_number);
    }
    printf("Final result: %llu\n", (unsigned long long)result);

    fclose(file);
    return 0;
}
#else
int main()
{
    char line[1024];
    int i;
    char* biggest_char;
    char* aux;
    char second_char;
    uint64_t biggest_number;
#ifndef TIRE_SIZE
    #define TIRE_SIZE 12
#endif
    char num[TIRE_SIZE + 1];
    int collected_numbers = 0;
    uint64_t result = 0;
    size_t line_size;
    ssize_t aux_size;
    FILE* file = fopen("input.txt", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    memset(line, 0, sizeof(line));
    while (fgets(line, sizeof(line), file))
    {     
        i = 0;
        biggest_char = NULL;
        // line_size = strlen(line);
        collected_numbers = 0;

        // if (line[line_size - 1] == '\n')
        //     line_size--;

        while (collected_numbers<TIRE_SIZE)
        {
            aux = biggest_char?biggest_char+1: &line[0];
            // printf("starting check on '%s'\n", aux);
            line_size = strlen(aux);
            if (aux[line_size - 1] == '\n')
                line_size--;

            biggest_char = NULL;
            i = 0;
            printf("checking line %s\n", aux);
            collected_numbers++;
            aux_size = i + collected_numbers - TIRE_SIZE;
                if (aux_size < 0)
                    aux_size = 0;

            // printf("conds: '%d' '%d' '%d', i:%d, collected:%d, line_size:%d\n", *aux, (*aux != '\n') , (aux_size) < line_size, i + collected_numbers - TIRE_SIZE, aux_size, line_size);
            printf("line_size: %zu\n", line_size);
            printf("aux_size: %zd\n", aux_size);
            printf("collected_numbers: %d\n", collected_numbers);
            printf("TIRE_SIZE: %d\n", TIRE_SIZE);
            printf("i: %d\n", i);
            printf("*aux: '%d'\n", aux);
            printf("*(aux+1): '%d'\n", *(aux+1));
            printf("sizes: %d\n", aux_size < line_size);
            while (*aux && (*aux != '\n') && (aux_size < line_size))
            {
                // printf("checking char '%c' biggest '%c'\n", *aux, biggest_char ? *biggest_char : ' ');
                if (!biggest_char)
                    biggest_char = aux;

                if (((*aux > *biggest_char) && (*(aux + (TIRE_SIZE - collected_numbers)) != '\0') && (*(aux + (TIRE_SIZE - collected_numbers)) != '\n')))
                {
                    printf("found new biggest char candidate: %s, %d, '%c'\n", aux, TIRE_SIZE-collected_numbers, *(aux + (TIRE_SIZE - collected_numbers)));
                    biggest_char = aux;
                    printf("new biggest char found: '%c'\n", *biggest_char);
                }

                i++;
                aux_size = i + collected_numbers - TIRE_SIZE; /* Already having collected number but not counting it. */
                if (aux_size < 0)
                    aux_size = 0;

                aux++;
            }

            if (!biggest_char)
            {
                fprintf(stderr, "No valid characters found in line\n");
                exit(1);
            }
            num[collected_numbers-1] = *biggest_char;
        }
        num[TIRE_SIZE] = '\0';
        biggest_number = strtoull(num, NULL, 10);
        result += biggest_number;
        printf("Biggest number formed: %llu\n", (unsigned long long)biggest_number);
        memset(line, 0, sizeof(line));

    }
    printf("Final result: %llu\n", (unsigned long long)result);

    fclose(file);
    return 0;
}
#endif
