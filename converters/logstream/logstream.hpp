/* class.hpp -- class definition
 *
 *			Ryan McDougall -- 20060101
 */

#ifndef _LOGSTREAM_HPP_
#define _LOGSTREAM_HPP_

//=============================================================================
//
#include <iostream>
#include <ostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <functional>
#include <cassert>

//-----------------------------------------------------------------------------
// Logging levels
enum log_level_t
{
    NONE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
};

template <typename C, typename T> class basic_logstream_factory;

//-----------------------------------------------------------------------------
// Extended traits class for logstream
template <typename CharType>
struct extended_char_traits
{
    static std::basic_streambuf <CharType,std::char_traits<CharType> >	
	*default_rdbuf = NULL;
    
    static const std::basic_string<CharType,std::char_traits<CharType> > 
	root_name;
};

template <>
struct extended_char_traits <char>
{
    static std::basic_streambuf <char,std::char_traits<char> >	
	*default_rdbuf;

    static const std::basic_string <char,std::char_traits<char> >
	root_name;
};

    	
template <>
struct extended_char_traits <wchar_t>
{
    static std::basic_streambuf <wchar_t,std::char_traits<wchar_t> >	
	*default_rdbuf;
    
    static const std::basic_string <wchar_t,std::char_traits<wchar_t> >
	root_name;
};

//-----------------------------------------------------------------------------
// Extend std::ostream to not log certain levels
// Declare operator<< externally to avoid hiding inherited methods

template <typename CharType, typename TraitType=std::char_traits<CharType> >
class basic_logstream : 
    public std::basic_ostream <CharType,TraitType>
{
    public:
	typedef std::basic_string <CharType,TraitType>		basic_string_type;
	typedef std::basic_ostream <CharType,TraitType>		basic_ostream_type;
	typedef std::basic_streambuf <CharType,TraitType>	basic_streambuf_type;
	typedef std::basic_ios <CharType,TraitType>		basic_ios_type;

	// constructor for writing to an existing rdbuf 
	// taken from an existing std::ostream
	explicit basic_logstream 
	    (const basic_string_type& name, log_level_t level, 
	     basic_ostream_type& o, 
	     basic_logstream* p = NULL) : 
		basic_ostream_type (o.rdbuf()), 
		name_ (name),
		level_ (level), 
		parent_ (p),
		bufhandl_ (NULL) 
	{ msgcontext_.push (NONE); }

	// constructor for writing to a shared external streambuf
	explicit basic_logstream 
	    (const basic_string_type& name, log_level_t level, 
	     basic_streambuf_type* o, 
	     basic_logstream* p = NULL) : 
		basic_ostream_type (o), 
		name_ (name),
		level_ (level), 
		parent_ (p),
		bufhandl_ (NULL)
	{ msgcontext_.push (NONE); }

	// constructor for writing to an external streambuf
	// that we assume ownership of (via auto_ptr)
	explicit basic_logstream 
	    (const basic_string_type& name, log_level_t level, 
	     std::auto_ptr<basic_streambuf_type> o, 
	     basic_logstream* p = NULL) : 
		basic_ostream_type (o.get()), 
		name_ (name),
		level_ (level), 
		parent_ (p),
		bufhandl_ (o)
	{ msgcontext_.push (NONE); }

	~basic_logstream () {} 

	// accessor and mutator methods
	
	// current logging level
	log_level_t level () const { return level_; }
	
	// the context for messages coming into the stream
	log_level_t context () const 
	{ 
	    assert (msgcontext_.size()); 
	    return msgcontext_.top(); 
	}

	// push a new context for incoming messages
	basic_logstream& push_context (log_level_t level) 
	{ 
	    msgcontext_.push (level); 
	    if (parent_) parent_->push_context (level);

	    return *this; 
	}

	// pop the current message context from the top
	basic_logstream& pop_context ()
	{
	    assert (msgcontext_.size());
	    msgcontext_.pop();
	    if (parent_) parent_->pop_context();

	    return *this;
	}

	basic_string_type name () const { return name_; }
	basic_logstream* parent () { return parent_; }

	friend class basic_logstream_factory <CharType,TraitType>;

    private:
	basic_string_type	name_;		// name of the stream
	log_level_t 		level_;		// level the stream is logging
	basic_logstream*	parent_;	// pointer to parent stream
	
	// stack of message context levels 
	// (since messages don't hold their own state, we track
	// input logging levels with a context stack)
	std::stack <log_level_t> 		msgcontext_;

	// handler for aquiring ownership of a rdbuf
	std::auto_ptr <basic_streambuf_type>	bufhandl_;

    private:
	basic_logstream();
	basic_logstream (const basic_logstream&);
	basic_logstream& operator = (const basic_logstream&);
};

