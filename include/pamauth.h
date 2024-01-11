/*
 *  Copyright Louis C Seifert III 2024  all rights reserved. 
 *  
 *This source file and all source files  included to test this header file 
 *are  included without warranty or suitability for any purpose if your using 
 *free software your accepting a certain amount of risk . This software 
 * is published under the XXX licence. 
 */

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <str2ucstr.h>
#include <memory>
extern "C" {
#include <security/pam_appl.h>
#include <security/pam_misc.h>
}
#ifndef QUICKLOG
#define QUICKLOG
#include <syslog.h>
void log_success(std::string user, int argc,char** argsv){
	std::string program="unknown_app";
	if (argc > 0){
		program=argsv[0];
	};
	openlog(program.c_str(), LOG_PID, LOG_AUTH);  // Open connection to syslog
	syslog(LOG_INFO, "User %s excuted %s and authenticated", user.c_str(),program.c_str());
	closelog();  // Close connection
}

void log_failed(std::string user, int argc, char**argsv ){
	 std::string program="pamauth_app";
        if (argc > 0){
                program=argsv[0];
        };
        openlog(program.c_str(), LOG_PID, LOG_AUTH);  // Open connection to syslog
        syslog(LOG_INFO, "User %s executed %s and failed to authenticate", user.c_str(), program.c_str());
        closelog();  // Close connection
}
#endif //QUICKLOG
#ifndef SECURITY_PAM_CPP_H
#define SECURITY_PAM_CPP_H
#define NAME_LIMIT 32
/*
static int my_pam_conv(int num_msg, const struct pam_message **msg, struct pam_response **resp, void * n) {
return 0;
}*/
struct pam_conv conv = {
	misc_conv,
	NULL
};

class PamAuth{
	protected:
	uid_t puid;
	passwd *ppw=nullptr;
	
	public:
	std::string username;
	pam_handle_t *pam_h = nullptr;
	//allows the user to ineract with the object for more ornate functionality
	bool isuser=false;
	int auth_attempts=3;
	bool isauthorized=false;
	int status;
	std::shared_ptr<char[]> user=nullptr;
	PamAuth(){
		puid=getuid();
		ppw=getpwuid(puid);
		char * user=strdup(ppw->pw_name);
		status=pam_start("check_user",user,&conv,&pam_h);
		if (status==PAM_SUCCESS){
			isuser=true;
		}
		delete(user);
		this->username=std::string(ppw->pw_name);
			
		
	}

	PamAuth(std::string servicename, std::string  user,pam_conv conv){
		
		//converts string into into uniqupte
		status=pam_start(ucs::str2ucstr(servicename).get(),ucs::str2ucstr(user).get(),&conv,&pam_h);
		if (status==PAM_SUCCESS){
			isuser=true;
		}
	}


	void log_success( int argc,char** argsv){
        std::string program="unknown";
        if (argc > 0){
                program=argsv[0];
        };
        openlog(program.c_str(), LOG_PID, LOG_AUTH);  // Open connection to syslog
        syslog(LOG_INFO, "User %s excuted %s and authenticated", username.c_str(),program.c_str());
        closelog();  // Close connection
	}

	void log_failed(int argc, char**argsv ){
        std::string program="unknown";
        if (argc > 0){
                program=argsv[0];
        };
        openlog(program.c_str(), LOG_PID, LOG_AUTH);  // Open connection to syslog
        syslog(LOG_INFO, "User %s executed %s and failed to authenticate", username.c_str(), program.c_str());
        closelog();  // Close connection
	}
	/*constructs the Pamauth service if you pass null runuser will be used
	 *  a genderal conv is included in the header file be sure to overriede this*/
	PamAuth(std::string servicename, pam_conv conv){
		puid=getuid();
		passwd *ppw = getpwuid(puid);
		std::string temp(ppw->pw_name);
		this->username=temp;
		if (!servicename.empty()){
			status=pam_start(servicename.c_str(), ppw->pw_name , &conv, &pam_h );
		       if ( status == PAM_SUCCESS){
				isuser=true;
			}
		}else{
			status=26;
		}
		free(ppw);
		
	};
int	authenticate(){
		if (isuser){
			status=PAM_TRY_AGAIN;
			status=pam_authenticate(pam_h,0);
			if (status==PAM_SUCCESS){
				return PAM_SUCCESS;
			}
		}
		return PAM_TRY_AGAIN;
	}
	~PamAuth(){
		if (isauthorized||isuser){
			pam_end(pam_h,0);
		}
	};
/*
 * If your just looking for a simple validation of a user with basic options
 * just call the quick_auth() it will ask the user for their username where ever you put it
 * keep in mind this is for command line only.
 */
static int quick_auth(){
	std::string checkuser("check_user");
	return PamAuth::quick_auth(checkuser, nullptr,"");
};
/*
 * Perform a quick authorization just takes a service name and returns after the user has authenticated
 * */
static int quick_auth(std::string&& service){
	return PamAuth::quick_auth(service, nullptr, "");
};

/*quick auth has no requirements it checks the user according toe the defined service
 * simplt provide the service name and null for runasuser which is not implemnted
 * max pas len is the system policy max path, the username pointer a std::string object pointer to 
 * the username if you want to get a username back. if you don't want to set a username 
 * or receive a 
 * */
static int quick_auth(std::string service, std::shared_ptr<std::string> username,  std::string runasuser){
	runasuser=runasuser;
	pam_handle_t* pamh = nullptr;
	uid_t uid = getuid();
	passwd *pwd = getpwuid(uid);
	int auth;
	char * user=strdup(pwd->pw_name);
	int i = 0 ;
	if(username!=nullptr){
		while (user[i]!='\0'&& i <= NAME_LIMIT)
		{
			username->push_back(user[i]);
			if ( i > NAME_LIMIT){
				std::cerr << "Authentication failed: EXCEEDED NAME LIMIT\n";
			}
		}
	}
	if (pwd) {
		std::cout << pwd->pw_name <<" enter your password: ";		
		int retval = pam_start(ucs::str2ucstr(service).get(), user , &conv, &pamh );
		std::cout << "\n";
		if (retval == PAM_SUCCESS){
                	auth=pam_authenticate(pamh,0);
			if (auth!=PAM_SUCCESS){
				pam_end(pamh,auth);
				free(user);
				return 1;
			}
			pam_end(pamh,auth);
		}
	};
	free(user);
return 0 ;
};
};
#endif /*SECURITY_PAM_CPP_h*/
