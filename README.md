# RP2040/FreeRTOS Template Project 

Forked from: https://github.com/LearnEmbeddedSystems/rp2040-freertos-template

# New Project From Template

Use the `Use This Template` button at the top of the page.  Name the repo, etc.

Clone the new project with:
```
git clone --recurse-submodules https://${GIT_TOKEN}@github.com/jwkohler/NEWREPONAME.git
```
> note: the GIT_TOKEN supplies the 'access token' for the account.  defined in .bashrc

# Clone This Project

```
git clone --recurse-submodules https://${GIT_TOKEN}@github.com/jwkohler/tmplt-rtos.git
```
> note: the GIT_TOKEN supplies the 'access token' for the account.  defined in .bashrc

# Build

```
mkdir build
cd build 
cmake ..
make 
picotool load src/tmplt-rtos.uf2 -f -v
```

Unmodified, this project will spawn a single task to rapidly blink the LED on and off. (Pin 25, the onboard LED.)


Use the 'Template' 
## Cloning 

Cloning repository, supplying an access token, seems to 'set' the token on the local clonse.  When 
cloned this way, VSCode and command line tools need no further authentication.

Access token put in environment as `GIT_TOKEN`.

```
git clone --recurse-submodules https://${GIT_TOKEN}@github.com/jwkohler/rp2040-freertos-template.git
```

## Reset Button

![image](https://github.com/user-attachments/assets/a0bee866-f4ba-4728-a5af-2523fc9392bc)
