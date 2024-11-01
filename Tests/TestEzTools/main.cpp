#include <TestEzTools.h>

////////////////////////////////////////////////////////////////////////////
//// ENTRY POINT ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
    if (argc > 1) {
        printf("Exec test : %s\n", argv[1]);
        return TestEzTools(argv[1]) ? 0 : 1;
    }
    // User testing
    return TestEzTools("TestEzCron_Format_Valid_Type_Interval") ? 0 : 1;
}
