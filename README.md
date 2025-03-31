# Clone 

```
git clone --recurse-submodules -j 8 https://github.com/jwkohler/demokw.git
```

> Note that the both pico-sdk and FreeRTOS projects are linked submodules.  Cloning these submodules can take 'a while'.

# Build

```
mkdir build
cd build 
cmake ..
make
./load
```

## Reset Button

![image](https://github.com/user-attachments/assets/a0bee866-f4ba-4728-a5af-2523fc9392bc)