// Standard types
typedef basic_logstream<char> logstream;
typedef basic_logstream<wchar_t> wlogstream;


//-----------------------------------------------------------------------------
// factory interface for constructing logstreams

template <typename CharType, typename TraitType=std::char_traits<CharType> >
class basic_logstream_factory
{
    public:
	typedef std::basic_string <CharType,TraitType> 		basic_string_type;
	typedef std::basic_streambuf <CharType,TraitType>	basic_streambuf_type;
	typedef basic_logstream <CharType,TraitType> 		basic_logstream_type;

	// internal tree Node for maintaining a namespace tree of loggers
	class Node
	{
	    public:
		typedef std::vector <Node*> child_list_type;

		// all nodes require a name, but a logstream can be added later
		Node (const basic_string_type& n, const basic_logstream_type *l = NULL) :
		    name (n), 
		    logger (l), 
		    buf (NULL), 
		    level (NONE) {}

		~Node () { delete logger; }

		const basic_string_type 	name;
		const basic_logstream_type	*logger;
		basic_streambuf_type		*buf;
		log_level_t			level;

		child_list_type 		children;

		static void dump (const Node* n, std::ostream& out)
		{
		    if (!n) return;

		    out << n->name << '[';

		    typename child_list_type::const_iterator i = n->children.begin();
		    typename child_list_type::const_iterator end = n->children.end();
		    while (i != end)
			dump (*i++, out);

		    out << ']';
		}
	};

	// Comparator object for comparing a Node's name given a pointer and string
	struct NodeCompare : 
	    public std::binary_function <const Node*, const basic_string_type, bool> {
		bool operator () (const Node *lhs, const basic_string_type& rhs) const { 
		    return (lhs->name == rhs); 
		}
	    };


    public:

	// Default Constructor -- every factory contains a root node
	basic_logstream_factory (basic_streambuf_type* buf = NULL, log_level_t l = NONE) : 
	    root_ (new Node (extended_char_traits <CharType>::root_name)), 
	    default_rdbuf_ (buf), 
	    default_level_ (l)
	{
	    // if no default streambuf is given, use std::(w)clog
	    if (!default_rdbuf_)
		default_rdbuf_ = extended_char_traits <CharType>::default_rdbuf;
	}

	// Destructor -- delete the tree recursivly
	~basic_logstream_factory() { delete_tree_ (root_); }

	// Factory method -- change the output buffer for the logger
	// mush be called before get_logstream, buffers cannot be live swapped
	// returns true on success
	// TODO: we can create an new logstream for the new buf, 
	// return the previous buf and expect the user to clean it up?
	
	void set_default_buffer (basic_streambuf_type* buf) { default_rdbuf_ = buf; }

	bool set_buffer (const basic_string_type& path, basic_streambuf_type* buf)
	{
	    Node *parent = NULL, *node = NULL;

	    find_node_ (path, parent, node);
	    node->buf = buf;

	    return (node->logger == NULL);
	}

	// Factory method -- change the log level for the logger
	basic_logstream_type* set_log_level (const basic_string_type& path, log_level_t level)
	{
	    basic_logstream_type *logger = NULL;
	    Node *parent = NULL, *node = NULL;

	    // lookup the correct node by path name
	    find_node_ (path, parent, node);

	    // set the level on the path node (for future loggers)
	    node->level = level;

	    // set the level on the logger if it exists
	    logger = const_cast <basic_logstream_type*> (node->logger);
	    if (logger)
		logger->level_ = level;

	    return logger;
	}

	// Factory method -- return a logger ptr for a unqiue path name
	basic_logstream_type& get_logstream (const basic_string_type& path)
	{
	    basic_logstream_type *logger = NULL;
	    Node *parent = NULL, *node = NULL;

	    find_node_ (path, parent, node);

	    // if there is no existing logger, create one
	    if (node->logger == NULL)
	    {
		log_level_t level = (node->level != NONE)? 
		    node->level : default_level_;

		basic_streambuf_type *buf = (node->buf)? 
		    node->buf : default_rdbuf_;

		basic_logstream_type *p = (parent)? 
		    const_cast <basic_logstream_type*> (parent->logger) : NULL;

		node->logger = new basic_logstream_type (path, level, buf, p);
	    }

	    logger = const_cast <basic_logstream_type*> (node->logger);

	    // make sure all parent-child relationships are maintained
	    typename Node::child_list_type::iterator i = node->children.begin();
	    typename Node::child_list_type::iterator end = node->children.end();
	    while (i != end)
		add_logger_to_children_ (*i++, logger);

	    return *logger;
	}

