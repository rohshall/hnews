#include <curl/curl.h>
#include <string>
#include <iostream>
#include <json/json.h>

/*
 * Write data callback function (called within the context of 
 * curl_easy_perform.
 */
size_t write_data( void *buffer, size_t size, size_t nmemb, void *userp )
{
  int segsize = size * nmemb;
  const char* src = (const char*) buffer;
  std::string* feed = (std::string*) userp;
  feed->append( src, segsize );
  /* Return the number of bytes received, indicating to curl that all is okay */
  return segsize;
}


void hnews_parse( std::string feed )
{
  json_object * jobj = json_tokener_parse( feed.c_str() );
  json_type type = json_object_get_type( jobj );
  std::cout << "Got JSON " << type << std::endl;
}
 

int main(void)
{
  CURL *curl;
  CURLcode res;
  std::string feed;
 
  curl = curl_easy_init();
  if( curl ) {
    curl_easy_setopt( curl, CURLOPT_URL, "http://hndroidapi.appspot.com/news/format/json/page/" );
    curl_easy_setopt( curl, CURLOPT_WRITEDATA, (void *)&feed );
    curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, write_data );
 
    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform( curl );
    /* Check for errors */ 
    if( res != CURLE_OK ) {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror( res ) << std::endl;
    } else {
      hnews_parse( feed );
    }
    /* always cleanup */ 
    curl_easy_cleanup( curl );
  }
  return 0;
}
