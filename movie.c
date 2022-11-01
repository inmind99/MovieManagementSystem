#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct date {
    int day, month, year;
}date;

typedef struct director_info{
    char* director_surname, *director_name;
}director_info;

typedef struct movie {
    int id;
    char* title;
    director_info* director;
    date* release_date;
}movie;

int readFile(struct movie**);
void printMovie(struct movie);
void deallocateMemory(struct movie*,int);
void printMenu();
void addNewMovie(struct movie**,int*);
void deleteMovie(struct movie**,int*);
void searchByTitle(struct movie*,int);
void searchByDirector(struct movie*,int);
struct movie* makeCopy(struct movie*,int);
int helperForQSort(const void *, const void *);
int binarySearch(struct movie*,int,int,char*);
void saveToFile(struct movie*,int);

int max=0;

int main()
{
    struct movie* movies;
    int size = readFile(&movies);
    int option=0;

    while(option!=5)
    {
        printMenu();
        scanf("%d",&option);

        switch (option)
        {
        case 0:
            for(int x=0;x<size;x++)
            {
                printMovie(movies[x]);
                printf("\n");
            }
            break;
        case 1:
            getchar();
            addNewMovie(&movies,&size);
            break;
        case 2:
            deleteMovie(&movies,&size);
            break;
        case 3:
            getchar();
            searchByTitle(movies,size);
            break;
        case 4:
            getchar();
            searchByDirector(movies,size);
            break;
        default:
            break;
        }
    }
    
    saveToFile(movies,size);
    deallocateMemory(movies,size);
    return 0;
}

int readFile(struct movie** movies)
{
    FILE* ptr;
    ptr = fopen("movies.txt","r");

    if(ptr == NULL)
    {
        printf("Error!");   
        exit(1);             
    }

    int size;

    fscanf(ptr,"%d", &size);

    *movies = malloc(sizeof(struct movie)*size);

    struct movie* m=*movies;
    char* buffer = malloc(sizeof(char)*100);
    int buffer_size=100;
    char garbage;

    for(int x=0;x<size;x++)
    {
        fscanf(ptr,"%d", &(m[x].id));
        if(max<m[x].id)
        {
            max=m[x].id;
        }
        fscanf(ptr,"%c",&garbage);
        getline(&buffer, (size_t*)&buffer_size, ptr);
        buffer[strlen(buffer)-1]='\0';
        m[x].title=malloc(sizeof(char)*(strlen(buffer) + 1));
        strcpy(m[x].title,buffer);
        m[x].director = malloc(sizeof(struct director_info));
        m[x].release_date = malloc (sizeof(struct date));
        fscanf(ptr,"%s",buffer);
        m[x].director->director_surname = malloc(sizeof(char)*(strlen(buffer) + 1));
        strcpy(m[x].director->director_surname,buffer);
        fscanf(ptr,"%s",buffer);
        m[x].director->director_name = malloc(sizeof(char)*(strlen(buffer)+1));
        strcpy(m[x].director->director_name,buffer);
        fscanf(ptr,"%d", &(m[x].release_date->day));
        fscanf(ptr,"%d", &(m[x].release_date->month));
        fscanf(ptr,"%d", &(m[x].release_date->year));
    }

    fclose(ptr);
    free(buffer);

    return size;
}

void printMovie(struct movie m)
{
    printf("ID: %d\n",m.id);
    printf("Title: %s\n",m.title);
    printf("Director's surname: %s\n",m.director->director_surname);
    printf("Director's name: %s\n",m.director->director_name);
    printf("Release Date: %d %d %d\n",m.release_date->day,m.release_date->month,m.release_date->year);
}

void deallocateMemory(struct movie* movies,int size)
{
    for(int x=0;x<size;x++)
    {
        free(movies[x].title);
        free(movies[x].director->director_surname);
        free(movies[x].director->director_name);
        free(movies[x].director);
        free(movies[x].release_date);
    }
    free(movies);
}

void printMenu()
{
    printf("0. Print all movies\n1. Insert a new movie\n2. Delete a movie based on movie’s id\n3. Search a movie based on movie’s title\n4. Search a movie based on movie’s director\n5. Exit\n");
}

