/*
 * Copyright (C) 2014 the contributors as stated in the AUTHORS file
 *
 * This file is part of open62541. open62541 is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser General
 * Public License, version 3 (as published by the Free Software Foundation) with
 * a static linking exception as stated in the LICENSE file provided with
 * open62541.
 *
 * open62541 is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 */

#ifndef UA_TYPES_H_
#define UA_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "ua_config.h"

/**
 * @defgroup types Datatypes
 *
 * @brief The built-in datatypes. The remaining datatypes are autogenerated from
 * XML descriptions as they are all enums or structures made up of the built-in
 * datatypes.
 *
 * All datatypes have similar functions with a common postfix. DO NOT CALL THESE
 * FUNCTIONS WITH NULL-POINTERS IF IT IS NOT EXPLICITLY PERMITTED.
 *
 * - _new: Allocates the memory for the type and runs _init on the returned
 *   variable. Returns null if no memory could be allocated.
 *
 * - _init: Sets all members to a "safe" standard, usually zero. Arrays (e.g.
 *   for strings) are set to a length of -1.
 *
 * - _copy: Copies a datatype. This performs a deep copy that iterates over the
 *    members. Copying into variants with an external data source is not
 *    permitted. If copying fails, a deleteMembers is performed and an error
 *    code returned.
 *
 * - _delete: Frees the memory where the datatype was stored. This performs an
 *   _deleteMembers internally if required.
 *
 * - _deleteMembers: Frees the memory of dynamically sized members (e.g. a
 *   string) of a datatype. This is useful when the datatype was allocated on
 *   the stack, whereas the dynamically sized members is heap-allocated. To
 *   reuse the variable, the remaining members (not dynamically allocated) need
 *   to be cleaned up with an _init.
 *
 * @{
 */

/** @brief A two-state logical value (true or false). */
typedef bool UA_Boolean;
#define UA_TRUE true
#define UA_FALSE false

/** @brief An integer value between -129 and 127. */
typedef int8_t UA_SByte;
#define UA_SBYTE_MAX 127
#define UA_SBYTE_MIN -128

/** @brief An integer value between 0 and 256. */
typedef uint8_t UA_Byte;
#define UA_BYTE_MAX 256
#define UA_BYTE_MIN 0

/** @brief An integer value between -32 768 and 32 767. */
typedef int16_t UA_Int16;
#define UA_INT16_MAX 32767
#define UA_INT16_MIN -32768

/** @brief An integer value between 0 and 65 535. */
typedef uint16_t UA_UInt16;
#define UA_UINT16_MAX 65535
#define UA_UINT16_MIN 0

/** @brief An integer value between -2 147 483 648 and 2 147 483 647. */
typedef int32_t UA_Int32;
#define UA_INT32_MAX 2147483647
#define UA_INT32_MIN −2147483648

/** @brief An integer value between 0 and 429 4967 295. */
typedef uint32_t UA_UInt32;
#define UA_UINT32_MAX 4294967295
#define UA_UINT32_MIN 0

/** @brief An integer value between -10 223 372 036 854 775 808 and 9 223 372 036 854 775 807 */
typedef int64_t UA_Int64;
#define UA_INT64_MAX 9223372036854775807
#define UA_INT64_MIN −9223372036854775808

/** @brief An integer value between 0 and 18 446 744 073 709 551 615. */
typedef uint64_t UA_UInt64;
#define UA_UINT64_MAX = 18446744073709551615
#define UA_UINT64_MIN = 0

/** @brief An IEEE single precision (32 bit) floating point value. */
typedef float UA_Float;

/** @brief An IEEE double precision (64 bit) floating point value. */
typedef double UA_Double;

/** @brief A sequence of Unicode characters. */
typedef struct {
    UA_Int32 length;
    UA_Byte *data;
} UA_String;

/** @brief An instance in time.
 * A DateTime value is encoded as a 64-bit signed integer which represents the
 * number of 100 nanosecond intervals since January 1, 1601 (UTC).
 */
typedef UA_Int64 UA_DateTime; // 100 nanosecond resolution

