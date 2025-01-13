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

/* clang-format off */
/*
 * Document-class: CapNG
 *
 * CapNG class.
 *
 * @example
 *  # Current process capability example
 *  require 'capng'
 *
 *  @capng = CapNG.new(:current_process)
 *  @capng.have_capability?(:effective, :dac_read_search)
 *
 * @example
 *  # Other process capability example
 *  require 'capng'
 *
 *  @capng = CapNG.new(:other_process, 12345)
 *  @capng.have_capability?(:effective, :dac_override)
 *
 */
/* clang-format on */

struct CapNG
{};

static void
capng_free(void* capng);

static const rb_data_type_t rb_capng_type = { "capng/capng",
                                              {
                                                0,
                                                capng_free,
                                                0,
                                              },
                                              NULL,
                                              NULL,
                                              RUBY_TYPED_FREE_IMMEDIATELY };

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
  obj = TypedData_Make_Struct(klass, struct CapNG, &rb_capng_type, capng);
  return obj;
}

static int
capng_get_file_descriptor(VALUE rb_file)
{
#ifdef HAVE_RB_IO_DESCRIPTOR
  return rb_io_descriptor(rb_file);
#else
  rb_io_t* fptr = NULL;

  fptr = RFILE(rb_file)->fptr;
  return fptr->fd;
#endif
}

/*
 * Initalize CapNG class.
 *
 * @overload initialize(target=nil, pid_or_file=nil)
 *   @option param target [String or Symbol] Specify capability target.
 *   @option param pid_or_file [String or Symbol] Querying XPath.
 * @return [nil]
 *
 */
static VALUE
rb_capng_initialize(int argc, VALUE* argv, VALUE self)
{
  VALUE rb_target, rb_pid;
  int result = 0;
  char* target = NULL;
  int pid = 0;

  rb_scan_args(argc, argv, "02", &rb_target, &rb_pid);

  if (NIL_P(rb_target)) {
    return Qnil;
  }

  if (RB_TYPE_P(rb_target, T_SYMBOL)) {
    target = RSTRING_PTR(rb_sym2str(rb_target));
  } else if (RB_TYPE_P(rb_target, T_STRING)) {
    target = StringValuePtr(rb_target);
  } else {
    rb_raise(rb_eArgError, "Expected a String or a Symbol instance for tagret argument");
  }

  if (strcmp(target, "current_process") == 0) {
    result = capng_get_caps_process();
    if (result != 0) {
      rb_raise(rb_eRuntimeError, "Couldn't get current process' capability");
    }
  } else if (strcmp(target, "other_process") == 0) {
    Check_Type(rb_pid, T_FIXNUM);

    pid = NUM2INT(rb_pid);
    capng_setpid(pid);
    result = capng_get_caps_process();
    if (result != 0) {
      rb_raise(rb_eRuntimeError, "Couldn't get current process' capability");
    }
  }

  return Qnil;
}

/*
 * Clear capabilities on specified target.
 *
 * @param rb_select_name_or_enum [Symbol or String or Fixnum] targets are CAPS, BOUNDS,
 *   BOTH, and AMBIENT for supported platform.
 *
 * @return [nil]
 *
 */
static VALUE
rb_capng_clear(VALUE self, VALUE rb_select_name_or_enum)
{
  capng_select_t select = 0;

  switch (TYPE(rb_select_name_or_enum)) {
    case T_SYMBOL:
      select =
        select_name_to_select_type(RSTRING_PTR(rb_sym2str(rb_select_name_or_enum)));
      break;
    case T_STRING:
      select = select_name_to_select_type(StringValuePtr(rb_select_name_or_enum));
      break;
    case T_FIXNUM:
      select = NUM2INT(rb_select_name_or_enum);
      break;
    default:
      rb_raise(rb_eArgError,
               "Expected a String or a Symbol instance, or a capability type constant");
  }

  capng_clear(select);

  return Qnil;
}

/*
 * Fill capabilities on specified target.
 *
 * @param rb_select_name_or_enum [Symbol or String or Fixnum] targets are CAPS, BOUNDS,
 *   BOTH, and AMBIENT for supported platform.
 *
 * @return [nil]
 *
 */
static VALUE
rb_capng_fill(VALUE self, VALUE rb_select_name_or_enum)
{
  capng_select_t select = 0;

  switch (TYPE(rb_select_name_or_enum)) {
    case T_SYMBOL:
      select =
        select_name_to_select_type(RSTRING_PTR(rb_sym2str(rb_select_name_or_enum)));
      break;
    case T_STRING:
      select = select_name_to_select_type(StringValuePtr(rb_select_name_or_enum));
      break;
    case T_FIXNUM:
      select = NUM2INT(rb_select_name_or_enum);
      break;
    default:
      rb_raise(rb_eArgError,
               "Expected a String or a Symbol instance, or a capability type constant");
  }

  capng_fill(select);

  return Qnil;
}

