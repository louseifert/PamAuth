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
 * This is a modern C++ wrapper to the c pam auth library
 */
#include <pamauth.h>
#include <string>
#include <cstdlib>
#include <gtest/gtest.h>

namespace asd
{
	TEST(GetUser, BasicTest)
	{
		std::string envusername(getenv("USER"));
		PamAuth pamauth;
		EXPECT_EQ(pamauth.username,envusername);
	}


	TEST(FailedLogin,BasicTest)
	{
		PamAuth pamauth;;
		int m=pamauth.authenticate();
		std::cout << "\n" << pamauth.username << m << "\n";
		EXPECT_NE(m,PAM_SUCCESS);

	}
	TEST(FailedQuickAuth, BasicTest)
	{
		EXPECT_NE(PamAuth::quick_auth(),PAM_SUCCESS);
	}

}


int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
