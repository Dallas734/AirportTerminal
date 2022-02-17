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

    char hourDeparture[2];
    char minuteDeparture[2];

    char timeDeparture[10];

    char hourArrival[2];
    char minuteArrival[2];

    char timeArrival[10];

    int countOfLeavers;
    char leavePoint[20];

    Airplane airplane;

    Passenger passenger;
};

FILE* f;
Flight flight;

int input();

void view();

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
            input();
            break;

        case 2:
            system("cls");
            view();
            break;

        }
    } while (key != 3);

    system("pause");
    return 0;
}


//Функция ввода
int input()
{
    int count = 0, count1 = 0;
    int a, b;
    bool checkInputValidation = true, checkStructuresInFile = false, checkSeats = false;
    char alph[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L' }; //массив букв для места
    char sym;
    f = NULL;

    Airplane airplaneOne("Boeing 737", 28, 6);
    char airplaneOneSaloon[28][6]{ 0 };
    for (int i = 0; i < airplaneOne.numberOfRows; i++)
    {
        for (int j = 0; j < airplaneOne.numberOfSeats; j++)
        {
            airplaneOneSaloon[i][j] = 0;
        }
    }


    Airplane airplaneTwo("Airbus A380", 25, 6);
    char airplaneTwoSaloon[25][6]{ 0 };

    Airplane airplaneThree("Boeing 767", 45, 8);
    char airplaneThreeSaloon[45][8]{ 0 };

    //считаем сколько структур было записано в файл
    if (fopen_s(&f, "Airport.dat", "rb") == 0) {
        fseek(f, 678, SEEK_SET);
        while (fread(&flight, sizeof(Flight), 1, f)) {
            count1 += 1;
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
        scanf_s("%d", &a);

        //если в файле были рейсы, считываем салоны самолетов с заполненными местами
        if (checkStructuresInFile == true && a == 2) {
            fseek(f, 0, SEEK_SET);
            fread(airplaneOneSaloon, 168, 1, f);
            fread(airplaneTwoSaloon, 150, 1, f);
            fread(airplaneThreeSaloon, 360, 1, f);
        }

        rewind(stdin);

        //выбор, связанный с диалогом
        switch (a) {
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


    if (a != 3) {                          //заполнение нового рейса
        do {
            printf("Введите самолет ");
            printf("\n1 - Boeing 737, 2 - Airbus A380, 3 - Boeing 767\n");
            scanf_s("%d", &b);

            switch (b) {                       //все данные созданного самолета, переходят в самолет, записанный в рейсе
            case 1:
                flight.airplane = airplaneOne;
                break;
            case 2:
                flight.airplane = airplaneTwo;
                break;
            case 3:
                flight.airplane = airplaneThree;
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

            printf("Введите место пассажира в формате ряд_место(напр. 5А ЛАТИНИЦЕЙ!): \n");//ввод места
            while (checkInputValidation || checkSeats) {
                scanf_s("%d%c", &(flight.passenger.row), &(flight.passenger.seat));
                checkSeats = false;
                for (int i = 0; i < flight.airplane.numberOfSeats; i++) {
                    if (flight.passenger.seat == alph[i]) {
                        //каждый break нужен для switch () и для корректного отображения типа ошибки 
                        switch (b) {
                        case 1:
                            if (flight.passenger.row <= airplaneOne.numberOfRows)
                            {
                                checkSeats = true;
                                checkInputValidation = false;
                                if (airplaneOneSaloon[flight.passenger.row - 1][i] == NULL)
                                {
                                    airplaneOneSaloon[flight.passenger.row - 1][i] = flight.passenger.seat;
                                    checkSeats = false;
                                    break;
                                }
                                break;
                            }
                            checkInputValidation = true;
                            break;
                        case 2:
                            if (flight.passenger.row <= airplaneTwo.numberOfRows)
                            {
                                checkSeats = true;
                                checkInputValidation = false;
                                if (airplaneTwoSaloon[flight.passenger.row - 1][i] == NULL)
                                {
                                    airplaneTwoSaloon[flight.passenger.row - 1][i] = flight.passenger.seat;
                                    checkSeats = false;
                                    break;
                                }
                                break;
                            }
                            checkInputValidation = true;
                            break;
                        case 3:
                            if (flight.passenger.row <= airplaneThree.numberOfRows)
                            {
                                checkSeats = true;
                                checkInputValidation = false;
                                if (airplaneThreeSaloon[flight.passenger.row - 1][i] == NULL)
                                {
                                    airplaneThreeSaloon[flight.passenger.row - 1][i] = flight.passenger.seat;
                                    checkSeats = false;
                                    break;
                                }
                                break;
                            }
                            checkInputValidation = true;
                            break;
                        }


                    }

                    if (checkSeats)
                    {
                        printf("Место занято! Выберетие другое:\n");
                        break;
                    }
                }
                if (checkInputValidation) printf("Место введено неправильно! Выберите другое:\n");
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
            fwrite(airplaneOneSaloon, 1, 168, f);               //запись сначала салонов, а потом рейса
            fwrite(airplaneTwoSaloon, 1, 150, f);
            fwrite(airplaneThreeSaloon, 1, 360, f);
            fseek(f, 678 + sizeof(Flight) * count1, SEEK_SET);
            fwrite(&flight, sizeof(flight), 1, f);

            count++;
            rewind(stdin);

            printf("Продолжить ввод? (д/н)\n");
            scanf_s("%c", &sym);
        } while (sym == 'д' || sym == 'Д');


    }
    system("cls");

    fclose(f);

    return count;

}

//функция вывода информации в консоль
void view() {                                   

    int count = 0, i = 0;
    fopen_s(&f, "Airport.dat", "rb");
    if (!f) {
        printf("Файл не найден!\n");
    }
    else {
        printf("База данных пассажиров и рейсов\n");
        fseek(f, 678, SEEK_SET);
        while (fread(&flight, sizeof(Flight), 1, f)) {
            i += 1;
            printf("================== % d ==================\n", i);
            printf("Самолет: %s\n", flight.airplane.name);
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


