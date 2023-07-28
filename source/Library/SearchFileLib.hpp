#ifndef SearchFileLib_HPP_INCLUDED
#define SearchFileLib_HPP_INCLUDED

#include <filesystem>
#include <list>
#include <string>
#include <memory>
#include <thread>
#include <functional>
#include <mutex>
#include <exception>
#include <iostream>

namespace Naxel
{


    std::string FindFile(const char* directory, const char* file_name);
    std::string RecursiveFindFile(const char* directory, const char* file_name);



    class Searcher {
    public:
        Searcher(std::filesystem::path directory, std::string file_name);
        ~Searcher();

        void StartSearch();

        void StopSearch() { stop_command = true; };
        std::string GetResult() { return result; };
        bool is_searching() { return _is_searching; };
        bool is_found() { return _is_found; };

    private:
        void Search(std::filesystem::path directory, std::string file_name);

        std::filesystem::path directory;
        std::string file_name;

        std::string result = "";
        bool _is_searching = true;
        bool _is_found = false;
        bool stop_command = false;
        std::thread my_thread;
    };

    class SearcherController {
    public:
        SearcherController(int thread_count_limit, std::filesystem::path find_directory, std::string file_name);

        bool StartSearch(); //returns true if search was successful started
        bool CheckStatus(); //returns true the search is finished / returns false if search continues

        std::string GetResult() { return result; };

    private:
        unsigned int thread_count_limit = 1;
        unsigned int current_threads_count = 0;
        std::string result = "";
        std::filesystem::path find_directory;
        std::string file_name;
        std::list<std::filesystem::path> sub_directories;
        std::vector<std::unique_ptr<Searcher>> searcher_list;
    };



}
#endif
