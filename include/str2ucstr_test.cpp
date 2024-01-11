#include <gtest/gtest.h>
#include <str2ucstr.h>
#include <memory>
#include <exception>

namespace
{
TEST(MyFunctionTest, BasicTest)
{
	//checks the chars of the string without the c string lib and insures that the last charater is 
	std::string str1="teststring";
	std::unique_ptr<char[]> charar(ucs::str2ucstr(str1));
	std::cout << str1 << charar << std::endl;
	std::string str2(charar.get());
	EXPECT_EQ(str1.c_str(),str2);
	std::cout << "end of test\n";
};

TEST(ExceptsTest, BasicTest)
{
	bool excepts=false;
	try
	{
		std::string str="";
		for (int i=0 ; i < STRLIM+1 ; i++)
		{
			str.append("a");
		}
		ucs::str2ucstr(str);
	}
	catch(ucs::StrLimException& exc)
	{
		excepts=true;
	}
	EXPECT_EQ(STRLIM,1024);
	EXPECT_EQ(excepts,true);
};
	
}//namespac


#include <gtest/gtest.h>

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
