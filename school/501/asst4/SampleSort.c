/*
//			CPSC 501 -- Assignment 4
//
//		A simple version of the parallel sorting algorithm Sample Sort.
//
//			Ryan McDougall -- 2002
//
//	Sources:	http://www.umiacs.umd.edu/research/EXPAR/papers/spaa96/spaa96.html
//			http://bitscap.bits-pilani.ac.in/param/
//
//--------------------------------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"



/*==================================================================================================
*/
#define SIZE 10



/*==================================================================================================
*/
static int integer_compare(const void *i, const void *j)
{
	if( *(int*)i > *(int*)j )
		return 1;
	if( *(int*)i < *(int*)j )
		return -1;
		
	return (0);
}



/*==================================================================================================
*/
main (int argc, char *argv[])
{
	/* Variable Declarations */

	int		num_processors, this_rank, root_rank = 0;
	int		i, j, k, num_elements, num_elements_block,
				  NoElementsToSort;
	int		count, temp;
	int		*local_data, *data_segment;
	int		*Splitter, *AllSplitter;
	int		*Buckets, *BucketBuffer, *LocalBucket;
	int		*OutputBuffer, *Output;
	
	double		end_time, start_time = 0.0;
	
	FILE		*local_dataFile, *fp;
	
	MPI_Status	status; 



	/**** Initialising ****/
	
	MPI_Init( &argc, &argv );
	MPI_Comm_size( MPI_COMM_WORLD, &num_processors );
	MPI_Comm_rank( MPI_COMM_WORLD, &this_rank );


	if( argc != 2 ) 
	{
		if( this_rank == root_rank ) printf( " Usage : run size\n" );
		MPI_Finalize();
		exit(0);
	}

	
	
	/**** Reading local_data ****/
  
	if( this_rank == root_rank )
	{
		start_time = MPI_Wtime();
		
		num_elements = atoi(argv[1]);
		
		local_data = (int *) malloc ( num_elements *sizeof(int));
		
		if( local_data == NULL )
		{
			printf("Error : Can not allocate memory \n");
    		}

  		
		/* Initialise random number generator  */ 
		
		printf ( "local_data for sorting \n\n ");
		srand48( (unsigned int) num_elements );
		
		for( i=0; i < num_elements; i++ )
		{
			local_data[i] = lrand48() % ( 2 * num_elements );
    		}
  	}
  	printf ( "\n\n ");
	
	

	/**** Sending Data ****/

	MPI_Bcast( &num_elements, 1, MPI_INT, 0, MPI_COMM_WORLD );

	if( ( num_elements % num_processors ) != 0 )
	{
		if(this_rank == root_rank)
		{
			printf("Number of Elements are not divisible by num_processors \n");
	        	MPI_Finalize();
			exit(0);
		}
	}


	num_elements_block	= num_elements / num_processors;
	data_segment		= (int*) malloc ( num_elements * sizeof(int) );


	MPI_Scatter( local_data, num_elements_block, MPI_INT, data_segment, 
			num_elements_block, MPI_INT, root_rank, MPI_COMM_WORLD );


	/**** Sorting Locally ****/
	
	qsort( (char*) data_segment, num_elements_block, sizeof(int), integer_compare );
	
	

	/**** Choosing Local Splitters ****/
	
	Splitter = (int*) malloc( sizeof(int) * ( num_processors-1 ) );
	
	for (i=0; i< (num_processors-1); i++)
	{
		Splitter[i] = data_segment[ num_elements/(num_processors*num_processors) * (i+1)];
	}
	
	 

	/**** Gathering Local Splitters at root_rank ****/
	
	AllSplitter = (int*) malloc( sizeof(int) * num_processors * ( num_processors-1 ) );
	
	MPI_Gather( Splitter, num_processors-1, MPI_INT, AllSplitter, num_processors-1, 
				  MPI_INT, root_rank, MPI_COMM_WORLD );
				  


	/**** Choosing Global Splitters ****/
	
	if( this_rank == root_rank )
	{
		qsort( (char*) AllSplitter, num_processors * (num_processors-1), sizeof(int), integer_compare );

		for (i=0; i<num_processors-1; i++)
		{
			Splitter[i] = AllSplitter[(num_processors-1)*(i+1)];
		}
	}

  
	/**** Broadcasting Global Splitters ****/
	
	MPI_Bcast (Splitter, num_processors-1, MPI_INT, 0, MPI_COMM_WORLD);

	

	/**** Creating num_processors Buckets locally ****/
	
	Buckets = (int *) malloc (sizeof (int) * ( num_elements + num_processors));
  
	j = 0;
	k = 1;

	for (i=0; i< num_elements_block; i++)
	{
		if(j < (num_processors-1))
		{
			if (data_segment[i] < Splitter[j])
			{
				Buckets[(( num_elements_block + 1) * j) + k++] = data_segment[i]; 
			}
       			else
			{
				Buckets[( num_elements_block + 1) * j] = k-1;
				k=1;
				j++;
				i--;
       			}
    		}
		else
		{
			Buckets[(( num_elements_block + 1) * j) + k++] = data_segment[i];
		}
  	}
	
 	Buckets[( num_elements_block + 1) * j] = k - 1;
	
	
      
	/**** Sending buckets to respective processors ****/

	BucketBuffer = (int *) malloc (sizeof (int) * ( num_elements + num_processors));

	MPI_Alltoall (Buckets,num_elements_block + 1, MPI_INT, BucketBuffer, 
					num_elements_block + 1, MPI_INT, MPI_COMM_WORLD);
					

	/**** Rearranging BucketBuffer ****/
	
	LocalBucket = (int *) malloc (sizeof (int) * 2 *num_elements / num_processors);

	count = 1;
  	for (j=0; j<num_processors; j++)
	{
		k = 1;
		for (i=0; i<BucketBuffer[( num_elements/num_processors + 1) * j]; i++) 
		{
			LocalBucket[count++] = BucketBuffer[( num_elements/num_processors + 1) * j + k++];
		}
  	}

	LocalBucket[0] = count-1;
	
	
	
	/**** Sorting Local Buckets ****/

	NoElementsToSort = LocalBucket[0];
	qsort ((char *) &LocalBucket[1], NoElementsToSort, sizeof(int), integer_compare); 



	/**** Gathering sorted sub blocks at root ****/
	
	if(this_rank == root_rank) 
	{
  		OutputBuffer = (int *) malloc (sizeof(int) * 2 *num_elements);
  		Output = (int *) malloc (sizeof (int) *num_elements);
	}
	
	MPI_Gather( LocalBucket, 2* num_elements_block, MPI_INT, OutputBuffer, 
			  2* num_elements_block, MPI_INT, root_rank, MPI_COMM_WORLD );

	
	/**** Rearranging output buffer ****/
	if( this_rank == root_rank )
	{
		count = 0;
		
		for(j=0; j<num_processors; j++)
		{
			k = 1;

			for(i=0; i<OutputBuffer[(2 *num_elements/num_processors) * j]; i++) 
			{
				Output[count++] = OutputBuffer[(2* num_elements/num_processors) * j + k++];
			}
    		}
		
		/**** Print the time elapsed ****/
		end_time = MPI_Wtime();
		printf( "Sort took %f Wall time, on %i processors.\n",
				end_time-start_time, num_processors );

		/**** Printng the output ****/
		
		if ((fp = fopen("sorted_data_out", "w")) == NULL)
		{
         		printf("Can't Open Output File \n");
      			exit(0);
    		}
		 
    		fprintf (fp, "Number of Elements to be sorted : %d \n",num_elements);

    		fprintf (fp, "The sorted sequence is : \n");

    		for (i=0; i< num_elements; i++)
		{
	      		fprintf(fp, "%d\n", Output[i]);
		}

		fclose(fp);
    		free(local_data);
  		free(OutputBuffer);
  		free(Output);
	}

  	free(data_segment);
  	free(Splitter);
  	free(AllSplitter);
  	free(Buckets);
  	free(BucketBuffer);
  	free(LocalBucket);

   /**** Finalize ****/
   MPI_Finalize();
}



