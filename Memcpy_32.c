
#include <stdio.h>
#include <stddef.h>
// needed for memset()
#include <string.h> 
#include <assert.h>

#define WORD_BOUNDARY_SIZE_IN_BYTES 4

size_t Memcpy_32 (void *pDest,const void *pSrc, size_t NumBytes){

    size_t NumBytesCoppied = 0;
    // initial alignment to Word boundary
    int InitialAlignmentSrc = ((size_t)pSrc) % WORD_BOUNDARY_SIZE_IN_BYTES;
    int InitialAlignmentDest= ((size_t)pDest) % WORD_BOUNDARY_SIZE_IN_BYTES;

    __uint8_t *pCharDest = pDest;
    const __uint8_t *pCharSrc = pSrc;

    // debug printf
    printf("Src and Dest pointers misaligned from Word boundaries by %d and %d bytes\n",InitialAlignmentSrc,InitialAlignmentDest);

    if (InitialAlignmentSrc == InitialAlignmentDest) {

        if (InitialAlignmentSrc != 0){
            
            printf("Copying %lu bytes to align to word boundaries...\n", WORD_BOUNDARY_SIZE_IN_BYTES - InitialAlignmentDest);

            for (size_t i = 0; ((i < (WORD_BOUNDARY_SIZE_IN_BYTES - InitialAlignmentDest)) && (NumBytesCoppied < NumBytes)); i++)
            {   // copy one byte at a time, slow but works around alignment issues.
                pCharDest[i] = pCharSrc[i];
                NumBytesCoppied++;
            }
        }

        // verify that we are aligned after this initial few bytes copy...
        assert( ((size_t)(&pCharDest[NumBytesCoppied])) % WORD_BOUNDARY_SIZE_IN_BYTES == 0);

        // If the pointers are aligned on word boundaries, and there is enough number of bytes to copy remaining
        //  to do this at least once without overflowing...
        if ((InitialAlignmentSrc == InitialAlignmentDest) && (NumBytesCoppied <= (NumBytes - sizeof(unsigned int))))
        {

            // assuming an architecture "word" length is equivalent to "unsigned int"
            // Logic note: the NumBytesCoppied guard above prevents neagtive results from wrap-around to a large number !
            size_t NumWordCopy =  (NumBytes - NumBytesCoppied) / sizeof(unsigned int);

            // debug printf
            printf("Copy by Word: %lu times\n",NumWordCopy);

            // Pickup pointers _starting_from_the_new_alignment_...
            unsigned int *pUIntDest = &pCharDest[NumBytesCoppied];
            const unsigned int *pUIntSrc = &pCharSrc[NumBytesCoppied];

            for (size_t i = 0; i < NumWordCopy; i++)
            {   // copy one word at a time
                pUIntDest[i] = pUIntSrc[i];
                NumBytesCoppied += sizeof(unsigned int);
            }
        }
    }
    else{
        printf("Misaligned copy Source vs Destination Pointers, unable to copy-by-Word...\n");
    }

    // This loop should start from NumBytesCoppied, not restarting from another "Remainder bytes" calculation!

    // debug printf
    printf("Copy remaining %lu bytes\n",NumBytes - NumBytesCoppied);

    // at the end of that, pickup any remaining bytes to finish out the copy...
    while (NumBytesCoppied < NumBytes)
    {   // copy one byte at a time, slow but works around alignment issues.
        pCharDest[NumBytesCoppied] = pCharSrc[NumBytesCoppied];
        NumBytesCoppied++;
    }

    return NumBytesCoppied;

} // end Memcpy_32 function


void main (void){

    const size_t TestBufferSize = 4096;

    __uint8_t TestBuffer[TestBufferSize];

    __uint8_t DestBuffer[TestBufferSize];

    // setup a test pattern in the buffer so we can verify copy
    for (size_t i = 0; i < TestBufferSize;i++) {
        TestBuffer[i] = i % __UINT8_MAX__;
    }

    ////////////  Test 1 /////////////////////////////////////////////////////

    // zero out the destination to verify we copy data into it...
    memset(&DestBuffer,0,TestBufferSize);

    size_t RetVal =  Memcpy_32(DestBuffer,TestBuffer,TestBufferSize);


    printf("Checking results...\n");
    // Verify the test pattern still appears in the destination
    for (size_t i = 0; i < TestBufferSize;i++) {
        assert (DestBuffer[i] == (i % __UINT8_MAX__));
    }
    assert (RetVal == TestBufferSize);
    printf("Test 1 Results OK\n\n");

    ////////////  Test 2 /////////////////////////////////////////////////////

    // zero out the destination to verify we copy data into it...
    memset(&DestBuffer,0,TestBufferSize);

    // intentionally misalign both the Src and Dest buffers by 1 byte
    RetVal =  Memcpy_32(&DestBuffer[1],&TestBuffer[1],TestBufferSize-1);


    printf("Checking results...\n");
    // Verify the test pattern still appears in the destination
    // (Skip the first byte which we intentionally did not copy)
    for (size_t i = 1; i < TestBufferSize - 1;i++) {
        assert (DestBuffer[i] == (i % __UINT8_MAX__));
    }
    assert (RetVal == TestBufferSize-1);
    printf("Test 2 Results OK\n\n");

    ////////////  Test 3 /////////////////////////////////////////////////////

    // zero out the destination to verify we copy data into it...
    memset(&DestBuffer,0,TestBufferSize);

    // intentionally misalign both the Src and Dest buffers by different bytes
    RetVal =  Memcpy_32(&DestBuffer[1],&TestBuffer[3],TestBufferSize-3);


    printf("Checking results...\n");
    // Verify the test pattern still appears in the destination
    // (Skip the first byte which we intentionally did not copy)
    for (size_t i = 1; i < TestBufferSize - 3;i++) {
        // They are "skewed" by 2 bytes difference, so (i+2) before modulo, to check proper results
        assert (DestBuffer[i] == ((i+ 2) % __UINT8_MAX__));
    }
    assert (RetVal == TestBufferSize-3);
    printf("Test 3 Results OK\n\n");



} // end main() function



// Example output:
/*

./Memcpy_32
Src and Dest pointers misaligned from Word boundaries by 0 and 0 bytes
Copy by Word: 1024 times
Copy remaining 0 bytes
Checking results...
Test 1 Results OK

Src and Dest pointers misaligned from Word boundaries by 1 and 1 bytes
Copying 3 bytes to align to word boundaries...
Copy by Word: 1023 times
Copy remaining 0 bytes
Checking results...
Test 2 Results OK

Src and Dest pointers misaligned from Word boundaries by 3 and 1 bytes
Misaligned copy Source vs Destination Pointers, unable to copy-by-Word...
Copy remaining 4093 bytes
Checking results...
Test 3 Results OK


*/