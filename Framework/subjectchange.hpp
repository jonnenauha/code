/* class.hpp -- class definition
 *
 *			Ryan McDougall -- 2009
 */

#ifndef _SUBJECT_CHANGE_HPP_
#define _SUBJECT_CHANGE_HPP_

namespace Framework
{
    struct SubjectChange
    {
        change_mask_t    *data;
        const size_t    data_size;
        const size_t    data_block_size;

        SubjectChange () :
            data_size (128), data_block_size (sizeof(change_mask_t)*8)
        {
            assert (data_block_size != 0);
            assert ((data_size % data_block_size) == 0);
            data = new change_mask_t [data_size/data_block_size];
        }


        SubjectChange (size_t s) :
            data_size (s), data_block_size (sizeof(change_mask_t)*8)
        {
            assert (data_block_size != 0);
            assert ((data_size % data_block_size) == 0);
            data = new change_mask_t [data_size/data_block_size];
        }

        ~SubjectChange () { delete [] data; }

        void SetBit (index_t i)
        {
            assert (i < data_size);
            index_t b (i / data_block_size);
            index_t n (i % data_block_size);

            data[b] |= (1 << n);
        }

        bool TestBit (index_t i)
        {
            assert (i < data_size);
            index_t b (i / data_block_size);
            index_t n (i % data_block_size);

            return (data[b] & (1 << n));
        }
    };
}

#endif //_SUBJECT_CHANGE_HPP_
