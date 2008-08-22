/* class.hpp -- class definition
 *
 *			Ryan McDougall -- 20060101
 */

#ifndef _LINITERATOR_HPP_
#define _LINITERATOR_HPP_

namespace ThreeDInc
{
    template <typename> class linear_iterator_base;
    template <typename,size_t> class linear_iterator;
    template <typename,size_t,size_t> class linear_transpose_iterator;
    template <typename,size_t,size_t> class matrix_iterator;
    template <typename,size_t,size_t> class matrix_transpose_iterator;

    template <typename IteratorType>
	struct linear_iterator_traits
	{
	    typedef typename IteratorType::iterator_category iterator_category;
	    
	    typedef typename IteratorType::value_type	value_type;
	    typedef typename IteratorType::pointer 	pointer;
	    typedef typename IteratorType::reference 	reference;
	    
	    typedef typename IteratorType::difference_type	difference_type;
	    typedef typename IteratorType::size_type 		size_type;
	    
	    static const size_type offset = IteratorType::offset;
	};
    
    template <typename IteratorType>
	struct matrix_iterator_traits
	{
	    typedef typename IteratorType::iterator_category iterator_category;
	    
	    typedef typename IteratorType::value_type	value_type;
	    typedef typename IteratorType::pointer 	pointer;
	    typedef typename IteratorType::reference 	reference;
	    
	    typedef typename IteratorType::difference_type	difference_type;
	    typedef typename IteratorType::size_type 		size_type;
	    
	    typedef typename IteratorType::base_value_type	base_value_type;
	    typedef typename IteratorType::base_pointer		base_pointer;
	    typedef typename IteratorType::base_reference	base_reference;

	    static const size_type item_offset = IteratorType::item_offset;
	    static const size_type line_offset = IteratorType::line_offset;
	    static const size_type size = IteratorType::size;
	};

    template<typename T>
	struct linear_iterator_traits<T*>
	{
	    typedef std::random_access_iterator_tag iterator_category;

	    typedef T		value_type;
	    typedef T*		pointer;
	    typedef T&		reference;
	    
	    typedef ptrdiff_t	difference_type;
	    typedef size_t	size_type;
	};

    template<typename T>
	struct linear_iterator_traits<const T*>
	{
	    typedef std::random_access_iterator_tag iterator_category;

	    typedef T		value_type;
	    typedef const T*	pointer;
	    typedef const T&	reference;
	    
	    typedef ptrdiff_t	difference_type;
	    typedef size_t	size_type;
	};

    template <typename T>
	class linear_iterator_base
	{
	    public:
		typedef std::random_access_iterator_tag iterator_category;

		typedef T 		value_type;
		typedef value_type*	pointer;
		typedef value_type&	reference;
		
		typedef ptrdiff_t	difference_type;
		typedef size_t 		size_type;

		linear_iterator_base () throw() : ptr_ (NULL) {}
		linear_iterator_base (const linear_iterator_base& copy) throw() : ptr_ (copy.ptr_) {}
		explicit linear_iterator_base (pointer p) throw() : ptr_ (p) {}

		bool operator == (const linear_iterator_base& rhs) const throw() { return ptr_ == rhs.ptr_; }
		bool operator < (const linear_iterator_base& rhs) const throw() { return ptr_ < rhs.ptr_; }

		operator pointer () throw() { return ptr_; }

		const pointer base () const throw() { return ptr_; }

	    protected:
		pointer	ptr_;
	};

    template <typename T, size_t Offset>
	class linear_iterator 
	: public linear_iterator_base <T>
	{
	    private:
		typedef linear_iterator this_type;
		typedef linear_iterator_base <T> parent_type;

	    public: 
		typedef std::random_access_iterator_tag iterator_category;

		typedef size_t size_type;
		static const size_type offset = Offset;

		typedef T 		value_type;
		typedef ptrdiff_t 	difference_type;
		typedef value_type*	pointer;
		typedef value_type&	reference;

		linear_iterator () throw() : parent_type () {}
		linear_iterator (const this_type& copy) throw() : parent_type (copy) {}
		linear_iterator (const linear_iterator_base<T>& copy) throw() : parent_type (copy) {}
		explicit linear_iterator (pointer p) throw() : parent_type (p) {}

		this_type& operator = (const this_type& rhs) throw() { this->ptr_ = rhs.ptr_; return *this; }

		this_type& operator ++ () throw() { this->ptr_ += offset; return *this; }
		this_type& operator -- () throw() { this->ptr_ -= offset; return *this; }
		this_type operator ++ (int) throw() { this->ptr_ += offset; return this_type (this->ptr_-offset); }
		this_type operator -- (int) throw() { this->ptr_ -= offset; return this_type (this->ptr_+offset); }

		this_type& operator += (const difference_type& n) throw() { this->ptr_ += n*offset; return *this; }
		this_type& operator -= (const difference_type& n) throw() { this->ptr_ -= n*offset; return *this; }

		this_type operator + (const difference_type& n) const throw() { return this_type (this->ptr_ + n*offset); }
		this_type operator - (const difference_type& n) const throw() { return this_type (this->ptr_ - n*offset); }

		reference operator [] (const difference_type& n) const throw() { return  *(this->ptr_ + n*offset); }
		reference operator * () const throw() { return *(this->ptr_); }
		pointer operator -> () const throw() { return this; }
	};

