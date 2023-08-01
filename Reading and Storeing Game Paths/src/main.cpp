#include "headers/headers.h"
#include "headers/GameManager.h"

int main()
{
	GameManager GameManager;
	GameManager.setReserveSize(100);

	int choice;

	do
	{
		std::cout << "Menu:" << std::endl;
		std::cout << "1. Add Game" << std::endl;
		std::cout << "2. View All Games" << std::endl;
		std::cout << "3. Open a Game" << std::endl;
		std::cout << "4. Clear Games" << std::endl;
		std::cout << "5. Load Save File" << std::endl;
		std::cout << "6. Exit" << std::endl;
		std::cout << "Enter Choice (1/2/3/4/5/6)" << std::endl;


		if (std::cin >> choice)
		{
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore remaining input after reading choice

			switch (choice)
			{
			case 1:
			{
				std::system("cls");
				std::string name, path, args;

				std::cout << "Enter Game Name: ";
				std::getline(std::cin, name);
				std::cout << "Enter Game Path: ";
				std::getline(std::cin, path);
				std::cout << "Any Command Arguments: ";
				std::getline(std::cin, args);
				GameManager.addGame(path, name, args);
				std::system("cls");
				GameManager.loadFromFile();
				break;
			}
			case 2: 
			{
				std::system("cls");
				GameManager.printGames();
				break;
			}
			case 3:
			{
				std::string name;
				std::cout << "\nEnter Game: \n> ";
				std::getline(std::cin, name);
				bool status = GameManager.openGame(name);
				if (status)
				{
					choice = 6;
					break;
				}
				std::cout << "Press Enter to continue...";
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::system("cls");
				break;
			}
			case 4:
			{
				std::system("cls");
				GameManager.deleteGames();
				break;
			}
			case 5:
			{
				std::system("cls");
				GameManager.loadFromFile();
				GameManager.loopThroughSaveArray();

				break;
			}
			default:
				std::system("cls");
				std::cin.clear();
			}
		}
		else
		{
			std::system("cls");
			std::cout << "Invalid input. Please enter a valid choice." << std::endl;
			std::cin.clear(); // Clear the stream's error state
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore remaining input
		}

	} while (choice != 6);

	return 0;
};