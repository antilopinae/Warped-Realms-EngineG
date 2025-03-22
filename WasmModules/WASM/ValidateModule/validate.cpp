#include <cstdlib>
#include <cstring>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

int ValidateValueProvided(const char* value,
    const char* error_message,
    char* return_error_message
) {
    if ((value == nullptr) || (value[0] == '\0')) {
        strcpy(return_error_message, error_message);
        return 0;
    }
    return 1;
}

#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_KEEPALIVE
#endif
int ValidateName(char* name,
    int maximum_length,
    char* return_error_message
) {
    if (ValidateValueProvided(name,
    "A Product Name must be provided.",
    return_error_message
    ) == 0) {
        return 0;
    }

    if (strlen(name) > maximum_length) {
        strcpy(return_error_message, "The Product Name is too long.");
        return 0;
    }

    return 1;
}

int IsCategoryIdInArray(char* selected_category_id,
    int* valid_category_ids,
    int array_length
) {
    int category_id = atoi(selected_category_id);
    for (int index = 0; index < array_length; index++) {
        if (valid_category_ids[index] == category_id) {
            return 1;
        }
    }

    return 0;
}

#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_KEEPALIVE
#endif
int ValidateCategory(char* category_id,
    int* valid_category_ids,
    int array_length,
    char* return_error_message
) {
    if (ValidateValueProvided(category_id,
        "A Product Category must be selected.",
        return_error_message
    ) == 0) {
        return 0;
    }

    if ((valid_category_ids == nullptr) || (array_length == 0)) {
        strcpy(return_error_message,
            "There are no Product Categories available.");
        return 0;
    }

    if (IsCategoryIdInArray(category_id, valid_category_ids,
        array_length) == 0) {
        strcpy(return_error_message, "The selected Product Category is not valid.");
        return 0;
    }

    return 1;
}

#ifdef __cplusplus
}
#endif