void addNewMovie(struct movie** movies, int* size)
{
    struct movie* m_ = malloc(sizeof(struct movie)*((*size)+1));

    for(int x=0;x<*size;x++)
    {
        m_[x]=(*movies)[x];
    }

    free(*movies);
    *movies=m_;
    (*size)++;

    char* buffer = malloc(sizeof(char)*100);
    int size_=100;

    struct movie* m=malloc(sizeof(struct movie));

    printf("Enter title: ");
    getline(&buffer,(size_t*)&size_,stdin);
    buffer[(strlen(buffer))-1]='\0';
    m->title = malloc(sizeof(char)*(strlen(buffer)+1));
    strcpy(m->title,buffer);

    m->director = malloc(sizeof(struct director_info));
    m->release_date = malloc(sizeof(struct date));
    
    printf("Enter directors surname: ");
    getline(&buffer,(size_t*)&size_,stdin);
    buffer[(strlen(buffer))-1]='\0';
    m->director->director_surname = malloc(sizeof(char)*(strlen(buffer)+1));
    strcpy(m->director->director_surname,buffer);

    printf("Enter directors name: ");
    getline(&buffer,(size_t*)&size_,stdin);
    buffer[(strlen(buffer))-1]='\0';
    m->director->director_name = malloc(sizeof(char)*(strlen(buffer)+1));
    strcpy(m->director->director_name,buffer);

    printf("Enter day of release: ");
    scanf("%d",&(m->release_date->day));

    printf("Enter month of release: ");
    scanf("%d",&(m->release_date->month));

    printf("Enter year of release: ");
    scanf("%d",&(m->release_date->year));

    max++;
    m->id=max;

    (*movies)[(*size)-1] = *m;

    free(buffer);
}

void deleteMovie(struct movie** movies,int* size)
{
    int id;
    printf("Enter id of the movie to be deleted: ");
    scanf("%d",&id);

    bool found=false;

    struct movie* m=*movies;

    for(int x=0;x<*size;x++)
    {
        if(m[x].id==id)
        {
            found=true;
            break;
        }
    }

    if(!found)
    {
        printf("No movie with this id found!\n");
        return;
    }

    struct movie* mvs=malloc(sizeof(struct movie)*((*size)-1));

    for(int x=0,y=0;x<*size;x++)
    {
        if(m[x].id!=id)
        {
            mvs[y]=m[x];
            y++;
        }
    }

    free(m);
    *movies = mvs;
    (*size)--;
}

void searchByTitle(struct movie* movies,int size)
{
    char* buffer = malloc(sizeof(char)*100);
    int b_size=100;
    printf("Enter movie title: ");
    getline(&buffer,(size_t*)&b_size,stdin);
    buffer[strlen(buffer)-1]='\0';

    int index=-1;

    for(int x=0;x<size;x++)
    {
        if(!strcmp(movies[x].title,buffer))
        {
            index=x;
            break;
        }
    }

    if(index==-1)
    {
        printf("Movie not found!\n");
        return;
    }

    printf("\n");
    printMovie(movies[index]);
    printf("\n");
}

struct movie* makeCopy(struct movie* movies,int size)
{
    struct movie* m=malloc(sizeof(struct movie)*size);
    for(int x=0;x<size;x++)
    {
        m[x]=movies[x];
    }

    return m;
}

int helperForQSort(const void* m1, const void* m2)
{
    struct movie* movie1=(struct movie*)m1;
    struct movie* movie2=(struct movie*)m2;
    return strcmp(movie1->director->director_surname,movie2->director->director_surname);
}

int binarySearch(struct movie* movies,int left,int right,char* key)
{
    if(left>right)
    {
        return -1;
    }

    int mid=(left+right)/2;

    int x = strcmp(movies[mid].director->director_surname,key);

    if(x==0)
    {
        return mid;
    }
    else if (x<0)
    {
        return binarySearch(movies,mid+1,right,key);
    }
    else
    {
        return binarySearch(movies,left,mid-1,key);
    }
}

void searchByDirector(struct movie* movies,int size)
{
    struct movie* m=makeCopy(movies,size);
    qsort(m,size,sizeof(struct movie),&helperForQSort);

    char buffer[100];
    printf("Enter directors surname: ");
    scanf("%s",buffer);

    int index = binarySearch(m,0,size-1,buffer);

    printf("\n");
    if(index == -1)
    {
        printf("Movie not found!\n");
        printf("\n");
        return;
    }

    for(;index>0;index--)
    {
        if(strcmp(m[index-1].director->director_surname,buffer))
        {
            break;
        }
    }

    for(;index<size;index++)
    {
        if(strcmp(m[index].director->director_surname,buffer))
        {
            break;
        }
        printMovie(m[index]);
        printf("\n");
    }
}

void saveToFile(struct movie* movies,int size)
{
    FILE* ptr=fopen("movies.txt","w");
    
    fprintf(ptr,"%d",size);

    for(int x=0;x<size;x++)
    {
        fprintf(ptr,"\n%d\n",movies[x].id);
        fprintf(ptr,"%s\n",movies[x].title);
        fprintf(ptr,"%s\n",movies[x].director->director_surname);
        fprintf(ptr,"%s\n",movies[x].director->director_name);
        fprintf(ptr,"%d\n",movies[x].release_date->day);
        fprintf(ptr,"%d\n",movies[x].release_date->month);
        fprintf(ptr,"%d",movies[x].release_date->year);
    }

    fclose(ptr);
}