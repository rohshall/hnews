# hnews - command-line Hacker News Reader in C++

## Dependencies

* libcurl: http://curl.haxx.se
* tinyxml2: https://github.com/leethomason/tinyxml2

However tinyxml2 is really tiny consisting of 1 .cpp and 1 .h file and is bundled with the application itself.

libcurl is commonly available in your package manager.

### Linux

For Arch Linux, you can install using:
```
sudo pacman -S curl
```
For Debian-based systems (including Ubuntu), 
```
sudo apt-get install curl
```
### Mac OS X
```
brew install curl  
```

## Build
```
g++ -o hnews hnews.cpp tinyxml2.cpp -lcurl
```
## Usage

```
./hnews
```
