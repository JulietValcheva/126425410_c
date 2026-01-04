#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_MOVIES 100             // Максимален брой филми
#define FILE_NAME "movies.txt"     // Името на файла за запис и зареждане

// Структура за филм
typedef struct {
    int id;               // Уникален идентификатор
    char title[50];       // Заглавие
    char genre[30];       // Жанр
    int year;             // Година на издаване
    float rating;         // Рейтинг (0-10)
    int duration;         // Продължителност в минути (>0)
} Movie;

Movie movies[MAX_MOVIES]; // Масив с всички филми
int movieCount = 0;       // Текущ брой филми

/* Прототипи на функциите */
void addMovie();       // Добавяне на нов филм
void viewMovies();     // Показване на всички филми
void searchMovie();    // Търсене на филм по заглавие
void sortByDuration(); // Сортиране по продължителност
void sortByRating();   // Сортиране по рейтинг
void saveToFile();     // Запис във файл
void loadFromFile();   // Зареждане от файл

int main() {
    int choice; // Избор от менюто

    loadFromFile(); // Зареждане на филмите при стартиране

    do {
        // Основно меню
        printf("\n=== Movie Database ===\n");
        printf("1. Add movie\n");
        printf("2. View movies\n");
        printf("3. Search by title\n");
        printf("4. Sort movies\n");
        printf("0. Exit\n");
        printf("Choice: ");

        scanf("%d", &choice);
        getchar(); // премахва остатъчния Enter

        switch (choice) {
            case 1:
                addMovie();      // Добавяне на нов филм
                saveToFile();    // Записване във файла
                break;
            case 2:
                viewMovies();    // Показване на всички филми
                break;
            case 3:
                searchMovie();   // Търсене по заглавие
                break;
            case 4: {
                int sortChoice;
                do {
                    // Подменю за сортиране
                    printf("\n--- Sort Movies ---\n");
                    printf("1. By duration (shortest to longest)\n");
                    printf("2. By rating (highest to lowest)\n");
                    printf("0. Back to main menu\n");
                    printf("Choice: ");
                    scanf("%d", &sortChoice);
                    getchar();

                    switch(sortChoice) {
                        case 1:
                            sortByDuration(); // Сортиране по продължителност
                            saveToFile();     // Записване след сортиране
                            viewMovies();     // Показване на резултата
                            break;
                        case 2:
                            sortByRating();   // Сортиране по рейтинг
                            saveToFile();     // Записване след сортиране
                            viewMovies();     // Показване на резултата
                            break;
                        case 0:
                            break; // Връщане в главното меню
                        default:
                            printf("Invalid choice!\n");
                    }
                } while(sortChoice != 0);
                break;
            }
            case 0:
                saveToFile(); // Запис при изход
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid choice!\n");
        }

    } while (choice != 0); // Повтаряне докато не се избере изход

    return 0;
}

/* ======================= Функции ======================= */

/* Функция за добавяне на нов филм с проверки */
void addMovie() {
    if (movieCount >= MAX_MOVIES) { // Проверка за пълна база
        printf("Database is full!\n");
        return;
    }

    Movie m;
    m.id = movieCount + 1; // Автоматично ID

    // Въвеждане на заглавие (не може да е празно)
    do {
        printf("Title: ");
        fgets(m.title, sizeof(m.title), stdin);
        m.title[strcspn(m.title, "\n")] = '\0';
        if (strlen(m.title) == 0)
            printf("Title cannot be empty!\n");
    } while(strlen(m.title) == 0);

    // Въвеждане на жанр (не може да е празно)
    do {
        printf("Genre: ");
        fgets(m.genre, sizeof(m.genre), stdin);
        m.genre[strcspn(m.genre, "\n")] = '\0';
        if (strlen(m.genre) == 0)
            printf("Genre cannot be empty!\n");
    } while(strlen(m.genre) == 0);

    // Въвеждане на година (между 1800 и текущата)
    int currentYear;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    currentYear = tm.tm_year + 1900;

    do {
        printf("Year (1800-%d): ", currentYear);
        scanf("%d", &m.year);
        getchar();
        if (m.year < 1800 || m.year > currentYear)
            printf("Invalid year!\n");
    } while(m.year < 1800 || m.year > currentYear);

    // Въвеждане на рейтинг (0-10)
    do {
        printf("Rating (0-10): ");
        scanf("%f", &m.rating);
        getchar();
        if (m.rating < 0 || m.rating > 10)
            printf("Invalid rating!\n");
    } while(m.rating < 0 || m.rating > 10);

    // Въвеждане на продължителност (>0)
    do {
        printf("Duration (minutes, >0): ");
        scanf("%d", &m.duration);
        getchar();
        if (m.duration <= 0)
            printf("Invalid duration!\n");
    } while(m.duration <= 0);

    movies[movieCount++] = m; // Добавяне на филма в масива
    printf("Movie added successfully.\n");
}

