#include <cstdlib>
#include <curl/curl.h>
#include <string>
#include <iostream>
#include <json/json.h>

// Hacker news URLs
const std::string HOME_PAGE_URL = "http://hndroidapi.appspot.com/news/format/json/page/";
const std::string COMMENTS_URL = "http://hndroidapi.appspot.com/nestedcomments/format/json/id/";

// colors to output text
const std::string KNRM = "\x1B[0m";
const std::string KRED = "\x1B[31m";
const std::string KGRN = "\x1B[32m";
const std::string KYEL = "\x1B[33m";
const std::string KBLU = "\x1B[34m";
const std::string KMAG = "\x1B[35m";
const std::string KCYN = "\x1B[36m";
const std::string KWHT = "\x1B[37m";

/*
 * Write data callback function (called within the context of 
 * curl_easy_perform.
 */
int write_data( void *buffer, int size, int nmemb, void *userp )
{
  int segsize = size * nmemb;
  const char* src = (const char*) buffer;
  std::string* feed = (std::string*) userp;
  feed->append( src, segsize );
  /* Return the number of bytes received, indicating to curl that all is okay */
  return segsize;
}

void comments_array_parse( json_object* j_array, int level )
{
  int j_array_size = json_object_array_length( j_array );
  for( int i = 0; i < j_array_size; i++ ) {
    json_object *j_article = json_object_array_get_idx( j_array, i );
    json_object *j_username = json_object_object_get( j_article, "username" );
    json_object *j_comment = json_object_object_get( j_article, "comment" );
    for( int l = 0; l < level; l++ ) {
      std::cout << '\t';
    }
    std::cout << KMAG << json_object_get_string( j_username ) << KNRM << ": " << json_object_get_string( j_comment ) << std::endl;
    std::cout << std::endl;
    json_object *j_children = json_object_object_get( j_article, "children" );
    comments_array_parse( j_children, level + 1 );
  }
}

void comments_parse( std::string feed )
{
  json_object *root = json_tokener_parse( feed.c_str() );
  if ( !root ) {
    std::cerr << "Unexpected JSON response error " << std::endl;
  } else {
    json_object *j_array = json_object_object_get( root, "items" );
    comments_array_parse( j_array, 0 );
  }
}

void home_page_parse( std::string feed )
{
  json_object *root = json_tokener_parse( feed.c_str() );
  if ( !root ) {
    std::cerr << "Unexpected JSON response error " << std::endl;
  } else {
    json_object *j_array = json_object_object_get( root, "items" );
    int j_array_size = json_object_array_length( j_array );
    // Skip the last item as it is just a placeholder
    for( int i = 0; i < j_array_size-1; i++ ) {
      json_object *j_article = json_object_array_get_idx( j_array, i );
      json_object *j_title = json_object_object_get( j_article, "title" );
      json_object *j_item_id = json_object_object_get( j_article, "item_id" );
      std::cout << KGRN << json_object_get_int( j_item_id ) << KNRM << ": ";
      std::cout << KCYN << json_object_get_string( j_title ) << KNRM << std::endl;
      json_object *j_url = json_object_object_get( j_article, "url" );
      std::cout << KMAG << json_object_get_string( j_url ) << KNRM << std::endl;
      json_object *j_comments_count = json_object_object_get( j_article, "comments" );
      if( j_comments_count != NULL ) {
        std::cout << "(" << json_object_get_string( j_comments_count ) << ")";
      }
      json_object *j_score = json_object_object_get( j_article, "score" );
      if( j_score != NULL ) {
        std::cout << "(" << json_object_get_string( j_score ) << ")";
      }
      // Add a newline if no comments or score is available 
      if( j_comments_count || j_score ) {
        std::cout << std::endl;
      }
      std::cout << std::endl;
    }
  }
}
 

int main( int argc, char *argv[] )
{
  CURL *curl;
  CURLcode res;
  std::string feed;
 
  curl = curl_easy_init();
  if( curl ) {
    curl_easy_setopt( curl, CURLOPT_WRITEDATA, (void *)&feed );
    curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, write_data );
 
    switch(argc) {
      case 1:
        curl_easy_setopt( curl, CURLOPT_URL, HOME_PAGE_URL.c_str() );
        break;
      case 2:
        curl_easy_setopt( curl, CURLOPT_URL, (COMMENTS_URL + argv[1]).c_str() );
        break;
      default:
        std::cerr << "Usage: hnews [item_id]" << std::endl;
        exit(1);
    }
    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform( curl );
    /* Check for errors */ 
    if( res != CURLE_OK ) {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror( res ) << std::endl;
    } else {
      if( argc == 1 ) {
        home_page_parse( feed );
      } else {
        comments_parse( feed );
      }
    }
    /* always cleanup */ 
    curl_easy_cleanup( curl );
  }
  return 0;
}
