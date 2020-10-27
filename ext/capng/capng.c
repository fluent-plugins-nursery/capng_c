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

struct CapNG {};

static void capng_free(void* capng);

static const rb_data_type_t rb_capng_type = {
  "capng/capng",
  {
    0,
    capng_free,
    0,
  },
  NULL,
  NULL,
  RUBY_TYPED_FREE_IMMEDIATELY
};

static void
capng_free(void* ptr)
{
  xfree(ptr);
}

static VALUE
rb_capng_alloc(VALUE klass)
{
  VALUE obj;
  struct CapNG* capng;
  obj = TypedData_Make_Struct(
    klass, struct CapNG, &rb_capng_type, capng);
  return obj;
}

static VALUE
rb_capng_initialize(VALUE self)
{
  return Qnil;
}

static VALUE
rb_capng_clear(VALUE self, VALUE rb_select_name_or_enum)
{
  capng_select_t select = 0;

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

  capng_clear(select);

  return Qnil;
}

static VALUE
rb_capng_fill(VALUE self, VALUE rb_select_name_or_enum)
{
  capng_select_t select = 0;

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

  capng_fill(select);

  return Qnil;
}

static VALUE
rb_capng_setpid(VALUE self, VALUE rb_pid)
{
  Check_Type(rb_pid, T_FIXNUM);

  capng_setpid(NUM2INT(rb_pid));

  return Qnil;
}

static VALUE
rb_capng_get_caps_process(VALUE self)
{
  int result = 0;
  result = capng_get_caps_process();

  if (result == 0)
    return Qtrue;
  else
    return Qfalse;
}

static VALUE
rb_capng_update(VALUE self,
                VALUE rb_action_name_or_action, VALUE rb_capability_name_or_type, VALUE rb_capability_or_name)
{
  int result = 0;
  unsigned int capability = 0;
  capng_type_t capability_type = 0;
  capng_act_t action = 0;

  switch (TYPE(rb_action_name_or_action)) {
  case T_SYMBOL:
    action = action_name_to_action_type(RSTRING_PTR(rb_sym2str(rb_action_name_or_action)));
    break;
  case T_STRING:
    action = action_name_to_action_type(StringValuePtr(rb_action_name_or_action));
    break;
  case T_FIXNUM:
    action = NUM2INT(rb_action_name_or_action);
    break;
  default:
    rb_raise(rb_eArgError, "Expected a String or a Symbol instance, or a capability type constant");
  }

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

  switch (TYPE(rb_capability_or_name)) {
  case T_SYMBOL:
    capability = capng_name_to_capability(RSTRING_PTR(rb_sym2str(rb_capability_or_name)));
    break;
  case T_STRING:
    capability = capng_name_to_capability(StringValuePtr(rb_capability_or_name));
    break;
  case T_FIXNUM:
    capability = NUM2INT(rb_capability_or_name);
    break;
  default:
    rb_raise(rb_eArgError, "Expected a String or a Symbol instance, or a capability constant");
  }

  result = capng_update(action, capability_type, capability);

  if (result == 0)
    return Qtrue;
  else
    return Qfalse;
}

static VALUE
rb_capng_apply(VALUE self, VALUE rb_action_set)
{
  int result = 0;

  Check_Type(rb_action_set, T_FIXNUM);

  result = capng_apply(NUM2INT(rb_action_set));

  if (result == 0)
    return Qtrue;
  else
    return Qfalse;
}

static VALUE
rb_capng_lock(VALUE self)
{
  int result = 0;

  result = capng_lock();

  if (result == 0)
    return Qtrue;
  else
    return Qfalse;
}

static VALUE
rb_capng_change_id(VALUE self, VALUE rb_uid, VALUE rb_gid, VALUE rb_flags)
{
  int result = 0;

  result = capng_change_id(NUM2INT(rb_uid), NUM2INT(rb_gid), NUM2INT(rb_flags));

  if (result == 0)
    return Qtrue;
  else
    rb_raise(rb_eRuntimeError, "Calling capng_change_id is failed with: (exitcode: %d)\n", result);
}

