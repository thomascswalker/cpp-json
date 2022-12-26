#ifndef INSPECTION_H
#define INSPECTION_H

/// <summary>
/// Type comparison between given object and provided type T.
/// </summary>
/// <returns>Whether the object is of the given type.</returns>
template<typename T, typename S> bool is_instance(const S obj)
{
    auto ptr = &obj;
    return static_cast<const T*>(ptr) != nullptr;
}

#endif