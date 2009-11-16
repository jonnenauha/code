/* main.cpp -- main module
 *
 *			Ryan McDougall -- 2009
 */

#include "main.h"

void test (QCoreApplication &app, QScriptEngine &engine)
{
    cout << "-- making existing object" << endl;
    ExistingClass *existing (new ExistingClass);
    QScriptValue js_existing = engine.newQObject (existing);
    engine.globalObject().setProperty ("existing", js_existing);

    cout << "-- making signal handlers" << endl;
    QScriptValue on_action = engine.evaluate ("function action_handler(val) { print('I got new value ', val); }");
    QScriptValue on_poke = engine.evaluate ("function poke_handler() { print('someone poked him'); }");

    engine.evaluate ("existing.poked.connect(poke_handler)");
    qScriptConnect (existing, SIGNAL (action(int)), QScriptValue(), on_action);
    QObject::connect (existing, SIGNAL (poked()), existing, SLOT (on_poke()));

    cout << "-- poke existing object" << endl;
    existing->lart (5);
    engine.evaluate ("existing.foo()");

    cout << "-- enhancing object" << endl;
    engine.globalObject().setProperty ("enhanced", js_existing);
    js_existing.setProperty ("capabilities", "enhanced mofo");
    js_existing.setProperty ("bar", "wtf");

    engine.evaluate ("enhanced.lart = function(v) { this.set_value (this.value() - v); }");
    
    cout << "-- testing object" << endl;
    cout << qPrintable (existing->property ("capabilities").toString()) << endl;
    cout << qPrintable (existing->property ("bar").toString()) << endl;
    cout << qPrintable (js_existing.property ("bar").toString()) << endl;

    engine.evaluate ("enhanced.lart(15)");
    engine.evaluate ("enhanced.foo()");

    existing->lart (20);
    existing->foo ();

    cout << "-- dumping object" << endl;
    engine.evaluate ("for (k in enhanced) { print ( enhanced + '[' + k + ']' + enhanced[k]); }");
}

//=============================================================================
// Main entry point
int
main (int argc, char** argv)
{
    QCoreApplication app (argc, argv);
    QScriptEngine engine;

    test (app, engine);

    return app.exec();
}
