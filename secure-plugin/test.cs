using System;

public class Test : SecurePlugin.SecurePlugin
{
    public Test () : base() {}

    public void DoSomethingFancy ()
    {
        if (Validated)
        {
            Console.WriteLine ("validated: Fancy Work!!!");
        }
    }

    static void Main (string[] arg)
    {
        Test t = new Test();
        t.DoSomethingFancy();
    }
}
