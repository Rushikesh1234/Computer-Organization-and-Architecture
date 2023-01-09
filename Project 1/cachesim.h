{\rtf1\ansi\ansicpg1252\cocoartf2638
\cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fnil\fcharset0 Menlo-Regular;}
{\colortbl;\red255\green255\blue255;\red64\green11\blue217;\red0\green0\blue0;\red180\green36\blue25;
\red200\green20\blue201;\red47\green180\blue29;}
{\*\expandedcolortbl;;\cssrgb\c32309\c18666\c88229;\csgray\c0;\cssrgb\c76411\c21697\c12527;
\cssrgb\c83397\c23074\c82666;\cssrgb\c20241\c73898\c14950;}
\paperw11900\paperh16840\margl1440\margr1440\vieww11520\viewh8400\viewkind0
\pard\tx560\tx1120\tx1680\tx2240\tx2800\tx3360\tx3920\tx4480\tx5040\tx5600\tx6160\tx6720\pardirnatural\partightenfactor0

\f0\fs22 \cf2 \CocoaLigature0 /*\cf3 \
\cf2          * CS3375 Computer Architecture\cf3 \
\cf2          * Course Project\cf3 \
\cf2          * Cache Simulator Design and Development\cf3 \
\cf2          * FALL \cf4 2017\cf3 \
\cf2          * By Yong Chen\cf3 \
\cf2 */\cf3 \
\
\
\cf5 #define BLOCK_SIZE  \cf4 16\cf5         \cf2 // Cache block size (or cache line size) in bytes (must be power of \cf4 2\cf2 ). \cf4 4\cf2  Bytes = \cf4 1\cf2  Word \cf3 \
\cf5 #define WAY_SIZE    \cf4 2\cf5          \cf2 /* Associativity; \cf4 1\cf2 -way = direct-mapped */\cf3 \
\cf5 #define CACHE_SIZE  \cf4 32768\cf5      \cf2 /* Cache capacity in bytes (must be power of \cf4 2\cf2 )*/\cf3 \
\cf5 #define NUM_BLOCKS  (CACHE_SIZE / BLOCK_SIZE)\cf3 \
\cf5 #define NUM_SETS    (NUM_BLOCKS / WAY_SIZE)\cf3 \
\
\cf2 /*Read the memory traces and convert it to binary */\cf3 \
\cf6 uint64_t\cf3  convert_address (\cf6 char\cf3  memory[]);\
\
\cf2 /*The data structure of direct-mapped cache */\cf3 \
\cf6 struct\cf3  direct_mapped_cache\
\{\
    \cf6 unsigned\cf3  valid_field[NUM_BLOCKS];   \cf2 /* Valid field */\cf3 \
    \cf6 unsigned\cf3  dirty_field[NUM_BLOCKS];   \cf2 /* Dirty field; since we don't distinguish writes and \\\\reads in this project yet, this field doesn't really matter */\cf3 \
    \cf6 uint64_t\cf3  tag_field[NUM_BLOCKS];     \cf2 /* Tag field */\cf3 \
    \cf6 char\cf3  data_field[NUM_BLOCKS][BLOCK_SIZE];    \cf2 /* Data field; since we don't really fetch data, \\\\this field doesn't really matter */\cf3 \
    \cf6 int\cf3  hits;                   \cf2 /* Hit count */\cf3 \
    \cf6 int\cf3  misses;                 \cf2 /* Miss count */\cf3 \
\};\
\
\cf2 /*Simulate the direct-mapped cache */\cf3 \
\cf6 void\cf3  direct_mapped_cache_access (\cf6 struct\cf3  direct_mapped_cache *cache, \cf6 uint64_t\cf3  address);\
\
\cf2 //The data structure of a fully-associative cache/\cf3 \
\cf6 struct\cf3  fully_associative_cache \{\
    \cf6 unsigned\cf3  valid_field[NUM_BLOCKS];   \cf2 /* Valid field */\cf3 \
    \cf6 unsigned\cf3  dirty_field[NUM_BLOCKS];   \cf2 /* Dirty field; since we don't distinguish writes and \\\\\cf3 \
\cf2                                            reads in this project yet, this field doesn't really matter */\cf3 \
    \cf6 uint64_t\cf3  tag_field[NUM_BLOCKS];     \cf2 /* Tag field */\cf3 \
    \cf6 char\cf3  data_field[NUM_BLOCKS][BLOCK_SIZE];  \cf2 /* Data field; since we don't really fetch data, \\\\\cf3 \
\cf2                                                  this field doesn't really matter */\cf3 \
    \cf6 int\cf3  block_address[NUM_BLOCKS];\
    \cf6 int\cf3  hits;                          \cf2 /* Hit count */\cf3 \
    \cf6 int\cf3  misses;                        \cf2 /* Miss count */\cf3 \
    \cf6 double\cf3  hit_rate;                   \cf2 /* Hit rate */\cf3 \
    \cf6 double\cf3  miss_rate;                  \cf2 /* Miss rate */\cf3 \
\};\
\
\cf2 /*Simulate the Fully associative cache */\cf3 \
\cf6 void\cf3  fully_associative_cache_access(\cf6 struct\cf3  fully_associative_cache *cache, \cf6 uint64_t\cf3  address);\
\
\cf2 //The data structure of a n-way-associative cache/\cf3 \
\cf6 struct\cf3  n_way_associative_cache \{\
    \cf6 unsigned\cf3  valid_field[NUM_SETS][WAY_SIZE];   \cf2 /* Valid field */\cf3 \
    \cf6 unsigned\cf3  dirty_field[NUM_SETS][WAY_SIZE];   \cf2 /* Dirty field; since we don't distinguish writes and \\\\\cf3 \
\cf2                                            reads in this project yet, this field doesn't really matter */\cf3 \
    \cf6 uint64_t\cf3  tag_field[NUM_SETS][WAY_SIZE];     \cf2 /* Tag field */\cf3 \
    \cf6 char\cf3  data_field[NUM_SETS][WAY_SIZE][BLOCK_SIZE];  \cf2 /* Data field; since we don't really fetch data, \\\\\cf3 \
\cf2                                                  this field doesn't really matter */\cf3 \
    \cf6 int\cf3  set_address[NUM_SETS][WAY_SIZE];\
    \cf6 int\cf3  hits;                          \cf2 /* Hit count */\cf3 \
    \cf6 int\cf3  misses;                        \cf2 /* Miss count */\cf3 \
    \cf6 double\cf3  hit_rate;                   \cf2 /* Hit rate */\cf3 \
    \cf6 double\cf3  miss_rate;                  \cf2 /* Miss rate */\cf3 \
\};\
\
\cf2 /*Simulate the N-way set associative cache */\cf3 \
\cf6 void\cf3  n_way_associative_cache_access(\cf6 struct\cf3  n_way_associative_cache *cache, \cf6 uint64_t\cf3  address);\
\
\
}