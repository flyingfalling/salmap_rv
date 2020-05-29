#include <salmap_rv/include/funcptrregistrar.hpp>

#include <salmap_rv/include/funcptrmap.hpp>


salmap_rv::detail::FuncPtrRegistrar::FuncPtrRegistrar(salmap_rv::detail::FuncPtrMap::FuncPtr funcPtr, const std::string funcName) {
  salmap_rv::detail::FuncPtrMap::getWritableMap().emplace(funcName, funcPtr);
}
