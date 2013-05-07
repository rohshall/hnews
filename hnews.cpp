#include <cstdlib>
#include <curl/curl.h>
#include <string>
#include <iostream>
#include "tinyxml2.h"

using namespace tinyxml2;

// Hacker news URLs
const std::string RSS_FEED_URL = "https://news.ycombinator.com/rss";


/*
 * Write data callback function (called within the context of 
 * curl_easy_perform.
 */
int write_data(void *buffer, int size, int nmemb, void *userp)
{
    int segsize = size * nmemb;
    const char* src = (const char*) buffer;
    std::string* feed = (std::string*) userp;
    feed->append(src, segsize);
    /* Return the number of bytes received, indicating to curl that all is okay */
    return segsize;
}


void rss_feed_parse(const std::string &feed)
{
    XMLDocument doc;
    doc.Parse(feed.c_str());
    XMLElement *rss = doc.FirstChildElement("rss");
    if (rss) {
        XMLElement *channel = rss->FirstChildElement("channel");
        if (channel) {
            for (XMLElement *article = channel->FirstChildElement("item"); article; article = article->NextSiblingElement("item")) {
                XMLElement *title = article->FirstChildElement("title"); 
                XMLElement *link = article->FirstChildElement("link"); 
                std::cout << title->GetText() << std::endl;
                std::cout << link->GetText() << std::endl;
                std::cout << std::endl;
            }
        }
    }
}
 

int main(int argc, char *argv[])
{
    CURL *curl;
    CURLcode res;
    std::string feed;
 
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&feed);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_URL, RSS_FEED_URL.c_str());
        /* Perform the request, res will get the return code */ 
        res = curl_easy_perform(curl);
        /* Check for errors */ 
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            rss_feed_parse(feed);
        }
        /* always cleanup */ 
      curl_easy_cleanup(curl);
    }
    return 0;
}
