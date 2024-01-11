/*
 * Author: Louis C Seifert III copyright 2024 all rights reserved
 * License: MiT license compatible this software depends on the pam libary software
 * Please include this notice with any software that you include any of this code in. 
 * sometimes developers or IT people are unemployed, notices like this help us 
 * get our name out there. There is no warranty of service for this software, 
 * no promise of suitability given or granted for any specific use. 
 * this software was tested to work with its core functionality but may have been modified 
 * and redistributed. Any deriviative of this should include this notice. 
 *
 */
#include <pamauth.h>
#include <string>

int main(int argc, char** argsv){
	

	PamAuth pamauth;
	if(pamauth.authenticate()!=PAM_SUCCESS){
		std::cerr << "auth failed\n";
		//log the event to syslog
		pamauth.log_failed(argc,argsv);
		return(PAM_SUCCESS);
	}
	pamauth.log_success(argc,argsv);
	
	/*
	 * Alterntively you can call a one liner 
	 * PamAuth::authenticate(); 
	 * this will take care of logging the user and program that is being executed.
	 */
		
	return -1;
}
