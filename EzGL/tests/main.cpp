#include "Test_glApi/Test_glApi.h"
#include <string>

int main(int argc, char** argv)
{
	if (argc > 1)
	{
		printf("Exec test : %s\n", argv[1]);
        return Test_glApi(argv[1]) ? 0 : 1;
	}

	return Test_glApi("Test_glApi_Uniforms_Parsing_Buffer_Widget_0") ? 0 : 1;

	return 0;
}