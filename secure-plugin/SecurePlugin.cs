using System;
using System.Text;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Net.NetworkInformation;
using System.Security;
using System.Security.Cryptography;
using System.Threading;
using System.Reflection;



namespace SecurePlugin
{
    public class PluginError : Exception
    {
        public PluginError () : base() {}
        public PluginError (string msg) : base(msg) {}
        public PluginError (string msg, Exception e) : base(msg, e) {}
    }

    public class SecurePlugin 
    {
        private RSA cipher;
        private Mono.Security.StrongName strong_name;
        private AssemblyName assembly_name;
        private string assembly_filename;

        private byte[] plugin_id;
        private byte[] license_id;

        private string ip_address;
        private string mac_address;

        private bool validated = false;

        public SecurePlugin ()
        {
            try
            {
                gather_credentials_();
                validate_credentials_();
            }
            catch (PluginError e)
            {
                Console.WriteLine (e.Message);
                validated = false;
            }
        }

        public bool Validated 
        { 
            get { return validated; } 
        }

        public byte[] PluginID
        {
            get { return plugin_id; }
        }

        public byte[] LicenseID
        {
            get { return license_id; }
        }

        public string IPAddress
        {
            get { return ip_address; }
        }

        public string MACAddress
        {
            get { return mac_address; }
        }

        private void gather_credentials_ ()
        {
            assembly_name = Assembly.GetExecutingAssembly().GetName();
            assembly_filename = Assembly.GetExecutingAssembly().Location;

            byte[] pubkey = assembly_name.GetPublicKey();
            if ((pubkey == null) || (pubkey.Length < 12)) 
                throw new PluginError ("This is not a strongly named assembly.");

            cipher = Mono.Security.Cryptography.CryptoConvert.FromCapiPublicKeyBlob (pubkey, 12);
            strong_name = new Mono.Security.StrongName (cipher);

            // verify plugin
            if (strong_name.Verify (assembly_filename) == false) 
                throw new PluginError ("This assembly is not securely signed.");

            // get pid
            plugin_id = strong_name.Hash (assembly_filename);

            // get lid
            Assembly a = Assembly.GetExecutingAssembly();
            foreach (string r in a.GetManifestResourceNames())
                if (r == "license.id")
                {
                    Stream s = a.GetManifestResourceStream (r);
                    if (s.Length != 20)
                        throw new PluginError ("License ID is corrupt. SHA1 hashes are 20 bytes.");

                    license_id = new byte [20];
                    s.Read (license_id, 0, 20);
                }

            // get mac addr
            foreach (NetworkInterface n in NetworkInterface.GetAllNetworkInterfaces())
                if (n.NetworkInterfaceType == NetworkInterfaceType.Ethernet)
                    mac_address = n.GetPhysicalAddress().ToString();

            // get ip addr
            ip_address = Dns.GetHostEntry (Dns.GetHostName ()).AddressList[0].ToString();
        }
        
        private void validate_credentials_ ()
        {
            byte[] recvbuf = new byte [4];
            string result = null;

            NetworkStream stream = null;
            TcpClient client = null;

            try 
            {
                client = new TcpClient ("127.0.0.1", 6969);
                stream = client.GetStream();

                Console.WriteLine (Convert.ToBase64String (LicenseID));
                Console.WriteLine (Convert.ToBase64String (PluginID));

                stream.Write (LicenseID, 0, LicenseID.Length);
                stream.Write (PluginID, 0, PluginID.Length);
                stream.Read (recvbuf, 0, recvbuf.Length);

                result = Encoding.ASCII.GetString (recvbuf, 0 , recvbuf.Length);
            } 
            catch (SocketException e) 
            {
                Console.WriteLine ("SocketException: {0}", e);
            }
            finally
            {
                stream.Close();         
                client.Close();         
            }

            if (result == "PASS")
                validated = true;
            else
                throw new PluginError ("Could not authorize Plugin.");
        }
    }
}
