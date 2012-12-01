#include <stdio.h>
#include <curl/curl.h>
 
int main(void)
{
  CURL *curl;
  CURLcode res;
  int wr_context = 0;
 
  curl = curl_easy_init();
  if( curl ) {
    curl_easy_setopt( curl, CURLOPT_URL, "http://hndroidapi.appspot.com/news/format/json/page/" );
    curl_easy_setopt( curl, CURLOPT_WRITEDATA, (void *)&wr_context );
    curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, write_data );
 
    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform( curl );
    /* Check for errors */ 
    if( res != CURLE_OK ) {
      fprintf( stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror( res ) );
    }
    /* always cleanup */ 
    curl_easy_cleanup( curl );
  }
  return 0;
}
