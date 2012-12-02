# hnews - Hacker News Reader in C++

Hacker news http://news.ycombinator.com

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

./hnews
lists the home-page articles along with their IDs and URLs.

To get the comments on articles:
./hnews <item_id>
Comments are hierarchical and their level is indicated by tabs. So, a child comment is printed below the parent comment after a tab.

