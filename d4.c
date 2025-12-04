#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

char *m_matrix;
int m_matrix_x = 0;
int m_matrix_y = 0;

#define MATRIX(x, y) m_matrix[(y)*m_matrix_x + (x)]

#define BLANK_CHAR '.'
#define OCCUPIED_CHAR '@'

bool has_n_empty_surroundings(int x, int y, int n)
{
    int empty_count = 0;
    int dx;
    int dy;
    int nx;
    int ny;

    // printf("Checking position (%d, %d) for at least %d empty surroundings\n", x, y, n);
    for (dy = -1; dy <= 1; dy++)
    {
        for (dx = -1; dx <= 1; dx++)
        {
            if (dx == 0 && dy == 0)
                continue;

            nx = x + dx;
            ny = y + dy;

            // printf("Checking neighbor (%d, %d)\n", nx, ny);
            if (nx < 0 || nx >= m_matrix_x || ny < 0 || ny >= m_matrix_y)
            {
                // printf("Neighbor (%d, %d) is out of bounds, counting as empty\n", nx, ny);
                if (nx == 6 && ny == 0)
                {
                    printf ("%d, %d(%d >= %d), %d, %d\n", nx < 0, nx >= m_matrix_x, nx, m_matrix_x, ny < 0, ny >= m_matrix_y);
                }
                empty_count++;
                if (empty_count >= n)
                    return true;
                continue;
            }

            if (MATRIX(nx, ny) == BLANK_CHAR)
            {
                // printf("Neighbor (%d, %d) is empty\n", nx, ny);
                empty_count++;
                if (empty_count >= n)
                    return true;
            }
        }
    }

    return false;
}

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
    file = fopen("input.txt", "r");

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

    const int n_surroundings = 5;
    printf("\nPositions with at least %d empty surroundings:\n",n_surroundings);
    int count = 0;
#ifdef LEVEL2
level2:
#endif
    for (j = 0; j < m_matrix_y; j++)
    {
        for (i = 0; i < m_matrix_x; i++)
        {
            if ((MATRIX(i,j) == OCCUPIED_CHAR) && has_n_empty_surroundings(i, j, n_surroundings))
            {
                printf("(%d, %d)\n", i, j);
                count++;
#ifdef LEVEL2
                MATRIX(i,j) = BLANK_CHAR;
                goto level2;
#endif
            }
        }
    }
    printf("Total positions found: %d\n", count);
    exit(1);
    return 0;
}
