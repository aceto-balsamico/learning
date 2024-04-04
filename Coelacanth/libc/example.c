
#include "../src/common_func.h"


//@@@function
void hello_world(){
	printf("Hello World\n");
	extract_lines("setting.conf", "[value]", "---", 1, 3);
}
//@@@function
int Test_Test() {
	printf("Test_Test\n");
	extract_lines("setting.conf", "[array]", "---", 1, 3);
	return 0;
}
// @@@function
int* addressptr(){
	printf("addressptr\n");
	return NULL;
}