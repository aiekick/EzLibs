#include <string>
#include <EzGraph/Test_EzGraph.h>

int main(int argc, char** argv) {
    if (argc > 1) {
        printf("Exec test : %s\n", argv[1]);
        return Test_EzGraph(argv[1]) ? 0 : 1;
    }
    return Test_EzGraph("Test_Graph_Evaluation_0") ? 0 : 1;
}