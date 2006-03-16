/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.13u-20020617-2210
 * 
 * This file is not intended to be easily readable and contains a number of 
 * coding conventions designed to improve portability and efficiency. Do not make
 * changes to this file unless you know what you are doing--modify the SWIG 
 * interface file instead. 
 * ----------------------------------------------------------------------------- */


#ifdef __cplusplus
template<class T> class SwigValueWrapper {
    T *tt;
public:
    inline SwigValueWrapper() : tt(0) { }
    inline ~SwigValueWrapper() { if (tt) delete tt; } 
    inline SwigValueWrapper& operator=(const T& t) { tt = new T(t); return *this; }
    inline operator T&() const { return *tt; }
    inline T *operator&() { return tt; }
};                                                    
#endif

/***********************************************************************
 * common.swg
 *
 *     This file contains generic SWIG runtime support for pointer
 *     type checking as well as a few commonly used macros to control
 *     external linkage.
 *
 * Author : David Beazley (beazley@cs.uchicago.edu)
 *
 * Copyright (c) 1999-2000, The University of Chicago
 * 
 * This file may be freely redistributed without license or fee provided
 * this copyright message remains intact.
 ************************************************************************/

#include <string.h>

#if defined(_WIN32) || defined(__WIN32__)
#       if defined(_MSC_VER)
#               if defined(STATIC_LINKED)
#                       define SWIGEXPORT(a) a
#                       define SWIGIMPORT(a) extern a
#               else
#                       define SWIGEXPORT(a) __declspec(dllexport) a
#                       define SWIGIMPORT(a) extern a
#               endif
#       else
#               if defined(__BORLANDC__)
#                       define SWIGEXPORT(a) a _export
#                       define SWIGIMPORT(a) a _export
#               else
#                       define SWIGEXPORT(a) a
#                       define SWIGIMPORT(a) a
#               endif
#       endif
#else
#       define SWIGEXPORT(a) a
#       define SWIGIMPORT(a) a
#endif

#ifdef SWIG_GLOBAL
#define SWIGRUNTIME(a) SWIGEXPORT(a)
#else
#define SWIGRUNTIME(a) static a
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void *(*swig_converter_func)(void *);
typedef struct swig_type_info *(*swig_dycast_func)(void **);

typedef struct swig_type_info {
  const char             *name;                 
  swig_converter_func     converter;
  const char             *str;
  swig_dycast_func        dcast;
  struct swig_type_info  *next;
  struct swig_type_info  *prev;
  void                   *clientdata;	
} swig_type_info;

#ifdef SWIG_NOINCLUDE

SWIGIMPORT(swig_type_info *) SWIG_TypeRegister(swig_type_info *);
SWIGIMPORT(swig_type_info *) SWIG_TypeCheck(char *c, swig_type_info *);
SWIGIMPORT(void *)           SWIG_TypeCast(swig_type_info *, void *);
SWIGIMPORT(swig_type_info *) SWIG_TypeDynamicCast(swig_type_info *, void **);
SWIGIMPORT(swig_type_info *) SWIG_TypeQuery(const char *);
SWIGIMPORT(void)             SWIG_TypeClientData(swig_type_info *, void *);

#else

static swig_type_info *swig_type_list = 0;

/* Register a type mapping with the type-checking */
SWIGRUNTIME(swig_type_info *)
SWIG_TypeRegister(swig_type_info *ti)
{
  swig_type_info *tc, *head, *ret, *next;
  /* Check to see if this type has already been registered */
  tc = swig_type_list;
  while (tc) {
    if (strcmp(tc->name, ti->name) == 0) {
      /* Already exists in the table.  Just add additional types to the list */
      head = tc;
      next = tc->next;
      goto l1;
    }
    tc = tc->prev;
  }
  head = ti;
  next = 0;

  /* Place in list */
  ti->prev = swig_type_list;
  swig_type_list = ti;

  /* Build linked lists */
 l1:
  ret = head;
  tc = ti + 1;
  /* Patch up the rest of the links */
  while (tc->name) {
    head->next = tc;
    tc->prev = head;
    head = tc;
    tc++;
  }
  head->next = next;
  return ret;
}

/* Check the typename */
SWIGRUNTIME(swig_type_info *) 
SWIG_TypeCheck(char *c, swig_type_info *ty)
{
  swig_type_info *s;
  if (!ty) return 0;        /* Void pointer */
  s = ty->next;             /* First element always just a name */
  while (s) {
    if (strcmp(s->name,c) == 0) {
      if (s == ty->next) return s;
      /* Move s to the top of the linked list */
      s->prev->next = s->next;
      if (s->next) {
	s->next->prev = s->prev;
      }
      /* Insert s as second element in the list */
      s->next = ty->next;
      if (ty->next) ty->next->prev = s;
      ty->next = s;
      return s;
    }
    s = s->next;
  }
  return 0;
}

