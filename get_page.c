#include "get_page.h"

char* get_page(const char *url_address) {
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();

	if (curl) {
		struct c_style_vector c_vector;
		c_vector.data = malloc(1);
		c_vector.size = 0;

		//set url_address
		curl_easy_setopt(curl, CURLOPT_URL, url_address);

		//use HTTP version 2
		curl_easy_setopt(curl, CURLOPT_HTTP_VERSION,
				(long) CURL_HTTP_VERSION_2);

		//callback function to retrieve response into string buffer
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_to_string_callback);

		//write to c_vector, which is basically string
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*) &c_vector);

		//don't print to stdout(too many logs)
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

		//perform curl action
		res = curl_easy_perform(curl);

		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed...\n");
			curl_easy_strerror(res);

		} else {
			//status okay
			return c_vector.data;
		}
		//never forget to clean up
		curl_easy_cleanup(curl);

	}

	return NULL;
}

size_t curl_to_string_callback(void *contents, size_t size, size_t nmemb,
		void *data) {
	//compute actual size
	size_t realsize = size * nmemb;
	struct c_style_vector *c_vector = (struct c_style_vector*) data;

	//reallocate buffer to fit actual size
	char *ptr = realloc(c_vector->data, c_vector->size + realsize + 1);

	if (!ptr) {
		//in case of out of memory
		printf("not enough memory (realloc failed.) \n");
		return 0;
	}

	//copy read data into buffer
	c_vector->data = ptr;
	memcpy(&(c_vector->data[c_vector->size]), contents, realsize);
	c_vector->size += realsize;
	c_vector->data[c_vector->size] = '\0';

	return realsize;

}
