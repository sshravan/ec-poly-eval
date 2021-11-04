## Installation requirements
[mcl](https://github.com/herumi/mcl) requires [GMP](https://gmplib.org/)

```bash
apt-get install libgmp-dev # on Ubuntu
```
## Build instructions

```bash
git clone --recursive git@github.com:sshravan/ec-poly-eval.git
cd ec-poly-eval
mkdir build
cd build
cmake ..
make && time ./src/main
```