/* Cast a pointer up an inheritance hierarchy */
SWIGRUNTIME(void *) 
SWIG_TypeCast(swig_type_info *ty, void *ptr) 
{
  if ((!ty) || (!ty->converter)) return ptr;
  return (*ty->converter)(ptr);
}

/* Dynamic pointer casting. Down an inheritance hierarchy */
SWIGRUNTIME(swig_type_info *) 
SWIG_TypeDynamicCast(swig_type_info *ty, void **ptr) 
{
  swig_type_info *lastty = ty;
  if (!ty || !ty->dcast) return ty;
  while (ty && (ty->dcast)) {
     ty = (*ty->dcast)(ptr);
     if (ty) lastty = ty;
  }
  return lastty;
}

/* Search for a swig_type_info structure */
SWIGRUNTIME(swig_type_info *)
SWIG_TypeQuery(const char *name) {
  swig_type_info *ty = swig_type_list;
  while (ty) {
    if (ty->str && (strcmp(name,ty->str) == 0)) return ty;
    if (ty->name && (strcmp(name,ty->name) == 0)) return ty;
    ty = ty->prev;
  }
  return 0;
}

/* Set the clientdata field for a type */
SWIGRUNTIME(void)
SWIG_TypeClientData(swig_type_info *ti, void *clientdata) {
  swig_type_info *tc, *equiv;
  if (ti->clientdata) return;
  ti->clientdata = clientdata;
  equiv = ti->next;
  while (equiv) {
    if (!equiv->converter) {
      tc = swig_type_list;
      while (tc) {
	if ((strcmp(tc->name, equiv->name) == 0))
	  SWIG_TypeClientData(tc,clientdata);
	tc = tc->prev;
      }
    }
    equiv = equiv->next;
  }
}
#endif

#ifdef __cplusplus
}

#endif

#include <tcl.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Constant table */

#define SWIG_TCL_INT     1
#define SWIG_TCL_FLOAT   2
#define SWIG_TCL_STRING  3
#define SWIG_TCL_POINTER 4
#define SWIG_TCL_BINARY  5

/* Constant information structure */
typedef struct swig_const_info {
    int type;
    char *name;
    long lvalue;
    double dvalue;
    void   *pvalue;
    swig_type_info **ptype;
} swig_const_info;

typedef int   (*swig_wrapper)(ClientData, Tcl_Interp *, int, Tcl_Obj *CONST []);
typedef int   (*swig_wrapper_func)(ClientData, Tcl_Interp *, int, Tcl_Obj *CONST []);
typedef char *(*swig_variable_func)(ClientData, Tcl_Interp *, char *, char *, int);
typedef void  (*swig_delete_func)(ClientData);

typedef struct swig_method { 
  const char     *name;
  swig_wrapper   method;
} swig_method;

typedef struct swig_attribute {
  const char     *name;
  swig_wrapper   getmethod;
  swig_wrapper   setmethod;
} swig_attribute;
  
typedef struct swig_class {
  const char         *name;
  swig_type_info   **type;
  swig_wrapper       constructor;
  void              (*destructor)(void *);
  swig_method        *methods;
  swig_attribute     *attributes;
  struct swig_class **bases;
} swig_class;

typedef struct swig_instance {
  Tcl_Obj       *thisptr;
  void          *thisvalue;
  swig_class   *classptr;
  int            destroy;
  Tcl_Command    cmdtok;
} swig_instance;

