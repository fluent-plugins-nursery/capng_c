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
 * Document-module: CapNG::Select
 *
 * Define CAPS, BOUNDS, BOTH, and AMBIENT (if available) constants.
 */
/* clang-format on */

#include <capng.h>

void Init_capng_enum_select(VALUE rb_cCapNG)
{
  VALUE rb_mSelect = rb_define_module_under(rb_cCapNG, "Select");

  // capng_select_t enum constants
  rb_define_const(rb_mSelect, "CAPS", INT2NUM(CAPNG_SELECT_CAPS));
  rb_define_const(rb_mSelect, "BOUNDS", INT2NUM(CAPNG_SELECT_BOUNDS));
  rb_define_const(rb_mSelect, "BOTH", INT2NUM(CAPNG_SELECT_BOTH));
#if defined(HAVE_CONST_CAPNG_SELECT_AMBIENT)
  rb_define_const(rb_mSelect, "AMBIENT", INT2NUM(CAPNG_SELECT_AMBIENT));
#endif
#if defined(HAVE_CONST_CAPNG_SELECT_ALL)
  rb_define_const(rb_mSelect, "ALL", INT2NUM(CAPNG_SELECT_ALL));
#endif
}
