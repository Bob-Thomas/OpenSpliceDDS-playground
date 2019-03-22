#pragma once
#include <string>
#include <fstream>
#include <experimental/filesystem>
#include <vector>
#include <regex>
#include <map>
namespace fs = std::experimental::filesystem::v1;

/**
 * @brief Is a class that helps finding and listing linux sys/class inputs
 * 
 */
class InputLister
{
  private:
    std::string path;
    std::regex filterRegex;
    std::vector<std::pair<std::string, std::string>> inputNames;

  public:
    /**
     * @brief Construct a new Input Lister object
     * 
     * @param path 
     * @param filter 
     */
    InputLister(std::string path, std::string filter) : path(path), filterRegex(std::regex(filter))
    {
        for (const auto &entry : fs::directory_iterator(path))
        {
            std::string name = entry.path().stem().generic_string();
            if (std::regex_search(name, this->filterRegex))
            {
                std::ifstream deviceFile(fs::path(entry.path() / fs::path("device/name")));
                std::string deviceName;
                if (deviceFile.is_open())
                {
                    getline(deviceFile, deviceName);
                    deviceFile.close();
                }
                this->inputNames.push_back(std::pair<std::string, std::string>(name, deviceName));
            }
        }
    }

    /**
     * @brief Fills the given stream with a list of available inputs
     * 
     * @param stream 
     */
    void showList(std::ostream &stream)
    {
        int counter = 0;
        for (const auto &input : inputNames)
        {
            stream << "[" << counter << "] - " << input.first << " - " << input.second << "\n";
            counter++;
        }
    }

    /**
     * @brief 
     * Checks the input list if a given index exists
     * 
     * @param index index of the input
     * @return true 
     * @return false 
     */
    bool isAvailableInput(unsigned int index)
    {
        return index >= 0 && index <= this->inputNames.size() - 1;
    }

    InputLister(InputLister const &) = delete;

    InputLister(InputLister &&) = default;
};