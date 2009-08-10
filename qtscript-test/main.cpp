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
    QCoreApplication app (argc, argv);
    QScriptEngine engine ();

    ExistingClass existing;
    existing.lart (5);
    existing.foo ();

    return app.exec();
}
