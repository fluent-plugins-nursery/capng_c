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

#ifndef _CAPNG_H_
#define _CAPNG_H_

#include <ruby.h>
#include <ruby/encoding.h>
#include <ruby/io.h>

#include <cap-ng.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

VALUE rb_cCapNG;
VALUE rb_cCapNGPrint;
VALUE rb_cCapability;
VALUE rb_cState;
VALUE rb_mAction;
VALUE rb_mSelect;
VALUE rb_mType;
VALUE rb_mResult;
VALUE rb_mPrint;
VALUE rb_mFlags;

capng_select_t select_name_to_select_type(char *select_name);
capng_act_t action_name_to_action_type(char *action_name);
capng_print_t print_name_to_print_type(char *print_name);
capng_type_t capability_type_name_to_capability_type(char *capability_name);

void Init_capng_capability(VALUE);
void Init_capng_enum(VALUE);
void Init_capng_print(VALUE);
void Init_capng_state(VALUE);
#endif // _CAPNG_H