/*
 * Specify process ID to retrieve other process capabilities.
 *
 * @param rb_pid [Fixnum] Process ID.
 *
 * @return [nil]
 *
 */
static VALUE
rb_capng_setpid(VALUE self, VALUE rb_pid)
{
  Check_Type(rb_pid, T_FIXNUM);

  capng_setpid(NUM2INT(rb_pid));

  return Qnil;
}

/*
 * Specify process ID to retrieve process capabilities.  If not
 * calling #setpid before, it returns current process' capabilities.
 *
 * @return [Boolean]
 *
 */
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

/*
 * Update capabilities.
 *
 * @param rb_action_name_or_action [Symbol or String or Fixnum] ADD or DROP.
 * @param rb_capability_name_or_type [Symbol or String or Fixnum]
 *   Effective/Inheritable/Permitted/Ambient (If supported) or their combinations
 * @param rb_capability_or_name [Symbol or String or Fixnum] Capability name or constants.
 *
 * @see: [CapNG::Capability])
 *
 * @return [Boolean]
 */
static VALUE
rb_capng_update(VALUE self, VALUE rb_action_name_or_action,
                VALUE rb_capability_name_or_type, VALUE rb_capability_or_name)
{
  int result = 0;
  int capability = 0;
  capng_type_t capability_type = 0;
  capng_act_t action = 0;

  switch (TYPE(rb_action_name_or_action)) {
    case T_SYMBOL:
      action =
        action_name_to_action_type(RSTRING_PTR(rb_sym2str(rb_action_name_or_action)));
      break;
    case T_STRING:
      action = action_name_to_action_type(StringValuePtr(rb_action_name_or_action));
      break;
    case T_FIXNUM:
      action = NUM2INT(rb_action_name_or_action);
      break;
    default:
      rb_raise(rb_eArgError,
               "Expected a String or a Symbol instance, or a capability type constant");
  }

  switch (TYPE(rb_capability_name_or_type)) {
    case T_SYMBOL:
      capability_type = capability_type_name_to_capability_type(
        RSTRING_PTR(rb_sym2str(rb_capability_name_or_type)));
      break;
    case T_STRING:
      capability_type = capability_type_name_to_capability_type(
        StringValuePtr(rb_capability_name_or_type));
      break;
    case T_FIXNUM:
      capability_type = NUM2INT(rb_capability_name_or_type);
      break;
    default:
      rb_raise(rb_eArgError,
               "Expected a String or a Symbol instance, or a capability type constant");
  }

  switch (TYPE(rb_capability_or_name)) {
    case T_SYMBOL:
      capability =
        capng_name_to_capability(RSTRING_PTR(rb_sym2str(rb_capability_or_name)));
      if (capability == -1) {
        rb_raise(rb_eRuntimeError, "Unknown capability: %s",
                 RSTRING_PTR(rb_sym2str(rb_capability_or_name)));
      }
      break;
    case T_STRING:
      capability = capng_name_to_capability(StringValuePtr(rb_capability_or_name));
      if (capability == -1) {
        rb_raise(rb_eRuntimeError, "Unknown capability: %s",
                 StringValuePtr(rb_capability_or_name));
      }
     break;
    case T_FIXNUM:
      capability = NUM2INT(rb_capability_or_name);
      break;
    default:
      rb_raise(rb_eArgError,
               "Expected a String or a Symbol instance, or a capability constant");
  }

  result = capng_update(action, capability_type, capability);

  if (result == 0)
    return Qtrue;
  else
    return Qfalse;
}

/*
 * Apply capabilities on specified target.
 *
 * @param rb_select_name_or_enum [Symbol or String or Fixnum]
 *   targets are CAPS, BOUNDS, BOTH, and AMBIENT for supported platform.
 *
 * @return [Boolean]
 *
 */
static VALUE
rb_capng_apply(VALUE self, VALUE rb_select_name_or_enum)
{
  int result = 0;
  capng_select_t select = 0;

  switch (TYPE(rb_select_name_or_enum)) {
    case T_SYMBOL:
      select =
        select_name_to_select_type(RSTRING_PTR(rb_sym2str(rb_select_name_or_enum)));
      break;
    case T_STRING:
      select = select_name_to_select_type(StringValuePtr(rb_select_name_or_enum));
      break;
    case T_FIXNUM:
      select = NUM2INT(rb_select_name_or_enum);
      break;
    default:
      rb_raise(rb_eArgError,
               "Expected a String or a Symbol instance, or a capability type constant");
  }

  result = capng_apply(select);

  if (result == 0)
    return Qtrue;
  else
    return Qfalse;
}

/*
 * Lock capabilities.
 *
 * @return [Boolean]
 *
 */
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

/*
 *  Change the credentials retaining capabilities.
 * @param rb_uid [Fixnum] User ID.
 * @param rb_gid [Fixnum] Group ID.
 * @param rb_flags [Fixnum] CapNG::Flags constants.
 *
 * @see: capng_change_id(3)
 */