#ifdef SWIG_NOINCLUDE
SWIGEXPORT(char *)    SWIG_PackData(char *c, void *ptr, int sz);
SWIGEXPORT(char *)    SWIG_UnpackData(char *c, void *ptr, int sz);
SWIGEXPORT(int)       SWIG_ConvertPtrFromString(Tcl_Interp *, char *, void **, swig_type_info *,int flags);
SWIGEXPORT(int)       SWIG_ConvertPtr(Tcl_Interp *, Tcl_Obj *, void **, swig_type_info *, int flags);
SWIGEXPORT(int)       SWIG_ConvertPacked(Tcl_Interp *, Tcl_Obj *, void *, int sz, swig_type_info *, int flags);
SWIGEXPORT(void)      SWIG_MakePtr(char *, void *, swig_type_info *, int flags);
SWIGEXPORT(Tcl_Obj *) SWIG_NewPointerObj(void *, swig_type_info *, int flags);
SWIGEXPORT(Tcl_Obj *) SWIG_NewPackedObj(void *, int sz, swig_type_info *, int flags);
SWIGEXPORT(int)       SWIG_GetArgs(Tcl_Interp *, int, Tcl_Obj *CONST [], const char *, ...);
SWIGEXPORT(char *)    SWIG_PointerTypeFromString(char *c);
SWIGEXPORT(void)      SWIG_Acquire(void *ptr);
SWIGEXPORT(int)       SWIG_Disown(void *ptr);
SWIGEXPORT(void)      SWIG_InstallConstants(Tcl_Interp *interp, struct swig_const_info constants[]);
SWIGEXPORT(Tcl_Obj *) SWIG_GetConstant(const char *key);
SWIGEXPORT(Tcl_Obj *) SWIG_NewInstanceObj(Tcl_Interp *interp, void *, swig_type_info *, int flags);
SWIGEXPORT(int)       SWIG_ObjectConstructor(ClientData, Tcl_Interp *, int, Tcl_Obj *CONST objv[]);
SWIGEXPORT(int)       SWIG_MethodCommand(ClientData, Tcl_Interp *, int, Tcl_Obj *CONST objv[]);
SWIGEXPORT(void)      SWIG_ObjectDelete(ClientData);
#else

/* Pack binary data into a string */
SWIGRUNTIME(char *)
SWIG_PackData(char *c, void *ptr, int sz) {
  static char hex[17] = "0123456789abcdef";
  int i;
  unsigned char *u = (unsigned char *) ptr;
  register unsigned char uu;
  for (i = 0; i < sz; i++,u++) {
    uu = *u;
    *(c++) = hex[(uu & 0xf0) >> 4];
    *(c++) = hex[uu & 0xf];
  }
  return c;
}

/* Unpack binary data from a string */
SWIGRUNTIME(char *)
SWIG_UnpackData(char *c, void *ptr, int sz) {
  register unsigned char uu = 0;
  register int d;
  unsigned char *u = (unsigned char *) ptr;
  int i;
  for (i = 0; i < sz; i++, u++) {
    d = *(c++);
    if ((d >= '0') && (d <= '9'))
      uu = ((d - '0') << 4);
    else if ((d >= 'a') && (d <= 'f'))
      uu = ((d - ('a'-10)) << 4);
    d = *(c++);
    if ((d >= '0') && (d <= '9'))
      uu |= (d - '0');
    else if ((d >= 'a') && (d <= 'f'))
      uu |= (d - ('a'-10));
    *u = uu;
  }
  return c;
}

/* Convert a pointer value */
SWIGRUNTIME(int)
SWIG_ConvertPtrFromString(Tcl_Interp *interp, char *c, void **ptr, swig_type_info *ty, int flags)
{
  swig_type_info *tc;
  /* Pointer values must start with leading underscore */
  if (*c != '_') {
    *ptr = (void *) 0;
    if (strcmp(c,"NULL") == 0) return TCL_OK;
    Tcl_SetResult(interp, (char *) "Type error. Expected a pointer", TCL_STATIC);
    return TCL_ERROR;
  }
  c++;
  c = SWIG_UnpackData(c,ptr,sizeof(void *));
  if (ty) {
    tc = SWIG_TypeCheck(c,ty);
    if (!tc) {
      Tcl_SetResult(interp, (char *) "Type error. Expected ", TCL_STATIC);
      Tcl_AppendElement(interp, (char *) ty->name);
      return TCL_ERROR;
    }
    *ptr = SWIG_TypeCast(tc,(void *) *ptr);
  }
  flags = 0;
  return TCL_OK;
}

/* Convert a pointer value */
SWIGRUNTIME(int)
SWIG_ConvertPtr(Tcl_Interp *interp, Tcl_Obj *oc, void **ptr, swig_type_info *ty, int flags)
{
  return SWIG_ConvertPtrFromString(interp, Tcl_GetStringFromObj(oc,NULL), ptr, ty, flags);
}

/* Convert a pointer value */
SWIGRUNTIME(char *)
SWIG_PointerTypeFromString(char *c) {
  char d;
  /* Pointer values must start with leading underscore. NULL has no type */
  if (*c != '_') {
    return 0;
  }
  c++;
  /* Extract hex value from pointer */
  while ((d = *c)) {
    if (!(((d >= '0') && (d <= '9')) || ((d >= 'a') && (d <= 'f')))) break;
    c++;
  }
  return c;
}

