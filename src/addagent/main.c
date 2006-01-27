/*   $OSSEC, main.c, v0.2, 2006/01/27, Daniel B. Cid$   */

/* Copyright (C) 2005,2006 Daniel B. Cid <dcid@ossec.net>
 * All right reserved.
 *
 * This program is a free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation
 */



#include "manage_agents.h"

/* print banner */
void print_banner()
{
    printf(BANNER, __name, __version);

    #ifdef CLIENT
    printf(BANNER_CLIENT);
    #else
    printf(BANNER_OPT);
    #endif

    return;
}


/** main **/
int main(int argc, char **argv)
{
    char *user_msg;
    char *dir = DEFAULTDIR;
    char *group = GROUPGLOBAL;
    gid_t gid;
    

    /* Setting the name */
    OS_SetName(ARGV0);
        
   
    /* Getting currently time */
    time1 = time(0);
    
    
    /* Getting the group name */
    gid = Privsep_GetGroup(group);
    if(gid < 0)
    {
	    ErrorExit(USER_ERROR,"",group);
    }
	
    
    /* Setting the group */
    if(Privsep_SetGroup(gid) < 0)
    {
	    ErrorExit(SETGID_ERROR,ARGV0,gid);
    }
    
    
    /* Chrooting to the default directory */
    if(Privsep_Chroot(dir) < 0)
    {
        ErrorExit(CHROOT_ERROR, ARGV0, dir);
    }


    /* Inside chroot now */
    nowChroot();
    
    
    /* Little shell */
    while(1)
    {
        print_banner();
   
        user_msg = read_from_user();
        
        /* All the allowed actions */
        switch(user_msg[0])
        {
            case 'A':
            case 'a':
                add_agent();
                break;
            case 'e':
            case 'E':
                k_extract();
                break;
            case 'i':
            case 'I':
                k_import();
                break;    
            case 'r':
            case 'R':
                remove_agent();
                break;
            case 'q':
            case 'Q':
                printf(EXIT);
                exit(0);    
            default:    
                printf("\n ** Invalid Action ** \n\n");
                break;            
        }

        continue;
        
    }
    return(0);
}


/* EOF */
