/**
* Copyright 2005-2015 ECMWF
*
* Licensed under the GNU Lesser General Public License which
* incorporates the terms and conditions of version 3 of the GNU
* General Public License.
* See LICENSE and gpl-3.0.txt for details.
*/

/*****************************************
 *  Enrico Fucile
 ****************************************/

#include "grib_api_internal.h"
#include <ctype.h>

/*
   This is used by make_class.pl

   START_CLASS_DEF
   CLASS      = accessor
   SUPER      = grib_accessor_class_gen
   IMPLEMENTS = init;dump
   IMPLEMENTS = unpack_string;unpack_string_array;unpack_long; unpack_double
   IMPLEMENTS = pack_long; pack_double
   IMPLEMENTS = value_count; destroy; get_native_type;
   MEMBERS    = grib_accessor* data_accessor

   END_CLASS_DEF

 */

/* START_CLASS_IMP */

/*

Don't edit anything between START_CLASS_IMP and END_CLASS_IMP
Instead edit values between START_CLASS_DEF and END_CLASS_DEF
or edit "accessor.class" and rerun ./make_class.pl

*/

static int  get_native_type(grib_accessor*);
static int pack_double(grib_accessor*, const double* val,size_t *len);
static int pack_long(grib_accessor*, const long* val,size_t *len);
static int unpack_double(grib_accessor*, double* val,size_t *len);
static int unpack_long(grib_accessor*, long* val,size_t *len);
static int unpack_string (grib_accessor*, char*, size_t *len);
static int unpack_string_array (grib_accessor*, char**, size_t *len);
static int value_count(grib_accessor*,long*);
static void destroy(grib_context*,grib_accessor*);
static void dump(grib_accessor*, grib_dumper*);
static void init(grib_accessor*,const long, grib_arguments* );
static void init_class(grib_accessor_class*);

typedef struct grib_accessor_unpack_bufr_values {
    grib_accessor          att;
/* Members defined in gen */
/* Members defined in unpack_bufr_values */
	grib_accessor* data_accessor;
} grib_accessor_unpack_bufr_values;

extern grib_accessor_class* grib_accessor_class_gen;

static grib_accessor_class _grib_accessor_class_unpack_bufr_values = {
    &grib_accessor_class_gen,                      /* super                     */
    "unpack_bufr_values",                      /* name                      */
    sizeof(grib_accessor_unpack_bufr_values),  /* size                      */
    0,                           /* inited */
    &init_class,                 /* init_class */
    &init,                       /* init                      */
    0,                  /* post_init                      */
    &destroy,                    /* free mem                       */
    &dump,                       /* describes himself         */
    0,                /* get length of section     */
    0,              /* get length of string      */
    &value_count,                /* get number of values      */
    0,                 /* get number of bytes      */
    0,                /* get offset to bytes           */
    &get_native_type,            /* get native type               */
    0,                /* get sub_section                */
    0,               /* grib_pack procedures long      */
    0,                 /* grib_pack procedures long      */
    &pack_long,                  /* grib_pack procedures long      */
    &unpack_long,                /* grib_unpack procedures long    */
    &pack_double,                /* grib_pack procedures double    */
    &unpack_double,              /* grib_unpack procedures double  */
    0,                /* grib_pack procedures string    */
    &unpack_string,              /* grib_unpack procedures string  */
    0,          /* grib_pack array procedures string    */
    &unpack_string_array,        /* grib_unpack array procedures string  */
    0,                 /* grib_pack procedures bytes     */
    0,               /* grib_unpack procedures bytes   */
    0,            /* pack_expression */
    0,              /* notify_change   */
    0,                /* update_size   */
    0,            /* preferred_size   */
    0,                    /* resize   */
    0,      /* nearest_smaller_value */
    0,                       /* next accessor    */
    0,                    /* compare vs. another accessor   */
    0,     /* unpack only ith value          */
    0,     /* unpack a subarray         */
    0,              		/* clear          */
};


grib_accessor_class* grib_accessor_class_unpack_bufr_values = &_grib_accessor_class_unpack_bufr_values;


