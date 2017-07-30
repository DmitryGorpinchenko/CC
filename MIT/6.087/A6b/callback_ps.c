#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct student
{
    char fname[100];
    char lname[100];
    int  year;
    int  age;
};

struct student class[] = {
    "Sean", "Penn", 2, 21,
    "Sean", "Connery", 4, 25,
    "Angelina", "Jolie", 3, 22,
    "Meryl", "Streep", 4, 29,
    "Robin", "Williams", 3, 32,
    "Bill", "Gates", 3, 17,
    "Jodie", "Foster", 4, 25,
    "John", "Travolta", 1, 17,
    "Isaac", "Newton", 2, 19,
    "Sarah", "Palin", 2, 19
};

int compare_first_name(const void* a, const void* b)
{
  return strcmp(((struct student*) a)->fname, ((struct student*) b)->fname);
}

int compare_last_name(const void* a, const void* b)
{
  return strcmp(((struct student*) a)->lname, ((struct student*) b)->lname);
}

void apply(void* arr, int n, int size, void (*fp)(void* prec, void* arg), void* arg)
{
    int i = 0;
    for(i = 0; i < n; i++)
    {
      fp((void*)(((char*) arr) + i * size), arg);
    }
}

void printrec(void* prec, void* arg)
{
    struct student* pstud = (struct student*) prec;
    printf("%-20s %-20s %2d %2d\n", pstud->fname, pstud->lname, pstud->year, pstud->age);
}

void isolder(void* prec, void* arg)
{
    struct student* pstud = (struct student*) prec;
    if(pstud->age > *((int*) arg)) 
    {
        printrec(prec, NULL);
    }  
}

int main()
{
    const size = sizeof(struct student);
    const int nstudents = sizeof(class) / size;
    int age;

    puts("Raw records:");
    puts("-------------------------------");  
    apply(class, nstudents, size, printrec, NULL);

    /*sort based on first name*/
    puts("Sorted by first name:");
    puts("-------------------------------");  
    qsort(class, nstudents, size, compare_first_name);
    apply(class, nstudents, size, printrec, NULL);

    /*sort based on last name*/
    puts("Sorted by last name:");
    puts("-------------------------------");  
    qsort(class, nstudents, size, compare_last_name);
    apply(class, nstudents, size, printrec, NULL);

    /*print people older than 20*/
    puts("People older than 20:");
    puts("-------------------------------");  
    age = 20;
    apply(class, nstudents, size, isolder, &age);
    return 0;
}

