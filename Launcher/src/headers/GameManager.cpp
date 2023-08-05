#include "GameManager.h"

using namespace std;

// Constructor, we call checkForDir() so it checks if the given directory exist everytime the object is created
GameManager::GameManager() { checkForDir(); };

// Deconstructor for class, called when the class goes out of scope
GameManager::~GameManager() { cout << "Destroyed Object" << endl; };

string config_path = "config/Game_Paths.json";

// Add a new game path to the JSON file
void GameManager::addGame(const string& path, const string& name, const string& args)
{
    json jsonData = json::object();
    fstream jsonFile(config_path);

    // Check if the file is open and valid
    if (jsonFile.is_open()) {
        // Read the content of the file into a string
        string jsonString((istreambuf_iterator<char>(jsonFile)), istreambuf_iterator<char>());

        // Check if the file is empty
        if (!jsonString.empty()) {
            // Attempt to parse the content as JSON
            try {
                jsonData = json::parse(jsonString);
                cout << "Successfully loaded config file." << endl;
            }
            catch (const json::parse_error& e) {
                cerr << "Error parsing JSON: " << e.what() << endl;
                return;
            }
        }

        jsonFile.close();
    }

    // Open config file as write
    jsonFile.open(config_path, ios::out | ios::trunc);

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

    cout << "Could not open config file." << endl;
    return;
}

// Stores all the Data From the JSON file into a vector array of type JSON
void GameManager::loadFromFile()
{
    _games.clear();

    try
    {
        json jsonData;
        ifstream jsonFile("config/Game_Paths.json");

        // Check if the file is open and valid
        if (jsonFile.is_open()) {
            // Read the content of the file into a string
            string jsonString((istreambuf_iterator<char>(jsonFile)), istreambuf_iterator<char>());

            // Check if the file is empty
            if (!jsonString.empty()) {
                // Attempt to parse the content as JSON
                try {
                    jsonData = json::parse(jsonString);
                    cout << "Successfully loaded config file." << endl;
                }
                catch (const json::parse_error& e) {
                    cerr << "Error parsing JSON: " << e.what() << endl;
                    return;
                }
            }

            jsonFile.close();
        }

        // Iterate through the array and populate the _games vector
        for (auto entry = jsonData.begin(); entry != jsonData.end(); ++entry)
        {
            const string& gameName = entry.key();
            const json& gameData = entry.value();

            json gameEntry;
            gameEntry["game_name"] = gameName;
            gameEntry["game_path"] = gameData.value("game_path", "");
            gameEntry["command_args"] = gameData.value("command_args", "");
            _games.emplace_back(gameEntry);
        }
    }
    catch (const json::parse_error& e)
    {
        cout << "JSON Parse Error: " << e.what() << endl;
    }
    catch (const exception& e)
    {
        cout << "Exception: " << e.what() << endl;
    }
}

// Print The List of Games Stored in the JSON file.
void GameManager::printGames()
{
    json jsonData;
    fstream jsonFile(config_path);

    // Check if the file is open and valid
    if (jsonFile.is_open()) {
        // Read the content of the file into a string
        string jsonString((istreambuf_iterator<char>(jsonFile)), istreambuf_iterator<char>());

        // Check if the file is empty
        if (!jsonString.empty()) {
            // Attempt to parse the content as JSON
            try {
                jsonData = json::parse(jsonString);
            }
            catch (const json::parse_error& e) {
                cerr << "Error parsing JSON: " << e.what() << endl;
                return;
            }
        }

        for (auto entry = jsonData.begin(); entry != jsonData.end(); ++entry)
        {
            cout << entry.key() << endl;
        }
        cout << "--------------------------------------" << endl;

        jsonFile.close();
    }
}

void GameManager::loopThroughSaveArray()
{
    for (int i = 0; i < _games.size(); i++)
    {
        cout << _games[i] << endl;
    }
}

bool GameManager::openGame(const string& gamename)
{
    try
    {
        json jsonData;
        ifstream jsonFile("config/Game_Paths.json");

        // Check if the file is open and valid
        if (jsonFile.is_open()) {
            // Read the content of the file into a string
            string jsonString((istreambuf_iterator<char>(jsonFile)), istreambuf_iterator<char>());

            // Check if the file is empty
            if (!jsonString.empty()) {
                // Attempt to parse the content as JSON
                try {
                    jsonData = json::parse(jsonString);
                    cout << "Successfully loaded config file." << endl;
                }
                catch (const json::parse_error& e) {
                    cerr << "Error parsing JSON: " << e.what() << endl;
                    return false;
                }
            }

            jsonFile.close();
        }

        if (!jsonData.contains(gamename))
        {
            cout << "Game Not Found!" << endl;
            return false;
        }

        const string tempArgs = jsonData[gamename].value("command_args", "");
        const string tempPath = jsonData[gamename].value("game_path", "");

        if (tempPath.empty())
        {
            cout << "Game Path not specified for '" << gamename << "'" << endl;
            return false;
        }

        const wchar_t* convertedArgs = convertStrToLPCWSTR(tempArgs);
        const wchar_t* convertedPath = convertStrToLPCWSTR(tempPath);

        ShellExecute(NULL, L"open", convertedPath, convertedArgs, NULL, SW_SHOWNORMAL);

        if (!tempArgs.empty())
        {
            wcout << "Opening '" << convertedPath << "' with args: " << convertedArgs << " ... " << endl;
        }
        else
        {
            wcout << "Opening '" << convertedPath << "' ... " << endl;
        }

        delete[] convertedPath;
        delete[] convertedArgs;
        return true;
    }
    catch (const json::parse_error& e)
    {
        cout << "JSON Parse Error: " << e.what() << endl;
        return false;
    }
    catch (const json::type_error& e)
    {
        cout << "JSON Type Error: " << e.what() << endl;
        cout << "Game '" << gamename << "' Probably wasnt added correctly" << endl;
        return false;
    }
}

// Opens the JSON file and clears all the data in it
void GameManager::deleteGames()
{
    ofstream file("config/Game_Paths.json", ios::trunc);
    if (!file.is_open())
    {
        cout << "Could not clear games file." << endl;
        return;
    }
    file.close();
}

// Checks to See if Directory "config" Exist, and if it doesnt it creates it. 
void GameManager::checkForDir()
{

    const wstring dirName = L"config";

    try
    {
        if (!filesystem::exists(dirName))
        {
            if (filesystem::create_directory(dirName))
            {
                cout << "Directory Created" << endl;
            }
            else
            {
                cerr << "Failed to Create Directory!" << endl;
            }
        }
    }
    catch (const filesystem::filesystem_error& ex)
    {
        cerr << "Error: " << ex.what() << endl;
    }
}

// Converts strings to wchar_t, Make sure to call delete[] on the converted string when done with it to avoid memory leaks
const wchar_t* GameManager::convertStrToLPCWSTR(const string& tempStr)
{
    wstring temp = wstring(tempStr.begin(), tempStr.end());
    wchar_t* convertedStr = new wchar_t[temp.size() + 1];
    wcscpy_s(convertedStr, temp.size() + 1, temp.c_str());

    return convertedStr;
}

// Set the Reserve Size of _games Array
void GameManager::setReserveSize()
{
    GameManager::_games.reserve(_games.size() + _games.size() / 2);
}