/* Convert a packed value value */
SWIGRUNTIME(int)
SWIG_ConvertPacked(Tcl_Interp *interp, Tcl_Obj *obj, void *ptr, int sz, swig_type_info *ty, int flags) {
  swig_type_info *tc;
  char  *c;
  
  if (!obj) goto type_error;
  c = Tcl_GetStringFromObj(obj,NULL);
  /* Pointer values must start with leading underscore */
  if (*c != '_') goto type_error;
  c++;
  c = SWIG_UnpackData(c,ptr,sz);
  if (ty) {
    tc = SWIG_TypeCheck(c,ty);
    if (!tc) goto type_error;
  }
  return TCL_OK;

type_error:

  if (flags) {
    if (ty) {
      Tcl_SetResult(interp, (char *) "Type error. Expected ", TCL_STATIC);
      Tcl_AppendElement(interp, (char *) ty->name);
      return TCL_ERROR;
    } else {
      Tcl_SetResult(interp, (char *) "Expected packed data.", TCL_STATIC);
      return TCL_ERROR;
    }
  }
  return TCL_ERROR;
}


/* Take a pointer and convert it to a string */
SWIGRUNTIME(void) 
SWIG_MakePtr(char *c, void *ptr, swig_type_info *ty, int flags) {
  if (ptr) {
    *(c++) = '_';
    c = SWIG_PackData(c,&ptr,sizeof(void *));
    strcpy(c,ty->name);
  } else {
    strcpy(c,(char *)"NULL");
  }
  flags = 0;
}

/* Create a new pointer object */
SWIGRUNTIME(Tcl_Obj *)
SWIG_NewPointerObj(void *ptr, swig_type_info *type, int flags) {
  Tcl_Obj *robj;
  char result[512];
  SWIG_MakePtr(result,ptr,type,flags);
  robj = Tcl_NewStringObj(result,-1);
  return robj;
}

SWIGRUNTIME(Tcl_Obj *)
SWIG_NewPackedObj(void *ptr, int sz, swig_type_info *type, int flags) {
  char result[1024];
  char *r = result;
  if ((2*sz + 1 + strlen(type->name)) > 1000) return 0;
  *(r++) = '_';
  r = SWIG_PackData(r,ptr,sz);
  strcpy(r,type->name);
  flags = 0;
  return Tcl_NewStringObj(result,-1);
}

static Tcl_HashTable   swigconstTable;
static int             swigconstTableinit = 0;

/* Install Constants */
SWIGRUNTIME(void)
SWIG_InstallConstants(Tcl_Interp *interp, swig_const_info constants[]) {
  int i;
  Tcl_Obj *obj;
  Tcl_HashEntry *entryPtr;
  int            newobj;

  if (!swigconstTableinit) {
    Tcl_InitHashTable(&swigconstTable, TCL_STRING_KEYS);
    swigconstTableinit = 1;
  }
  for (i = 0; constants[i].type; i++) {
    switch(constants[i].type) {
    case SWIG_TCL_INT:
      obj = Tcl_NewIntObj(constants[i].lvalue);
      break;
    case SWIG_TCL_FLOAT:
      obj = Tcl_NewDoubleObj(constants[i].dvalue);
      break;
    case SWIG_TCL_STRING:
      obj = Tcl_NewStringObj((char *) constants[i].pvalue,-1);
      break;
    case SWIG_TCL_POINTER:
      obj = SWIG_NewPointerObj(constants[i].pvalue, *(constants[i]).ptype,0);
      break;
    case SWIG_TCL_BINARY:
      obj = SWIG_NewPackedObj(constants[i].pvalue, constants[i].lvalue, *(constants[i].ptype),0);
      break;
    default:
      obj = 0;
      break;
    }
    if (obj) {
      Tcl_ObjSetVar2(interp,Tcl_NewStringObj(constants[i].name,-1), NULL, obj, TCL_GLOBAL_ONLY);
      entryPtr = Tcl_CreateHashEntry(&swigconstTable, constants[i].name, &newobj);
      Tcl_SetHashValue(entryPtr, (ClientData) obj);
    }
  }
}

SWIGRUNTIME(Tcl_Obj *)
SWIG_GetConstant(const char *key) {
  Tcl_HashEntry *entryPtr;
  if (!swigconstTableinit) return 0;
  entryPtr = Tcl_FindHashEntry(&swigconstTable, key);
  if (entryPtr) {
    return (Tcl_Obj *) Tcl_GetHashValue(entryPtr);
  }
  printf("Searching %s\n", key);
  return 0;
}

