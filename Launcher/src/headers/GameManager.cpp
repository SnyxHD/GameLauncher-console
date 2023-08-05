#include "GameManager.h"

// Constructor, we call checkForDir() so it checks if the given directory exist everytime the object is created
GameManager::GameManager() { checkForDir(); };

// Deconstructor for class, called when the class goes out of scope
GameManager::~GameManager() { std::cout << "Destroyed Object" << std::endl; };

std::string config_path = "config/Game_Paths.json";

// Add a new game path to the JSON file
void GameManager::addGame(const std::string& path, const std::string& name, const std::string& args)
{
    json jsonData = json::object();
    std::fstream jsonFile(config_path); // Use the more generalized fstream so the variable can be reused for reading and writing

    // Check if the file is open and valid
    if (jsonFile.is_open()) {
        // Read the content of the file into a string
        std::string jsonString((std::istreambuf_iterator<char>(jsonFile)), std::istreambuf_iterator<char>());

        // Check if the file is empty
        if (!jsonString.empty()) {
            // Attempt to parse the content as JSON
            try {
                jsonData = json::parse(jsonString);
                std::cout << "Successfully loaded config file." << std::endl;
            }
            catch (const json::parse_error& e) {
                std::cerr << "Error parsing JSON: " << e.what() << std::endl;
                return;
            }
        }

        jsonFile.close();
    }

    // Open config file as write
    jsonFile.open(config_path, std::ios::out | std::ios::trunc);

    if (jsonFile.is_open()) {
        // Add new game to jsonData
        jsonData[name] = {
            { "game_path", path },
            { "command_args", args}
        };

        // Dump jsonData into config file
        jsonFile << jsonData.dump(4);
        jsonFile.close();

        setReserveSize();
        loadFromFile();
        return;
    }

    std::cout << "Could not open config file." << std::endl;
    return;
}

