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
  json_object *root = json_tokener_parse( feed.c_str() );
  if ( !root ) {
    std::cerr << "Unexpected JSON response error " << std::endl;
  } else {
    json_object *j_array = json_object_object_get( root, "items" );
    size_t j_array_size = json_object_array_length( j_array );
    for( size_t i = 0; i < j_array_size-1; i++ ) {
      json_object *j_article = json_object_array_get_idx( j_array, i );
      json_object *j_title = json_object_object_get( j_article, "title" );
      std::cout << i << ": " << json_object_get_string( j_title ) << std::endl;
      json_object *j_url = json_object_object_get( j_article, "url" );
      std::cout << json_object_get_string( j_url ) << std::endl;
      json_object *j_comments_count = json_object_object_get( j_article, "comments" );
      if( j_comments_count != NULL ) {
        std::cout << " (" << json_object_get_string( j_comments_count ) << ")";
      }
      json_object *j_score = json_object_object_get( j_article, "score" );
      if( j_score != NULL ) {
        std::cout << " (" << json_object_get_string( j_score ) << ")";
      }
      if( j_comments_count || j_score ) {
        std::cout << std::endl;
      }
      std::cout << std::endl;
    }
  }
  std::cout.flush();
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
