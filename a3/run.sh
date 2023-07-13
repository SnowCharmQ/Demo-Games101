mkdir build
cd build/
cmake ..
make
./a3 normal.png normal
./a3 phong.png phong
./a3 texture.png texture
./a3 bump.png bump
./a3 displacement.png displacement
./a3 normal.png normal -b
./a3 phong.png phong -b
./a3 texture.png texture -b
./a3 bump.png bump -b
./a3 displacement.png displacement -b