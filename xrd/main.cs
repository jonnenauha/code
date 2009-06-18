using System;
using System.IO;
using System.Xml;
using System.Xml.XPath;
using System.Collections.Generic;

public class XRDParser
{
    public class XRDDocument
    {
        public class XRDLink 
        {
            protected string relation_;
            protected string uri_template_;
            protected Uri uri_;
            protected string media_type_;

            XRDLink (string relation, string uritemplate, Uri uri, string mediatype)
            {
                relation_ = relation;
                uri_template_ = uritemplate;
                uri_ = uri;
                media_type_ = mediatype;
            }

            public string Relation { get { return relation_; } }
            public string UriTemplate { get { return uri_template_; } }
            public Uri Uri { get { return uri_; } }
            public string MediaType { get { return media_type_; } }
        }

        protected string expires_ = new string (); //TODO: should be a time data type
        protected string subject_ = new string ();
        protected List <string> aliases_ = new List <string> (); 
        protected List <string> types_ = new List <string> (); 
        protected List <XRDLink> links_ = new List <XRDLinks> (); 
    }

    protected XPathDocument doc_;
    protected XPathNavigator cursor_;

    protected XPathExpression expires_exp_;
    protected XPathExpression subject_exp_;
    protected XPathExpression aliases_exp_;
    protected XPathExpression types_exp_;
    
    public XRDParser (Stream xrd)
    {
        doc_ = new XPathDocument (xrd);
        cursor_ = doc_.CreateNavigator();
        expires_exp_ = cursor_.Compile ("/Expires");
        subject_exp_ = cursor_.Compile ("/Subject");
        aliases_exp_ = cursor_.Compile ("/Alias");
        types_exp_ = cursor_.Compile ("/Type");
    }

    public XRDDocument Document
    {
        get
        {
            XRDDocument result = new XRDDocument();
            set_all_ (cursor_.Select (expires_exp_), result.expires_);
        }
    }

    private void set_all_ <T> (XPathNodeIterator iter, List <T> list)
    {
        while (iter.MoveNext())
            list.Add (iter.Current.Value);
    }

    public static void Main()
    {

    }
}
