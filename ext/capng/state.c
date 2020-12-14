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
 * Document-class: CapNG::State
 *
 * Handle CapNG state.
 *
 * @example
 *  require 'capng'
 *
 *  @state = CapNG::State.new
 *  @state.save
 *  # Some capability operations
 *  @state.restore
 */
/* clang-format on */

#include <capng.h>

struct CapNGState
{
  void* state;
};

static void
capng_state_free(void* capng);

static const rb_data_type_t rb_capng_state_type = { "capng/state",
                                                    {
                                                      0,
                                                      capng_state_free,
                                                      0,
                                                    },
                                                    NULL,
                                                    NULL,
                                                    RUBY_TYPED_FREE_IMMEDIATELY };

static void
capng_state_free(void* ptr)
{
  struct CapNGState* state = (struct CapNGState*)ptr;
  if (state) {
    state->state = NULL;
  }

  xfree(ptr);
}

static VALUE
rb_capng_state_alloc(VALUE klass)
{
  VALUE obj;
  struct CapNGState* capng_state;
  obj =
    TypedData_Make_Struct(klass, struct CapNGState, &rb_capng_state_type, capng_state);
  return obj;
}

/*
 * Initalize State class.
 *
 * @return [nil]
 *
 */
static VALUE
rb_capng_state_initialize(VALUE self)
{
  struct CapNGState* capng_state;

  TypedData_Get_Struct(self, struct CapNGState, &rb_capng_state_type, capng_state);

  capng_state->state = NULL;
  return Qnil;
}

/*
 * Save current capability state.
 *
 * @return [nil]
 *
 */
static VALUE
rb_capng_state_save(VALUE self)
{
  struct CapNGState* capng_state;
  void* state = NULL;

  TypedData_Get_Struct(self, struct CapNGState, &rb_capng_state_type, capng_state);

  state = capng_save_state();
  capng_state->state = state;

  return Qnil;
}

/*
 * Restore saved capability state.
 *
 * @return [nil]
 *
 */

static VALUE
rb_capng_state_restore(VALUE self)
{
  struct CapNGState* capng_state;
  void* state = NULL;

  TypedData_Get_Struct(self, struct CapNGState, &rb_capng_state_type, capng_state);

  state = capng_state->state;
  capng_restore_state(&state);

  return Qnil;
}

void
Init_capng_state(VALUE rb_cCapNG)
{
  VALUE rb_cState = rb_define_class_under(rb_cCapNG, "State", rb_cObject);

  rb_define_alloc_func(rb_cState, rb_capng_state_alloc);

  rb_define_method(rb_cState, "initialize", rb_capng_state_initialize, 0);
  rb_define_method(rb_cState, "save", rb_capng_state_save, 0);
  rb_define_method(rb_cState, "restore", rb_capng_state_restore, 0);
}
