#include "headers.h"

using json = nlohmann::json;

class GameManager
{
private:
	std::vector<json> _games;
public:
	GameManager();
	~GameManager();

	void addGame(const std::string& path, const std::string& name, const std::string& args);
	
	void loadFromFile();

	void loopThroughSaveArray();

	void printGames();

	bool openGame(const std::string& gamename);

	void deleteGames();

	void checkForDir();

	const wchar_t* convertStrToLPCWSTR(const std::string& tempStr);

	void setReserveSize();
};

