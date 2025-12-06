#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "ft_list.h"

typedef struct
{
    list_item_t item;

    uint64_t num;

    #ifdef LEVEL1
    char str[21];
    #else
    char* str;
    #endif
    size_t str_size;
} t_number;

typedef enum
{
    ADD = 0,
    SUB,
    MUL,
    DIV,
    MAX
} t_sign;

typedef struct
{
    t_sign sign;

    char* symbol;
} t_sign_symbols;

typedef struct 
{
    list_item_t item;
    t_number* numbers;

    t_sign sign;
#ifndef LEVEL1
    size_t column_size;
#endif

} t_columns;

t_sign_symbols m_relation[MAX+1] = {
    {ADD, "+"},
    {SUB, "-"},
    {MUL, "*"},
    {DIV, "/"},
    {MAX, "max"}
};

static t_columns* m_columns = NULL;

#ifdef LEVEL1
int main()
{
    char line[16384];
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
    uint64_t number = 0;
    size_t line_size;
    ssize_t aux_size;
    size_t i;
    size_t j;
    FILE* file = fopen("i", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    char* minus_pos;
    memset(line, 0, sizeof(line));
    file = fopen("i", "r");

    t_columns* column;
    t_number* new_number;
    t_sign symbol;
    j = 0;
    while (fgets(line, sizeof(line), file))
    {
        aux = line;
        i = 0;
start:
        printf("Processing line[%zu]: %s\n", i, aux);
        while (isspace(*aux))
        {
            aux++;
        }
        if (*aux == '\0')
        {
            memset(line, 0, sizeof(line));
            continue;
        }

        symbol = ADD;
        while (symbol < MAX)
        {
            size_t symbol_len = strlen(m_relation[symbol].symbol);
            if (strncmp(aux, m_relation[symbol].symbol, symbol_len) == 0)
            {
                break;
            }
            symbol++;
        }
        if (symbol != MAX)
        {
            column = m_columns;
            while (column != NULL)
            {
                if (column->sign == MAX)
                {
                    column->sign = symbol;
                    printf("Set column %zu to sign %s\n", i, m_relation[symbol].symbol);
                    aux++;
                    i++;
                    break;
                }
                column = FT_LIST_GET_NEXT(&m_columns, column);
            }
            if (column != NULL)
            {
                goto start;
            }
            else
            {
                printf("Unknown symbol encountered: %c\n", *aux);
                aux++;
                memset(line, 0, sizeof(line));
                continue;
            }
        }

        number = strtoull(aux, &biggest_char, 10);
        if (biggest_char == aux)
        {
            printf("Invalid character encountered: %c\n", *aux);
            aux++;
                memset(line, 0, sizeof(line));
            continue;
        }

        aux = biggest_char;
        printf("Read number: %llu\n", (unsigned long long)number);
        result += number;
        column = m_columns;
        j = 0;
        while (column != NULL)
        {
            if (j == i)
                break;
            j++;
            column = FT_LIST_GET_NEXT(&m_columns, column);
        }
        if (column == NULL)
        {
            column = malloc(sizeof(t_columns));
            if (column == NULL)
            {
                perror("Error allocating memory");
                exit(1);
            }
            column->sign = MAX;
            column->numbers = NULL;
            FT_LIST_ADD_LAST(&m_columns, column);
        }

        new_number = malloc(sizeof(t_number));
        if (new_number == NULL)
        {
            perror("Error2 allocating memory");
            exit(1);
        }

        new_number->num = number;
        snprintf(new_number->str, sizeof(new_number->str), "%llu", (unsigned long long)number);
        new_number->str_size = strlen(new_number->str);
        printf("Adding number %llu to column %p\n", (unsigned long long)new_number->num, column);
        FT_LIST_ADD_LAST(&column->numbers, new_number);
        i++;
        goto start;
    }

    column = m_columns;
    result = 0;
    while (column != NULL)
    {
        symbol = column->sign;
        new_number = column->numbers;
        uint64_t column_result = 0;
        while (new_number != NULL)
        {
            // printf("Column sign: %s, number: %llu\n", m_relation[symbol].symbol, (unsigned long long)new_number->num);
            // result += new_number->num;
            if (column_result == 0)
            {
                column_result = new_number->num;
                new_number = FT_LIST_GET_NEXT(&column->numbers, new_number);
                memset(line, 0, sizeof(line));
                continue;
            }
            switch (symbol)
            {
                case ADD:
                    column_result += new_number->num;
                    break;
                case SUB:
                    column_result -= new_number->num;
                    break;
                case MUL:
                    column_result *= new_number->num;
                    break;
                case DIV:
                    if (new_number->num != 0)
                    {
                        column_result /= new_number->num;
                    }
                    break;
                default:
                    break;
            }
            new_number = FT_LIST_GET_NEXT(&column->numbers, new_number);
        }
        // printf("Column result: %llu\n", (unsigned long long)column_result);
        result += column_result;
        // printf("After processing column with sign %s, result is %llu\n", m_relation[symbol].symbol, (unsigned long long)result);
        column = FT_LIST_GET_NEXT(&m_columns, column);
    }

    printf("Total valid numbers: %llu\n", (unsigned long long)result);

    exit(1);
    return 0;
}
#else
int main()
{
    char line[16384];
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
    uint64_t number = 0;
    size_t line_size;
    ssize_t aux_size;
    size_t i;
    size_t j;
    FILE* file = fopen("i", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    char* minus_pos;
    memset(line, 0, sizeof(line));
    fgets(line, sizeof(line), file);
    line_size = strlen(line);
    char* file_content;

    file_content = malloc(((line_size+1) * 5) + 1);

    fclose(file);
    file = fopen("i", "r");

    t_columns* column;
    t_number* new_number;
    t_sign symbol;
    j = 0;
    i = 0;
    memset(line, 0, sizeof(line));
    while (fgets(line, sizeof(line), file))
    {
        printf("Read line: %s", line);
        strcpy(&file_content[i * line_size], line);
        i++;
        memset(line, 0, sizeof(line));
    }

    printf("File content loaded, size: %zu\n", strlen(file_content));
    printf("%s\n", file_content);

    aux = strtok(file_content, "\n");
    while (aux != NULL)
    {
        printf("Processing line[%zu]: %s\n", j, aux);
        if (isdigit(*aux) || isspace(*aux))
        {
            aux = strtok(NULL, "\n");
            continue;
        }
        printf("Non-digit line detected: %s\n", aux);

        i = 0;
        while (aux[i] != '\0')
        {
            j = 0;
            printf("Processing character: '%s'\n", aux+i);
            column = malloc(sizeof(t_columns));
            if (column == NULL)
            {
                perror("Error allocating memory for column");
                exit(1);
            }

            symbol = ADD;
            while (symbol < MAX)
            {
                size_t symbol_len = strlen(m_relation[symbol].symbol);
                if (strncmp(&aux[i], m_relation[symbol].symbol, symbol_len) == 0)
                {
                    break;
                }
                symbol++;
            }
            if (symbol == MAX)
            {
                printf("Unknown symbol encountered: %c\n", aux[i]);
                exit(1);
                continue;
            }
            j++;
            while (isspace(aux[i+j]))
            {
                j++;
            }
            column->column_size = j-1;
            column->sign = symbol;
            column->numbers = NULL;
            i += j;
            printf("Adding column with sign %s and size %zu\n", m_relation[symbol].symbol, column->column_size);
            FT_LIST_ADD_LAST(&m_columns, column);
        }

        aux = strtok(NULL, "\n");
    }

    fclose(file);
    file = fopen("i", "r");

    j = 0;
    i = 0;
    memset(line, 0, sizeof(line));
    while (fgets(line, sizeof(line), file))
    {
        printf("Read line: %s", line);
        strcpy(&file_content[i * line_size], line);
        i++;
        memset(line, 0, sizeof(line));
    }

    printf("file_content:\n%s\n", file_content);
    aux = strtok(file_content, "\n");
    while (aux != NULL)
    {
        printf("Processing line: %s\n", aux);
        if (!isdigit(*aux) && !isspace(*aux))
        {
            printf("Non-digit line detected: %s\n", aux);
            break;
        }

        column = m_columns;
        i = 0;
        while (column != NULL)
        {
            new_number = malloc(sizeof(t_number));
            
            new_number->str_size = column->column_size;
            new_number->str = malloc(new_number->str_size + 1);
            new_number->str[new_number->str_size] = '\0';
            strncpy(new_number->str, aux+i, new_number->str_size);
            printf("Read number string: '%s'\n", new_number->str);
            i+= column->column_size+1;
            FT_LIST_ADD_LAST(&column->numbers, new_number);
            column = FT_LIST_GET_NEXT(&m_columns, column);
        }

        aux = strtok(NULL, "\n");
    }

    column = m_columns;
    result = 0;
    while (column != NULL)
    {
        uint64_t column_result = 0;
        uint64_t biggest_size = column->column_size;
        t_number calc_number;
        calc_number.str = malloc(biggest_size + 1);
        calc_number.str[biggest_size] = '\0';
        printf("Processing column with sign %s and size %zu\n", m_relation[column->sign].symbol, biggest_size);
        while (biggest_size > 0)
        {
            new_number = column->numbers;
            i = 0;
            printf("%p\n", new_number);
            while (new_number != NULL)
            {
                printf("Considering number '%s' with size %zu for position %zu (biggest_size %zu)\n", new_number->str, new_number->str_size, i, biggest_size);
                if (new_number->str_size < biggest_size)
                {
                    new_number = FT_LIST_GET_NEXT(&column->numbers, new_number);
                    continue;
                }


                calc_number.str[i] = new_number->str[biggest_size - 1];
                printf("Adding digit '%c' from number '%s' to position %zu\n", new_number->str[biggest_size - 1], new_number->str, i);
                i++;
                new_number = FT_LIST_GET_NEXT(&column->numbers, new_number);
            }
            printf("Calculating for column with sign %s, size %zu, numbers: %s\n", m_relation[column->sign].symbol, biggest_size, calc_number.str);
            calc_number.str[i] = '\0';
            biggest_size--;

            if (column_result == 0)
            {
                column_result = strtoull(calc_number.str, NULL, 10);
                continue;
            }
            switch (column->sign)
            {
                case ADD:
                    column_result += strtoull(calc_number.str, NULL, 10);
                    break;
                case SUB:
                    column_result -= strtoull(calc_number.str, NULL, 10);
                    break;
                case MUL:
                    column_result *= strtoull(calc_number.str, NULL, 10);
                    break;
                case DIV:
                    if (strtoull(calc_number.str, NULL, 10) != 0)
                    {
                        column_result /= strtoull(calc_number.str, NULL, 10);
                    }
                    break;
                default:
                    break;
            }
            printf("Converted number: %llu\n", (unsigned long long)column_result);
        }
        result += column_result;
        column = FT_LIST_GET_NEXT(&m_columns, column);

    }



    printf("Total valid numbers: %llu\n", (unsigned long long)result);

    exit(1);
    return 0;
}

#endif


