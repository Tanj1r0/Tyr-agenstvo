#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <Windows.h>
#include <locale>

class Tour {
private:
    std::string destination;
    std::string startDate;
    std::string duration;
    std::string availableSeats;
    std::string price;

public:
    Tour(const std::string& destination, const std::string& startDate, const std::string& duration,
        const std::string& availableSeats, const std::string& price)
        : destination(destination), startDate(startDate), duration(duration),
        availableSeats(availableSeats), price(price) {}
    std::string getDestination() const {
        return destination;
    }

    std::string getStartDate() const {
        return startDate;
    }

    std::string getDuration() const {
        return duration;
    }

    std::string getAvailableSeats() const {
        return availableSeats;
    }

    std::string getPrice() const {
        return price;
    }

    bool operator<(const Tour& other) const {
        return destination < other.destination;
    }

    bool operator>(const Tour& other) const {
        return price > other.price;
    }

    bool operator==(const Tour& other) const {
        return startDate == other.startDate;
    }
    void display() const {
        std::cout << "Направление: " << destination << std::endl;
        std::cout << "Дата начала: " << startDate << std::endl;
        std::cout << "Продолжительность: " << duration << " дней" << std::endl;
        std::cout << "Доступные места: " << availableSeats << std::endl;
        std::cout << "Цена: " << price << " рублей" << std::endl;
    }

    bool containsSubstring(const std::string& substring) const {
        std::string lowerCaseData = destination + startDate + duration +
            availableSeats + price;

        std::string lowerCaseSubstring = substring;
        std::transform(lowerCaseData.begin(), lowerCaseData.end(), lowerCaseData.begin(),
            [](unsigned char c) -> char { return std::tolower(c, std::locale("")); });
        std::transform(lowerCaseSubstring.begin(), lowerCaseSubstring.end(), lowerCaseSubstring.begin(),
            [](unsigned char c) -> char { return std::tolower(c, std::locale("")); });

        return lowerCaseData.find(lowerCaseSubstring) != std::string::npos;
    }


};

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::vector<Tour> tours;

    std::ifstream inputFile("tours_data.txt");
    if (inputFile.is_open()) {
        std::string line;
        while (std::getline(inputFile, line)) {
            std::stringstream ss(line);
            std::string destination, startDate, duration, availableSeats, price;

            std::getline(ss, destination, ',');
            std::getline(ss, startDate, ',');
            std::getline(ss, duration, ',');
            std::getline(ss, availableSeats, ',');
            std::getline(ss, price, ',');

            tours.emplace_back(destination, startDate, duration, availableSeats, price);
        }
        inputFile.close();
    }
    else {
        std::cout << "Ошибка открытия файла tours_data.txt" << std::endl;
        return 1;
    }

    int choice = 0;
    while (choice != 5) {
        std::cout << "1. Поиск тура\n";
        std::cout << "2. Сортировка туров\n";
        std::cout << "3. Вывод всех туров\n";
        std::cout << "4. Добавить тур\n";
        std::cout << "5. Выход\n";
        std::cout << "Выберите пункт меню: ";

        int choice;
        std::cin >> choice;

        if (choice == 1) {
            std::string searchQuery;
            std::cout << "Введите запрос для поиска: ";
            std::cin >> searchQuery;

            bool found = false;
            for (const auto& tour : tours) {
                if (tour.containsSubstring(searchQuery)) {
                    tour.display();
                    found = true;
                }
            }

            if (!found) {
                std::cout << "Туры с такими данными не найдены.\n";
            }
        }
        else if (choice == 2) {
            int sortChoice;
            std::cout << "Выберите критерий сортировки:\n";
            std::cout << "1. По направлению\n";
            std::cout << "2. По цене\n";
            std::cout << "3. По дате начала\n";
            std::cout << "Ваш выбор: ";
            std::cin >> sortChoice;

            switch (sortChoice) {
            case 1:
                std::sort(tours.begin(), tours.end());
                std::cout << "Туры отсортированы по направлению.\n";
                break;
            case 2:
                std::sort(tours.begin(), tours.end(), std::greater<Tour>());
                std::cout << "Туры отсортированы по цене (по убыванию).\n";
                break;
            case 3:
                std::sort(tours.begin(), tours.end(), [](const Tour& a, const Tour& b) {
                    return a.getStartDate() < b.getStartDate();
                    });
                std::cout << "Туры отсортированы по дате начала.\n";
                break;
            default:
                std::cout << "Неверный выбор. Пожалуйста, попробуйте снова.\n";
                break;
            }
        }
        else if (choice == 3) {
            for (const auto& tour : tours) {
                tour.display();
                std::cout << "--------------------------\n";
            }
        }
        else if (choice == 4) {
            std::string destination, startDate, duration, availableSeats, price;
            std::cin.ignore();

            std::cout << "Введите направление тура: ";
            std::getline(std::cin, destination);
            std::cout << "Введите дату начала тура: ";
            std::getline(std::cin, startDate);
            std::cout << "Введите продолжительность тура (в днях): ";
            std::getline(std::cin, duration);

            std::cout << "Введите количество доступных мест: ";
            std::getline(std::cin, availableSeats);

            std::cout << "Введите цену тура: ";
            std::getline(std::cin, price);

            Tour newTour(destination, startDate, duration, availableSeats, price);
            tours.push_back(newTour);
            std::cout << "Тур добавлен.\n";



            std::ofstream outputFile("tours_data.txt", std::ios::out | std::ios::app);
            if (outputFile.is_open()) {
                // Запись данных нового тура в файл
                outputFile << newTour.getDestination() << "," << newTour.getStartDate() << ","
                    << newTour.getDuration() << "," << newTour.getAvailableSeats() << "," << newTour.getPrice() << "\n";
                outputFile.close();
                std::cout << "Тур добавлен и данные обновлены в файле.\n";
            }
            else {
                std::cout << "Ошибка открытия файла для записи.\n";
            }
        }
        else if (choice == 5) {
            break;
        }
        else {
            std::cout << "Неверный выбор. Пожалуйста, попробуйте снова.\n";
        }
        system("pause");
        system("cls");  // Очистка консоли для следующего вывода меню
    }


    return 0;
}
