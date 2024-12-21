#ifndef HAFIF_SERIALIZER_TPP
#define HAFIF_SERIALIZER_TPP

#include "serializer.h"

#include <fstream>
#include <iostream>
#include <format>

template <typename T1, typename T2>
void hafif::serialize_umap_to_file(const std::unordered_map<T1, T2> &umap, const std::string &filename)
{
    std::ofstream file(filename, std::ios::binary);

    if (!file.is_open())
    {
        std::cerr << std::format("Cannot open file {}", filename);
        return;
    }

    for (auto it : umap)
    {
        file.write(reinterpret_cast<const char*>(&it.first), sizeof(it.first));
        file.write(reinterpret_cast<const char*>(&it.second), sizeof(it.second));
    }

    file.close();
}

template <typename T1, typename T2>
void hafif::deserialize_umap_from_file(std::unordered_map<T1, T2> &umap, const std::string &filename)
{
    T1 key;
    T2 value;

    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open())
    {
        std::cerr << std::format("Cannot open file {}", filename);
        return;
    }

    while (
        file.read(reinterpret_cast<char*>(&key), sizeof(key)) && 
        file.read(reinterpret_cast<char*>(&value), sizeof(value))
        )
    {
        umap[key] = value;
    }

    file.close();
    
}

#endif // HAFIF_SERIALIZER_TPP