cmake_minimum_required(VERSION 3.26.0)

# --- Dependencies ---

# include(Zlib.cmake)
# include(Libpng.cmake)

# SDL2_image
if(CUSTOM_DEP_HANDLER_INCLUDED)
    set(SDL_SHARED ON CACHE BOOL "" FORCE)
    set(SDL_STATIC OFF CACHE BOOL "" FORCE)

    find_or_clone_dependency(SDL2_image
        "https://github.com/libsdl-org/SDL_image.git"
        "release-2.8.8"
        CMAKE_ARGS
            -DSDL2_IMAGE_SHARED=ON
            -DSDL2_IMAGE_STATIC=OFF
            -DCMAKE_POSITION_INDEPENDENT_CODE=ON
            -DSDL2_IMAGE_SAMPLES=OFF
            -DSDL2_IMAGE_TEST=OFF
            -DSDL2_IMAGE_VENDORED=ON

            -DLOAD_BMP=OFF
            -DLOAD_GIF=OFF
            -DLOAD_LBM=OFF
            -DLOAD_PCX=OFF
            -DLOAD_PNM=OFF
            -DLOAD_SVG=OFF                    # Требует libnanosvg (если нужно SVG)
            -DLOAD_TGA=OFF
            -DLOAD_XCF=OFF
            -DLOAD_XPM=OFF
            -DLOAD_XV=OFF
            -DLOAD_WEBP=OFF                  # Требует libwebp (если нужен WebP)
            -DLOAD_JPG=OFF                     # Требует libjpeg или libjpeg-turbo (почти всегда нужен)
            -DLOAD_PNG=ON                     # Требует libpng и zlib (почти всегда нужен)
            -DLOAD_TIF=OFF                    # Требует libtiff (если нужен TIFF)
            -DLOAD_JPGXL=OFF                  # Требует libjxl (если нужен JPEG XL)
            -DLOAD_AVIF=OFF                   # Требует libavif (если нужен AVIF)

        EXPECTED_TARGET SDL2_image::SDL2_image
    )
else()
    message(FATAL_ERROR "SDL2_image target not created.")
endif()
