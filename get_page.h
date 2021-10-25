#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
//

struct c_style_vector{
	size_t size;
	char *data;
};

size_t curl_to_string_callback(void *contents, size_t size, size_t nmemb, void *data);