static VALUE
rb_capng_have_capabilities_p(VALUE self, VALUE rb_select_name_or_enum)
{
  int result = 0;
  capng_select_t select = 0;

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
  result = capng_have_capabilities(select);

  return INT2NUM(result);
}

static VALUE
rb_capng_have_capability_p(VALUE self, VALUE rb_capability_name_or_type, VALUE rb_capability_or_name)
{
  int result = 0;
  unsigned int capability = 0;
  capng_type_t capability_type = 0;

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

  switch (TYPE(rb_capability_or_name)) {
  case T_SYMBOL:
    capability = capng_name_to_capability(RSTRING_PTR(rb_sym2str(rb_capability_or_name)));
    break;
  case T_STRING:
    capability = capng_name_to_capability(StringValuePtr(rb_capability_or_name));
    break;
  case T_FIXNUM:
    capability = NUM2INT(rb_capability_or_name);
    break;
  default:
    rb_raise(rb_eArgError, "Expected a String or a Symbol instance, or a capability constant");
  }

  result = capng_have_capability(capability_type, capability);

  if (result == 1)
    return Qtrue;
  else
    return Qfalse;
}

static VALUE
rb_capng_get_caps_file(VALUE self, VALUE rb_file)
{
  int result = 0, fd = 0;
  rb_io_t *fptr = NULL;

  Check_Type(rb_file, T_FILE);

  if (NIL_P(rb_file)) {
    return Qfalse;
  }
  fptr = RFILE(rb_file)->fptr;
  fd = fptr->fd;
  result = capng_get_caps_fd(fd);

  if (result == 0)
    return Qtrue;
  else
    return Qfalse;
}

static VALUE
rb_capng_apply_caps_file(VALUE self, VALUE rb_file)
{
  int result = 0, fd = 0;
  rb_io_t *fptr = NULL;

  Check_Type(rb_file, T_FILE);

  if (NIL_P(rb_file)) {
    return Qfalse;
  }

  fptr = RFILE(rb_file)->fptr;
  fd = fptr->fd;
  result = capng_apply_caps_fd(fd);

  if (result == 0)
    return Qtrue;
  else
    return Qfalse;
}


void
Init_capng(void)
{
  rb_cCapNG = rb_define_class("CapNG", rb_cObject);

  rb_define_alloc_func(rb_cCapNG, rb_capng_alloc);

  rb_define_method(rb_cCapNG, "initialize", rb_capng_initialize, 0);
  rb_define_method(rb_cCapNG, "clear", rb_capng_clear, 1);
  rb_define_method(rb_cCapNG, "fill", rb_capng_fill, 1);
  rb_define_method(rb_cCapNG, "setpid", rb_capng_setpid, 1);
  rb_define_method(rb_cCapNG, "caps_process", rb_capng_get_caps_process, 0);
  rb_define_method(rb_cCapNG, "get_caps_process", rb_capng_get_caps_process, 0);
  rb_define_method(rb_cCapNG, "update", rb_capng_update, 3);
  rb_define_method(rb_cCapNG, "apply", rb_capng_apply, 1);
  rb_define_method(rb_cCapNG, "lock", rb_capng_lock, 0);
  rb_define_method(rb_cCapNG, "change_id", rb_capng_change_id, 3);
  rb_define_method(rb_cCapNG, "have_capabilities?", rb_capng_have_capabilities_p, 1);
  rb_define_method(rb_cCapNG, "have_capability?", rb_capng_have_capability_p, 2);
  rb_define_method(rb_cCapNG, "get_caps_file", rb_capng_get_caps_file, 1);
  rb_define_method(rb_cCapNG, "caps_file", rb_capng_get_caps_file, 1);
  rb_define_method(rb_cCapNG, "apply_caps_file", rb_capng_apply_caps_file, 1);

  Init_capng_enum(rb_cCapNG);
  Init_capng_capability(rb_cCapNG);
  Init_capng_print(rb_cCapNG);
  Init_capng_state(rb_cCapNG);
}
