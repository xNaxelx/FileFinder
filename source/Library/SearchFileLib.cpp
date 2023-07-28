#include "SearchFileLib.hpp"


std::string Naxel::FindFile(const char* directory, const char* file_name)
{
	for (const auto& entry : std::filesystem::directory_iterator(directory))
	{
		if (entry.path().filename().string().compare(file_name) == 0)
		{
			return entry.path().string();
		}
	}

	return "\0";
}

std::string Naxel::RecursiveFindFile(const char* directory, const char* file_name)
{
	for (const auto& entry : std::filesystem::recursive_directory_iterator(directory))
	{
		if (entry.path().filename().string().compare(file_name) == 0)
		{
			return entry.path().string();
		}
	}

	return "\0";
}



Naxel::Searcher::Searcher(std::filesystem::path directory, std::string file_name) :
	directory(directory), file_name(file_name)
{

}

void Naxel::Searcher::StartSearch()
{
	my_thread = std::thread(std::bind(&Searcher::Search, this, directory, file_name));
	my_thread.detach();
}

void Naxel::Searcher::Search(std::filesystem::path directory, std::string file_name)
{
	_is_searching = true;
	_is_found = false;
	stop_command = false;
	for (const auto& entry : std::filesystem::recursive_directory_iterator(directory))
	{
		if (stop_command)
		{
			break;
		}
		if (entry.path().filename().string().compare(file_name) == 0)
		{
			result = entry.path().string();
			_is_searching = false;
			_is_found = true;
			stop_command = true;

			break;
		}
	}
		
	_is_searching = false;
}

Naxel::Searcher::~Searcher()
{
	my_thread.~thread();
}



Naxel::SearcherController::SearcherController(int thread_count_limit, std::filesystem::path find_directory, std::string file_name) :
	thread_count_limit(thread_count_limit), find_directory(find_directory), file_name(file_name)
{
	for (const auto& entry : std::filesystem::directory_iterator(find_directory))
	{
		if (entry.is_directory())
		{
			sub_directories.push_back(entry);
		}
	}
}

bool Naxel::SearcherController::StartSearch()
{
	bool temp = false;
	for (; current_threads_count < thread_count_limit && sub_directories.size() != 0; current_threads_count++)
	{
		searcher_list.push_back(std::make_unique<Searcher>(sub_directories.front(), file_name));
		sub_directories.pop_front();

		searcher_list.back()->StartSearch();
		
		temp = true;
	}
	if (current_threads_count != 0)
	{
		temp = true;
	}
	return temp;
}

bool Naxel::SearcherController::CheckStatus()
{
	if (searcher_list.size() == 0)
	{
		return true;
	}

	for (auto i = searcher_list.cbegin(); i != searcher_list.cend(); i++)
	{
		if ((*i)->is_searching() == false)
		{
			if ((*(i))->is_found())
			{
				for (const auto& ii : searcher_list)
				{
					ii->StopSearch();
				}

				result = (*(i))->GetResult();
				return true;
			}
			else
			{
				(*i)->StopSearch();
				searcher_list.erase((i));
				current_threads_count--;
				return !StartSearch();
			}
		}
	}

	return false; 
}
