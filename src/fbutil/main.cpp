#include "FirebirdConfig.h"

// This is mark's test program

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

using namespace std;


int main(int argc, char* argv[]) {



    char* data = readline("cop that>");
    while (data != NULL) {
        cout << data << endl;
        add_history(data);
        data = readline("cop that>");        
    }

    string answer = FirebirdConfig::getSysString("waldo1");

    cout << answer << endl;

}