/** @brief A 16 byte value that can be used as a globally unique identifier. */
typedef struct {
    UA_UInt32 data1;
    UA_UInt16 data2;
    UA_UInt16 data3;
    UA_Byte   data4[8];
} UA_Guid;

/** @brief A sequence of octets. */
typedef UA_String UA_ByteString;

/** @brief An XML element. */
typedef UA_String UA_XmlElement;

/** @brief An identifier for a node in the address space of an OPC UA Server. */
/* The shortened numeric types are introduced during encoding. */
typedef struct {
    UA_UInt16 namespaceIndex;
    enum {
        UA_NODEIDTYPE_NUMERIC    = 2,
        UA_NODEIDTYPE_STRING     = 3,
        UA_NODEIDTYPE_GUID       = 4,
        UA_NODEIDTYPE_BYTESTRING = 5
    } identifierType;
    union {
        UA_UInt32     numeric;
        UA_String     string;
        UA_Guid       guid;
        UA_ByteString byteString;
    } identifier;
} UA_NodeId;

/** @brief A NodeId that allows the namespace URI to be specified instead of an index. */
typedef struct {
    UA_NodeId nodeId;
    UA_String namespaceUri; // not encoded if length=-1
    UA_UInt32 serverIndex;  // not encoded if 0
} UA_ExpandedNodeId;

#include "ua_statuscodes.h"
/** @brief A numeric identifier for a error or condition that is associated with a value or an operation. */
typedef enum UA_StatusCode UA_StatusCode; // StatusCodes aren't an enum(=int) since 32 unsigned bits are needed. See also ua_statuscodes.h */

/** @brief A name qualified by a namespace. */
typedef struct {
    UA_UInt16 namespaceIndex;
    UA_String name;
} UA_QualifiedName;

/** @brief Human readable text with an optional locale identifier. */
typedef struct {
    UA_String locale;
    UA_String text;
} UA_LocalizedText;

/** @brief A structure that contains an application specific data type that may
    not be recognized by the receiver. */
typedef struct {
    UA_NodeId typeId;
    enum {
        UA_EXTENSIONOBJECT_ENCODINGMASK_NOBODYISENCODED  = 0,
        UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISBYTESTRING = 1,
        UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISXML        = 2
    } encoding;
    UA_ByteString body; // contains either the bytestring or a pointer to the memory-object
} UA_ExtensionObject;

/** @brief Pointers to data that is stored in memory. The "type" of the data is
    stored in the variant itself. */
typedef struct {
    UA_Int32  arrayLength;        // total number of elements in the data-pointer
    void     *dataPtr;
    UA_Int32  arrayDimensionsLength;
    UA_Int32 *arrayDimensions;
} UA_VariantData;

/** @brief A datasource is the interface to interact with a local data provider.
 *
 *  Implementors of datasources need to provide functions for the callbacks in
 *  this structure. After every read, the handle needs to be released to
 *  indicate that the pointer is no longer accessed. As a rule, datasources are
 *  never copied, but only their content. The only way to write into a
 *  datasource is via the write-service. */
typedef struct {
    const void *handle;
    UA_StatusCode (*read)(const void *handle, const UA_VariantData **);
    void (*release)(const void *handle, const UA_VariantData *);
    UA_StatusCode (*write)(const void **handle, const UA_VariantData *);
    void (*delete)(const void *handle);
} UA_VariantDataSource;

struct UA_DataType;
typedef struct UA_DataType UA_DataType; 

/** @brief Variants store (arrays of) any data type. Either they provide a pointer to the data in
    memory, or functions from which the data can be accessed. Variants are replaced together with
    the data they store (exception: use a data source).*/
typedef struct {
    const UA_DataType *type;
    UA_NodeId typeId;
    enum {
        UA_VARIANT_DATA, ///< The data is "owned" by this variant (copied and deleted together)
        UA_VARIANT_DATA_NODELETE, /**< The data is "borrowed" by the variant and shall not be
                                       deleted at the end of this variant's lifecycle. It is not
                                       possible to overwrite borrowed data due to concurrent access.
                                       Use a custom datasource with a mutex. */
        UA_VARIANT_DATASOURCE /**< The data is provided externally. Call the functions in the
                                   datasource to get a current version */
    } storageType;
    union {
        UA_VariantData       data;
        UA_VariantDataSource datasource;
    } storage;
} UA_Variant;

