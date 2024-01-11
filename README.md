# PamAuth

## C++ wrapper for the C pam libraries to make it trivial for a quick user check. 

I was getting ready to add this to another project and I figured why not create a class that allows you to call the check user in one function and possibly extend it to ask for user credentials in the gui... the initial focus was on the command line. Created to allow mere mortals to add linux user level auth to a command line application. 
#
## Table of contents.

- [Installation](#installation)

## Installation

1. requires libpam or libpam0g
2. apt get libpam0g
3. If it fails to compile you must take the following action
 a. source ./configure.sh
 b. make clean
 c. make test //this performs the tests of the basic functionality so far
 d. make fulltest // this requires the user to authenticate with the linux pam system
 e. make creates the binary which is just an example of the library

4 .Must specify environment variables in the make file, such as which directories </br>contain the security/pam*.h files known as includes_dir
5. Must Specify the so_dir variable to the location of the /usr/lib/GNU*******.so lib folder 
6. I would recommend setting permissions on the resulting executable with auth to 550

7. Adding this to your executable makes it so that if your browser is compromised what ever executable you are protecting requires authentication. 
8. You can use a one liner to add the authentication and have it log to the log
PamAuth::quick_auth(argc,argsv);


