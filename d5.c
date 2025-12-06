#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "ft_list.h"

typedef struct 
{
    list_item_t item;
    uint64_t range_min;
    uint64_t range_max;
} fresh_food_t;

static fresh_food_t* m_ranges = NULL;

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
    uint64_t number = 0;
    size_t line_size;
    ssize_t aux_size;
    FILE* file = fopen("input.txt", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    char* minus_pos;
    memset(line, 0, sizeof(line));
    file = fopen("input.txt", "r");

    fresh_food_t* range_node;
    while (fgets(line, sizeof(line), file))
    {
        minus_pos = strchr(line, '-');
        if (minus_pos)
        {
            range_node = malloc(sizeof(fresh_food_t));
            if (!range_node)
            {
                perror("Error allocating memory for range node");
                fclose(file);
                return 1;
            }
            *minus_pos = '\0';
            range_node->range_min = strtoull(line, NULL, 10);
            second_char = *(minus_pos + 1);
            range_node->range_max = strtoull(minus_pos + 1, NULL, 10);
            printf("Range: %llu - %llu\n", range_node->range_min, range_node->range_max);
            
            FT_LIST_ADD_LAST(&m_ranges, range_node);
        }
        else
        {
            range_node = m_ranges;
            number = strtoull(line, NULL, 10);
            while (range_node)
            {
                if (number >= range_node->range_min && number <= range_node->range_max)
                {
                    printf("Number %llu is in range %llu - %llu\n", number, range_node->range_min, range_node->range_max);
                    result++;
                    break;
                }
                range_node = FT_LIST_GET_NEXT(&m_ranges, range_node);
            }
        }

    }

    printf("Total valid numbers: %llu\n", (unsigned long long)result);

    /* part 2 */
    result = 0;
    fresh_food_t* range_node2;
    uint64_t overlap_min;
    uint64_t overlap_max;
    uint64_t a1;
    uint64_t a2;
    uint64_t b1;
    uint64_t b2;
    range_node = m_ranges;
    range_node = m_ranges;
    while (range_node)
    {
        range_node2 = FT_LIST_GET_NEXT(&m_ranges, range_node);
        while (range_node2)
        {
            fresh_food_t *next2 = FT_LIST_GET_NEXT(&m_ranges, range_node2);

            a1 = range_node->range_min;
            a2 = range_node->range_max;
            b1 = range_node2->range_min;
            b2 = range_node2->range_max;

            overlap_min = a1 > b1 ? a1 : b1;
            overlap_max = a2 < b2 ? a2 : b2;

            if (overlap_min <= overlap_max)
            {
                if (b1 >= a1 && b2 <= a2)
                {
                    FT_LIST_POP(&m_ranges, range_node2);
                    free(range_node2);
                }
                else if (b1 < a1 && b2 >= a1 && b2 <= a2)
                {
                    // printf("Shrinking right of %llu-%llu to %llu-%llu\n", b1, b2, b1, a1 - 1);
                    range_node2->range_max = a1 - 1;
                }
                else if (b1 >= a1 && b1 <= a2 && b2 > a2)
                {
                    // printf("Shrinking left of %llu-%llu to %llu-%llu\n", b1, b2, a2 + 1, b2);
                    range_node2->range_min = a2 + 1;
                }
                else if (b1 < a1 && b2 > a2)
                {
                    fresh_food_t *new_node = malloc(sizeof(fresh_food_t));
                    if (!new_node)
                        exit(1);

                    new_node->range_min = a2 + 1;
                    new_node->range_max = b2;
                    FT_LIST_ADD_LAST(&m_ranges,new_node);

                    range_node2->range_max = a1 - 1;
                }

                if (range_node2->range_min > range_node2->range_max)
                {
                    FT_LIST_POP(&m_ranges, range_node2);
                    free(range_node2);
                }
            }

            range_node2 = next2;
        }

        range_node = FT_LIST_GET_NEXT(&m_ranges, range_node);
    }

    result = 0;
    range_node = m_ranges;
    while (range_node)
    {
        result += (range_node->range_max - range_node->range_min + 1);
        range_node = FT_LIST_GET_NEXT(&m_ranges, range_node);
    }
    printf("Total distinct numbers: %llu\n", (unsigned long long)result);

    exit(1);
    return 0;
}
