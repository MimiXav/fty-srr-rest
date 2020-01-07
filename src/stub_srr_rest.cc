/*  =========================================================================
    stub_srr_rest - REST API for Save, restore and reset

    Copyright (C) 2014 - 2018 Eaton

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
    =========================================================================
*/

/*
@header
    stub_srr_rest - REST API for Save, restore and reset
@discuss
@end
*/

#include "fty_srr_rest_classes.h"

//  Structure of our class

struct _stub_srr_rest_t {
    int filler;     //  Declare class properties here
};


//  --------------------------------------------------------------------------
//  Create a new stub_srr_rest

stub_srr_rest_t *
stub_srr_rest_new (void)
{
    stub_srr_rest_t *self = (stub_srr_rest_t *) zmalloc (sizeof (stub_srr_rest_t));
    assert (self);
    //  Initialize class properties here
    return self;
}


//  --------------------------------------------------------------------------
//  Destroy the stub_srr_rest

void
stub_srr_rest_destroy (stub_srr_rest_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        stub_srr_rest_t *self = *self_p;
        //  Free class properties here
        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}

//  --------------------------------------------------------------------------
//  Self test of this class

// If your selftest reads SCMed fixture data, please keep it in
// src/selftest-ro; if your test creates filesystem objects, please
// do so under src/selftest-rw.
// The following pattern is suggested for C selftest code:
//    char *filename = NULL;
//    filename = zsys_sprintf ("%s/%s", SELFTEST_DIR_RO, "mytemplate.file");
//    assert (filename);
//    ... use the "filename" for I/O ...
//    zstr_free (&filename);
// This way the same "filename" variable can be reused for many subtests.
#define SELFTEST_DIR_RO "src/selftest-ro"
#define SELFTEST_DIR_RW "src/selftest-rw"

void
stub_srr_rest_test (bool verbose)
{
    printf (" * stub_srr_rest: ");

    //  @selftest
    //  Simple create/destroy test
    stub_srr_rest_t *self = stub_srr_rest_new ();
    assert (self);
    stub_srr_rest_destroy (&self);
    //  @end
    printf ("OK\n");
}
