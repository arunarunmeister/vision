
#ifndef _RUBY_EXT_H_
#define _RUBY_EXT_H_

#include <ruby.h>

#define RUBY_NAME_MAX (256)

#define REXT_PRINT(message, ...) do { printf("[%s:%u] "message, __FUNCTION__, __LINE__, ## __VA_ARGS__); } while (0)

enum rext_method_type_e {
    REXT_METHOD_SINGLETON,
    REXT_METHOD_NORMAL,
    REXT_METHOD_MODULE,
    REXT_METHOD_GLOBAL
};

// forward definitions for cross-referencings
typedef struct _rext_object_t rext_object_t;
typedef struct _rext_module_t rext_module_t;

typedef struct _rext_variable_t {
    const char name[RUBY_NAME_MAX];
    VALUE *pval;
    VALUE (*getter)(ANYARGS);
    void (*setter)(ANYARGS);
} rext_variable_t;

typedef struct _rext_const_t {
    const char name[RUBY_NAME_MAX];
    int type;
    union {
        long l;
        double f;
    } value;
} rext_const_t;

typedef struct _rext_method_t {
    const char name[RUBY_NAME_MAX];
    int argc;
    VALUE (*func)(ANYARGS);
} rext_method_t;

struct _rext_object_t {
    VALUE typeVal;
    VALUE *base;
    const char name[RUBY_NAME_MAX];
    VALUE (*allocate)(VALUE klass);
    rext_object_t *objects;
    long numObjects;
    rext_const_t *constants;
    long numConstants;
    rext_method_t *methods;
    long numMethods;
};

struct _rext_module_t {
    VALUE module;
    const char name[RUBY_NAME_MAX];
    rext_module_t *modules;
    long numModules;
    rext_object_t *objects;
    long numObjects;
    rext_const_t *constants;
    long numConstants;
    rext_method_t *methods;
    long numMethods;
};

void rubyext_variables(rext_variable_t *variables, long numVariables);
void rubyext_constants(VALUE parent, rext_const_t *constants, long numConstants);
void rubyext_methods(VALUE parent, long type, rext_method_t *methods, long numMethods);
void rubyext_objects(VALUE parent, rext_object_t *objects, long numObjects);
void rubyext_modules(VALUE parent, rext_module_t *modules, long numModules);

#endif

