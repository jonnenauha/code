/* class.hpp -- class definition
 *
 *			Ryan McDougall -- 20070101
 */

#ifndef _CLASS_HPP_
#define _CLASS_HPP_

//=============================================================================
//
class Class
{
    //-------------------------------------------------------------------------
    public:
	// Constructors
	Class ()
	{}

	Class (const Class& rhs)
	{}

	// Virtual copy constructor
	virtual Class* clone () const
	{
	    return new Class (*this); 
	}

	// Destructor
	virtual ~Class ()
	{
	    try
	    {}
	    catch (...)
	    {}
	}

	// Assignment operator
	Class& operator= (const Class& rhs)
	{ 
	    if (this == &rhs)
		return *this;

	    Class t (rhs);
	    swap (*this, t);

	    return *this;
	}
	

    //-------------------------------------------------------------------------
    private:
	// Implementation variables

	// Implementation methods

	// Hidden methods
};

#endif //_CLASS_HPP_