/** @brief A data value with an associated status code and timestamps. */
typedef struct {
    UA_Boolean    hasVariant : 1;
    UA_Boolean    hasStatus : 1;
    UA_Boolean    hasSourceTimestamp : 1;
    UA_Boolean    hasServerTimestamp : 1;
    UA_Boolean    hasSourcePicoseconds : 1;
    UA_Boolean    hasServerPicoseconds : 1;
    UA_Variant    value;
    UA_StatusCode status;
    UA_DateTime   sourceTimestamp;
    UA_Int16      sourcePicoseconds;
    UA_DateTime   serverTimestamp;
    UA_Int16      serverPicoseconds;
} UA_DataValue;

/** @brief A structure that contains detailed error and diagnostic information associated with a StatusCode. */
typedef struct UA_DiagnosticInfo {
    UA_Boolean    hasSymbolicId : 1;
    UA_Boolean    hasNamespaceUri : 1;
    UA_Boolean    hasLocalizedText : 1;
    UA_Boolean    hasLocale : 1;
    UA_Boolean    hasAdditionalInfo : 1;
    UA_Boolean    hasInnerStatusCode : 1;
    UA_Boolean    hasInnerDiagnosticInfo : 1;
    UA_Int32      symbolicId;
    UA_Int32      namespaceUri;
    UA_Int32      localizedText;
    UA_Int32      locale;
    UA_String     additionalInfo;
    UA_StatusCode innerStatusCode;
    struct UA_DiagnosticInfo *innerDiagnosticInfo;
} UA_DiagnosticInfo;

#define UA_TYPE_HANDLING_FUNCTIONS(TYPE)                             \
    TYPE UA_EXPORT * TYPE##_new(void);                               \
    void UA_EXPORT TYPE##_init(TYPE * p);                            \
    void UA_EXPORT TYPE##_delete(TYPE * p);                          \
    void UA_EXPORT TYPE##_deleteMembers(TYPE * p);                   \
    UA_StatusCode UA_EXPORT TYPE##_copy(const TYPE *src, TYPE *dst);

/* Functions for all types */
UA_TYPE_HANDLING_FUNCTIONS(UA_Boolean)
UA_TYPE_HANDLING_FUNCTIONS(UA_SByte)
UA_TYPE_HANDLING_FUNCTIONS(UA_Byte)
UA_TYPE_HANDLING_FUNCTIONS(UA_Int16)
UA_TYPE_HANDLING_FUNCTIONS(UA_UInt16)
UA_TYPE_HANDLING_FUNCTIONS(UA_Int32)
UA_TYPE_HANDLING_FUNCTIONS(UA_UInt32)
UA_TYPE_HANDLING_FUNCTIONS(UA_Int64)
UA_TYPE_HANDLING_FUNCTIONS(UA_UInt64)
UA_TYPE_HANDLING_FUNCTIONS(UA_Float)
UA_TYPE_HANDLING_FUNCTIONS(UA_Double)
UA_TYPE_HANDLING_FUNCTIONS(UA_String)
#define UA_DateTime_new UA_Int64_new
#define UA_DateTime_init UA_Int64_init
#define UA_DateTime_delete UA_Int64_delete
#define UA_DateTime_deleteMembers UA_Int64_deleteMembers
#define UA_DateTime_copy UA_Int64_copy
UA_TYPE_HANDLING_FUNCTIONS(UA_Guid)
#define UA_ByteString_new UA_String_new
#define UA_ByteString_init UA_String_init
#define UA_ByteString_delete UA_String_delete
#define UA_ByteString_deleteMembers UA_String_deleteMembers
#define UA_ByteString_copy UA_String_copy
#define UA_XmlElement_new UA_String_new
#define UA_XmlElement_init UA_String_init
#define UA_XmlElement_delete UA_String_delete
#define UA_XmlElement_deleteMembers UA_String_deleteMembers
#define UA_XmlElement_copy UA_String_copy
UA_TYPE_HANDLING_FUNCTIONS(UA_NodeId)
UA_TYPE_HANDLING_FUNCTIONS(UA_ExpandedNodeId)
#define UA_StatusCode_new UA_Int32_new
#define UA_StatusCode_init UA_Int32_init
#define UA_StatusCode_delete UA_Int32_delete
#define UA_StatusCode_deleteMembers UA_Int32_deleteMembers
#define UA_StatusCode_copy UA_Int32_copy
UA_TYPE_HANDLING_FUNCTIONS(UA_QualifiedName)
UA_TYPE_HANDLING_FUNCTIONS(UA_LocalizedText)
UA_TYPE_HANDLING_FUNCTIONS(UA_ExtensionObject)
UA_TYPE_HANDLING_FUNCTIONS(UA_DataValue)
UA_TYPE_HANDLING_FUNCTIONS(UA_Variant)
UA_TYPE_HANDLING_FUNCTIONS(UA_DiagnosticInfo)