/* Get arguments */
SWIGRUNTIME(int)
SWIG_GetArgs(Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[], const char *fmt, ...)
{
  int        argno = 0, opt = 0, tempi;
  double     tempd;
  const char *c;
  va_list    ap;
  void      *vptr;
  Tcl_Obj   *obj = 0;
  swig_type_info *ty;

  va_start(ap,fmt);
  for (c = fmt; (*c && (*c != ':') && (*c != ';')); c++,argno++) {
    if (*c == '|') {
      opt = 1;
      c++;
    }
    if (argno >= (objc-1)) {
      if (!opt) {
	Tcl_SetResult(interp, (char *) "Wrong # args. ", TCL_STATIC);
	goto argerror;
      } else {
	va_end(ap);
	return TCL_OK;
      }
    }

    vptr = va_arg(ap,void *);
    if (vptr) {
      if (isupper(*c)) {
	obj = SWIG_GetConstant(Tcl_GetStringFromObj(objv[argno+1],0));
	if (!obj) obj = objv[argno+1];
      } else {
	obj = objv[argno+1];
      }
      switch(*c) {
      case 'i': case 'I':
      case 'l': case 'L':
      case 'h': case 'H':
      case 'b': case 'B':
	if (Tcl_GetIntFromObj(interp,obj,&tempi) != TCL_OK) goto argerror;
	if ((*c == 'i') || (*c == 'I')) *((int *)vptr) = tempi;
	else if ((*c == 'l') || (*c == 'L')) *((long *)vptr) = tempi;
	else if ((*c == 'h') || (*c == 'H')) *((short*)vptr) = tempi;
	else if ((*c == 'b') || (*c == 'B')) *((unsigned char *)vptr) = tempi;
	break;
      case 'f': case 'F':
      case 'd': case 'D':
	if (Tcl_GetDoubleFromObj(interp,obj,&tempd) != TCL_OK) goto argerror;
	if ((*c == 'f') || (*c == 'F')) *((float *) vptr) = (float)tempd;
	else if ((*c == 'd') || (*c == 'D')) *((double*) vptr) = tempd;
	break;
      case 's': case 'S':
	if (*(c+1) == '#') {
	  int *vlptr = (int *) va_arg(ap, void *);
	  *((char **) vptr) = Tcl_GetStringFromObj(obj, vlptr);
	  c++;
	} else {
	  *((char **)vptr) = Tcl_GetStringFromObj(obj,NULL);
	}
	break;
      case 'c': case 'C':
	*((char *)vptr) = *(Tcl_GetStringFromObj(obj,NULL));
	break;
      case 'p': case 'P':
	ty = (swig_type_info *) va_arg(ap, void *);
	if (SWIG_ConvertPtr(interp, obj, (void **) vptr, ty,0) == TCL_ERROR) goto argerror;
	break;
      case 'o': case 'O':
	*((Tcl_Obj **)vptr) = objv[argno+1];
	break;
      default:
	break;
      }
    }
  }
  
  if ((*c != ';') && ((objc-1) > argno)) {
    Tcl_SetResult(interp, (char *) "Wrong # args.", TCL_STATIC);
    goto argerror;
  }
  va_end(ap);
  return TCL_OK;
  
 argerror:
  {
    char temp[32];
    sprintf(temp,"%d", argno+1);
    c = strchr(fmt,':');
    if (!c) c = strchr(fmt,';');
    if (!c) c = (char *)"";
    Tcl_AppendResult(interp,c," argument ", temp, NULL);
    va_end(ap);
    return TCL_ERROR;
  }
}

/* Object support */
static Tcl_HashTable  swigobjectTable;
static int            swigobjectTableinit = 0;

/* Acquire ownership of a pointer */
SWIGRUNTIME(void)
SWIG_Acquire(void *ptr)
{
  Tcl_HashEntry *entryPtr;
  int newobj;
  if (!swigobjectTableinit) {
    Tcl_InitHashTable(&swigobjectTable, TCL_ONE_WORD_KEYS);
    swigobjectTableinit = 1;
  }
  entryPtr = Tcl_CreateHashEntry(&swigobjectTable, (char *) ptr, &newobj);
}

/* Disown a pointer.  Returns 1 if we owned it to begin with */
SWIGRUNTIME(int)
SWIG_Disown(void *ptr)
{
  Tcl_HashEntry *entryPtr;
  if (!swigobjectTableinit) return 0;
  entryPtr = Tcl_FindHashEntry(&swigobjectTable, (char *) ptr);
  if (entryPtr) {
    Tcl_DeleteHashEntry(entryPtr);
    return 1;
  } 
  return 0;
}

SWIGRUNTIME(void)
SWIG_ObjectDelete(ClientData clientData) {
  swig_instance *si = (swig_instance *) clientData;
  if ((si) && (si->destroy) && (SWIG_Disown(si->thisvalue))) {
    if (si->classptr->destructor) {
      (si->classptr->destructor)(si->thisvalue);
    }
  }
  Tcl_DecrRefCount(si->thisptr);
  free(si);
}

