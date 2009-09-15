using System;
using System.Data;
using System.Data.SqlClient;
using Npgsql;

namespace SecurePlugin
{
    public class DataBase
    {
        private string connection_string;
        readonly static string table_name = "customers";
        readonly static string license_id_col_name = "lid";
        readonly static string plugin_id_col_name = "pid";

        public DataBase ()
        {
            connection_string = 
                "Server=127.0.0.1;" +
                "Port=5432;" +
                "Database=secure-plugin-test;" +
                "User ID=test;" +
                "Password=123456;";
        }

        public DataBase (string conn)
        {
            connection_string = conn;
        }

        public bool Insert (byte[] lid, byte[] pid)
        {
            string lidstr = Convert.ToBase64String (lid);
            string pidstr = Convert.ToBase64String (pid);
            string sql = "INSERT INTO " + table_name + " (" 
                + license_id_col_name + ", " + plugin_id_col_name 
                + ") VALUES ('" + lidstr + "', '" + pidstr + "')";

            return execute_ (sql, null);
        }

        public bool Verify (byte[] lid, byte[] pid)
        {
            string lidstr = Convert.ToBase64String (lid);
            string pidstr = Convert.ToBase64String (pid);
            string sql = "SELECT " + license_id_col_name + ", " + plugin_id_col_name + " FROM " + table_name;

            return execute_ (sql, new DataVerifier (lidstr, pidstr));
        }

        public void Print ()
        {
            string sql = "SELECT " + license_id_col_name + ", " + plugin_id_col_name + " FROM " + table_name;
            execute_ (sql, new DataPrinter());
        }

        private interface IDataHandler 
        {
            bool Apply (IDataReader reader);
        }

        private class DataVerifier : IDataHandler
        {
            private string lid;
            private string pid;

            public DataVerifier (string license_id, string plugin_id)
            {
                lid = license_id;
                pid = plugin_id;
            }

            public bool Apply (IDataReader reader)
            {
                while (reader.Read()) 
                {
                    if ((lid == (string) reader [license_id_col_name]) 
                            && (pid == (string) reader [plugin_id_col_name]))
                        return true;
                }

                return false;
            }
        }

        private class DataPrinter : IDataHandler
        {
            public bool Apply (IDataReader reader)
            {
                while (reader.Read()) 
                {
                    string lid = (string) reader[license_id_col_name];
                    string pid = (string) reader[plugin_id_col_name];
                    Console.WriteLine ("Name: " + lid + " " + pid);
                }

                return true;
            }
        }

        private bool execute_ (string sql, IDataHandler handle)
        {
            bool success = false;
            try 
            {
                IDbConnection dbcon;
                using (dbcon = new NpgsqlConnection (connection_string)) 
                {
                    dbcon.Open();
                    using (IDbCommand dbcmd = dbcon.CreateCommand()) 
                    {
                        dbcmd.CommandText = sql;
                        using (IDataReader reader = dbcmd.ExecuteReader()) 
                        {
                            if (handle != null)
                                success = handle.Apply (reader);
                            else
                                success = true;
                        }
                    }
                }
            }
            catch (SqlException e)
            { }

            return success;
        }

    }
}
