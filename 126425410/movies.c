#include <stdio.h>
#include <string.h>

#define MAX_MOVIES 100            // Максимален брой филми
#define FILE_NAME "movies.txt"    // Файл за запис и зареждане

// Структура, която описва един филм
typedef struct {
    int id;            
    char title[50];    
    char genre[30];    
    int year;           
    float rating;       
    int duration;       
} Movie;

// Масив за всички филми
Movie movies[MAX_MOVIES];
int movieCount = 0;// Брояч на текущия брой филми

/* Прототипи на функциите */
void addMovie();       
void viewMovies();      
void searchMovie();     
void sortByDuration();  
void sortByRating();    
void saveToFile();      
void loadFromFile();    

int main() {
    int choice; // Избор от менюто

    loadFromFile(); // Зареждаме филмите от файла при стартиране

    do {
        // Главно меню
        printf("\n=== Movie Database ===\n");
        printf("1. Add movie\n");
        printf("2. View movies\n");
        printf("3. Search by title\n");
        printf("4. Sort movies\n");
        printf("0. Exit\n");
        printf("Choice: ");

        scanf("%d", &choice);
        getchar(); // Премахване на Enter от буфера

        switch (choice) {
            case 1:
                addMovie();   
                saveToFile(); 
                break;
            case 2:
                viewMovies(); 
                break;
            case 3:
                searchMovie();
                break;
            case 4: {
                int s;  // Избор за вида сортиране
                printf("1. By duration\n");
                printf("2. By rating\n");
                printf("Choice: ");
                scanf("%d", &s);
                getchar();

                if (s == 1)
                    sortByDuration();
                else if (s == 2)
                    sortByRating();
                else
                    printf("Invalid choice!\n");

                saveToFile();
                break;
            }
            case 0:
                saveToFile(); // Запис при изход
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid choice!\n");
        }

    } while (choice != 0); // Повтаряме, докато не изберем Exit

    return 0;
}

/* Функция за добавяне на нов филм */
void addMovie() {
    if (movieCount >= MAX_MOVIES) { // Проверка дали има място
        printf("Database is full!\n");
        return;
    }

    Movie m; //Създаване на нов филм
    m.id = movieCount + 1; // Автоматично ID

    printf("Title: ");
    fgets(m.title, sizeof(m.title), stdin);
    m.title[strcspn(m.title, "\n")] = '\0';

    if (strlen(m.title) == 0) {
        printf("Title cannot be empty!\n");
        return;
    }

    printf("Genre: ");
    fgets(m.genre, sizeof(m.genre), stdin);
    m.genre[strcspn(m.genre, "\n")] = '\0';// Премахване на нов ред

    printf("Year: ");
    scanf("%d", &m.year);

    printf("Rating (0-10): ");
    scanf("%f", &m.rating);
    getchar();

    if (m.rating < 0 || m.rating > 10) {
        printf("Invalid rating!\n");
        return;
    }

    printf("Duration (minutes): ");
    scanf("%d", &m.duration);
    getchar();

    if (m.duration <= 0) {
        printf("Invalid duration!\n");
        return;
    }

    movies[movieCount++] = m; // Добавяме филма в масива
    printf("Movie added successfully.\n");
}

/* Функция за показване на всички филми */
void viewMovies() {
    if (movieCount == 0) {
        printf("No movies available.\n");
        return;
    }
 // Обхождаме масива и печатаме всеки филм
    for (int i = 0; i < movieCount; i++) {
        printf("\nID: %d\n", movies[i].id);
        printf("Title: %s\n", movies[i].title);
        printf("Genre: %s\n", movies[i].genre);
        printf("Year: %d\n", movies[i].year);
        printf("Rating: %.1f\n", movies[i].rating);
        printf("Duration: %d minutes\n", movies[i].duration);
    }
}

/* Функция за търсене на филм по заглавие */
void searchMovie() {
    char title[50];
    int found = 0;

    printf("Enter title: ");
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = '\0';

    for (int i = 0; i < movieCount; i++) {
        if (strcmp(movies[i].title, title) == 0) { // Сравняваме заглавията
            printf("Found: %s (%d)\n", movies[i].title, movies[i].year);
            found = 1;
        }
    }

    if (!found)
        printf("Movie not found.\n");
}
/* И двете сортирания са чрез Bubble sort */
/* Сортиране по продължителност */
void sortByDuration() {
    for (int i = 0; i < movieCount - 1; i++)
        for (int j = 0; j < movieCount - i - 1; j++)
            if (movies[j].duration > movies[j + 1].duration) {
                Movie t = movies[j];
                movies[j] = movies[j + 1];
                movies[j + 1] = t;
            }

    printf("Movies sorted by duration.\n");
}

/* Сортиране по рейтинг */
void sortByRating() {
    for (int i = 0; i < movieCount - 1; i++)
        for (int j = 0; j < movieCount - i - 1; j++)
            if (movies[j].rating < movies[j + 1].rating) {
                Movie t = movies[j];
                movies[j] = movies[j + 1];
                movies[j + 1] = t;
            }

    printf("Movies sorted by rating.\n");
}

/* Запис на филмите във файл */
void saveToFile() {
    FILE *f = fopen(FILE_NAME, "w");
    if (!f) return;
// Записваме всеки филм на отделен ред
    for (int i = 0; i < movieCount; i++) {
        fprintf(f, "%d;%s;%s;%d;%.1f;%d\n",
                movies[i].id,
                movies[i].title,
                movies[i].genre,
                movies[i].year,
                movies[i].rating,
                movies[i].duration);
    }

    fclose(f);
}

/* Зареждане на филмите от файл */
void loadFromFile() {
    FILE *f = fopen(FILE_NAME, "r");
    if (!f) return;

    while (fscanf(f, "%d;%49[^;];%29[^;];%d;%f;%d\n", // Четем ред по ред, докато има данни
                  &movies[movieCount].id,
                  movies[movieCount].title,
                  movies[movieCount].genre,
                  &movies[movieCount].year,
                  &movies[movieCount].rating,
                  &movies[movieCount].duration) == 6) {
        movieCount++;
    }

    fclose(f);
}

