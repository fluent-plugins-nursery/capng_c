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

struct CapNGCapability {};

static void capng_capability_free(void* capng);

static const rb_data_type_t rb_capng_capability_type = {
  "capng_capability/c_runtime",
  {
    0,
    capng_capability_free,
    0,
  },
  NULL,
  NULL,
  RUBY_TYPED_FREE_IMMEDIATELY
};

static void
capng_capability_free(void* ptr)
{
  xfree(ptr);
}

static VALUE
rb_capng_capability_alloc(VALUE klass)
{
  VALUE obj;
  struct CapNGCapability* capng_capability;
  obj = TypedData_Make_Struct(
    klass, struct CapNGCapability, &rb_capng_capability_type, capng_capability);
  return obj;
}

static VALUE
rb_capng_capability_initialize(VALUE self)
{
  return Qnil;
}

static VALUE
rb_capng_capability_to_name(VALUE self, VALUE rb_capability)
{
  const char *name = capng_capability_to_name(NUM2UINT(rb_capability));

  if (name)
    return rb_str_new2(name);
  else
    return rb_str_new2("unknown");
}

static VALUE
rb_capng_capability_from_name(VALUE self, VALUE rb_capability_name_or_symbol)
{
  unsigned int capability;

  switch (TYPE(rb_capability_name_or_symbol)) {
  case T_SYMBOL:
    capability = capng_name_to_capability(RSTRING_PTR(rb_sym2str(rb_capability_name_or_symbol)));
    break;
  case T_STRING:
    capability = capng_name_to_capability(StringValuePtr(rb_capability_name_or_symbol));
    break;
  default:
    rb_raise(rb_eArgError, "Expected a String or a Symbol instance");
  }
  return INT2NUM(capability);
}

