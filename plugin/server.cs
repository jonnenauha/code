using System;
using System.IO;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Reflection;
using System.Threading;


public class Server
{
    class LicenseHandler
    {
        private TcpClient client;
        private NetworkStream stream;
        private SecurePlugin.DataBase db;

        private byte[] license_id = new byte [20];
        private byte[] plugin_id = new byte [20];

        public LicenseHandler (TcpClient c, SecurePlugin.DataBase d)
        {
            db = d;
            client = c;
            stream = client.GetStream();
        }

        public bool Verify ()
        {
            return db.Verify (license_id, plugin_id);
        }
    
        public void Start (object data)
        {
            byte[] sendbuf;

            try
            {
                stream.Read (license_id, 0, license_id.Length);
                stream.Read (plugin_id, 0, plugin_id.Length);

                Console.WriteLine (Convert.ToBase64String (license_id));
                Console.WriteLine (Convert.ToBase64String (plugin_id));

                if (Verify ())
                    sendbuf = Encoding.ASCII.GetBytes ("PASS");
                else
                    sendbuf = Encoding.ASCII.GetBytes ("FAIL");

                stream.Write (sendbuf, 0, sendbuf.Length);
            }
            
            finally
            {
                Dispose ();
            }
        }

        public void Dispose ()
        {
            stream.Close ();
            client.Close ();
        }
    }


    static void Main (string[] arg)
    {
        SecurePlugin.Factory factory = new SecurePlugin.Factory ("test-pub-priv-key.snk");
        SecurePlugin.IDPair pair = factory.BuildSecurePlugin (arg[0]);

        SecurePlugin.DataBase db = new SecurePlugin.DataBase();
        db.Insert (pair.license_id, pair.plugin_id);
        db.Print();

        TcpClient client = null;
        TcpListener server = null;

        try
        {
            server = new TcpListener (IPAddress.Parse ("127.0.0.1"), 6969);
            server.Start();

            while (true)
                ThreadPool.QueueUserWorkItem 
                    (new LicenseHandler (server.AcceptTcpClient (), db).Start);
        }
        catch (SocketException e)
        {
            Console.WriteLine ("SocketException: {0}", e);
        }
        finally
        {
            server.Stop();
        }
    }
}
