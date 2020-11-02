/* capng_c */
/* Copyright 2020- Hiroshi Hatake*/
/* */
/* Licensed under the Apache License, Version 2.0 (the "License"); */
/* you may not use this file except in compliance with the License. */
/* You may obtain a copy of the License at */
/*     http://www.apache.org/licenses/LICENSE-2.0 */
/* Unless required by applicable law or agreed to in writing, software */
/* distributed under the License is distributed on an "AS IS" BASIS, */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. */
/* See the License for the specific language governing permissions and */
/* limitations under the License. */

#include <capng.h>

struct CapNGPrint {};

static void capng_print_free(void* capng);

static const rb_data_type_t rb_capng_print_type = {
  "capng/print",
  {
    0,
    capng_print_free,
    0,
  },
  NULL,
  NULL,
  RUBY_TYPED_FREE_IMMEDIATELY
};

static void
capng_print_free(void* ptr)
{
  xfree(ptr);
}

static VALUE
rb_capng_print_alloc(VALUE klass)
{
  VALUE obj;
  struct CapNGPrint* capng_print;
  obj = TypedData_Make_Struct(
    klass, struct CapNGPrint, &rb_capng_print_type, capng_print);
  return obj;
}

static VALUE
rb_capng_print_initialize(VALUE self)
{
  return Qnil;
}

static VALUE
rb_capng_print_caps_text(VALUE self, VALUE rb_where_name_or_type, VALUE rb_capability_name_or_type)
{
  char *result = NULL;
  capng_type_t capability_type = 0;
  capng_print_t print_type = 0;

  switch (TYPE(rb_capability_name_or_type)) {
  case T_SYMBOL:
    capability_type = capability_type_name_to_capability_type(RSTRING_PTR(rb_sym2str(rb_capability_name_or_type)));
    break;
  case T_STRING:
    capability_type = capability_type_name_to_capability_type(StringValuePtr(rb_capability_name_or_type));
    break;
  case T_FIXNUM:
    capability_type = NUM2INT(rb_capability_name_or_type);
    break;
  default:
    rb_raise(rb_eArgError, "Expected a String or a Symbol instance, or a capability type constant");
  }

  switch (TYPE(rb_where_name_or_type)) {
  case T_SYMBOL:
    print_type = print_name_to_print_type(RSTRING_PTR(rb_sym2str(rb_where_name_or_type)));
    break;
  case T_STRING:
    print_type = print_name_to_print_type(StringValuePtr(rb_where_name_or_type));
    break;
  case T_FIXNUM:
    print_type = NUM2INT(rb_where_name_or_type);
    break;
  default:
    rb_raise(rb_eArgError, "Expected a String or a Symbol instance, or a print type constant");
  }

  switch (print_type) {
  case CAPNG_PRINT_STDOUT:
    capng_print_caps_text(CAPNG_PRINT_STDOUT, capability_type);
    break;
  case CAPNG_PRINT_BUFFER:
    result = capng_print_caps_text(CAPNG_PRINT_BUFFER, capability_type);
  }

  if (result)
    return rb_str_new2(result);
  else
    return rb_str_new2("none");
}

static VALUE
rb_capng_print_caps_numeric(VALUE self, VALUE rb_where_name_or_type, VALUE rb_select_name_or_enum)
{
  char *result = NULL;
  capng_select_t select = 0;
  capng_print_t print_type = 0;

  switch (TYPE(rb_where_name_or_type)) {
  case T_SYMBOL:
    print_type = print_name_to_print_type(RSTRING_PTR(rb_sym2str(rb_where_name_or_type)));
    break;
  case T_STRING:
    print_type = print_name_to_print_type(StringValuePtr(rb_where_name_or_type));
    break;
  case T_FIXNUM:
    print_type = NUM2INT(rb_where_name_or_type);
    break;
  default:
    rb_raise(rb_eArgError, "Expected a String or a Symbol instance, or a print type constant");
  }

  switch (TYPE(rb_select_name_or_enum)) {
  case T_SYMBOL:
    select = select_name_to_select_type(RSTRING_PTR(rb_sym2str(rb_select_name_or_enum)));
    break;
  case T_STRING:
    select = select_name_to_select_type(StringValuePtr(rb_select_name_or_enum));
    break;
  case T_FIXNUM:
    select = NUM2INT(rb_select_name_or_enum);
    break;
  default:
    rb_raise(rb_eArgError, "Expected a String or a Symbol instance, or a capability type constant");
  }

  switch (print_type) {
  case CAPNG_PRINT_STDOUT:
    capng_print_caps_numeric(CAPNG_PRINT_STDOUT, select);
    break;
  case CAPNG_PRINT_BUFFER:
    result = capng_print_caps_numeric(CAPNG_PRINT_BUFFER, select);
  }

  if (result)
    return rb_str_new2(result);
  else
    return rb_str_new2("none");
}

void Init_capng_print(VALUE rb_cCapNG)
{
  VALUE rb_cCapNGPrint = rb_define_class_under(rb_cCapNG, "Print", rb_cObject);

  rb_define_alloc_func(rb_cCapNGPrint, rb_capng_print_alloc);

  rb_define_method(rb_cCapNGPrint, "initialize", rb_capng_print_initialize, 0);
  rb_define_method(rb_cCapNGPrint, "caps_text", rb_capng_print_caps_text, 2);
  rb_define_method(rb_cCapNGPrint, "caps_numeric", rb_capng_print_caps_numeric, 2);

  // capng_print_t enum constants
  rb_define_const(rb_cCapNGPrint, "STDOUT", LONG2NUM(CAPNG_PRINT_STDOUT));
  rb_define_const(rb_cCapNGPrint, "BUFFER", LONG2NUM(CAPNG_PRINT_BUFFER));
}
