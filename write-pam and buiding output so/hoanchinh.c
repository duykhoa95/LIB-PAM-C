/*******************************************************************************
 * file:        2ndfactor.c
 * author:      ben servoz
 * description: PAM module to provide 2nd factor authentication
 * notes:       instructions at http://ben.akrin.com/?p=1068
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>




/* expected hook */
PAM_EXTERN int pam_sm_setcred( pam_handle_t *pamh, int flags, int argc, const char **argv ) {
	return PAM_SUCCESS ;
}
struct string {
  char *ptr;
  size_t len;
};

void init_string(struct string *s) {
  s->len = 0;
  s->ptr = malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}
void xoabokhoangtrang(char *xau)
{
    char *temp=(char*)malloc(strlen(xau)+1);
    int k=0;
    for(int i=0;i<strlen(xau);i++)
        if (xau[i]!=32) temp[k++]=xau[i];
    temp[k]=0;
    strcpy(xau,temp);
    free(temp);
}  
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
  size_t new_len = s->len + size*nmemb;
  s->ptr = realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}

/* this function is ripped from pam_unix/support.c, it lets us do IO via PAM */
int converse( pam_handle_t *pamh, int nargs, struct pam_message **message, struct pam_response **response ) {
	int retval ;
	struct pam_conv *conv ;

	retval = pam_get_item( pamh, PAM_CONV, (const void **) &conv ) ; 
	if( retval==PAM_SUCCESS ) {
		retval = conv->conv( nargs, (const struct pam_message **) message, response, conv->appdata_ptr ) ;
	}

	return retval ;
}


/* expected hook, this is where custom stuff happens */
PAM_EXTERN int pam_sm_authenticate( pam_handle_t *pamh, int flags,int argc, const char **argv ) {
	int retval ;
	int i ;
    
	/* these guys will be used by converse() */
	char *password ;
	struct pam_message msg[1],*pmsg[1];
	struct pam_response *resp;
	
	/* retrieving parameters */
	int got_base_url  = 0 ;
	char base_url[256] ;
	for( i=0 ; i<argc ; i++ ) {
		if( strncmp(argv[i], "base_url=", 9)==0 ) {
			strncpy( base_url, argv[i]+9, 256 ) ;
			got_base_url = 1 ;
		}
	}
	if( got_base_url==0) {
		return PAM_AUTH_ERR ;
	}

	/* getting the username that was used in the previous authentication */
	const char *username ;
    	if( (retval = pam_get_user(pamh,&username,"login: "))!=PAM_SUCCESS ) {
		return retval ;
	}
	/* setting up conversation call prompting for one-time code */
	pmsg[0] = &msg[0] ;
	msg[0].msg_style = PAM_PROMPT_ECHO_ON ;
	msg[0].msg = "Nhap pass:" ;
	resp = NULL ;
	if( (retval = converse(pamh, 1 , pmsg, &resp))!=PAM_SUCCESS ) {
		// if this function fails, make sure that ChallengeResponseAuthentication in sshd_config is set to yes
		return retval ;
	}
	/* retrieving user input */
	if( resp ) {
		if( (flags & PAM_DISALLOW_NULL_AUTHTOK) && resp[0].resp == NULL ) {
	    		free( resp );
	    		return PAM_AUTH_ERR;
		}
		password = resp[ 0 ].resp;
		resp[ 0 ].resp = NULL; 		  				  
    	} else {
		return PAM_CONV_ERR;
	}

	/* building URL */
	char url_with_params[strlen(base_url) + strlen("?username=") + strlen(username) + strlen("&password=") + strlen(password)] ;
	strcpy( url_with_params, base_url ) ;
	strcat( url_with_params, "?username=" ) ;
	strcat( url_with_params, username ) ;
	strcat( url_with_params, "&password=" ) ;
	strcat( url_with_params, password ) ;

	/* HTTP request to service that will dispatch the code */
	  CURL *curl;

  curl = curl_easy_init();
  if(curl) {
    struct string s;
    init_string(&s);

    curl_easy_setopt(curl, CURLOPT_URL,url_with_params );
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
     curl_easy_perform(curl);

    printf("%s\n", s.ptr);
    //xoabokhoangtrang(s.ptr);
    if(strcmp(s.ptr,"true")==0)
    {
		printf("true");
		return PAM_SUCCESS ;
	}
	else
	{
		printf("false");
		return PAM_AUTH_ERR ;
	}
    free(s.ptr);

    /* always cleanup */
    curl_easy_cleanup(curl);
  }

	/* we shouldn't read this point, but if we do, we might as well return something bad */
	return PAM_AUTH_ERR ;
}