/* Function to invoke object methods given an instance */
SWIGRUNTIME(int)
SWIG_MethodCommand(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST _objv[])
{
  char *method,   *attrname;
  swig_instance   *inst = (swig_instance *) clientData;
  swig_method     *meth;
  swig_attribute  *attr;
  Tcl_Obj         *oldarg;
  Tcl_Obj         **objv;
  int              rcode;
  swig_class      *cls;
  swig_class      *cls_stack[64];
  int              cls_stack_bi[64];
  int              cls_stack_top = 0;
  int              numconf = 2;
  int              bi;

  objv = (Tcl_Obj **) _objv;
  if (objc < 2) {
    Tcl_SetResult(interp, (char *) "wrong # args.", TCL_STATIC);
    return TCL_ERROR;
  }
  method = Tcl_GetStringFromObj(objv[1],NULL);
  if (strcmp(method,"-acquire") == 0) {
    inst->destroy = 1;
    SWIG_Acquire(inst->thisvalue);
    return TCL_OK;
  } 
  if (strcmp(method,"-disown") == 0) {
    if (inst->destroy) {
      SWIG_Disown(inst->thisvalue);
    }
    inst->destroy = 0;
    return TCL_OK;
  }
  if (strcmp(method,"-delete") == 0) {
    Tcl_DeleteCommandFromToken(interp,inst->cmdtok);
    return TCL_OK;
  }
  cls_stack[cls_stack_top] = inst->classptr;
  cls_stack_bi[cls_stack_top] = -1;
  cls = inst->classptr;
  while (1) {
    bi = cls_stack_bi[cls_stack_top];
    cls = cls_stack[cls_stack_top];
    if (bi != -1) {
      cls = cls->bases[bi];
      if (cls) {
	cls_stack_bi[cls_stack_top]++;
	cls_stack_top++;
	cls_stack[cls_stack_top] = cls;
	cls_stack_bi[cls_stack_top] = -1;
	continue;
      }
    }
    if (!cls) {
      cls_stack_top--;
      if (cls_stack_top < 0) break;
      else continue;
    }
    cls_stack_bi[cls_stack_top]++;

    meth = cls->methods;
    /* Check for methods */
    while (meth && meth->name) {
      if (strcmp(meth->name,method) == 0) {
	oldarg = objv[1];
	objv[1] = inst->thisptr;
	Tcl_IncrRefCount(inst->thisptr);
	rcode = (*meth->method)(clientData,interp,objc,objv);
	objv[1] = oldarg;
	Tcl_DecrRefCount(inst->thisptr);
	return rcode;
      }
      meth++;
    }
    /* Check class methods for a match */
    if (strcmp(method,"cget") == 0) {
      if (objc < 3) {
	Tcl_SetResult(interp, (char *) "wrong # args.", TCL_STATIC);
	return TCL_ERROR;
      }
      attrname = Tcl_GetStringFromObj(objv[2],NULL);
      attr = cls->attributes;
      while (attr && attr->name) {
	if ((strcmp(attr->name, attrname) == 0) && (attr->getmethod)) {
	  oldarg = objv[1];
	  objv[1] = inst->thisptr;
	  Tcl_IncrRefCount(inst->thisptr);
	  rcode = (*attr->getmethod)(clientData,interp,2, objv);
	  objv[1] = oldarg;
	  Tcl_DecrRefCount(inst->thisptr);
	  return rcode;
	}
	attr++;
      }
      if (strcmp(attrname, "-this") == 0) {
	Tcl_SetObjResult(interp, Tcl_DuplicateObj(inst->thisptr));
	return TCL_OK;
      }
    } else if (strcmp(method, "configure") == 0) {
      int i;
      if (objc < 4) {
	Tcl_SetResult(interp, (char *) "wrong # args.", TCL_STATIC);
	return TCL_ERROR;
      }
      i = 2;
      while (i < objc) {
	attrname = Tcl_GetStringFromObj(objv[i],NULL);
	attr = cls->attributes;
	while (attr && attr->name) {
	  if ((strcmp(attr->name, attrname) == 0) && (attr->setmethod)) {
	    oldarg = objv[i];
	    objv[i] = inst->thisptr;
	    Tcl_IncrRefCount(inst->thisptr);
	    rcode = (*attr->setmethod)(clientData,interp,3, &objv[i-1]);
	    objv[i] = oldarg;
	    Tcl_DecrRefCount(inst->thisptr);
	    if (rcode != TCL_OK) return rcode;
	    numconf += 2;
	  }
	  attr++;
	}
	i+=2;
      }
    }
  }
  if (strcmp(method,"configure") == 0) {
    if (numconf >= objc) {
      return TCL_OK;
    } else {
      Tcl_SetResult(interp,(char *) "Invalid attribute name.", TCL_STATIC);
      return TCL_ERROR;
    }
  }
  if (strcmp(method,"cget") == 0) {
      Tcl_SetResult(interp,(char *) "Invalid attribute name.", TCL_STATIC);
      return TCL_ERROR;
  }

  Tcl_SetResult(interp, (char *) "Invalid method. Must be one of: configure cget -acquire -disown -delete", TCL_STATIC);
  cls = inst->classptr;
  bi = 0;
  while (cls) {
    meth = cls->methods;
    while (meth && meth->name) {
      char *cr = (char *) Tcl_GetStringResult(interp);
      if (!strstr(strchr(cr,':'), meth->name))
	Tcl_AppendElement(interp, (char *) meth->name);
      meth++;
    }
    cls = inst->classptr->bases[bi++];
  }
  return TCL_ERROR;
}

