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

/* clang-format off */
/*
 * Document-module: CapNG::Type
 *
 * Define EFFECTIVE, PERMITTED, INHERITABLE, BOUNDING_SET and AMBIENT (if available) constants.
 */
/* clang-format on */

#include <capng.h>

void Init_capng_enum_type(VALUE rb_cCapNG)
{
  VALUE rb_mType = rb_define_module_under(rb_cCapNG, "Type");

  // capng_type_t enum constants
  rb_define_const(rb_mType, "EFFECTIVE", INT2NUM(CAPNG_EFFECTIVE));
  rb_define_const(rb_mType, "PERMITTED", INT2NUM(CAPNG_PERMITTED));
  rb_define_const(rb_mType, "INHERITABLE", INT2NUM(CAPNG_INHERITABLE));
  rb_define_const(rb_mType, "BOUNDING_SET", INT2NUM(CAPNG_BOUNDING_SET));
#if defined(HAVE_CONST_CAPNG_AMBIENT)
  rb_define_const(rb_mType, "AMBIENT", INT2NUM(CAPNG_AMBIENT));
#endif
}