static VALUE
rb_capng_change_id(VALUE self, VALUE rb_uid, VALUE rb_gid, VALUE rb_flags)
{
  int result = 0;

  result = capng_change_id(NUM2INT(rb_uid), NUM2INT(rb_gid), NUM2INT(rb_flags));

  if (result == 0)
    return Qtrue;
  else
    rb_raise(rb_eRuntimeError,
             "Calling capng_change_id is failed with: (exitcode: %d)\n",
             result);
}

/*
 * Check whether capabilities on specified target or not.
 *
 * @param rb_select_name_or_enum [Symbol or String or Fixnum]
 *   targets are CAPS, BOUNDS, BOTH, and AMBIENT for supported platform.
 *
 * @return [Integer]
 *
 */
static VALUE
rb_capng_have_capabilities_p(VALUE self, VALUE rb_select_name_or_enum)
{
  int result = 0;
  capng_select_t select = 0;

  switch (TYPE(rb_select_name_or_enum)) {
    case T_SYMBOL:
      select =
        select_name_to_select_type(RSTRING_PTR(rb_sym2str(rb_select_name_or_enum)));
      break;
    case T_STRING:
      select = select_name_to_select_type(StringValuePtr(rb_select_name_or_enum));
      break;
    case T_FIXNUM:
      select = NUM2INT(rb_select_name_or_enum);
      break;
    default:
      rb_raise(rb_eArgError,
               "Expected a String or a Symbol instance, or a capability type constant");
  }
  result = capng_have_capabilities(select);

  return INT2NUM(result);
}

/*
 * Check whether capabilities on specified target or not.
 *
 * @param rb_capability_name_or_type [Symbol or String or Fixnum] types are EFFECTIVE,
 *   INHERITABLE, PERMITTED, and AMBIENT for supported platform.
 * @param rb_capability_or_name [Symbol or String or Fixnum]
 *   Capability name or constants.
 *
 * @see: [CapNG::Capability]
 *
 * @return [Boolean]
 *
 */
static VALUE
rb_capng_have_capability_p(VALUE self, VALUE rb_capability_name_or_type,
                           VALUE rb_capability_or_name)
{
  int result = 0;
  unsigned int capability = 0;
  capng_type_t capability_type = 0;

  switch (TYPE(rb_capability_name_or_type)) {
    case T_SYMBOL:
      capability_type = capability_type_name_to_capability_type(
        RSTRING_PTR(rb_sym2str(rb_capability_name_or_type)));
      break;
    case T_STRING:
      capability_type = capability_type_name_to_capability_type(
        StringValuePtr(rb_capability_name_or_type));
      break;
    case T_FIXNUM:
      capability_type = NUM2INT(rb_capability_name_or_type);
      break;
    default:
      rb_raise(rb_eArgError,
               "Expected a String or a Symbol instance, or a capability type constant");
  }

  switch (TYPE(rb_capability_or_name)) {
    case T_SYMBOL:
      capability =
        capng_name_to_capability(RSTRING_PTR(rb_sym2str(rb_capability_or_name)));
      break;
    case T_STRING:
      capability = capng_name_to_capability(StringValuePtr(rb_capability_or_name));
      break;
    case T_FIXNUM:
      capability = NUM2INT(rb_capability_or_name);
      break;
    default:
      rb_raise(rb_eArgError,
               "Expected a String or a Symbol instance, or a capability constant");
  }

  result = capng_have_capability(capability_type, capability);

  if (result == 1)
    return Qtrue;
  else
    return Qfalse;
}

/*
 * Retrieve capabilities from file.
 *
 * @param rb_file [File] target file object
 *
 * @return [Boolean]
 *
 */
static VALUE
rb_capng_get_caps_file(VALUE self, VALUE rb_file)
{
  int result = 0, fd = 0;

  Check_Type(rb_file, T_FILE);

  if (NIL_P(rb_file)) {
    return Qfalse;
  }
  fd = capng_get_file_descriptor(rb_file);
  result = capng_get_caps_fd(fd);

  if (result == 0)
    return Qtrue;
  else
    return Qfalse;
}

/*
 * Apply capabilities on specified target (file specific version).
 *
 * @param rb_file [File] target file object
 *
 * @return [Boolean]
 *
 */
static VALUE
rb_capng_apply_caps_file(VALUE self, VALUE rb_file)
{
  int result = 0, fd = 0;

  Check_Type(rb_file, T_FILE);

  if (NIL_P(rb_file)) {
    return Qfalse;
  }

  fd = capng_get_file_descriptor(rb_file);
  result = capng_apply_caps_fd(fd);

  if (result == 0)
    return Qtrue;
  else
    return Qfalse;
}

void
Init_capng(void)
{
  VALUE rb_cCapNG = rb_define_class("CapNG", rb_cObject);

  rb_define_alloc_func(rb_cCapNG, rb_capng_alloc);

  rb_define_method(rb_cCapNG, "initialize", rb_capng_initialize, -1);
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
