#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cstdio>
#include <clocale>
#include <windows.h>

using namespace std;

struct Airplane {                                   

    char name[20];
    int numberOfRows;
    int numberOfSeats;
    char** saloon;

    Airplane()
    {
    };

    Airplane (const char inputName[], int inputNumberOfRows, int inputNumberOfSeats)
    {
        strcpy_s(name, inputName);
        numberOfRows = inputNumberOfRows;
        numberOfSeats = inputNumberOfSeats;
    }

    //метод, возвращающий все кол-во мест
    int GetAllSeats()
    {
       return numberOfRows * numberOfSeats;
    }

    //метод, создающий салон самолета
    void createSaloon(int countOfRows, int countOfSeats)
    {
        char** rows = new char* [countOfRows];
        for (int i = 0; i < countOfRows; i++)
        {
            rows[i] = new char[countOfSeats];
        }

        saloon = rows;
    }
};


struct Passenger {                                  

    char name[50];                   
    int baggageWeight;
    int children;
    int row;                        
    char seat;

};
    
struct Flight {                                      

    int number;
    char destination[50];
    char stopoverPoint[50];

    char timeDeparture[10];

    char timeArrival[10];

    int countOfLeavers;
    char leavePoint[20];

    char nameOfAirplane[20]; 

    Passenger passenger;
};

FILE* f;
Flight flight;

void Input();

void View();

void WriteInFile(char** saloon, int numberOfRows, int numberOfSeats, FILE* f);

void ReadFromFile(char** saloon, int numberOfRows, int numberOfSeats, FILE* f);

//функция, реализующая выбор места в салоне
int FillSaloon(bool& checkSeats, bool& checkInputValidation, int numberOfSeats, char alphabet[], int selectedAirplaneKey, Airplane airplaneOne, Airplane airplaneTwo, Airplane airplaneThree, Flight flight);

//составляющая FillSaloon(), проверяющая корректность ввода и вакантность места
int FillSaloonChecker(Flight flight, Airplane airplane, bool& checkSeats, bool& checkInputValidation, int i);

//memset массива-салона
void ClearSaloon(Airplane airplane);

int main()
{
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    int key;
    
    do {
        printf("ТЕРМИНАЛ УПРАВЛЕНИЯ АЭРОПОРТОМ\n\n");
        printf("Здравствуйте! Треминал готов к работе. Выберите операцию...\n");
        printf("1 - Ввод данных\n");
        printf("2 - Просмотр данных\n");
        printf("3 - Закрыть терминал\n");
        scanf_s("%d", &key);

        switch (key) {
        case 1:
            system("cls");
            Input();
            break;

        case 2:
            system("cls");
            View();
            break;

        }
    } while (key != 3);

    system("pause");
    return 0;
}


