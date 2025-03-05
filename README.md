# Build
You will need c++17 to build this code.
```cpp
g++ update_apt_mirrors.cpp -std=c++20 -o apt_changer
```

# Usage
It is very convinient to use the program. Run it as root and pass your desired mirror to it.
```sh
sudo ./apt_changer <mirror>
```

# Reverting changes
This program saves the prevoius mirror file with the format: `/etc/apt/sources.list-old` and you can revert changes if anything went wrong.
