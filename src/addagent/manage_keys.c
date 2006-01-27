/*   $OSSEC, manage_keys.c, v0.2, 2006/01/27, Daniel B. Cid$   */

/* Copyright (C) 2005 Daniel B. Cid <dcid@ossec.net>
 * All right reserved.
 *
 * This program is a free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation
 */



#include "manage_agents.h"


/* b64 function prototypes */
char *decode_base64(const char *src);
char *encode_base64(int size, char *src);


/* Import a key */
int k_import()
{
    FILE *fp;
    char *user_input;
    char *b64_dec;
   
    char *name; char *ip; char *tmp_key;
     
    char line_read[FILE_SIZE +1];
    

    printf(IMPORT_KEY);

    user_input = read_from_user();
    
    b64_dec = decode_base64(user_input);
    if(b64_dec == NULL)
    {
        printf(NO_KEY);
        return(0);
    }
    
    memset(line_read, '\0', FILE_SIZE +1);
    strncpy(line_read, b64_dec, FILE_SIZE);

    name = index(b64_dec, ' ');
    if(name && strlen(line_read) < FILE_SIZE)
    {
        *name = '\0';
        name++;
        ip = index(name, ' ');
        if(ip)
        {
            *ip = '\0';
            ip++;

            tmp_key = index(ip, ' ');
            if(!tmp_key)
            {
                printf(NO_KEY);
                return(0);
            }
            *tmp_key = '\0';
           
            printf(AGENT_INFO, b64_dec, name, ip); 
            fflush(stdout);

            user_input = read_from_user();

            if(user_input[0] == 'y' || user_input[0] == 'Y')
            {
                fp = fopen(KEYS_FILE,"w");
                if(!fp)
                {
                    ErrorExit(FOPEN_ERROR, ARGV0, KEYS_FILE);
                }
                fprintf(fp,"%s\n",line_read);
                fclose(fp);
                printf(AGENT_ADD);
                return(1);
            }
        }
    }
    
    printf(NO_KEY);
    return(0);

}


/* extract base64 for a specific agent */
int k_extract()
{
    FILE *fp;
    char *user_input;
    char *b64_enc;
    char line_read[FILE_SIZE +1];
    char n_id[USER_SIZE +1];

    if(!print_agents())
    {
        printf(NO_AGENT);
        return(0);
    }


    printf(EXTRACT_KEY);
    fflush(stdout);

    user_input = read_from_user();

    
    if(!IDExist(user_input))
    {
        printf(NO_ID, user_input);
    }

    
    /* Trying to open the auth file */
    fp = fopen(AUTH_FILE, "r");
    if(!fp)
    {
        ErrorExit(FOPEN_ERROR, ARGV0);
    }
    
    fsetpos(fp, &fp_pos);

    memset(n_id, '\0', USER_SIZE +1);
    strncpy(n_id, user_input, USER_SIZE -1);
    
    
    if(fgets(line_read, FILE_SIZE, fp) == NULL)
    {
        printf(ERROR_KEYS);
        exit(1);
    }
    chomp(line_read);

    
    b64_enc = encode_base64(strlen(line_read),line_read);
    if(b64_enc == NULL)
    {
        printf(EXTRACT_ERROR);
        exit(1);
    }

    printf(EXTRACT_MSG, n_id, b64_enc);
    read_from_user();

    free(b64_enc);

    return(0);
}


/* EOF */
