using System;
using System.IO;
using System.Xml;
using System.Xml.XPath;
using System.Collections.Generic;

public class XRDParseException : System.Exception
{
    public XRDParseException () : base () {}
    public XRDParseException (string msg) : base (msg) {}
    public XRDParseException (string msg, System.Exception inner) : base (msg, inner) {}
}

public class XRDParser
{
    public class XRDLink 
    {
        internal string relation_;
        internal string uri_template_;
        internal Uri uri_;
        internal string media_type_;

        internal XRDLink (string relation, Uri uri, string mediatype)
        {
            relation_ = relation;
            uri_ = uri;
            media_type_ = mediatype;
        }

        public string Relation { get { return relation_; } }
        public Uri Uri { get { return uri_; } }
        public string MediaType { get { return media_type_; } }
    }

    public class XRDDocument
    {
        internal string expires_; //TODO: should be a time data type
        internal string subject_; 
        internal List <string> aliases_ = new List <string> (); 
        internal List <string> types_ = new List <string> (); 
        internal List <XRDLink> links_ = new List <XRDLink> (); 
        
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
    private XPathExpression link_rel_exp_;
    private XPathExpression link_uri_exp_;
    private XPathExpression link_mediatype_exp_;
    
    public XRDParser (Stream xrd)
    {
        doc_ = new XPathDocument (xrd);
        result_ = new XRDDocument ();
        cursor_ = doc_.CreateNavigator();

        expires_exp_ = cursor_.Compile ("/XRD/Expires");
        subject_exp_ = cursor_.Compile ("/XRD/Subject");
        aliases_exp_ = cursor_.Compile ("/XRD/Alias");
        types_exp_ = cursor_.Compile ("/XRD/Type");

        link_rel_exp_ = cursor_.Compile ("/XRD/Link/Rel");
        link_uri_exp_ = cursor_.Compile ("/XRD/Link/URI");
        link_mediatype_exp_ = cursor_.Compile ("/XRD/Link/MediaType");
    }

    public XRDDocument Document
    {
        get
        {
            if (!parsed_) parse_ ();
            return result_;
        }
    }

    private void parse_ ()
    {
        var expires = get_all_ (cursor_.Select (expires_exp_));
        var subject = get_all_ (cursor_.Select (subject_exp_));
        var aliases = get_all_ (cursor_.Select (aliases_exp_));
        var types = get_all_ (cursor_.Select (types_exp_));

        var rel = get_all_ (cursor_.Select (link_rel_exp_));
        var uri = get_all_ (cursor_.Select (link_uri_exp_));
        var mediatype = get_all_ (cursor_.Select (link_mediatype_exp_));

        if ((expires.Count != 1) || (subject.Count != 1) || (aliases.Count < 1) || (types.Count < 1)) 
            throw new XRDParseException ();

        result_.expires_ = expires[0];
        result_.subject_ = subject[0];
        result_.aliases_ = aliases;
        result_.types_ = types;

        if ((rel.Count != uri.Count) || (uri.Count != mediatype.Count))
            throw new XRDParseException ();

        int nlinks = rel.Count;
        for (int i=0; i < nlinks; ++i)
            result_.links_.Add 
                (new XRDLink (rel[i], new Uri (uri[i]), mediatype[i]));
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
        var xrdfile = new FileStream ("xrd.xml", FileMode.Open);
        var parser = new XRDParser (xrdfile);
        var doc = parser.Document;
        var links = doc.Links;

        Console.WriteLine ("expires: " + doc.Expires);
        Console.WriteLine ("subject: " + doc.Subject);
        foreach (string s in doc.Aliases) Console.WriteLine ("aliases: " + s);
        foreach (string s in doc.Types) Console.WriteLine ("types: " + s);

        for (int i=0; i < links.Count; ++i)
        {
            Console.WriteLine ("link {0} rel: {1}", i, links[i].Relation);
            Console.WriteLine ("link {0} uri: {1}", i, links[i].Uri);
            Console.WriteLine ("link {0} mediatype: {1}", i, links[i].MediaType);
        }

    }
}
