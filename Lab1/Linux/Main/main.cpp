#include <iostream>
#include "number.h"
#include <dlfcn.h>

void test_explicit() {
    void *handle = dlopen("libVector.so", RTLD_NOW);
    if (!handle) {
        std::cerr << "Cannot open library: " << dlerror() << std::endl;
        return;
    }

    dlerror();

    typedef void* (*CreateVectorFuncWithParam)(Number, Number);
    typedef void* (*CreateVectorFunc)();
    typedef Number (*GetRadiusFunction)(void*);
    typedef Number (*DeleteVectorFunc)(void*);

    CreateVectorFuncWithParam create_vector_with_p = (CreateVectorFuncWithParam)dlsym(handle, "CreateVector2");
    CreateVectorFunc create_vector = (CreateVectorFunc)dlsym(handle,"CreateVector");
    GetRadiusFunction get_radius = (GetRadiusFunction)dlsym(handle, "getRadius");
    DeleteVectorFunc delete_vector = (DeleteVectorFunc)dlsym(handle, "DeleteVector");

    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol 'create_vector': " << dlsym_error << std::endl;
        dlclose(handle);
        return;
    }

    void* vector = create_vector();
    void* vector2 = create_vector_with_p(Number(4), Number(3));
    Number radius0 = get_radius(vector);
    Number radius = get_radius(vector2);
    std::cout<<radius0.GetNumber()<<std::endl;
    std::cout<<radius.GetNumber()<<std::endl;

    delete_vector(vector);
    delete_vector(vector2);
    dlclose(handle);
    return;
}

int main() {
    test_explicit();
}

