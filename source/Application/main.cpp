#include <iostream>
#include <filesystem>
#include <string>
#include "../Library/SearchFileLib.hpp"
#include <chrono>

#define THREADS_FOR_SEARCH_LIMIT 8


int main(int argc, char* argv[])
{
    setlocale(LC_ALL, ".UTF8");

    std::string file_name;
    std::string result = "";
    if (argc == 1)
    {
        std::cout << "Enter the name of the file you are looking for\n";

        std::getline(std::cin, file_name);
    }
    else if (argc == 2)
    {
        file_name = argv[1];
    }

    result = Naxel::FindFile(std::filesystem::current_path().string().c_str(), file_name.c_str());

    if (result != "")
    {

    }
    else
    {
        Naxel::SearcherController searcher(THREADS_FOR_SEARCH_LIMIT, std::filesystem::current_path(), file_name);

        searcher.StartSearch();

        while (searcher.CheckStatus() == false)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(50));
        }

        result = searcher.GetResult();
    }
    
    if (result != "\0")
    {
        std::cout << result << std::endl;
    }
    else
    {
        std::cout << "File not found\n";
    }
    
#ifdef _WIN32
    system("PAUSE");
#endif
    return 0;
}

