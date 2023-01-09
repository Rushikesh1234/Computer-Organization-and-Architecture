{\rtf1\ansi\ansicpg1252\cocoartf2638
\cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fnil\fcharset0 Menlo-Regular;}
{\colortbl;\red255\green255\blue255;\red200\green20\blue201;\red180\green36\blue25;\red0\green0\blue0;
\red47\green180\blue29;\red193\green101\blue28;\red64\green11\blue217;}
{\*\expandedcolortbl;;\cssrgb\c83397\c23074\c82666;\cssrgb\c76411\c21697\c12527;\csgray\c0;
\cssrgb\c20241\c73898\c14950;\cssrgb\c80555\c47366\c13837;\cssrgb\c32309\c18666\c88229;}
\paperw11900\paperh16840\margl1440\margr1440\vieww18560\viewh10560\viewkind0
\pard\tx560\tx1120\tx1680\tx2240\tx2800\tx3360\tx3920\tx4480\tx5040\tx5600\tx6160\tx6720\pardirnatural\partightenfactor0

\f0\fs22 \cf2 \CocoaLigature0 #include \cf3 <stdio.h>\cf4 \
\cf2 #include \cf3 <stdint.h>\cf4 \
\cf2 #include \cf3 <string.h>\cf4 \
\cf2 #include \cf3 <math.h>\cf4 \
\cf2 #include \cf3 <time.h>\cf4 \
\cf2 #include \cf3 <stdlib.h>\cf4 \
\cf2 #include \cf3 "cachesim.h"\cf4 \
\
\cf5 struct\cf4  L1Cache\
\{\
    \cf5 unsigned\cf4  valid_field[\cf3 1024\cf4 ];\
    \cf5 unsigned\cf4  dirty_field[\cf3 1024\cf4 ];\
    \cf5 uint64_t\cf4  tag_field[\cf3 1024\cf4 ];\
    \cf5 char\cf4  data_field[\cf3 1024\cf4 ][\cf3 64\cf4 ];\
    \cf5 int\cf4  hits;\
    \cf5 int\cf4  misses;\
\};\
\
\cf5 struct\cf4  L2Cache\
\{\
    \cf5 unsigned\cf4  valid_field[\cf3 16384\cf4 ];\
    \cf5 unsigned\cf4  dirty_field[\cf3 16384\cf4 ];\
    \cf5 uint64_t\cf4  tag_field[\cf3 16384\cf4 ];\
    \cf5 char\cf4  data_field[\cf3 16384\cf4 ][\cf3 64\cf4 ];\
    \cf5 int\cf4  hits;\
    \cf5 int\cf4  misses;\
\};\
\
\cf5 int\cf4  isDataExistsInCacheL1(\cf5 uint64_t\cf4  address, \cf5 int\cf4  nway, \cf5 struct\cf4  L1Cache *l1)\
\{\
    \cf5 uint64_t\cf4  block_addr = address >> (\cf5 unsigned\cf4 )log2(\cf3 64\cf4 );\
    \cf5 int\cf4  setNumber = block_addr % \cf3 512\cf4 ;\
    \cf5 uint64_t\cf4  tag = block_addr >> (\cf5 unsigned\cf4 )log2(\cf3 512\cf4 );\
    \cf5 int\cf4  startIndex = ((\cf5 int\cf4 )setNumber) * nway;\
    \cf5 int\cf4  nwayTemp = nway;\
    \cf5 int\cf4  loopIndex = startIndex;\
    \cf6 while\cf4  (nwayTemp > \cf3 0\cf4 )\
    \{\
        \cf6 if\cf4  (l1->valid_field[loopIndex] && l1->tag_field[loopIndex] == tag)\
        \{\
            \cf6 return\cf4  \cf3 1\cf4 ;\
        \}\
        loopIndex += \cf3 1\cf4 ;\
        nwayTemp--;\
    \}\
    \cf6 return\cf4  \cf3 0\cf4 ;\
\}\
\
\
\cf5 int\cf4  isDataExistsInCacheL2(\cf5 uint64_t\cf4  address, \cf5 int\cf4  nway, \cf5 struct\cf4  L2Cache *l2)\
\{\
    \cf5 uint64_t\cf4  block_addr = address >> (\cf5 unsigned\cf4 )log2(\cf3 64\cf4 );\
    \cf5 int\cf4  setNumber = block_addr % \cf3 2048\cf4 ;\
    \cf5 uint64_t\cf4  tag = block_addr >> (\cf5 unsigned\cf4 )log2(\cf3 2048\cf4 );\
    \cf5 int\cf4  startIndex = ((\cf5 int\cf4 )setNumber) * nway;\
    \cf5 int\cf4  nwayTemp = nway;\
    \cf5 int\cf4  loopIndex = startIndex;\
    \cf6 while\cf4  (nwayTemp > \cf3 0\cf4 )\
    \{\
        \cf6 if\cf4  (l2->valid_field[loopIndex] && l2->tag_field[loopIndex] == tag)\
        \{\
            \cf6 return\cf4  \cf3 1\cf4 ;\
        \}\
        loopIndex += \cf3 1\cf4 ;\
        nwayTemp--;\
    \}\
    \cf6 return\cf4  \cf3 0\cf4 ;\
\}\
\
\
\cf5 void\cf4  insertDataInL1Cache(\cf5 uint64_t\cf4  address, \cf5 int\cf4  nway, \cf5 struct\cf4  L1Cache *l1)\
\{\
    \cf5 uint64_t\cf4  block_addr = address >> (\cf5 unsigned\cf4 )log2(\cf3 64\cf4 );\
    \cf5 int\cf4  setNumber = block_addr % \cf3 512\cf4 ;\
    \cf5 uint64_t\cf4  tag = block_addr >> (\cf5 unsigned\cf4 )log2(\cf3 512\cf4 );\
    \cf5 int\cf4  startIndex = ((\cf5 int\cf4 )setNumber) * nway;\
    \cf5 int\cf4  nwayTemp = nway;\
    \cf5 int\cf4  loopIndex = startIndex;\
    \cf5 int\cf4  isAnySpaceEmpty = \cf3 0\cf4 ;\
    \cf5 int\cf4  endIndex = startIndex + nway - \cf3 1\cf4 ;\
    \cf6 while\cf4  (nwayTemp > \cf3 0\cf4 )\
    \{\
        \cf6 if\cf4  (l1->valid_field[loopIndex] == \cf3 0\cf4 )\
        \{\
            isAnySpaceEmpty = \cf3 1\cf4 ;\
        \}\
        loopIndex++;\
        nwayTemp--;\
    \}\
    \cf6 if\cf4  (isAnySpaceEmpty > \cf3 0\cf4 )\
    \{\
        nwayTemp = nway;\
        loopIndex = startIndex;\
        \cf6 while\cf4  (nwayTemp > \cf3 0\cf4 )\
        \{\
            \cf6 if\cf4  (l1->valid_field[loopIndex] == \cf3 0\cf4 )\
            \{\
                l1->valid_field[loopIndex] = \cf3 1\cf4 ;\
                l1->tag_field[loopIndex] = tag;\
                \cf6 break\cf4 ;\
            \}\
\
            loopIndex += \cf3 1\cf4 ;\
            nwayTemp--;\
        \}\
    \}\
    \cf6 else\cf4 \
    \{\
        \cf5 int\cf4  randomIndex = (rand() % (endIndex - startIndex + \cf3 1\cf4 )) + startIndex;\
        \cf7 //   printf(\cf3 "Picking a rand variable %d"\cf7 ,randomIndex);\cf4 \
        l1->valid_field[randomIndex] = \cf3 1\cf4 ;\
        l1->tag_field[randomIndex] = tag;\
    \}\
\}\
\
\cf5 void\cf4  insertDataInL2Cache(\cf5 uint64_t\cf4  address, \cf5 int\cf4  nway, \cf5 struct\cf4  L2Cache *l2)\
\{\
\
    \cf5 uint64_t\cf4  block_addr = address >> (\cf5 unsigned\cf4 )log2(\cf3 64\cf4 );\
    \cf5 int\cf4  setNumber = block_addr % \cf3 2048\cf4 ;\
    \cf5 uint64_t\cf4  tag = block_addr >> (\cf5 unsigned\cf4 )log2(\cf3 2048\cf4 );\
    \cf5 int\cf4  startIndex = ((\cf5 int\cf4 )setNumber) * nway;\
    \cf5 int\cf4  nwayTemp = nway;\
    \cf5 int\cf4  loopIndex = startIndex;\
    \cf5 int\cf4  isAnySpaceEmpty = \cf3 0\cf4 ;\
    \cf5 int\cf4  endIndex = startIndex + nway - \cf3 1\cf4 ;\
    \cf6 while\cf4  (nwayTemp > \cf3 0\cf4 )\
    \{\
        \cf6 if\cf4  (l2->valid_field[loopIndex] == \cf3 0\cf4 )\
        \{\
            isAnySpaceEmpty = \cf3 1\cf4 ;\
        \}\
        loopIndex++;\
        nwayTemp--;\
    \}\
    \cf6 if\cf4  (isAnySpaceEmpty > \cf3 0\cf4 )\
    \{\
        nwayTemp = nway;\
        loopIndex = startIndex;\
        \cf6 while\cf4  (nwayTemp > \cf3 0\cf4 )\
        \{\
            \cf6 if\cf4  (l2->valid_field[loopIndex] == \cf3 0\cf4 )\
            \{\
                l2->valid_field[loopIndex] = \cf3 1\cf4 ;\
                l2->tag_field[loopIndex] = tag;\
                \cf6 break\cf4 ;\
            \}\
\
            loopIndex += \cf3 1\cf4 ;\
            nwayTemp--;\
        \}\
    \}\
    \cf6 else\cf4 \
    \{\
        \cf5 int\cf4  randomIndex = (rand() % (endIndex - startIndex + \cf3 1\cf4 )) + startIndex;\
        \cf7 //   printf(\cf3 "Picking a rand variable %d"\cf7 ,randomIndex);\cf4 \
        l2->valid_field[randomIndex] = \cf3 1\cf4 ;\
        l2->tag_field[randomIndex] = tag;\
    \}\
\}\
\
\
\cf5 int\cf4  main (\cf5 int\cf4  argc, \cf5 char\cf4  *argv[])\
\{\
    \cf6 if\cf4  (argc != \cf3 3\cf4 )\
    \{\
        printf (\cf3 "Usage: \cf2 %s\cf3  <direct> <trace file name>\cf2 \\n\cf3 "\cf4 , argv[\cf3 0\cf4 ]);\
        \cf6 return\cf4  \cf3 1\cf4 ;\
    \}\
\
    \cf2 #ifdef DBG\cf4 \
    printf(\cf3 "\cf2 \\n\cf3 ***********************************************************\cf2 \\n\\n\cf3 "\cf4 );\
    printf (\cf3 "BLOCK SIZE = \cf2 %d\cf3  Bytes\cf2 \\n\cf3 "\cf4 , BLOCK_SIZE);\
    printf (\cf3 "\cf2 %d\cf3 -WAY\cf2 \\n\cf3 "\cf4 , WAY_SIZE);\
    printf (\cf3 "CACHE SIZE = \cf2 %d\cf3  Bytes\cf2 \\n\cf3 "\cf4 , CACHE_SIZE);\
    printf (\cf3 "NUMBER OF BLOCKS = \cf2 %d\\n\cf3 "\cf4 , NUM_BLOCKS);\
    printf (\cf3 "NUMBER OF SETS = \cf2 %d\\n\cf3 "\cf4 , NUM_SETS);\
    printf (\cf3 "\cf2 \\n\cf3 "\cf4 );\
    \cf2 #endif\cf4 \
    srand ((\cf5 unsigned\cf4  \cf5 int\cf4 ) time (\cf3 NULL\cf4 ));\
    \cf5 char\cf4  *trace_file_name = argv[\cf3 2\cf4 ];\
    \cf5 struct\cf4  direct_mapped_cache d_cache;\
    \cf5 struct\cf4  fully_associative_cache a_cache;\
    \cf5 struct\cf4  n_way_associative_cache n_cache;\
    \cf5 char\cf4  mem_request[\cf3 20\cf4 ];\
    \cf5 uint64_t\cf4  address;\
    \cf5 FILE\cf4  *fp;\
\
    \cf7 /* Initialization */\cf4 \
    \cf6 for\cf4  (\cf5 int\cf4  i = \cf3 0\cf4 ; i < NUM_BLOCKS; i++)\
   \{\
        d_cache.valid_field[i] = \cf3 0\cf4 ;\
        d_cache.dirty_field[i] = \cf3 0\cf4 ;\
        d_cache.tag_field[i] = \cf3 0\cf4 ;\
    \}\
    d_cache.hits = \cf3 0\cf4 ;\
    d_cache.misses = \cf3 0\cf4 ;\
\
    \cf7 /* Opening the memory trace file */\cf4 \
    fp = fopen (trace_file_name, \cf3 "r"\cf4 );\
\
    \cf7 /*********** Part \cf3 1\cf7  **************/\
\
\cf4 \
    \cf6 if\cf4  (strncmp (argv[\cf3 1\cf4 ], \cf3 "direct"\cf4 , \cf3 6\cf4 ) == \cf3 0\cf4 )\
    \{\
        \cf7 /* Simulating direct-mapped cache */\cf4 \
        \cf7 /* Read the memory request address and access the cache */\cf4 \
        \cf6 while\cf4  (fgets (mem_request, \cf3 20\cf4 , fp) != \cf3 NULL\cf4 )\
        \{\
            address = convert_address (mem_request);\
                direct_mapped_cache_access (&d_cache, address);\
            \}\
\
        \cf5 double\cf4  totalMemoryAccess = d_cache.hits + d_cache.misses;\
        \cf5 double\cf4  miss_rate = (d_cache.misses / totalMemoryAccess) * \cf3 100\cf4 ;\
        \cf5 double\cf4  hit_rate = (d_cache.hits / totalMemoryAccess) * \cf3 100\cf4 ;\
\
        \cf7 /*Print out the results */\cf4 \
\
        printf (\cf3 "\cf2 \\n\cf3 ==================================\cf2 \\n\cf3 "\cf4 );\
        printf (\cf3 "Cache type:    Direct-Mapped Cache\cf2 \\n\cf3 "\cf4 );\
        printf (\cf3 "==================================\cf2 \\n\cf3 "\cf4 );\
        printf (\cf3 "Cache Hits:    \cf2 %d\\n\cf3 "\cf4 , d_cache.hits);\
        printf (\cf3 "Cache Misses:  \cf2 %d\\n\cf3 "\cf4 , d_cache.misses);\
        printf (\cf3 "Cache Hit rate :\cf2 %g\\n\cf3 "\cf4 , miss_rate);\
        printf (\cf3 "Cache miss rate:\cf2 %g\\n\cf3 "\cf4 , hit_rate);\
        printf (\cf3 "\cf2 \\n\\n\\n\\n\cf3 "\cf4 );\
    \}\
\
    \cf7 /*********** Part \cf3 2\cf7  **************/\cf4 \
\
    \cf6 else\cf4  \cf6 if\cf4  (strncmp (argv[\cf3 1\cf4 ], \cf3 "fully"\cf4 , \cf3 6\cf4 ) == \cf3 0\cf4 )\
    \{\
        \cf7 /* Simulating fully-associative-mapped cache */\cf4 \
        \cf7 /* Initialization */\cf4 \
        \cf6 for\cf4  (\cf5 int\cf4  i = \cf3 0\cf4 ; i < NUM_BLOCKS; i++)\
            \{\
                a_cache.valid_field[i] = \cf3 0\cf4 ;\
                a_cache.dirty_field[i] = \cf3 0\cf4 ;\
                a_cache.tag_field[i] = \cf3 0\cf4 ;\
                a_cache.block_address[i] = \cf3 0\cf4 ;\
            \}\
        a_cache.hits = \cf3 0\cf4 ;\
        a_cache.misses = \cf3 0\cf4 ;\
        \cf7 /* Read the memory request address and access the cache */\cf4 \
        \cf6 while\cf4  (fgets (mem_request, \cf3 20\cf4 , fp) != \cf3 NULL\cf4 )\
            \{\
                address = convert_address (mem_request);\
                fully_associative_cache_access (&a_cache, address);\
            \}\
\
\
        \cf5 double\cf4  totalMemoryAccess = a_cache.hits + a_cache.misses;\
        \cf5 double\cf4  miss_rate = (a_cache.misses / totalMemoryAccess) * \cf3 100\cf4 ;\
        \cf5 double\cf4  hit_rate = (a_cache.hits / totalMemoryAccess) * \cf3 100\cf4 ;\
\
        \cf7 /*Print out the results */\cf4 \
        printf (\cf3 "\cf2 \\n\cf3 ==================================\cf2 \\n\cf3 "\cf4 );\
        printf (\cf3 "Cache type:    Fully-Associative-Mapped Cache\cf2 \\n\cf3 "\cf4 );\
        printf (\cf3 "==================================\cf2 \\n\cf3 "\cf4 );\
        printf (\cf3 "Cache Hits:    \cf2 %d\\n\cf3 "\cf4 , a_cache.hits);\
        printf (\cf3 "Cache Misses:  \cf2 %d\\n\cf3 "\cf4 , a_cache.misses);\
        printf (\cf3 "Cache Hit Rate: \cf2 %g\\n\cf3 "\cf4 , hit_rate);\
        printf (\cf3 "Cache Miss Rate: \cf2 %g\\n\cf3 "\cf4 , miss_rate);\
        printf (\cf3 "\cf2 \\n\\n\\n\\n\cf3 "\cf4 );\
    \}\
\
    \cf7 /*********** Part \cf3 2\cf7  **************/\cf4 \
\
    \cf6 else\cf4  \cf6 if\cf4  (strncmp (argv[\cf3 1\cf4 ], \cf3 "n-way"\cf4 , \cf3 6\cf4 ) == \cf3 0\cf4 )\
    \{\
        \cf7 /* Simulating n-way-associative-mapped cache */\cf4 \
        \cf7 /* Initialization */\cf4 \
        \cf6 for\cf4  (\cf5 int\cf4  i = \cf3 0\cf4 ; i < NUM_SETS; i++)\
            \{\
                \cf6 for\cf4  (\cf5 int\cf4  j = \cf3 0\cf4 ; j < WAY_SIZE; j++)\
                \{\
                    n_cache.valid_field[i][j] = \cf3 0\cf4 ;\
                    n_cache.dirty_field[i][j] = \cf3 0\cf4 ;\
                    n_cache.tag_field[i][j] = \cf3 0\cf4 ;\
                    n_cache.set_address[i][j] = \cf3 0\cf4 ;\
                \}\
            \}\
        n_cache.hits = \cf3 0\cf4 ;\
        n_cache.misses = \cf3 0\cf4 ;\
\
        \cf7 /* Read the memory request address and access the cache */\cf4 \
        \cf6 while\cf4  (fgets (mem_request, \cf3 20\cf4 , fp) != \cf3 NULL\cf4 )\
            \{\
                address = convert_address (mem_request);\
                n_way_associative_cache_access (&n_cache, address);\
            \}\
\
        \cf5 double\cf4  totalMemoryAccess = n_cache.hits + n_cache.misses;\
        \cf5 double\cf4  miss_rate = (n_cache.misses / totalMemoryAccess) * \cf3 100\cf4 ;\
        \cf5 double\cf4  hit_rate = (n_cache.hits / totalMemoryAccess) * \cf3 100\cf4 ;\
\
        \cf7 /*Print out the results */\cf4 \
        printf (\cf3 "\cf2 \\n\cf3 ==================================\cf2 \\n\cf3 "\cf4 );\
        printf (\cf3 "Cache type:    N-Way-Associative-Mapped Cache\cf2 \\n\cf3 "\cf4 );\
        printf (\cf3 "==================================\cf2 \\n\cf3 "\cf4 );\
        printf (\cf3 "Cache Hits:    \cf2 %d\\n\cf3 "\cf4 , n_cache.hits);\
        printf (\cf3 "Cache Misses:  \cf2 %d\\n\cf3 "\cf4 , n_cache.misses);\
        printf (\cf3 "Cache Hit Rate: \cf2 %0.2f\\n\cf3 "\cf4 , hit_rate);\
        printf (\cf3 "Cache Miss Rate: \cf2 %0.2f\\n\cf3 "\cf4 , miss_rate);\
        printf (\cf3 "\cf2 \\n\\n\\n\\n\cf3 "\cf4 );\
    \}\
\
    \cf7 /*********** Part \cf3 3\cf7  **************/\cf4 \
\
    \cf6 else\cf4  \cf6 if\cf4  (strncmp (argv[\cf3 1\cf4 ], \cf3 "two-level"\cf4 , \cf3 6\cf4 ) == \cf3 0\cf4 )\
    \{\
        \cf5 struct\cf4  L1Cache l1;\
        \cf5 struct\cf4  L2Cache l2;\
        \cf5 char\cf4  mem_request[\cf3 20\cf4 ];\
        \cf5 int\cf4  numberOfBlocksinl1 = \cf3 1024\cf4 ;\
        \cf5 int\cf4  numberOfBocksinl2 = \cf3 16384\cf4 ;\
        \cf5 int\cf4  l1nway = \cf3 2\cf4 ;\
        \cf5 int\cf4  l2nway = \cf3 8\cf4 ;\
        \cf5 int\cf4  numberOfSetsl1 = \cf3 512\cf4 ;\
        \cf5 int\cf4  numberOfSetsl2 = \cf3 2048\cf4 ;\
        \cf6 for\cf4  (\cf5 int\cf4  i = \cf3 0\cf4 ; i < numberOfBlocksinl1; i++)\
        \{\
                l1.valid_field[i] = \cf3 0\cf4 ;\
                l1.dirty_field[i] = \cf3 0\cf4 ;\
                l1.tag_field[i] = \cf3 0\cf4 ;\
        \}\
        \cf6 for\cf4  (\cf5 int\cf4  i = \cf3 0\cf4 ; i < numberOfBocksinl2; i++)\
        \{\
                l2.valid_field[i] = \cf3 0\cf4 ;\
                l2.dirty_field[i] = \cf3 0\cf4 ;\
                l2.tag_field[i] = \cf3 0\cf4 ;\
        \}\
\
        l1.hits = \cf3 0\cf4 ;\
        l1.misses = \cf3 0\cf4 ;\
        l2.hits = \cf3 0\cf4 ;\
        l2.misses = \cf3 0\cf4 ;\
\
\
        \cf6 while\cf4  (fgets(mem_request, \cf3 20\cf4 , fp) != \cf3 NULL\cf4 )\
        \{\
            address = convert_address(mem_request);\
            \cf5 int\cf4  dataInL1 = isDataExistsInCacheL1(address, l1nway, &l1);\
            \cf6 if\cf4  (dataInL1 == \cf3 1\cf4 )\
            \{\
                l1.hits++;\
                l2.hits++;\
            \}\
            \cf6 else\cf4 \
            \{\
                l1.misses++;\
                \cf5 int\cf4  dataInL2 = isDataExistsInCacheL2(address, l2nway, &l2);\
                \cf6 if\cf4  (dataInL2)\
                \{\
                    l2.hits += \cf3 1\cf4 ;\
                \}\
                \cf6 else\cf4 \
                \{\
                    l2.misses++;\
                    insertDataInL2Cache(address, l2nway, &l2);\
                \}\
                insertDataInL1Cache(address, l1nway, &l1);\
            \}\
        \}\
        printf(\cf3 "\cf2 \\n\cf3 ==================================\cf2 \\n\cf3 "\cf4 );\
        printf(\cf3 "Cache type:     l1\cf2 \\n\cf3 "\cf4 );\
        printf(\cf3 "==================================\cf2 \\n\cf3 "\cf4 );\
        printf(\cf3 "Cache Hits:    \cf2 %d\\n\cf3 "\cf4 , l1.hits);\
        printf(\cf3 "Cache Misses:  \cf2 %d\\n\cf3 "\cf4 , l1.misses);\
        printf(\cf3 "Cache Hit Rate : \cf2 %0.9f%%\\n\cf3 "\cf4 , ((\cf5 float\cf4 )l1.hits / (\cf5 float\cf4 )(l1.hits + l1.misses)) * \cf3 100\cf4 );\
        printf(\cf3 "Cache Miss Rate : \cf2 %0.9f%%\\n\cf3 "\cf4 , ((\cf5 float\cf4 )l1.misses / (\cf5 float\cf4 )(l1.hits + l1.misses)) * \cf3 100\cf4 );\
        printf(\cf3 "\cf2 \\n\cf3 "\cf4 );\
        printf(\cf3 "\cf2 \\n\cf3 ==================================\cf2 \\n\cf3 "\cf4 );\
        printf(\cf3 "Cache type:     l2\cf2 \\n\cf3 "\cf4 );\
        printf(\cf3 "==================================\cf2 \\n\cf3 "\cf4 );\
        printf(\cf3 "Cache Hits:    \cf2 %d\\n\cf3 "\cf4 , l2.hits);\
        printf(\cf3 "Cache Misses:  \cf2 %d\\n\cf3 "\cf4 , l2.misses);\
        printf(\cf3 "Cache Hit Rate : \cf2 %0.9f%%\\n\cf3 "\cf4 , ((\cf5 float\cf4 )l2.hits / (\cf5 float\cf4 )(l2.hits + l2.misses)) * \cf3 100\cf4 );\
        printf(\cf3 "Cache Miss Rate : \cf2 %0.9f%%\\n\cf3 "\cf4 , ((\cf5 float\cf4 )l2.misses / (\cf5 float\cf4 )(l2.hits + l2.misses)) * \cf3 100\cf4 );\
        printf(\cf3 "\cf2 \\n\cf3 "\cf4 );\
\
    \}\
\
    fclose (fp);\
\
    \cf6 return\cf4  \cf3 0\cf4 ;\
\}\
\
\
\cf5 uint64_t\cf4  convert_address (\cf5 char\cf4  memory_addr[])\
\pard\tx560\tx1120\tx1680\tx2240\tx2800\tx3360\tx3920\tx4480\tx5040\tx5600\tx6160\tx6720\pardirnatural\partightenfactor0
\cf7 /* Converts the physical \cf3 32\cf7 -bit address in the trace file to the \cf3 "binary"\cf7  \\\\ * (a uint64 that can have bitwise operations on it) */\cf4 \
\{\
    \cf5 uint64_t\cf4  binary = \cf3 0\cf4 ;\
    \cf5 int\cf4  i = \cf3 0\cf4 ;\
\
    \cf6 while\cf4  (memory_addr[i] != \cf2 '\\n'\cf4 )\
    \{\
        \cf6 if\cf4  (memory_addr[i] <= \cf3 '9'\cf4  && memory_addr[i] >= \cf3 '0'\cf4 )\
        \{\
            binary = (binary * \cf3 16\cf4 ) + (memory_addr[i] - \cf3 '0'\cf4 );\
        \}\
        \cf6 else\cf4 \
            \{\
                \cf6 if\cf4  (memory_addr[i] == \cf3 'a'\cf4  || memory_addr[i] == \cf3 'A'\cf4 )\
                \{\
                    binary = (binary * \cf3 16\cf4 ) + \cf3 10\cf4 ;\
                \}\
                \cf6 if\cf4  (memory_addr[i] == \cf3 'b'\cf4  || memory_addr[i] == \cf3 'B'\cf4 )\
                \{\
                    binary = (binary * \cf3 16\cf4 ) + \cf3 11\cf4 ;\
                \}\
                \cf6 if\cf4  (memory_addr[i] == \cf3 'c'\cf4  || memory_addr[i] == \cf3 'C'\cf4 )\
                \{\
                    binary = (binary * \cf3 16\cf4 ) + \cf3 12\cf4 ;\
                \}\
                \cf6 if\cf4  (memory_addr[i] == \cf3 'd'\cf4  || memory_addr[i] == \cf3 'D'\cf4 )\
                \{\
                    binary = (binary * \cf3 16\cf4 ) + \cf3 13\cf4 ;\
                \}\
            \cf6 if\cf4  (memory_addr[i] == \cf3 'e'\cf4  || memory_addr[i] == \cf3 'E'\cf4 )\
            \{\
                binary = (binary * \cf3 16\cf4 ) + \cf3 14\cf4 ;\
            \}\
            \cf6 if\cf4  (memory_addr[i] == \cf3 'f'\cf4  || memory_addr[i] == \cf3 'F'\cf4 )\
            \{\
                binary = (binary * \cf3 16\cf4 ) + \cf3 15\cf4 ;\
            \}\
            \}\
            i++;\
    \}\
\
\
\pard\tx560\tx1120\tx1680\tx2240\tx2800\tx3360\tx3920\tx4480\tx5040\tx5600\tx6160\tx6720\pardirnatural\partightenfactor0
\cf2 #ifdef DBG\cf4 \
    printf (\cf3 "\cf2 %s\cf3  converted to \cf2 %llu\\n\cf3 "\cf4 , memory_addr, binary);\
\cf2 #endif\cf4 \
  \cf6 return\cf4  binary;\
\}\
\
\cf5 void\cf4  direct_mapped_cache_access (\cf5 struct\cf4  direct_mapped_cache *cache, \cf5 uint64_t\cf4  address)\
\{\
    \cf5 uint64_t\cf4  block_addr = address >> (\cf5 unsigned\cf4 ) log2 (BLOCK_SIZE);\
    \cf5 uint64_t\cf4  index = block_addr % NUM_BLOCKS;\
    \cf5 uint64_t\cf4  tag = block_addr >> (\cf5 unsigned\cf4 ) log2 (NUM_BLOCKS);\
\
    \cf2 #ifdef DBG\cf4 \
    printf(\cf3 "Memory address: \cf2 %llu\cf3 , Block address: \cf2 %llu\cf3 , Index: \cf2 %llu\cf3 , Tag: \cf2 %llu\cf3  "\cf4 , address, block_addr, index, tag);\
    \cf2 #endif\cf4 \
\
    \cf6 if\cf4  (cache->valid_field[index] && cache->tag_field[index] == tag)\
    \{\
        \cf7 /* Cache hit */\cf4 \
        cache->hits += \cf3 1\cf4 ;\
        \cf2 #ifdef DBG\cf4 \
        printf (\cf3 "Hit!\cf2 \\n\cf3 "\cf4 );\
        \cf2 #endif\cf4 \
    \}\
    \cf6 else\cf4 \
    \{\
        \cf7 /* Cache miss */\cf4 \
        cache->misses += \cf3 1\cf4 ;\
\
        \cf2 #ifdef DBG\cf4 \
        printf (\cf3 "Miss!\cf2 \\n\cf3 "\cf4 );\
        \cf2 #endif\cf4 \
\
        \cf6 if\cf4  (cache->valid_field[index] && cache->dirty_field[index])\
            \{\
                \cf7 /* Write the cache block back to memory */\cf4 \
            \}\
            cache->tag_field[index] = tag;\
            cache->valid_field[index] = \cf3 1\cf4 ;\
            cache->dirty_field[index] = \cf3 0\cf4 ;\
    \}\
\}\
\cf5 void\cf4  fully_associative_cache_access (\cf5 struct\cf4  fully_associative_cache *cache, \cf5 uint64_t\cf4  address)\
\{\
    \cf5 uint64_t\cf4  block_addr = address >> (\cf5 unsigned\cf4 ) log2 (BLOCK_SIZE);\
    \cf5 uint64_t\cf4  index = block_addr % NUM_BLOCKS;\
    \cf5 uint64_t\cf4  tag = block_addr >> (\cf5 unsigned\cf4 ) log2 (NUM_BLOCKS);\
    \cf5 int\cf4  flag = -\cf3 1\cf4 ;              \cf7 /* -\cf3 1\cf7  if cache miss otherwise flag = index */\cf4 \
    \cf5 int\cf4  r = rand () % NUM_BLOCKS;       \cf7 /* random replacement policy */\cf4 \
    \cf5 int\cf4  empty = -\cf3 1\cf4 ;             \cf7 /* stores the cache index of an empty block */\cf4 \
\
    \cf2 #ifdef DBG\cf4 \
    printf(\cf3 "Memory address: \cf2 %llu\cf3 , Block address: \cf2 %llu\cf3 , Index: \cf2 %llu\cf3 , Tag: \cf2 %llu\cf3  "\cf4 , address, block_addr, index, tag);\
    \cf2 #endif\cf4 \
\
    \cf6 for\cf4  (\cf5 int\cf4  num = \cf3 0\cf4 ; num < NUM_BLOCKS; num++)\
    \{\
        \cf6 if\cf4  (cache->tag_field[num] == tag && cache->block_address[num] == index && cache->valid_field[num])\
            \{\
                \cf7 /* Cache hit */\cf4 \
                cache->hits += \cf3 1\cf4 ;\
                flag = num;\
\
            \cf2 #ifdef DBG\cf4 \
                  printf (\cf3 "Hit!\cf2 \\n\cf3 "\cf4 );\
            \cf2 #endif\cf4 \
\
                \cf6 break\cf4 ;\
            \}\
        \cf6 else\cf4  \cf6 if\cf4  (!cache->valid_field[num])\
            \{\
                empty = num;\
            \}\
    \}\
    \cf6 if\cf4  (flag == -\cf3 1\cf4 )\
    \{\
        \cf7 /* Cache miss */\cf4 \
        cache->misses += \cf3 1\cf4 ;\
\
        \cf2 #ifdef DBG\cf4 \
        printf (\cf3 "Miss!\cf2 \\n\cf3 "\cf4 );\
        \cf2 #endif\cf4 \
\
        \cf6 if\cf4  (cache->valid_field[flag] && cache->dirty_field[flag])\
        \{\
                \cf7 /* Write the cache block back to memory */\cf4 \
            \}\
\
        \cf6 if\cf4  (empty == -\cf3 1\cf4 )\
            \{\
                \cf7 /* if the cache is full replace a random block */\cf4 \
                cache->tag_field[r] = tag;\
                cache->valid_field[r] = \cf3 1\cf4 ;\
                cache->dirty_field[r] = \cf3 0\cf4 ;\
                cache->block_address[r] = index;\
            \}\
        \cf6 else\cf4 \
            \{\
                \cf7 /* if the cache is not full write into an empty block */\cf4 \
                cache->tag_field[empty] = tag;\
                cache->valid_field[empty] = \cf3 1\cf4 ;\
                cache->dirty_field[empty] = \cf3 0\cf4 ;\
                cache->block_address[empty] = index;\
            \}\
    \}\
\}\
\
\cf5 void\cf4  n_way_associative_cache_access (\cf5 struct\cf4  n_way_associative_cache *cache, \cf5 uint64_t\cf4  address)\
\{\
    \cf5 uint64_t\cf4  block_addr = address >> (\cf5 unsigned\cf4 ) log2 (BLOCK_SIZE);\
    \cf5 uint64_t\cf4  index = block_addr % NUM_BLOCKS;\
    \cf5 uint64_t\cf4  set = index % NUM_SETS;\
    \cf5 uint64_t\cf4  tag = block_addr >> (\cf5 unsigned\cf4 ) log2 (NUM_BLOCKS);\
    \cf5 uint64_t\cf4  flag = -\cf3 1\cf4 ;         \cf7 /* -\cf3 1\cf7  if cache miss otherwise flag = way # */\cf4 \
    \cf5 int\cf4  r = rand () % WAY_SIZE; \cf7 /* random replacement policy */\cf4 \
    \cf5 int64_t\cf4  empty = -\cf3 1\cf4 ;         \cf7 /* stores the cache index of an empty block */\cf4 \
\
    \cf2 #ifdef DBG\cf4 \
    printf(\cf3 "Memory address: \cf2 %llu\cf3 , Block address: \cf2 %llu\cf3 , Index: \cf2 %llu\cf3 , Set: \cf2 %llu\cf3 , Tag: \cf2 %llu\cf3  "\cf4 , address, block_addr, index, set, tag);\
    \cf2 #endif\cf4 \
\
    \cf6 for\cf4  (\cf5 int\cf4  i = \cf3 0\cf4 ; i < WAY_SIZE; i++)\
    \{\
        \cf6 if\cf4  (cache->tag_field[set][i] == tag && cache->set_address[set][i] == index && cache->valid_field[set][i])\
            \{\
                \cf7 /* Search every way within the appropriate set for a hit */\cf4 \
\
            \cf7 /* Cache hit */\cf4 \
                cache->hits += \cf3 1\cf4 ;\
                flag = i;\
\
            \cf2 #ifdef DBG\cf4 \
                printf (\cf3 "Hit!\cf2 \\n\cf3 "\cf4 );\
            \cf2 #endif\cf4 \
\
                \cf6 break\cf4 ;\
            \}\
        \cf6 if\cf4  (!cache->valid_field[set][i])\
            \{\
                empty = i;\
            \}\
    \}\
\
    \cf6 if\cf4  (flag == -\cf3 1\cf4 )\
    \{\
        \cf7 /* Cache miss */\cf4 \
        cache->misses += \cf3 1\cf4 ;\
\
        \cf2 #ifdef DBG\cf4 \
        printf (\cf3 "Miss!\cf2 \\n\cf3 "\cf4 );\
        \cf2 #endif\cf4 \
\
        \cf6 if\cf4  (cache->valid_field[set][flag] && cache->dirty_field[set][flag])\
            \{\
                \cf7 /* Write the cache block back to memory */\cf4 \
            \}\
\
        \cf6 if\cf4  (empty == -\cf3 1\cf4 )\
            \{\
                \cf7 /* if the cache is full replace a random block */\cf4 \
                cache->tag_field[set][r] = tag;\
                cache->valid_field[set][r] = \cf3 1\cf4 ;\
                cache->dirty_field[set][r] = \cf3 0\cf4 ;\
                cache->set_address[set][r] = index;\
            \}\
        \cf6 else\cf4 \
            \{\
                \cf7 /* if the cache is not full write into an empty block */\cf4 \
                cache->tag_field[set][empty] = tag;\
                cache->valid_field[set][empty] = \cf3 1\cf4 ;\
                cache->dirty_field[set][empty] = \cf3 0\cf4 ;\
                cache->set_address[set][empty] = index;\
            \}\
    \}\
\}}