void
Init_capng_capability(VALUE rb_cCapNG)
{
  rb_cCapability = rb_define_class_under(rb_cCapNG, "Capability", rb_cObject);

  rb_define_alloc_func(rb_cCapability, rb_capng_capability_alloc);

  rb_define_method(rb_cCapability, "initialize", rb_capng_capability_initialize, 0);
  rb_define_method(rb_cCapability, "to_name", rb_capng_capability_to_name, 1);
  rb_define_method(rb_cCapability, "from_name", rb_capng_capability_from_name, 1);

  // Capability constants.
  rb_define_const(rb_cCapability, "CHOWN", INT2NUM(CAP_CHOWN));
  rb_define_const(rb_cCapability, "DAC_OVERRIDE", INT2NUM(CAP_DAC_OVERRIDE));
  rb_define_const(rb_cCapability, "DAC_READ_SEARCH", INT2NUM(CAP_DAC_READ_SEARCH));
  rb_define_const(rb_cCapability, "FOWNER", INT2NUM(CAP_FOWNER));
  rb_define_const(rb_cCapability, "FSETID", INT2NUM(CAP_FSETID));
  rb_define_const(rb_cCapability, "KILL", INT2NUM(CAP_KILL));
  rb_define_const(rb_cCapability, "SETGID", INT2NUM(CAP_SETGID));
  rb_define_const(rb_cCapability, "SETUID", INT2NUM(CAP_SETUID));
  rb_define_const(rb_cCapability, "SETPCAP", INT2NUM(CAP_SETPCAP));
  rb_define_const(rb_cCapability, "LINUX_IMMUTABLE", INT2NUM(CAP_LINUX_IMMUTABLE));
  rb_define_const(rb_cCapability, "NET_BIND_SERIVCE", INT2NUM(CAP_NET_BIND_SERVICE));
  rb_define_const(rb_cCapability, "NET_BROATCAST", INT2NUM(CAP_NET_BROADCAST));
  rb_define_const(rb_cCapability, "NET_ADMIN", INT2NUM(CAP_NET_ADMIN));
  rb_define_const(rb_cCapability, "NET_RAW", INT2NUM(CAP_NET_RAW));
  rb_define_const(rb_cCapability, "IPC_LOCK", INT2NUM(CAP_IPC_LOCK));
  rb_define_const(rb_cCapability, "IPC_OWNER", INT2NUM(CAP_IPC_OWNER));
  rb_define_const(rb_cCapability, "SYS_MODULE", INT2NUM(CAP_SYS_MODULE));
  rb_define_const(rb_cCapability, "SYS_RAWIO", INT2NUM(CAP_SYS_RAWIO));
  rb_define_const(rb_cCapability, "SYS_CHROOT", INT2NUM(CAP_SYS_CHROOT));
  rb_define_const(rb_cCapability, "SYS_PTRACE", INT2NUM(CAP_SYS_PTRACE));
  rb_define_const(rb_cCapability, "SYS_PACCT", INT2NUM(CAP_SYS_PACCT));
  rb_define_const(rb_cCapability, "SYS_ADMIN", INT2NUM(CAP_SYS_ADMIN));
  rb_define_const(rb_cCapability, "SYS_BOOT", INT2NUM(CAP_SYS_BOOT));
  rb_define_const(rb_cCapability, "SYS_NICE", INT2NUM(CAP_SYS_NICE));
  rb_define_const(rb_cCapability, "SYS_RESOURCE", INT2NUM(CAP_SYS_RESOURCE));
  rb_define_const(rb_cCapability, "SYS_TIME", INT2NUM(CAP_SYS_TIME));
  rb_define_const(rb_cCapability, "TTY_CONFIG", INT2NUM(CAP_SYS_TTY_CONFIG));
  rb_define_const(rb_cCapability, "MKNOD", INT2NUM(CAP_MKNOD));
  rb_define_const(rb_cCapability, "LEASE", INT2NUM(CAP_LEASE));
  rb_define_const(rb_cCapability, "AUDIT_WRITE", INT2NUM(CAP_AUDIT_WRITE));
  rb_define_const(rb_cCapability, "AUDIT_CONTROL", INT2NUM(CAP_AUDIT_CONTROL));
#ifdef CAP_SETFCAP
  rb_define_const(rb_cCapability, "SETFCAP", INT2NUM(CAP_SETFCAP));
#endif
#ifdef CAP_MAC_OVERRIDE
  rb_define_const(rb_cCapability, "MAC_OVERRIDE", INT2NUM(CAP_MAC_OVERRIDE));
#endif
#ifdef CAP_MAC_ADMIN
  rb_define_const(rb_cCapability, "MAC_ADMIN", INT2NUM(CAP_MAC_ADMIN));
#endif
#ifdef CAP_SYSLOG
  rb_define_const(rb_cCapability, "SYSLOG", INT2NUM(CAP_SYSLOG));
#endif
#if defined(CAP_EPOLLWAKEUP) && defined(CAP_BLOCK_SUSPEND)
#error "Both CAP_EPOLLWAKEUP and CAP_BLOCK_SUSPEND are defined"
#endif
#ifdef CAP_EPOLLWAKEUP
  rb_define_const(rb_cCapability, "EPOLLWAKEUP", INT2NUM(CAP_EPOLLWAKEUP));
#endif
#ifdef CAP_WAKE_ALARM
  rb_define_const(rb_cCapability, "WAKE_ALARM", INT2NUM(CAP_WAKE_ALARM));
#endif
#ifdef CAP_BLOCK_SUSPEND
  rb_define_const(rb_cCapability, "BLOCK_SUSPEND", INT2NUM(CAP_BLOCK_SUSPEND));
#endif
#ifdef CAP_AUDIT_READ
  rb_define_const(rb_cCapability, "AUDIT_READ", INT2NUM(CAP_AUDIT_READ));
#endif
#ifdef CAP_PERFMON
  rb_define_const(rb_cCapability, "PERFMON", INT2NUM(CAP_PERFMON));
#endif
#ifdef CAP_BPF
  rb_define_const(rb_cCapability, "BPF", INT2NUM(CAP_BPF));
#endif
#ifdef CAP_CHECKPOINT_RESTORE
  rb_define_const(rb_cCapability, "CHECKPOINT_RESTORE", INT2NUM(CAP_CHECKPOINT_RESTORE));
#endif
}