    template <typename T, size_t NumLines, size_t NumItems>
	class linear_transpose_iterator 
	: public linear_iterator_base <T>
	{
	    private:
		typedef linear_transpose_iterator this_type;
		typedef linear_iterator_base <T> parent_type;

	    public: 
		typedef std::random_access_iterator_tag iterator_category;

		typedef size_t size_type;
		static const size_type offset = NumItems;
		static const size_type size = NumLines * NumItems;

		typedef T 		value_type;
		typedef ptrdiff_t 	difference_type;
		typedef value_type*	pointer;
		typedef value_type&	reference;

		linear_transpose_iterator () throw() : parent_type (), seqn_ (0) {}
		linear_transpose_iterator (const this_type& copy) throw() : parent_type (copy), seqn_ (copy.seqn_) {}
		linear_transpose_iterator (const linear_iterator_base<T>& copy) throw() : parent_type (copy), seqn_ (0) {}
		explicit linear_transpose_iterator (pointer p) throw() : parent_type (p), seqn_ (0) {}

		this_type& operator = (const this_type& rhs) throw() 
		{ 
		    this->ptr_ = rhs.ptr_; 
		    this->seqn_ = rhs.seqn_; 
		    return *this; 
		}

		this_type& operator ++ () throw() { ++seqn_; return *this; }
		this_type& operator -- () throw() { --seqn_; return *this; }
		this_type operator ++ (int) throw() { this_type t (*this); ++seqn_; return t; }
		this_type operator -- (int) throw() { this_type t (*this); --seqn_; return t; }

		this_type& operator += (const difference_type& n) throw() { seqn_ += n; return *this; }
		this_type& operator -= (const difference_type& n) throw() { seqn_ -= n; return *this; }

		this_type operator + (const difference_type& n) const throw() { this_type t (*this); return t += n; }
		this_type operator - (const difference_type& n) const throw() { this_type t (*this); return t -= n; }

		reference operator [] (const difference_type& n) const throw() { difference_type i (index_(n)); return this->ptr_ [i]; }
		reference operator * () const throw() { difference_type i (index_(0)); return this->ptr_ [i]; }
		pointer operator -> () const throw() { difference_type i (index_(0)); return this->ptr_ + i; }

		bool operator == (const this_type& rhs) const throw() { return this->ptr_ == rhs.ptr_ && seqn_ == rhs.seqn_; }
		bool operator < (const this_type& rhs) const throw() { return this->ptr_ == rhs.ptr_ && seqn_ < rhs.seqn_; }

		bool operator == (const linear_iterator_base<T>& rhs) const throw() { return (this->ptr_ + index_(0)) == rhs.base(); }
		bool operator < (const linear_iterator_base<T>& rhs) const throw() { return (this->ptr_ + index_(0)) < rhs.base(); }

	    private:
		inline difference_type index_ (difference_type n) const throw() 
		{
		    return (seqn_ < size-1)? ((seqn_ + n) * offset) % (size-1)
			: (seqn_ == size-1)? seqn_ 
			: size;
		}

	    private:
		difference_type seqn_;
	};

    template <typename T, size_t NumLines, size_t NumItems>
	class matrix_iterator 
	{
	    private:
		typedef matrix_iterator this_type;

	    public: 
		typedef std::random_access_iterator_tag iterator_category;

		typedef size_t size_type;
		static const size_type item_offset = 1;
		static const size_type line_offset = NumLines;
		static const size_type size = NumLines * NumItems;

		typedef linear_iterator <T,item_offset>	value_type;
		typedef ptrdiff_t 		difference_type;
		typedef value_type*		pointer;
		typedef value_type&		reference;

		typedef T 			base_value_type;
		typedef base_value_type*	base_pointer;
		typedef base_value_type&	base_reference;

		matrix_iterator () throw() : ptr_ (), seqn_ (0) {}
		matrix_iterator (const this_type& copy) throw() : ptr_ (copy.ptr_), seqn_ (copy.seqn_) {}
		matrix_iterator (const linear_iterator_base<T>& copy) throw() : ptr_ (copy.ptr_), seqn_ (0) {}
		explicit matrix_iterator (base_pointer p) throw() : ptr_ (p), seqn_ (0) {}