/**********************************************/
/* Custom functions for the builtin datatypes */
/**********************************************/

/* String */
#define UA_STRING_NULL (UA_String) {-1, (UA_Byte*)0 }
#define UA_STRING_ASSIGN(VARIABLE, STRING) do { \
        VARIABLE.length = sizeof(STRING)-1;     \
        VARIABLE.data   = (UA_Byte *)STRING; } while(0)

UA_StatusCode UA_EXPORT UA_String_copycstring(char const *src, UA_String *dst);
UA_StatusCode UA_EXPORT UA_String_copyprintf(char const *fmt, UA_String *dst, ...);
UA_Boolean UA_EXPORT UA_String_equal(const UA_String *string1, const UA_String *string2);

/* DateTime */
UA_DateTime UA_EXPORT UA_DateTime_now(void);
typedef struct UA_DateTimeStruct {
    UA_Int16 nanoSec;
    UA_Int16 microSec;
    UA_Int16 milliSec;
    UA_Int16 sec;
    UA_Int16 min;
    UA_Int16 hour;
    UA_Int16 day;
    UA_Int16 month;
    UA_Int16 year;
} UA_DateTimeStruct;
UA_DateTimeStruct UA_EXPORT UA_DateTime_toStruct(UA_DateTime time);
UA_StatusCode UA_EXPORT UA_DateTime_toString(UA_DateTime time, UA_String *timeString);

/* Guid */
UA_Boolean UA_EXPORT UA_Guid_equal(const UA_Guid *g1, const UA_Guid *g2);
/** Do not use for security-critical entropy! */
UA_Guid UA_EXPORT UA_Guid_random(UA_UInt32 *seed);

/* ByteString */
UA_Boolean UA_EXPORT UA_ByteString_equal(const UA_ByteString *string1, const UA_ByteString *string2);
UA_StatusCode UA_EXPORT UA_ByteString_newMembers(UA_ByteString *p, UA_Int32 length);

/* NodeId */
UA_Boolean UA_EXPORT UA_NodeId_equal(const UA_NodeId *n1, const UA_NodeId *n2);
UA_Boolean UA_EXPORT UA_NodeId_isNull(const UA_NodeId *p);
#define UA_NODEID_ASSIGN(VARIABLE, NUMERICID, NAMESPACE) do { \
    VARIABLE.namespaceIndex = NAMESPACE;                      \
    VARIABLE.identifierType = UA_NODEIDTYPE_NUMERIC;          \
    VARIABLE.identifier.numeric = NUMERICID; } while(0);
#define UA_NODEID_STATIC(NUMERICID, NAMESPACE)                          \
    (UA_NodeId){.namespaceIndex = NAMESPACE, .identifierType = UA_NODEIDTYPE_NUMERIC, \
            .identifier.numeric = NUMERICID}

/* ExpandedNodeId */
UA_Boolean UA_EXPORT UA_ExpandedNodeId_isNull(const UA_ExpandedNodeId *p);
#define UA_EXPANDEDNODEID_STATIC(NUMERICID, NAMESPACE)                  \
    (UA_ExpandedNodeId){.nodeId = {.namespaceIndex = NAMESPACE, .identifierType = UA_NODEIDTYPE_NUMERIC, \
                                   .identifier.numeric = NUMERICID},    \
            .serverIndex = 0, .namespaceUri = {.length = -1, .data = UA_NULL} }