static void init_class(grib_accessor_class* c)
{
	c->next_offset	=	(*(c->super))->next_offset;
	c->string_length	=	(*(c->super))->string_length;
	c->byte_count	=	(*(c->super))->byte_count;
	c->byte_offset	=	(*(c->super))->byte_offset;
	c->sub_section	=	(*(c->super))->sub_section;
	c->pack_missing	=	(*(c->super))->pack_missing;
	c->is_missing	=	(*(c->super))->is_missing;
	c->pack_string	=	(*(c->super))->pack_string;
	c->pack_string_array	=	(*(c->super))->pack_string_array;
	c->pack_bytes	=	(*(c->super))->pack_bytes;
	c->unpack_bytes	=	(*(c->super))->unpack_bytes;
	c->pack_expression	=	(*(c->super))->pack_expression;
	c->notify_change	=	(*(c->super))->notify_change;
	c->update_size	=	(*(c->super))->update_size;
	c->preferred_size	=	(*(c->super))->preferred_size;
	c->resize	=	(*(c->super))->resize;
	c->nearest_smaller_value	=	(*(c->super))->nearest_smaller_value;
	c->next	=	(*(c->super))->next;
	c->compare	=	(*(c->super))->compare;
	c->unpack_double_element	=	(*(c->super))->unpack_double_element;
	c->unpack_double_subarray	=	(*(c->super))->unpack_double_subarray;
	c->clear	=	(*(c->super))->clear;
}

/* END_CLASS_IMP */

/*TODO move all those typedef in a unique .h file*/
typedef struct grib_accessor_bufr_compressed_data {
    grib_accessor          att;
/* Members defined in gen */
/* Members defined in bufr_compressed_data */
        const char* offsetSection4Name;
        const char* offsetBeforeDataName;
        const char* offsetEndSection4Name;
        const char* section4LengthName;
        const char* numberOfSubsetsName;
        const char* subsetNumberName;
        const char* expandedDescriptorsName;
        const char* elementsName;
        const char* abbreviationName;
        const char* typeName;
        const char* nameName;
        const char* unitName;
        const char* referenceName;
        const char* scaleName;
        const char* widthName;
        const char* codeFlags;
        long* code;
        long* expandedDescriptors;
        char** abbreviation;
        int* index;
        grib_trie* abbreviationTrie;
        char** type;
        char** names;
        char** units;
        long* reference;
        long* scale;
        long* width;
        long numberOfElements;
        long numberOfSubsets;
        size_t numberOfDescriptors;
        double* values;
        int* is_constant;
        double* constant;
        int dirty;
} grib_accessor_bufr_compressed_data;

static void init(grib_accessor* a, const long len, grib_arguments* params) {

  char* key;
  grib_accessor_unpack_bufr_values* self = (grib_accessor_unpack_bufr_values*)a;
  key = (char*)grib_arguments_get_name(a->parent->h,params,0);
  self->data_accessor=grib_find_accessor(a->parent->h,key);

  a->length = 0;
}

static void dump(grib_accessor* a, grib_dumper* dumper)
{
    return;
}

static int unpack_string_array (grib_accessor* a, char** buffer, size_t *len)
{
  grib_accessor_unpack_bufr_values* self = (grib_accessor_unpack_bufr_values*)a;
  grib_accessor* data=(grib_accessor*)self->data_accessor;

  return grib_unpack_double(data,0,0);
}

static int unpack_string (grib_accessor* a, char* buffer, size_t *len)
{
  grib_accessor_unpack_bufr_values* self = (grib_accessor_unpack_bufr_values*)a;
  grib_accessor* data=(grib_accessor*)self->data_accessor;

  return grib_unpack_double(data,0,0);
}

static int unpack_long (grib_accessor* a, long* val, size_t *len)
{
  grib_accessor_unpack_bufr_values* self = (grib_accessor_unpack_bufr_values*)a;
  grib_accessor* data=(grib_accessor*)self->data_accessor;

  return grib_unpack_double(data,0,0);
}

static int unpack_double (grib_accessor* a, double* val, size_t *len)
{
  grib_accessor_unpack_bufr_values* self = (grib_accessor_unpack_bufr_values*)a;
  grib_accessor* data=(grib_accessor*)self->data_accessor;

  return grib_unpack_double(data,0,0);
}

static int value_count(grib_accessor* a,long* count)
{
  *count=1;
  return 0;
}

static void destroy(grib_context* context,grib_accessor* a)
{
    return;
}

static int  get_native_type(grib_accessor* a){
  return GRIB_TYPE_LONG;
}

static int    pack_long   (grib_accessor* a, const long* val, size_t *len)
{
  grib_accessor_unpack_bufr_values* self = (grib_accessor_unpack_bufr_values*)a;
  grib_accessor* data=(grib_accessor*)self->data_accessor;

  return grib_unpack_double(data,0,0);
}


static int    pack_double   (grib_accessor* a, const double* val, size_t *len)
{
  grib_accessor_unpack_bufr_values* self = (grib_accessor_unpack_bufr_values*)a;
  grib_accessor* data=(grib_accessor*)self->data_accessor;

  return grib_unpack_double(data,0,0);
}

