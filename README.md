# Cloth-simulation

Cloth-Simulation using mass-spring system.

### Run
```
    $ g++ src/*.cpp -std=c++11 -I/usr/local/Cellar/eigen/3.3.9/include/eigen3 -framework OpenGl -I/usr/local/include -lglfw -lGLEW
    $ g++-11 -fopenmp src/*.cpp -std=c++11 -I/usr/local/Cellar/eigen/3.3.9/include/eigen3 -framework OpenGl -lglfw -lGLEW -I/usr/local/include -L/usr/local/lib
```

## Built With
* OpenGl-3.3
* c++17

### Result
![alt-text](https://j.gifs.com/1WY7xG.gif)

### TODO
* Fast Mass springs
* Tearing
* Lighting issues
* Better Time Stepping





