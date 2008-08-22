using System;
using System.Text;
using System.IO;
using System.Security;
using System.Security.Policy;
using System.Security.Cryptography;
using System.Threading;
using System.Reflection;
using System.CodeDom.Compiler;

using Mono.Security;


namespace SecurePlugin
{
    public class FactoryError : Exception
    {
        public FactoryError () : base() {}
        public FactoryError (string msg) : base(msg) {}
        public FactoryError (string msg, Exception e) : base(msg, e) {}
    }

    public struct IDPair
    {
        public byte[] license_id;
        public byte[] plugin_id;
    }

    public class Factory
    {
        private Mono.Security.StrongName strong_name; 
        private SHA1 hasher = new SHA1CryptoServiceProvider();
        private RandomNumberGenerator rander = new RNGCryptoServiceProvider();

        static readonly string plugin_name = "SecurePlugin";
        static readonly string license_name = "license.id";
        static readonly string switchtoken = "-";

        public Factory (string key_filename)
        {
            strong_name = new Mono.Security.StrongName (ReadFromFile (key_filename));
        }

        public byte[] GenerateLicenseID (string email)
        {
            if (email.Length == 0)
                throw new FactoryError ("No email given");

            byte[] input = Encoding.UTF8.GetBytes (email);
            byte[] salt = new byte [8];
            byte[] saltedinput = new byte [salt.Length + input.Length];

            rander.GetNonZeroBytes (salt);

            Buffer.BlockCopy (salt, 0, saltedinput, 0, salt.Length);
            Buffer.BlockCopy (input, 0, saltedinput, salt.Length, input.Length);

            return hasher.ComputeHash (saltedinput);
        }

        public byte[] GeneratePluginID (string assembly_filename)
        {
            return strong_name.Hash (assembly_filename);
        }

        public IDPair BuildSecurePlugin (string licensee_email)
        {
            IDPair pair = new IDPair();

            // create LicenseID
            pair.license_id = GenerateLicenseID (licensee_email);
            WriteToFile (license_name, pair.license_id);

            // compile to dll
            string input = plugin_name + ".cs";
            string output = plugin_name + ".dll";
            string dep = "Mono.Security.dll";
            string keyfile = "test-pub-priv-key.snk";

            CompilerParameters parm = new CompilerParameters();
            parm.OutputAssembly = output;
            parm.ReferencedAssemblies.Add (dep);
            parm.EmbeddedResources.Add (license_name);
            parm.CompilerOptions = switchtoken + "keyfile:" + keyfile;

            CodeDomProvider cmp = CodeDomProvider.CreateProvider ("CSharp");
            CompilerResults res = cmp.CompileAssemblyFromFile (parm, input);

            if (res.Errors.Count > 0)
            {
                foreach (CompilerError err in res.Errors)
                    Console.WriteLine ("!!! " + err.ToString());
            }
            else
                Console.WriteLine ("compiled: " + res.PathToAssembly);

            // create PluginID
            pair.plugin_id = GeneratePluginID (output);

            return pair;
        }

        public static byte[] ReadFromFile (string filename) 
        {
            byte[] data = null;
            FileStream fs = File.Open (filename, FileMode.Open, FileAccess.Read, FileShare.Read);
            try 
            {
                data = new byte [fs.Length];
                fs.Read (data, 0, data.Length);
            }
            finally 
            {
                fs.Close ();
            }

            return data;
        }

        public static void WriteToFile (string filename, byte[] data) 
        {
            FileStream fs = File.Open (filename, FileMode.Create, FileAccess.Write, FileShare.Read);
            try 
            {
                fs.Write (data, 0, data.Length);
            }
            finally 
            {
                fs.Close ();
            }
        }
    }
}
