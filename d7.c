#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "ft_list.h"

char *m_matrix;
int m_matrix_x = 0;
int m_matrix_y = 0;

typedef struct
{
    list_item_t item;

    int x;
    int y;

} t_next_pos;

static t_next_pos* m_next_pos = NULL;
static t_next_pos* m_already_done = NULL;

#define MATRIX(x, y) m_matrix[(y)*m_matrix_x + (x)]

#define BLANK_CHAR '.'
#define OCCUPIED_CHAR '^'
#define USED_SPLITTER 'X'
#define PRINTED_CHAR '|'

bool is_already_evaluated(int x, int y)
{
    t_next_pos* next = m_already_done;
    while (next != NULL)
    {
        if (next->x == x && next->y == y)
        {
            return true;
        }
        next = FT_LIST_GET_NEXT(&m_already_done, next);
    }
    return false;
}

bool is_already_to_process(int x, int y)
{
    t_next_pos* next = m_next_pos;
    while (next != NULL)
    {
        if (next->x == x && next->y == y)
        {
            return true;
        }
        next = FT_LIST_GET_NEXT(&m_next_pos, next);
    }
    next = m_already_done;
    while (next != NULL)
    {
        if (next->x == x && next->y == y)
        {
            return true;
        }
        next = FT_LIST_GET_NEXT(&m_already_done, next);
    }
    return false;
}

// 1599
// 1669
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
    FILE* file = fopen("i", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }


    memset(line, 0, sizeof(line));

    fgets(line, sizeof(line), file);
    m_matrix_x = strlen(line);
    if (line[m_matrix_x - 1] == '\n')
        m_matrix_x--;

    int file_fd = fileno(file);
    lseek(file_fd, 0, SEEK_SET);
    size_t file_size = 0;
    file_size = lseek(file_fd, 0, SEEK_END);
    lseek(file_fd, 0, SEEK_SET);
    printf("File size: %zu bytes\n", file_size);
    m_matrix_y = (file_size / (m_matrix_x)); // +1 for newline


    printf("Matrix dimensions: %d x %d\n", m_matrix_x, m_matrix_y);

    m_matrix = malloc(m_matrix_x * m_matrix_y);
    if (m_matrix == NULL)
    {
        perror("Error allocating memory for matrix");
        fclose(file);
        return 1;
    }

    fclose(file);
    file = fopen("i", "r");

    memset(m_matrix, 0, m_matrix_x * m_matrix_y);
    int j = 0;
    while (fgets(line, sizeof(line), file))
    {
        if (line[m_matrix_x] == '\n')
            line[m_matrix_x] = '\0';

        for (i = 0; i < m_matrix_x; i++)
        {
            MATRIX(i, j) = line[i];
        }
        printf("Read line: %s", line);
        printf("Into matrix row %d\n", j);
        j++;
        // m_matrix_y++;
    }

    printf("Matrix dimensions: %d x %d\n", m_matrix_x, m_matrix_y);
    printf("matrix: %s\n", m_matrix);
    printf("Matrix content:\n");
    for (j = 0; j < m_matrix_y; j++)
    {
        for (i = 0; i < m_matrix_x; i++)
        {
            printf("%c", MATRIX(i, j));
        }
        printf("\n");
    }

    t_next_pos* next;
    t_next_pos* prev;
    t_next_pos* new;
    for (i = 0; i < m_matrix_x; i++)
    {
        if (MATRIX(i, 0) == 'S')
        {
            new = malloc(sizeof(t_next_pos));
            new->x = i;
            new->y = 0;
            FT_LIST_ADD_LAST(&m_next_pos, new);
            break;
        }
    }
    printf("Start position found at (%d,0)\n", i);

    next = m_next_pos;
    int x;
    int y;
    uint64_t splits = 0;
    bool splitted_right;
    bool splitted_left;
    while (next != NULL)
    {
        x = next->x;
        y = next->y;
        MATRIX(x, y) = PRINTED_CHAR;
        printf("Next position to process: (%d,%d)\n", x, y);
        prev = next;
        next = FT_LIST_GET_NEXT(&m_next_pos, next);
        FT_LIST_POP(&m_next_pos, prev);
        FT_LIST_ADD_LAST(&m_already_done, prev);
        if (!next)
        {
            m_next_pos = NULL;
        }
        while (1)
        {
            if (MATRIX(x,y) == USED_SPLITTER)
            {
                printf("Position (%d,%d) is already a used splitter, stopping processing this path\n", x, y);
                break;
            }
            printf("Marking position (%d,%d) as printed\n", x, y);
            if ((MATRIX(x, y) == OCCUPIED_CHAR) && !is_already_evaluated(x, y))
            {
                MATRIX(x, y) = USED_SPLITTER;
                printf("splitting at (%d,%d), total splits: %llu\n", x, y, splits);
                splitted_left = false;
                splitted_right = false;
                if (x+1 < m_matrix_x)
                {
                    if (is_already_to_process(x+1, y))
                    {
                        printf("Position (%d,%d) is already scheduled to process, not adding again\n", x+1, y);
                    }
                    else
                    {
                        new = malloc(sizeof(t_next_pos));
                        new->x = x+1;
                        new->y = y;
                        FT_LIST_ADD_LAST(&m_next_pos, new);
                        printf("Added next position (%d,%d)\n", x+1, y);
                        splitted_right = true;
                    }
                }
                if (x-1 >= 0)
                {
                    if (is_already_to_process(x-1, y))
                    {
                        printf("Position (%d,%d) is already scheduled to process, not adding again\n", x-1, y);
                    }
                    else
                    {
                        new = malloc(sizeof(t_next_pos));
                        new->x = x-1;
                        new->y = y;
                        FT_LIST_ADD_LAST(&m_next_pos, new);
                        printf("Added next position (%d,%d)\n", x-1, y);
                        splitted_left = true;
                    }
                }
                // if (splitted_left || splitted_right)
                    splits++;

                // new = malloc(sizeof(t_next_pos));
                // new->x = x-1;
                // new->y = y;
                // FT_LIST_ADD_LAST(&m_next_pos, new);
                printf("Found occupied at (%d,%d), adding to next positions\n", x, y);
                break;
            }
            MATRIX(x, y) = PRINTED_CHAR;
            y++;
            if (y >= m_matrix_y)
            {
                printf("Reached bottom of matrix at (%d,%d), stopping\n", x, y);
                break;
            }
        }
        // while (1);
        if (!next && m_next_pos)
        {
            next = m_next_pos;
        }

    }
end:

    printf("Matrix dimensions: %d x %d\n", m_matrix_x, m_matrix_y);
    printf("matrix: %s\n", m_matrix);
    printf("Matrix content:\n");
    for (j = 0; j < m_matrix_y; j++)
    {
        for (i = 0; i < m_matrix_x; i++)
        {
            printf("%c", MATRIX(i, j));
        }
        printf("\n");
    }
    printf("Total splits encountered: %llu\n", splits);
    exit(1);
    return 0;
}
