//
// Created by stefan on 8/15/21.
//

#ifndef OVERTONE_JSON_H
#define OVERTONE_JSON_H


#include <vector>
#include <string>
#include <iostream>

class JSON
{
public:
    explicit JSON(std::string directory_path):
        directory_path(std::move(directory_path))
    {
    }
    template <typename Type> void
    save_vector(const std::vector<Type> &vector,
                const std::string &file_name);
private:
    const std::string directory_path;
};

template <typename Type> void JSON::save_vector(const std::vector<Type> &vector,
                                                const std::string &file_name)
{
    std::string file_path = directory_path + file_name;
    std::ofstream file(file_path);
    if (file)
    {
        auto size = vector.size();
        file << "[";
        for (decltype(size) index = 0; index != (size - 1); ++index)
        {
            file << vector[index] << ", ";
        }
        file << vector[size - 1] << "]";
    }
    else
    {
        throw std::runtime_error("Couldn't open file: " + file_path);
    }
}

#endif //OVERTONE_JSON_H