//Функция ввода
void Input()
{
    int countStructures = 0;
    int selectedMenuKey, selectedAirplaneKey;
    int numberOfSeats;
    bool checkInputValidation = true, checkStructuresInFile = false, checkSeats = false;
    char alphabet[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L' }; //массив букв для места
    char sym;
    f = NULL;

    Airplane airplaneOne("Boeing 737", 28, 6);
    airplaneOne.createSaloon(28, 6);
    ClearSaloon(airplaneOne);

    Airplane airplaneTwo("Airbus A380", 25, 6);
    airplaneTwo.createSaloon(25, 6);
    ClearSaloon(airplaneTwo);

    Airplane airplaneThree("Boeing 767", 45, 8);
    airplaneThree.createSaloon(45, 8);
    ClearSaloon(airplaneThree);

    //считаем сколько структур было записано в файл
    if (fopen_s(&f, "Airport.dat", "rb") == 0) {
        fseek(f, 678, SEEK_SET);
        while (fread(&flight, sizeof(Flight), 1, f)) {
            countStructures++;
            checkStructuresInFile = true;
        }
        fclose(f);
    }

    //начало диалога с пользователем
    if (fopen_s(&f, "Airport.dat", "rb+") != 0) {
        printf("Файл не найден. Создается новый...\n");
        fopen_s(&f, "Airport.dat", "wb+");
    }
    else {
        printf("Файл найден. Выберите операцию...\n");
        printf("1 - создать новый файл\n");
        printf("2 - добавить данные уже в существующий\n");
        printf("3 - выйти из операции ввода\n");
        scanf_s("%d", &selectedMenuKey);

        //если в файле были рейсы, считываем салоны самолетов с заполненными местами
        if (checkStructuresInFile == true && selectedMenuKey == 2)
        {
            fseek(f, 0, SEEK_SET);
            ReadFromFile(airplaneOne.saloon, airplaneOne.numberOfRows, airplaneOne.numberOfSeats, f);
            ReadFromFile(airplaneTwo.saloon, airplaneTwo.numberOfRows, airplaneTwo.numberOfSeats, f);
            ReadFromFile(airplaneThree.saloon, airplaneThree.numberOfRows, airplaneThree.numberOfSeats, f);
        }

        rewind(stdin);

        //выбор, связанный с диалогом
        switch (selectedMenuKey)
        {
        case 1:
            fclose(f);
            fopen_s(&f, "Airport.dat", "wb+");
            break;
        case 2:
            fclose(f);
            fopen_s(&f, "Airport.dat", "rb+");
            break;
        case 3:
            fclose(f);
            break;
        }
    }

    //заполнение нового рейса
    if (selectedMenuKey != 3) {                          
        do {
            printf("Введите самолет ");
            printf("\n1 - Boeing 737, 2 - Airbus A380, 3 - Boeing 767\n");
            scanf_s("%d", &selectedAirplaneKey);

            //запись количества сидений 
            switch (selectedAirplaneKey) {                       
            case 1:
                strcpy_s(flight.nameOfAirplane, airplaneOne.name);
                numberOfSeats = airplaneOne.numberOfSeats;
                break;
            case 2:
                strcpy_s(flight.nameOfAirplane, airplaneTwo.name);
                numberOfSeats = airplaneTwo.numberOfSeats;
                break;
            case 3:
                strcpy_s(flight.nameOfAirplane, airplaneThree.name);
                numberOfSeats = airplaneThree.numberOfSeats;
                break;
            }

            printf("Введите номер рейса\n");
            scanf_s("%d", &(flight.number));

            printf("Введите ФИО пассажира:\n");
            rewind(stdin);
            gets_s(flight.passenger.name);

            rewind(stdin);

            printf("Введите вес багажа:\n ");
            scanf_s("%d", &(flight.passenger.baggageWeight));

            while (checkInputValidation || checkSeats)
            {
                printf("Введите место пассажира в формате ряд_место(напр. 5А ЛАТИНИЦЕЙ!): \n");
                scanf_s("%d%c", &flight.passenger.row, &flight.passenger.seat);
                FillSaloon(checkSeats, checkInputValidation, numberOfSeats, alphabet, selectedAirplaneKey, airplaneOne, airplaneTwo, airplaneThree, flight);
            }
            checkInputValidation = true;

            printf("Введите количество детей:\n");
            scanf_s("%d", &(flight.passenger.children));

            printf("Введите место назначения:\n");
            rewind(stdin);
            gets_s(flight.destination);

            printf("Введите пункт промежуточной посадки:\n");
            rewind(stdin);
            gets_s(flight.stopoverPoint);

            printf("Введите время вылета в форамте ч : мин: \n");
            rewind(stdin);
            gets_s(flight.timeDeparture);


            printf("Введите время прибытия в формате ч : мин: \n");
            rewind(stdin);
            gets_s(flight.timeArrival);
 

            fflush(f);
            rewind(f);

            WriteInFile(airplaneOne.saloon, airplaneOne.numberOfRows, airplaneOne.numberOfSeats, f);
            WriteInFile(airplaneTwo.saloon, airplaneTwo.numberOfRows, airplaneTwo.numberOfSeats, f);
            WriteInFile(airplaneThree.saloon, airplaneThree.numberOfRows, airplaneThree.numberOfSeats, f);
          
            fseek(f, 678 + sizeof(Flight) * countStructures, SEEK_SET);
            fwrite(&flight, sizeof(flight), 1, f);
            rewind(stdin);

            printf("Продолжить ввод? (д/н)\n");
            scanf_s("%c", &sym);
        } while (sym == 'д' || sym == 'Д');


    }
    system("cls");

    fclose(f);
}

//функция вывода информации в консоль
void View() {                                   

    int count = 0, i = 0;
    fopen_s(&f, "Airport.dat", "rb");
    if (!f) {
        printf("Файл не найден!\n");
    }
    else {
        printf("База данных пассажиров и рейсов\n");
        fseek(f, 678, SEEK_SET); //сделать функцию автоматизирующу рассчет 678
        while (fread(&flight, sizeof(Flight), 1, f)) {
            i += 1;
            printf("================== % d ==================\n", i);
            printf("Самолет: %s\n", flight.nameOfAirplane);
            printf("ФИО пассажира: %s\n", flight.passenger.name);
            printf("Вес багажа: %d\n", flight.passenger.baggageWeight);
            printf("Место пассажира: %d%c\n", flight.passenger.row, flight.passenger.seat);
            printf("Количество детей: %d\n", flight.passenger.children);
            printf("Место назначения: %s\n", flight.destination);
            printf("Пункт промежуточной посадки: %s\n", flight.stopoverPoint);
            printf("Время вылета: %s\n", flight.timeDeparture);
            printf("Время прибытия: %s\n", flight.timeArrival);
        }
        printf("=========================================\n");
        fclose(f);
    }
    system("pause");

    system("cls");

}

void ClearSaloon(Airplane airplane) 
{
    for (int i = 0; i < airplane.numberOfRows; i++)
    {
        memset(airplane.saloon[i], 0, sizeof(airplane.saloon[i]));
    }
}


void WriteInFile(char** saloon, int numberOfRows, int numberOfSeats, FILE* f)
{
    
    for (int i = 0; i < numberOfRows; i++)
    {
        for (int j = 0; j < numberOfSeats; j++)
        {
            fwrite(&(saloon[i][j]), sizeof(saloon[i][j]), 1, f);
        }
    }
}

void ReadFromFile(char** saloon, int numberOfRows, int numberOfSeats, FILE* f)
{
    for (int i = 0; i < numberOfRows; i++)
    {
        for (int j = 0; j < numberOfSeats; j++)
        {
            fread(&(saloon[i][j]), sizeof(saloon[i][j]), 1, f);
        }
    }
}

int FillSaloon(bool &checkSeats, bool &checkInputValidation, int numberOfSeats, char alphabet[], int selectedAirplaneKey, Airplane airplaneOne, Airplane airplaneTwo, Airplane airplaneThree, Flight flight)
{    
    checkSeats = false;
    for (int i = 0; i < numberOfSeats; i++) {
        if (flight.passenger.seat == alphabet[i]) {
            //каждый break нужен для switch () и для корректного отображения типа ошибки 
            switch (selectedAirplaneKey) {
            case 1:
                FillSaloonChecker(flight, airplaneOne, checkSeats, checkInputValidation, i);
                break;
            case 2:
                FillSaloonChecker(flight, airplaneOne, checkSeats, checkInputValidation, i);
                break;
            case 3:
                FillSaloonChecker(flight, airplaneOne, checkSeats, checkInputValidation, i);
                break;
            }
        }
    }

    if (checkSeats)
    {
        printf("Место занято! Выберетие другое:\n");
        return 0;
    }
        
    if (checkInputValidation)
    {
        printf("Место введено неправильно! Выберите другое:\n");
        return 0;
    }
    
    return 1;
}

int FillSaloonChecker (Flight flight, Airplane airplane, bool &checkSeats, bool &checkInputValidation, int i)
{
    if (flight.passenger.row <= airplane.numberOfRows)
    {
        checkSeats = true;
        checkInputValidation = false;
        if (airplane.saloon[flight.passenger.row - 1][i] == NULL)
        {
            airplane.saloon[flight.passenger.row - 1][i] = flight.passenger.seat;
            checkSeats = false;
            return 1;
        }
        return 0;
    }
    checkInputValidation = true;
    return -1;
}