/* Функция за показване на всички филми */
void viewMovies() {
    if (movieCount == 0) {
        printf("No movies available.\n");
        return;
    }

    for (int i = 0; i < movieCount; i++) {
        printf("\nID: %d\n", movies[i].id);
        printf("Title: %s\n", movies[i].title);
        printf("Genre: %s\n", movies[i].genre);
        printf("Year: %d\n", movies[i].year);
        printf("Rating: %.1f\n", movies[i].rating);
        printf("Duration: %d minutes\n", movies[i].duration);
        printf("------------------------\n");
    }
}

/* Функция за търсене по заглавие */
void searchMovie() {
    char search[50];
    int found = 0;

    printf("Enter title: ");
    fgets(search, sizeof(search), stdin);
    search[strcspn(search, "\n")] = '\0';

    if (strlen(search) == 0) {
        printf("Title cannot be empty!\n");
        return;
    }

    for (int i = 0; i < movieCount; i++) {
        if (strcmp(movies[i].title, search) == 0) {
            printf("\nMovie found:\n");
            printf("Title: %s\n", movies[i].title);
            printf("Genre: %s\n", movies[i].genre);
            printf("Year: %d\n", movies[i].year);
            printf("Rating: %.1f\n", movies[i].rating);
            printf("Duration: %d minutes\n", movies[i].duration);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Movie not found.\n");
}

/* Функция за сортиране по продължителност (най-кратък към най-дълъг) */
void sortByDuration() {
    if (movieCount < 2) { // Проверка дали има достатъчно филми
        printf("Not enough movies to sort.\n");
        return;
    }

    for (int i = 0; i < movieCount - 1; i++) {
        for (int j = 0; j < movieCount - i - 1; j++) {
            if (movies[j].duration > movies[j + 1].duration) {
                Movie temp = movies[j];
                movies[j] = movies[j + 1];
                movies[j + 1] = temp;
            }
        }
    }
    printf("Movies sorted by duration.\n");
}

/* Функция за сортиране по рейтинг (най-висок към най-нисък) */
void sortByRating() {
    if (movieCount < 2) { // Проверка дали има достатъчно филми
        printf("Not enough movies to sort.\n");
        return;
    }

    for (int i = 0; i < movieCount - 1; i++) {
        for (int j = 0; j < movieCount - i - 1; j++) {
            if (movies[j].rating < movies[j + 1].rating) {
                Movie temp = movies[j];
                movies[j] = movies[j + 1];
                movies[j + 1] = temp;
            }
        }
    }
    printf("Movies sorted by rating.\n");
}

/* Функция за запис във файл */
void saveToFile() {
    FILE *f = fopen(FILE_NAME, "w");
    if (!f) {
        printf("File error!\n");
        return;
    }

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

/* Функция за зареждане от файл */
void loadFromFile() {
    FILE *f = fopen(FILE_NAME, "r");
    if (!f) return; // Ако файлът не съществува

    while (movieCount < MAX_MOVIES &&
           fscanf(f, "%d;%49[^;];%29[^;];%d;%f;%d\n",
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