    private:
	
	// split a path into a list of names
	void split_ (const basic_string_type& s, CharType c, std::vector<basic_string_type>& v)
	{
	    typename basic_string_type::size_type begin = 0;
	    typename basic_string_type::size_type curr = s.find (c);
	    typename basic_string_type::size_type end = basic_string_type::npos;

	    while (curr != end)
	    {
		v.push_back (s.substr (begin, curr-begin));
		begin = ++curr;
		curr = s.find (c, curr);
	    }

	    v.push_back (s.substr (begin));
	}

	void add_logger_to_children_ (Node *n, basic_logstream_type *l)
	{
	    if (n->logger)
	    {
		basic_logstream_type *child = 
		    const_cast <basic_logstream_type*> (n->logger);

		child->parent_ = l;
		return;
	    }

	    typename Node::child_list_type::iterator i = n->children.begin();
	    typename Node::child_list_type::iterator end = n->children.end();
	    while (i != end)
		add_logger_to_children_ (*i++, l);
	}

	void find_node_ (const basic_string_type& path, Node*& parent, Node*& node)
	{
	    // split the path into an array by delimiter
	    std::vector <basic_string_type> pathlist;
	    split_ (path, path_delim, pathlist);

	    size_t size = pathlist.size();

	    node = root_;
	    Node *next = NULL;
	    typename Node::child_list_type::iterator iter;

	    // for each step in the path
	    for (size_t i=0; i < size; ++i)
	    {
		// look for existing node in the path
		iter = find_if (node->children.begin(), node->children.end(), 
				bind2nd (NodeCompare(), pathlist[i]));

		// if not found, create a new path
		if (iter == node->children.end())
		{
		    next = new Node (pathlist[i], NULL);
		    node->children.push_back (next);
		}
		else
		    next = *iter;

		// walk to the next branch in path
		node = next;

		// if node has a logger, consider it a possible parent
		if (node->logger)
		    parent = node;
	    }
	}

	void delete_tree_ (Node *n)
	{
	    if (n && n->children.size())
	    {
		typename Node::child_list_type::iterator i = n->children.begin();
		typename Node::child_list_type::iterator end = n->children.end();
		while (i != end)
		    delete_tree_ (*i++);
	    }

	    delete n;
	}

    private:
	Node			*root_;	// default root log node

	basic_streambuf_type	*default_rdbuf_;
	log_level_t 		default_level_;

	// TODO: support more delimiters ('/', '::')
	static const CharType 	path_delim = '.';

    private:
	basic_logstream_factory (const basic_logstream_factory&);
	basic_logstream_factory& operator = (const basic_logstream_factory&);
};


// Standard types
typedef basic_logstream_factory<char> logstream_factory;
typedef basic_logstream_factory<wchar_t> wlogstream_factory;

// Convenience function for getting factories and logstreams
template <typename C, typename T>
basic_logstream_factory<C,T> & get_logstream_factory ()
{
    static basic_logstream_factory<C,T> factory_;
    return factory_;
}

logstream& get_logstream (const std::string& path);
wlogstream& get_logstream (const std::wstring& path);


//-----------------------------------------------------------------------------
// Forward all input to ostream if logging is set at the given level
// and forward the message to the parent

// These forwarding funcitons implements the core logstream functionality: 
// pass the message on to the parent, and print only if the level matches

template <typename C, typename T, typename OutputType>
static inline void logstream_forward_output_ 
(basic_logstream <C,T>& out, const OutputType& msg)
{
    typedef std::basic_ostream <C,T> basic_ostream_type;
    typedef typename basic_ostream_type::sentry sentry;

    basic_logstream <C,T> *p = out.parent();
    if (p) *p << msg;

    if ((out.context() >= out.level()) && sentry (out))
	reinterpret_cast <basic_ostream_type&> 
	    (out) << msg;
}

template <typename C, typename T, typename OutputType>
static inline void logstream_forward_manip_ 
(basic_logstream <C,T>& out, const OutputType& msg)
{
    typedef std::basic_ostream <C,T> basic_ostream_type;

    basic_logstream <C,T> *p = out.parent();
    if (p) *p << msg;

    if (out.context() >= out.level())
	reinterpret_cast <basic_ostream_type&> 
	    (out) << msg;
}

// Interface for printing all primitive types
/*template <typename C, typename T, typename OutputType>
basic_logstream <C,T>& operator << 
(basic_logstream <C,T>& out, const OutputType& msg)
{
    logstream_forward_output_ (out, msg);
    return out;
}
*/

#endif //_LOGSTREAM_HPP_
