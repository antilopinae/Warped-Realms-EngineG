sudo pacman -S git cmake ninja sdl2 glew

git clone https://github.com/mosra/corrade.git
cd corrade
mkdir build && cd build

cmake .. \
  -DCMAKE_INSTALL_PREFIX=$HOME/corrade-install \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_SHARED_LIBS=ON

cmake --build . --parallel
cmake --install .

cd ~
git clone https://github.com/mosra/magnum.git
cd magnum
mkdir build && cd build


cmake .. \
  -DCMAKE_PREFIX_PATH=$HOME/corrade-install \
  -DCMAKE_INSTALL_PREFIX=$HOME/magnum-install \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_SHARED_LIBS=ON \
  -DWITH_GL=ON \
  -DWITH_SDL2APPLICATION=ON \
  -DWITH_SHADERS=ON \
  -DWITH_PRIMITIVES=ON \
  -DWITH_MESHTOOLS=ON \
  -DWITH_SCENEGRAPH=ON \
  -DWITH_TEXTURETOOLS=ON

cmake --build . --parallel
cmake --install .


