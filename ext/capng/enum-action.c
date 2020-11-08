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
 * Document-module: CapNG::Action
 *
 * Define ADD or Drop constants.
 *
 */
/* clang-format on */

#include <capng.h>

void
Init_capng_enum_action(VALUE rb_cCapNG)
{
  VALUE rb_mAction = rb_define_module_under(rb_cCapNG, "Action");

  // capng_cat_t enum constants
  /* Mark as DROP action */
  rb_define_const(rb_mAction, "DROP", INT2NUM(CAPNG_DROP));
  /* Mark as ADD action */
  rb_define_const(rb_mAction, "ADD", INT2NUM(CAPNG_ADD));
}