/* Function to create objects */
SWIGRUNTIME(int)
SWIG_ObjectConstructor(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
    Tcl_Obj          *newObj = 0;
    void             *thisvalue = 0;
    swig_instance   *newinst = 0;
    swig_class      *classptr = (swig_class *) clientData;
    swig_wrapper     cons = 0;
    char             *name = 0;
    int               firstarg = 0;
    int               thisarg = 0;
    int               destroy = 1; 

    if (!classptr) {
      Tcl_SetResult(interp, (char *) "swig: internal runtime error. No class object defined.", TCL_STATIC);
      return TCL_ERROR;
    }
    cons = classptr->constructor;
    if (objc > 1) {
      char *s = Tcl_GetStringFromObj(objv[1],NULL);
      if (strcmp(s,"-this") == 0) {
	thisarg = 2;
	cons = 0;
      } else if (strcmp(s,"-args") == 0) {
	firstarg = 1;
      } else if (objc == 2) {
	firstarg = 1;
	name = s;
      } else if (objc >= 3) {
	char *s1;
	name = s;
	s1 = Tcl_GetStringFromObj(objv[2],NULL);
	if (strcmp(s1,"-this") == 0) {
	  thisarg = 3;
	  cons = 0;
	} else {
	  firstarg = 1;
	}
      }
    }
    if (cons) {
      int result;
      result = (*cons)(0, interp, objc-firstarg, &objv[firstarg]);
      if (result != TCL_OK) {
	return result;
      }
      newObj = Tcl_DuplicateObj(Tcl_GetObjResult(interp));
      if (!name) name = Tcl_GetStringFromObj(newObj,NULL);
    } else if (thisarg > 0) {
      if (thisarg < objc) {
	destroy = 0;
	newObj = Tcl_DuplicateObj(objv[thisarg]);
	if (!name) name = Tcl_GetStringFromObj(newObj,NULL);
      } else {
	Tcl_SetResult(interp, (char *) "wrong # args.", TCL_STATIC);
	return TCL_ERROR;
      }
    } else {
      Tcl_SetResult(interp, (char *) "No constructor available.", TCL_STATIC);
      return TCL_ERROR;
    }
    if (SWIG_ConvertPtr(interp,newObj, (void **) &thisvalue, *(classptr->type),0) == TCL_ERROR) {
      Tcl_DecrRefCount(newObj);
      return TCL_ERROR;
    }
    newinst = (swig_instance *) malloc(sizeof(swig_instance));
    newinst->thisptr = newObj;
    Tcl_IncrRefCount(newObj);
    newinst->thisvalue = thisvalue;
    newinst->classptr = classptr;
    newinst->destroy = destroy;
    if (destroy) {
      SWIG_Acquire(thisvalue);
    }
    newinst->cmdtok = Tcl_CreateObjCommand(interp,name, (swig_wrapper) SWIG_MethodCommand, (ClientData) newinst, (swig_delete_func) SWIG_ObjectDelete);
    return TCL_OK;
}


/* This function takes the current result and turns it into an object command */
SWIGRUNTIME(Tcl_Obj *)
SWIG_NewInstanceObj(Tcl_Interp *interp, void *thisvalue, swig_type_info *type, int flags) {
  Tcl_Obj *robj = SWIG_NewPointerObj(thisvalue, type,0);
  /* Check to see if this pointer belongs to a class or not */
  if ((type->clientdata) && (interp)) {
    Tcl_CmdInfo    ci;
    char          *name;
    name = Tcl_GetStringFromObj(robj,NULL);
    if (!Tcl_GetCommandInfo(interp,name, &ci) || (flags)) {
      swig_instance *newinst = (swig_instance *) malloc(sizeof(swig_instance));
      newinst->thisptr = Tcl_DuplicateObj(robj);
      Tcl_IncrRefCount(newinst->thisptr);
      newinst->thisvalue = thisvalue;
      newinst->classptr = (swig_class *) type->clientdata;
      newinst->destroy = flags;
      newinst->cmdtok = Tcl_CreateObjCommand(interp, Tcl_GetStringFromObj(robj,NULL), (swig_wrapper_func) SWIG_MethodCommand, (ClientData) newinst, (swig_delete_func) SWIG_ObjectDelete);
      if (flags) {
	SWIG_Acquire(thisvalue);
      }
    }
  }
  return robj;
}

