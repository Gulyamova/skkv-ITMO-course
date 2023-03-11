#include "phonebook.h"
#include "quicksort.h"
#include <cstring>
#include <new>
#ifndef RETURN_H
#define RETURN_H
#include "return_codes.h"
#endif
using namespace std;
int main(int argc, char **argv) {
    if (argc != 3)
    {
        fprintf(stderr, "wrong number of arguments passed to program(\n");
        return ERROR_INVALID_PARAMETER;
    }
    FILE *in = NULL;
    FILE *out = NULL;
    in = fopen(argv[1], "r");
    if (in == NULL)
    {
        fprintf(stderr, "Failed to open input file(\n");
        return ERROR_FILE_NOT_FOUND;
    }
    char type[10] = "";
    char mode[15] = "";
    fscanf(in, "%s\n%s", type, mode);
    size_t size;
    fscanf(in, "%zu", &size);
    if (strcmp(type, "int") == 0)
    {
        int *arr = new (nothrow) int[size];
        if (arr == nullptr)
        {
            fprintf(stderr, "Couldn't allocate memory for array\n");
            fclose(in);
            return ERROR_NOT_ENOUGH_MEMORY;
        }
        for (size_t i = 0; i < size; i++)
        {
            fscanf(in, "%d", &arr[i]);
        }
        fclose(in);
        if (strcmp(mode, "ascending") == 0)
            quickSort<int, false>(arr, (size_t) 0, (size_t) (size - 1));
        else if (strcmp(mode, "descending") == 0)
            quickSort<int, true>(arr, (size_t) 0, (size_t) (size - 1));
        else
        {
            fprintf(stderr, "Unavailable sorting option\n");
            fclose(in);
            return ERROR_INVALID_DATA;
        }
        out = fopen(argv[2], "w");
        for (size_t i = 0; i < size; i++)
        {
            fprintf(out, "%d\n", arr[i]);
        }
        fclose(out);
        delete[] arr;
    }
    else
            if (strcmp(type, "float") == 0)
    {
        float *arr = new (nothrow) float[size];
        if (arr == nullptr)
        {
            fprintf(stderr, "Couldn't allocate memory for array\n");
            fclose(in);
            return ERROR_NOT_ENOUGH_MEMORY;
        }
        for (size_t i = 0; i < size; i++)
        {
            fscanf(in, "%f", &arr[i]);
        }
        fclose(in);
        if (strcmp(mode, "ascending") == 0)
            quickSort<float, false>(arr, (size_t) 0, (size_t) (size - 1));
        else
            if (strcmp(mode, "descending") == 0)
        {
            quickSort<float, true>(arr, (size_t) 0, (size_t) (size - 1));
        }
        else
        {
            fprintf(stderr, "Unavailable sorting option\n");
            fclose(in);
            return ERROR_INVALID_DATA;
        }
        out = fopen(argv[2], "w");
        for (size_t i = 0; i < size; i++)
        {
            fprintf(out, "%g\n", arr[i]);
        }
        fclose(out);
        delete[] arr;
    }
    else
            if (strcmp(type, "phonebook") == 0)
    {
        phonebook *arr = new (nothrow) phonebook[size];
        if (arr == nullptr)
        {
            fprintf(stderr, "Couldn't allocate memory for array\n");
            fclose(in);
            return ERROR_NOT_ENOUGH_MEMORY;
        }
        char surname[25];
        char name[25];
        char patronymic[25];
        long long number;
        for (size_t i = 0; i < size; i++)
        {
            fscanf(in, "%s %s %s %lld", surname, name, patronymic, &number);
            arr[i].set_surname(surname);
            arr[i].set_name(name);
            arr[i].set_patronymic(patronymic);
            arr[i].set_number(number);
        }
        fclose(in);
        if (strcmp(mode, "ascending") == 0)
            quickSort<phonebook, false>(arr, (size_t) 0, (size_t) (size - 1));
        else if (strcmp(mode, "descending") == 0)
            quickSort<phonebook , true>(arr, (size_t) 0, (size_t) (size - 1));
        else
        {
            fprintf(stderr, "Unavailable sorting option\n");
            fclose(in);
            return ERROR_INVALID_DATA;
        }

        out = fopen(argv[2], "w");
        char ans[25];
        for (size_t i = 0; i < size; i++)
        {
            arr[i].get_surname(ans);
            fprintf(out, "%s ", ans);
            arr[i].get_name(ans);
            fprintf(out, "%s ", ans);
            arr[i].get_patronymic(ans);
            fprintf(out, "%s ", ans);
            number = arr[i].get_number();
            fprintf(out, "%lld\n", number);
        }
        fclose(out);
        delete[] arr;
    }
    else
    {
        fprintf(stderr, "Invalid parameter type\n");
        return ERROR_INVALID_PARAMETER;
    }
    return ERROR_SUCCESS;
}
