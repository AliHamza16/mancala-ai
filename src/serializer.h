#ifndef HAFIF_SERIALIZER_H
#define HAFIF_SERIALIZER_H

#include <unordered_map>
#include <string>


namespace hafif  {
    template <typename T1, typename T2>
    void serialize_umap_to_file(const std::unordered_map<T1, T2>& umap, const std::string& filename);
    
    template <typename T1, typename T2>
    void deserialize_umap_from_file(std::unordered_map<T1, T2>& umap, const std::string& filename);
}

#include "serializer.tpp"

#endif // HAFIF_SERIALIZER_H
