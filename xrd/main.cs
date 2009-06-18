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
            private string relation_;
            private string uri_template_;
            private Uri uri_;
            private string media_type_;

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

        private string expires_; //TODO: should be a time data type
        private string subject_; 
        private List <string> aliases_ = new List <string> (); 
        private List <string> types_ = new List <string> (); 
        private List <XRDLink> links_ = new List <XRDLink> (); 
        
        public string Expires { get { return expires_; } }
        public string Subject { get { return subject_; } }
        public List <string> Aliases { get { return aliases_; } }
        public List <string> Types { get { return types_; } }
        public List <XRDLink> Links { get { return links_; } }
    }

    private XPathDocument doc_;
    private XPathNavigator cursor_;

    private XRDDocument result_;
    private bool parsed_ = false;

    private XPathExpression expires_exp_;
    private XPathExpression subject_exp_;
    private XPathExpression aliases_exp_;
    private XPathExpression types_exp_;
    
    public XRDParser (Stream xrd)
    {
        doc_ = new XPathDocument (xrd);
        result_ = new XRDDocument ();
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
            if (!parsed_)
            {
                var expires = get_all_ (cursor_.Select (expires_exp_));
                var subject = get_all_ (cursor_.Select (subject_exp_));
                var aliases = get_all_ (cursor_.Select (aliases_exp_));
                var types = get_all_ (cursor_.Select (types_exp_));
            }

            return result_;
        }
    }

    private List <string> get_all_ (XPathNodeIterator iter)
    {
        var list = new List <string> ();
        while (iter.MoveNext())
            list.Add (iter.Current.Value);
        return list;
    }

    public static void Main()
    {

    }
}