#endif

/* Structure for command table */
typedef struct {
  const char *name;
  int       (*wrapper)(ClientData, Tcl_Interp *, int, Tcl_Obj *CONST []);
  ClientData  clientdata;
} swig_command_info;

/* Structure for variable linking table */
typedef struct {
  const char *name;
  void *addr;
  char * (*get)(ClientData, Tcl_Interp *, char *, char *, int);
  char * (*set)(ClientData, Tcl_Interp *, char *, char *, int);
} swig_var_info;

#ifdef __cplusplus
}
#endif






/* -------- TYPES TABLE (BEGIN) -------- */

static swig_type_info *swig_types[1];

/* -------- TYPES TABLE (END) -------- */

#define SWIG_init    Mklbooster_Init
#define SWIG_name    "mklbooster"
#define SWIG_prefix  ""
#define SWIG_version "0.0"


#ifdef __cplusplus
extern "C" {
#endif
#ifdef MAC_TCL
#pragma export on
#endif
SWIGEXPORT(int) SWIG_init(Tcl_Interp *);
#ifdef MAC_TCL
#pragma export off
#endif
#ifdef __cplusplus
}
#endif




/*------------------------------------------------------------------------------
 *
 *	Copyright (c) 2002-2006 by M. V. Dmitrievsky and V. N. Kutrunov
 *	See COPYING file for copying and redistribution conditions.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; version 2 of the License.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	Contact info: surfit.sourceforge.net
 *----------------------------------------------------------------------------*/

#include "mklbooster_ie.h"
#include "mklbooster.h"


#ifdef __cplusplus
extern "C" {
#endif

static swig_command_info swig_commands[] = {
    {0, 0, 0}
};

static swig_var_info swig_variables[] = {
    {0,0,0,0}
};

static swig_const_info swig_constants[] = {
    {0,0,0,0,0,0}
};

/* -------- TYPE CONVERSION AND EQUIVALENCE RULES (BEGIN) -------- */


static swig_type_info *swig_types_initial[] = {
0
};


/* -------- TYPE CONVERSION AND EQUIVALENCE RULES (END) -------- */

#ifdef __cplusplus
}
#endif

SWIGEXPORT(int) SWIG_init(Tcl_Interp *interp) {
    int i;
    static int added_types = 0;
    if (interp == 0) return TCL_ERROR;
    #ifdef USE_TCL_STUBS
    if (Tcl_InitStubs(interp, (char*)"8.1", 0) == NULL) {
        return TCL_ERROR;
    }
    #endif
    
    Tcl_PkgProvide(interp, (char*)SWIG_name, (char*)SWIG_version);
    
    #ifdef SWIG_namespace
    Tcl_Eval(interp, "namespace eval " SWIG_namespace " { }");
    #endif
    if (!added_types) {
        for (i = 0; swig_types_initial[i]; i++) {
            swig_types[i] = SWIG_TypeRegister(swig_types_initial[i]);
        }
        added_types = 1;
    }
    for (i = 0; swig_commands[i].name; i++) {
        Tcl_CreateObjCommand(interp, (char *) swig_commands[i].name, (swig_wrapper_func) swig_commands[i].wrapper, swig_commands[i].clientdata, NULL);
    }
    for (i = 0; swig_variables[i].name; i++) {
        Tcl_SetVar(interp, (char *) swig_variables[i].name, (char *) "", TCL_GLOBAL_ONLY);
        Tcl_TraceVar(interp, (char *) swig_variables[i].name, TCL_TRACE_READS | TCL_GLOBAL_ONLY, (Tcl_VarTraceProc *) swig_variables[i].get, (ClientData) swig_variables[i].addr);
        Tcl_TraceVar(interp, (char *) swig_variables[i].name, TCL_TRACE_WRITES | TCL_GLOBAL_ONLY, (Tcl_VarTraceProc *) swig_variables[i].set, (ClientData) swig_variables[i].addr);
    }
    SWIG_InstallConstants(interp, swig_constants);
    
    
    surfit::init_mkl_solvers();
    
    return TCL_OK;
}