/* QualifiedName */
UA_StatusCode UA_EXPORT UA_QualifiedName_copycstring(char const *src, UA_QualifiedName *dst);
void UA_EXPORT UA_QualifiedName_printf(char const *label, const UA_QualifiedName *qn);
#define UA_QUALIFIEDNAME_ASSIGN(VARIABLE, STRING) do {          \
        VARIABLE.namespaceIndex = 0;                            \
        UA_STRING_ASSIGN(VARIABLE.name, STRING); } while(0)
    

/* LocalizedText */
UA_StatusCode UA_EXPORT UA_LocalizedText_copycstring(char const *src, UA_LocalizedText *dst);

/* Variant */
UA_StatusCode UA_EXPORT UA_Variant_setValue(UA_Variant *v, void *p, UA_UInt16 typeIndex);
UA_StatusCode UA_EXPORT UA_Variant_copySetValue(UA_Variant *v, const void *p, UA_UInt16 typeIndex);
UA_StatusCode UA_EXPORT UA_Variant_setArray(UA_Variant *v, void *array, UA_Int32 noElements, UA_UInt16 typeIndex);
UA_StatusCode UA_EXPORT UA_Variant_copySetArray(UA_Variant *v, const void *array, UA_Int32 noElements, UA_UInt16 typeIndex);

/****************************/
/* Structured Type Handling */
/****************************/

#define UA_MAX_TYPE_MEMBERS 13 // Maximum number of members per complex type

#ifndef _WIN32
# define UA_BITFIELD(SIZE) : SIZE
#else
# define UA_BITFIELD(SIZE)
#endif

typedef struct {
    UA_UInt16 memberTypeIndex UA_BITFIELD(9); ///< Index of the member in the datatypetable
    UA_Boolean namespaceZero UA_BITFIELD(1); /**< The type of the member is defined in namespace
                                                  zero. In this implementation, types from custom
                                                  namespace may contain members from the same
                                                  namespace or ns0 only.*/
    UA_Byte padding UA_BITFIELD(5); /**< How much padding is there before this member element? For
                                         arrays this is split into 2 bytes padding for for the
                                         length index (max 4 bytes) and 3 bytes padding for the
                                         pointer (max 8 bytes) */
    UA_Boolean isArray UA_BITFIELD(1); ///< The member is an array of the given type
} UA_DataTypeMember;
    
struct UA_DataType {
    size_t memSize UA_BITFIELD(16); ///< Size of the struct in memory
    size_t typeIndex UA_BITFIELD(13); ///< Index of the type in the datatytypetable
    UA_Boolean namespaceZero UA_BITFIELD(1); ///< The type is defined in namespace zero.
    UA_Boolean fixedSize UA_BITFIELD(1); ///< The type (and its members) contains no pointers
    UA_Boolean zeroCopyable UA_BITFIELD(1); ///< Can the type be copied directly off the stream?
    UA_Byte membersSize; ///< How many members does the type have?
    UA_DataTypeMember members[UA_MAX_TYPE_MEMBERS];
};

void UA_EXPORT * UA_new(const UA_DataType *dataType);
void UA_EXPORT UA_init(void *p, const UA_DataType *dataType);
UA_StatusCode UA_EXPORT UA_copy(const void *src, void *dst, const UA_DataType *dataType);
void UA_EXPORT UA_deleteMembers(void *p, const UA_DataType *dataType);
void UA_EXPORT UA_delete(void *p, const UA_DataType *dataType);

/********************/
/* Array operations */
/********************/

#define MAX_ARRAY_SIZE 104857600 // arrays must be smaller than 100MB

UA_StatusCode UA_EXPORT UA_Array_new(void **p, UA_Int32 noElements, const UA_DataType *dataType);
UA_StatusCode UA_EXPORT UA_Array_copy(const void *src, UA_Int32 noElements, void **dst, const UA_DataType *dataType);
void UA_EXPORT UA_Array_delete(void *p, UA_Int32 noElements, const UA_DataType *dataType);

/** @} */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* UA_TYPES_H_ */
