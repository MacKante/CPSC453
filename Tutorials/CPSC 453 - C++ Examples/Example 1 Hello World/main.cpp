#include <iostream> //Include library header with <> brackets

// Declaring we are using the std namespace (std::cout shortened to just cout)
// Be careful with this though, as this can create name conflicts
using namespace std;

// Entry point into program. provides two variables and int indicating how many 
// command line inputs there are and an array of C-strings (more on this later) 
// of the inputs (first always the name of the program eg. "a.exe" or "a.out")
int main(int argc, char* argv[]){

    // Print statement to the command line (std::ostream, output stream)
    cout << "Hello World from: " << argv[0] << endl;

    // Preprocessor variable representing 0 (pasted when compiled).
    return EXIT_SUCCESS; 
}