		this_type& operator = (const this_type& rhs) throw() 
		{ 
		    ptr_ = rhs.ptr_; 
		    seqn_ = rhs.seqn_; 
		    return *this; 
		}

		this_type& operator ++ () throw() { ++seqn_; return *this; }
		this_type& operator -- () throw() { --seqn_; return *this; }
		this_type operator ++ (int) throw() { this_type t (*this); ++seqn_; return t; }
		this_type operator -- (int) throw() { this_type t (*this); --seqn_; return t; }

		this_type& operator += (const difference_type& n) throw() { seqn_ += n; return *this; }
		this_type& operator -= (const difference_type& n) throw() { seqn_ -= n; return *this; }

		this_type operator + (const difference_type& n) const throw() { this_type t (*this); return t += n; }
		this_type operator - (const difference_type& n) const throw() { this_type t (*this); return t -= n; }

		value_type operator [] (const difference_type& n) const throw() { return value_type (ptr_ + index_(n)); }
		value_type operator * () const throw() { return value_type (ptr_ + index_(0)); }

		bool operator == (const this_type& rhs) const throw() { return ptr_ == rhs.ptr_ && seqn_ == rhs.seqn_; }
		bool operator < (const this_type& rhs) const throw() { return ptr_ == rhs.ptr_ && seqn_ < rhs.seqn_; }

		bool operator == (const linear_iterator_base<T>& rhs) const throw() { return (ptr_ + index_(0)) == rhs.base(); }
		bool operator < (const linear_iterator_base<T>& rhs) const throw() { return (ptr_ + index_(0)) < rhs.base(); }

	    private:
		inline difference_type index_ (difference_type n) const throw() 
		{ return (seqn_ + n) * line_offset; }

	    protected:
		base_pointer	ptr_;
		difference_type seqn_;
	};

    template <typename T, size_t NumLines, size_t NumItems>
	class matrix_transpose_iterator
	{
	    private:
		typedef matrix_transpose_iterator this_type;

	    public: 
		typedef std::random_access_iterator_tag iterator_category;
		
		typedef size_t size_type;
		static const size_type item_offset = NumItems;
		static const size_type line_offset = 1;
		static const size_type size = NumLines * NumItems;

		typedef linear_iterator <T,item_offset>	value_type;
		typedef ptrdiff_t 		difference_type;
		typedef value_type*		pointer;
		typedef value_type&		reference;

		typedef T 			base_value_type;
		typedef base_value_type*	base_pointer;
		typedef base_value_type&	base_reference;

		matrix_transpose_iterator () throw() : ptr_ (), seqn_ (0) {}
		matrix_transpose_iterator (const this_type& copy) throw() : ptr_ (copy.ptr_), seqn_ (copy.seqn_) {}
		matrix_transpose_iterator (const linear_iterator_base<T>& copy) throw() : ptr_ (copy.ptr_), seqn_ (0) {}
		explicit matrix_transpose_iterator (base_pointer p) throw() : ptr_ (p), seqn_ (0) {}
		
		matrix_transpose_iterator& operator = (const this_type& rhs) throw() 
		{ 
		    this->ptr_ = rhs.ptr_; 
		    this->seqn_ = rhs.seqn_; 
		    return *this; 
		}

		this_type& operator ++ () throw() { ++seqn_; return *this; }
		this_type& operator -- () throw() { --seqn_; return *this; }
		this_type operator ++ (int) throw() { this_type t (*this); ++seqn_; return t; }
		this_type operator -- (int) throw() { this_type t (*this); --seqn_; return t; }

		this_type& operator += (const difference_type& n) throw() { seqn_ += n; return *this; }
		this_type& operator -= (const difference_type& n) throw() { seqn_ -= n; return *this; }

		this_type operator + (const difference_type& n) const throw() { this_type t (*this); return t += n; }
		this_type operator - (const difference_type& n) const throw() { this_type t (*this); return t -= n; }

		value_type operator [] (const difference_type& n) const throw() { return value_type (ptr_ + index_(n)); }
		value_type operator * () const throw() { return value_type (ptr_ + index_(0)); }

		bool operator == (const this_type& rhs) const throw() { return ptr_ == rhs.ptr_ && seqn_ == rhs.seqn_; }
		bool operator < (const this_type& rhs) const throw() { return ptr_ == rhs.ptr_ && seqn_ < rhs.seqn_; }

		bool operator == (const linear_iterator_base<T>& rhs) const throw() { return (ptr_ + index_(0)) == rhs.base(); }
		bool operator < (const linear_iterator_base<T>& rhs) const throw() { return (ptr_ + index_(0)) < rhs.base(); }

	    private:
		inline difference_type index_ (difference_type n) const throw() 
		{ return (this->seqn_ < NumItems)? this->seqn_ + n : size; }
	    
	    protected:
		base_pointer	ptr_;
		difference_type seqn_;
	};
}

#endif //_LINITERATOR_HPP_