// Stores all the Data From the JSON file into a vector array of type JSON
void GameManager::loadFromFile()
{
    // TODO: Load All The Games from Json file into a std::vector<>

    _games.clear(); // Clear the existing games vector before loading from file.

    try
    {
        json jsonData;
        std::ifstream file("config/Game_Paths.json"); // Input file stream to read JSON data.

        // Check if the file is empty or not readable
        if (file.peek() == std::ifstream::traits_type::eof())
        {
            std::cout << "File Empty Or Not Readable!" << std::endl;
            return; // Early return as there's nothing to read from the file.
        }

        // Parse the JSON data from the file
        file >> jsonData;
        file.close();

        // Check if the JSON data is an array
        if (jsonData.is_object())
        {
            // Iterate through the array and populate the _games vector
            for (auto it = jsonData.begin(); it != jsonData.end(); ++it)
            {
                const std::string& gameName = it.key();
                const json& gameData = it.value();

                json gameEntry;
                gameEntry["game_name"] = gameName;
                gameEntry["game_path"] = gameData.value("game_path", "");
                gameEntry["command_args"] = gameData.value("command_args", "");
                _games.emplace_back(gameEntry);
            }
        }
        else
        {
            std::cout << "JSON data is not an object." << std::endl;
            return; // Early return as the data format is incorrect.
        }
    }
    catch (const json::parse_error& e)
    {
        std::cout << "JSON Parse Error: " << e.what() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}

// Print The List of Games Stored in the JSON file.
void GameManager::printGames()
{
    json jsonData;
    std::fstream jsonFile(config_path); // Use the more generalized fstream so the variable can be reused for reading and writing

    // Check if the file is open and valid
    if (jsonFile.is_open()) {
        // Read the content of the file into a string
        std::string jsonString((std::istreambuf_iterator<char>(jsonFile)), std::istreambuf_iterator<char>());

        // Check if the file is empty
        if (!jsonString.empty()) {
            // Attempt to parse the content as JSON
            try {
                jsonData = json::parse(jsonString);
            }
            catch (const json::parse_error& e) {
                std::cerr << "Error parsing JSON: " << e.what() << std::endl;
                return;
            }
        }

        for (auto i = jsonData.begin(); i != jsonData.end(); ++i)
        {
            std::cout << i.key() << std::endl;
        }
        std::cout << "--------------------------------------" << std::endl;

        jsonFile.close();
    }
}

void GameManager::loopThroughSaveArray()
{
    for (int i = 0; i < _games.size(); i++)
    {
        std::cout << _games[i] << "\n" << std::endl;
    }
}

// Opens Path From JSON file
// TODO: Handle empty paths, and throw and exception if the file is not an .exe
bool GameManager::openGame(const std::string& gamename)
{
    try
    {
        // TODO: Open File containing all saved games and search for game name and open it
        json fileData;
        std::ifstream file("config/Game_Paths.json");

        file >> fileData;
        if (fileData.find(gamename) == fileData.end())
        {
            std::cout << "Game Not Found!" << std::endl;
            return false;
        }

        const std::string tempArgs = fileData[gamename].value("command_args", "");
        const std::string tempPath = fileData[gamename].value("game_path", "");

        if (tempPath.empty())
        {
            std::cout << "Game Path not specified for '" << gamename << "'" << std::endl;
            return false;
        }

        const wchar_t* convertedArgs = convertStrToLPCWSTR(tempArgs); // Converts std::string to type const wchar_t* | LPCWSTR
        const wchar_t* convertedPath = convertStrToLPCWSTR(tempPath); // Converts std::string to type const wchar_t* | LPCWSTR


        //ShellExecute(NULL, L"open", convertedPath, NULL, NULL, SW_SHOWNORMAL);

        ShellExecute(NULL, L"open", convertedPath, convertedArgs, NULL, SW_SHOWNORMAL);

        if (!tempArgs.empty())
        {
            std::wcout << "Opening " << convertedPath << " " << convertedArgs << ".... " << std::endl;
        }
        else
        {
            std::wcout << "Opening " << convertedPath << ".... " << std::endl;
        }

        delete[] convertedPath;
        delete[] convertedArgs;
        return true;
    }
    catch (const json::parse_error& e)
    {
        std::cout << "JSON Parse Error: " << e.what() << std::endl;
        return false;
    }
    catch (const json::type_error& e)
    {
        std::cout << "JSON Type Error: " << e.what() << std::endl;
        std::cout << "Game '" << gamename << "' Probably wasnt added correctly" << std::endl;
        return false;
    }
}

// Opens the JSON file and clears all the data in it
void GameManager::deleteGames()
{
    std::ofstream file("config/Game_Paths.json", std::ios::trunc);
    if (!file.is_open())
    {
        std::cout << "ERROR OPENING FILE" << std::endl;
        return;
    }
    file.close();
}

// Checks to See if Directory "config" Exist, and if it doesnt it creates it. 
void GameManager::checkForDir()
{

    const std::wstring dirName = L"config";

    try
    {
        if (!std::filesystem::exists(dirName))
        {
            if (std::filesystem::create_directory(dirName))
            {
                std::cout << "Directory Created" << std::endl;
            }
            else
            {
                std::cerr << "Failed to Create Directory!" << std::endl;
            }
        }
    }
    catch (const std::filesystem::filesystem_error& ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
    }
}

// Converts strings to wchar_t, Make sure to call delete[] on the converted string when done with it to avoid memory leaks
const wchar_t* GameManager::convertStrToLPCWSTR(const std::string& tempStr)
{
    std::wstring temp = std::wstring(tempStr.begin(), tempStr.end());
    wchar_t* convertedStr = new wchar_t[temp.size() + 1];
    wcscpy_s(convertedStr, temp.size() + 1, temp.c_str());

    return convertedStr;
}

// Set the Reserve Size of _games Array
void GameManager::setReserveSize()
{
    GameManager::_games.reserve(_games.size() + _games.size() / 2);
}