# hnews - command-line Hacker News Reader in C++

Hacker news http://news.ycombinator.com is an excellent news resource for developers. 
Though it does not have official APIs, http://hndroidapi.appspot.com/ provides ths 
APIs that this program uses to get articles and comments.

## Dependencies

* libcurl: http://curl.haxx.se
* libjson-c: https://github.com/json-c/json-c

These libraries are commonly available in your package manager.

For Arch Linux, you can install using:

sudo pacman -S json-c curl

For Debian-based systems (including Ubuntu), 

sudo apt-get install json-c curl

## Build
g++ -o hnews hnews.cpp -lcurl -ljson

## Usage

### List the articles on home page

**./hnews**  

lists the articles along with their IDs and URLs.  
Most terminals on unix-based systems are capable of opening the URLs from the terminal output.  
The article IDs are used in the following command to get the comments on these articles.  

### Get the comments on articles:  

**./hnews item_id**  

Comments are hierarchical and their level is indicated by tabs. So, a child comment is printed below the parent comment after a tab.

