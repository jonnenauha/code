/* main.cpp -- main module
 *
 *			Ryan McDougall -- 2009
 */

#include "main.h"

//=============================================================================
// Main entry point
int
main (int argc, char** argv)
{
    QApplication app (argc, argv); 
    Window win;

    win.show();

    return app.exec();
}
