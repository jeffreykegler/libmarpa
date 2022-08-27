<!--
Copyright 2018 Jeffrey Kegler
Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
-->

# The Kollos code

# Table of contents
<!--
cd kollos && ../lua/lua toc.lua < kollos.md
-->
* [About Kollos](#about-kollos)
* [Abbreviations](#abbreviations)
* [Development Notes](#development-notes)
  * [Preparing for beta release](#preparing-for-beta-release)
  * [To Do](#to-do)
    * [TODO notes](#todo-notes)
  * [Terminology fixes](#terminology-fixes)
  * [Kollos assumes core libraries are loaded](#kollos-assumes-core-libraries-are-loaded)
  * [New lexer features](#new-lexer-features)
  * [Discard default statement?](#discard-default-statement)
  * [Remove Libmarpa trace facility](#remove-libmarpa-trace-facility)
  * [Remove Libmarpa lookers](#remove-libmarpa-lookers)
* [Concepts](#concepts)
  * [External, inner and internal](#external-inner-and-internal)
  * [Symbol names, IDs and forms](#symbol-names-ids-and-forms)
* [Kollos object](#kollos-object)
* [Kollos registry objects](#kollos-registry-objects)
* [SLIF grammar (SLG) class](#slif-grammar-slg-class)
  * [SLG fields](#slg-fields)
  * [SLG constructor](#slg-constructor)
  * [SLG accessors](#slg-accessors)
  * [Mutators](#mutators)
  * [Hash to runtime processing](#hash-to-runtime-processing)
    * [Add a G1 rule](#add-a-g1-rule)
* [Lexeme class](#lexeme-class)
* [Block class](#block-class)
  * [SLR fields](#slr-fields)
  * [Block checking methods](#block-checking-methods)
* [SLIF recognizer (SLR) class](#slif-recognizer-slr-class)
  * [SLR fields](#slr-fields-DUP)
  * [SLR constructors](#slr-constructors)
  * [Internal reading](#internal-reading)
  * [High-level external reading](#high-level-external-reading)
  * [Low-level external reading](#low-level-external-reading)
  * [Evaluation](#evaluation)
  * [Locations](#locations)
  * [Events](#events)
  * [Progress reporting](#progress-reporting)
  * [SLR diagnostics](#slr-diagnostics)
* [SLIF valuer (SLV) class](#slif-valuer-slv-class)
  * [SLV fields](#slv-fields)
  * [SLV constructor](#slv-constructor)
  * [SLV mutators](#slv-mutators)
    * [Set the value of a stack entry](#set-the-value-of-a-stack-entry)
  * [SLV accessors](#slv-accessors)
    * [Return the value of a stack entry](#return-the-value-of-a-stack-entry)
    * [Return the top index of the stack](#return-the-top-index-of-the-stack)
  * [SLV diagnostics](#slv-diagnostics)
* [Abstract Syntax Forest (ASF) class](#abstract-syntax-forest-asf-class)
  * [ASF fields](#asf-fields)
  * [ASF constructor](#asf-constructor)
  * [ASF mutators](#asf-mutators)
  * [ASF accessors](#asf-accessors)
* [Abstract Syntax Forest (ASF2) class](#abstract-syntax-forest-asf2-class)
  * [ASF2 fields](#asf2-fields)
  * [ASF2 constructor](#asf2-constructor)
  * [ASF2 mutators](#asf2-mutators)
  * [ASF2 accessors](#asf2-accessors)
  * [ASF2 diagnostics](#asf2-diagnostics)
* [Glade class](#glade-class)
  * [Glade fields](#glade-fields)
* [Kollos semantics](#kollos-semantics)
  * [VM operations](#vm-operations)
    * [VM add op utility](#vm-add-op-utility)
    * [VM debug operation](#vm-debug-operation)
    * [VM no-op operation](#vm-no-op-operation)
    * [VM bail operation](#vm-bail-operation)
    * [VM result operations](#vm-result-operations)
    * [VM "result is undef" operation](#vm-result-is-undef-operation)
    * [VM "result is token value" operation](#vm-result-is-token-value-operation)
    * [VM "result is N of RHS" operation](#vm-result-is-n-of-rhs-operation)
    * [VM "result is N of sequence" operation](#vm-result-is-n-of-sequence-operation)
    * [VM operation: result is constant](#vm-operation-result-is-constant)
    * [Operation of the values array](#operation-of-the-values-array)
    * [VM "push undef" operation](#vm-push-undef-operation)
    * [VM "push one" operation](#vm-push-one-operation)
    * [Find current token literal](#find-current-token-literal)
    * [VM "push values" operation](#vm-push-values-operation)
    * [VM operation: push start location](#vm-operation-push-start-location)
    * [VM operation: push length](#vm-operation-push-length)
    * [VM operation: push G1 start location](#vm-operation-push-g1-start-location)
    * [VM operation: push G1 length](#vm-operation-push-g1-length)
    * [VM operation: push constant onto values array](#vm-operation-push-constant-onto-values-array)
    * [VM operation: set the array blessing](#vm-operation-set-the-array-blessing)
    * [VM operation: result is array](#vm-operation-result-is-array)
    * [VM operation: callback](#vm-operation-callback)
  * [Run the virtual machine](#run-the-virtual-machine)
  * [Find and perform the VM operations](#find-and-perform-the-vm-operations)
  * [VM-related utilities for use in the Perl code](#vm-related-utilities-for-use-in-the-perl-code)
    * [Return operation key given its name](#return-operation-key-given-its-name)
    * [Return operation name given its key](#return-operation-name-given-its-key)
  * [Input](#input)
* [Lexeme (LEX) class](#lexeme-lex-class)
  * [LEX fields](#lex-fields)
* [External rule (XRL) class](#external-rule-xrl-class)
  * [XRL fields](#xrl-fields)
* [External production (XPR) class](#external-production-xpr-class)
  * [XPR fields](#xpr-fields)
* [Internal rule (IRL) class](#internal-rule-irl-class)
  * [IRL fields](#irl-fields)
* [External symbol (XSY) class](#external-symbol-xsy-class)
  * [XSY fields](#xsy-fields)
  * [XSY accessors](#xsy-accessors)
* [Inner symbol (ISY) class](#inner-symbol-isy-class)
  * [ISY fields](#isy-fields)
  * [ISY accessors](#isy-accessors)
* [Libmarpa grammar wrapper (LMG) class](#libmarpa-grammar-wrapper-lmg-class)
  * [LMG fields](#lmg-fields)
  * [LMG constructor](#lmg-constructor)
  * [LMG accessors](#lmg-accessors)
* [Libmarpa recognizer wrapper (LMR) class](#libmarpa-recognizer-wrapper-lmr-class)
  * [LMR fields](#lmr-fields)
  * [Functions for tracing Earley sets](#functions-for-tracing-earley-sets)
* [Libmarpa valuer wrapper class](#libmarpa-valuer-wrapper-class)
  * [Adjust metal tables](#adjust-metal-tables)
* [Libmarpa interface](#libmarpa-interface)
  * [Standard template methods](#standard-template-methods)
  * [Libmarpa class constructors](#libmarpa-class-constructors)
* [Output](#output)
  * [The main Lua code file](#the-main-lua-code-file)
    * [Preliminaries to the main code](#preliminaries-to-the-main-code)
  * [The Kollos C code file](#the-kollos-c-code-file)
    * [Stuff from okollos](#stuff-from-okollos)
    * [Kollos metal loader](#kollos-metal-loader)
    * [Preliminaries to the C library code](#preliminaries-to-the-c-library-code)
  * [The Kollos C header file](#the-kollos-c-header-file)
    * [Preliminaries to the C header file](#preliminaries-to-the-c-header-file)
* [Internal utilities](#internal-utilities)
  * [Coroutines](#coroutines)
* [Exceptions](#exceptions)
* [Meta-coding](#meta-coding)
  * [Metacode execution sequence](#metacode-execution-sequence)
  * [Dedent method](#dedent-method)
  * [`c_safe_string` method](#c-safe-string-method)
  * [Meta code argument processing](#meta-code-argument-processing)
* [Kollos non-locals](#kollos-non-locals)
  * [Create a sandbox](#create-a-sandbox)
  * [Constants: Ranking methods](#constants-ranking-methods)
* [Kollos utilities](#kollos-utilities)
  * [VLQ (Variable-Length Quantity)](#vlq-variable-length-quantity)

## About Kollos

This is the code for Kollos, the "middle layer" of Marpa.
Below it is Libmarpa, a library written in
the C language which contains the actual parse engine.
Above it is code in a higher level language -- at this point Perl.

This document is evolving.  Most of the "middle layer" is still
in the Perl code or in the Perl XS, and not represented here.
This document only contains those portions converted to Lua or
to Lua-centeric C code.

The intent is that eventually
all the code in this file will be "pure"
Kollos -- no Perl knowledge.
That is not the case at the moment.

## Abbreviations

* desc -- Short for "description".  Used a lot for
strings which "describe" something, usually for the
purpose of being assembled into a larger string,
and usually as part of some message.

* giter -- An *Iter*ator factory (aka generator).
In _Programming Lua_,
Roberto insists that the argument of the generic
for is not an iterator, but an iterator generator.

* iter -- An *Iter*ator.
But see also "giter".

* ix -- Index

* pcs -- Short for "pieces".  Often used as table name,
where the tables purposes is to assemble a larger string,
which is usually a message,
and which is usually assembled using `table.concat()`.

## Development Notes

This section is primarily for the convenience of the
active developers.
Readers trying to familiarize themselves with Kollos
may want to skip it
in their first readings.

### Stack

This is a (in very rough order) "stack"
of items to do short term.

#### Allow lexemes on L0 RHS

### Preparing for beta release

The following is a checklist of things to do before beta.
It is not fully settled -- items may be added.

* Remove null-ranking.

* `g1_to_block_first(loc)` now returns a value when
`loc` is one past the last g1 location.
Fix this.

* Re-read the docs from beginning to end,
to make sure they're consistent and consistently up to date.

* Change DSL "hide" syntax from
`( <a> <b> <c>)`
to `(- <a> <b> <c>-)`.

* Remove `{}` syntax for grouping statements.

* Rename grammar, recognizer methods, classes.

* Change lexeme priorities from per-location to absolute.

* Investigate treatment of duplicate tokens -- make into soft
  error?  What about other similar error codes?

* Clean up DSL syntax.  Eliminate, or reduce use of,
or make more orthogonal use of,
pseudo-rules and pseudo-symbols.
I have in mind the following constructs:

    - Start pseudo-rule: `:start ::= Script`

    - Discard pseudo-rule: `:discard ~ whitespace event => ws`
      and `:discard ~ [,] event => comma=off`

    - Lexeme pseudo-rule: `:lexeme ~ <say keyword> priority => 1`

    - Default pseudo-rule: `:default ::= action => [symbol, name, values]`

    - Discard default statement: `discard default = event => :symbol=on`

    - Lexeme default statement: `lexeme default = action => [ name, value ]`

### To Do

#### TODO notes

Throughout the code, the string "TODO" marks notes
for action during or after development.
Not all of these are included or mentioned
in these "Development Notes".

### Terminology fixes

* Check all uses of "pauses", eliminating those where
  it refers to parse events.

* Eliminate all uses of "physical" and "virtual" wrt
  parse location.

### Kollos assumes core libraries are loaded

Currently Kollos assumes that the
core libraries are loaded.
Going forward, it needs to "require" them,
like an ordinary Lua library.

### New lexer features

*  Changing priorities to be "non-local".  Current priorities only break
ties for tokens at the same location.  "Non-local" means if there is a
priority 2 lexeme of any length and/or eagerness, you will get that
lexeme, and not any lexeme of priority 1 or lower.

* Lookahead?

### Discard default statement?

[ This is a leftover
design note from some time ago, for a never-implemented
feature.
I'm keeping this note until I decide about this feature,
one way or the other. ]

The `discard default` statement would be modeled on the
`lexeme default` statement.  An example:

```
   discard default => event => ::name=off
```

### Remove Libmarpa trace facility

This is replaced by the traversers.

### Remove Libmarpa lookers

These are replaced by the traversers.

## Concepts

This section describes those Kollos concepts who do
not fall conveniently into one of the code sections.

### External, inner and internal

The Kollos upper layers and Libmarpa both
makes substantial use of rule rewriting.
As a result,
Kollos has external rules (xrl's),
inner rules (irl's)
and internal rules (nrl's).
External rules contain external symbols (xsy's).
Inner rules contain inner symbols (isy's).
Internal rules contain internal symbols (nsy's).

Xrl's and xsy's are the
only rules and symbols
intended to be visible to the SLIF user.
Irl's, isy's, nrl's and nsy's are
intended to be seen by Kollos
developers and maintainers only.

Libmarpa and the
Kollos upper layers
each have
two sets of rules and symbols,
one pre-rewrite and one post-rewrite.
The xrl's and xsy's are the Kollos upper
layer's pre-rewrite rules and symbols.

The irl's and isy's are the Kollos upper
layer's post-rewrite rules and symbols.
The irl's and isy's are also Libmarpa's
pre-rewrite rules and symbols.
Irl's and isy's
are, therefore, a "common language"
that Libmarpa and the Kollos upper layers share.

Libmarpa's post-rewrite symbols are
the nrl's and nsy's.
Nrl's and nsy's are
intended to be known only to Libmarpa,
and to Libmarpa's developers and maintainers.

For historical reasons,
and confusingly,
some of Libmarpa's
internal methods refer to nrl's as irl's.
The names of these methods will eventually be
changed.
They are being kept for now so that
Kollos's Libmarpa does not diverge
too far from
Marpa::R2's Libmarpa.

### Symbol names, IDs and forms

Symbols are show several "forms".

#### Name and ID

Name and ID are unique identifiers, available for
all valid symbols,
and only for valid symbols.
IDs are integers.
Names are strings.
The symbol name may be an internal creation,
subject to change in future versions of Kollos.

#### DSL form

DSL form is the form as it appears in the SLIF DSL.
It does not vary as long as the DSL does not vary.
It is not guaranteed unique and many valid symbols
will not have a DSL form.

#### Display form

Display form is a string
available
for all valid symbols,
and for invalid symbols IDs as well.
It is Kollos's idea of "best" form for
display.
Display forms are not necessarily unique.

In the case of an invalid symbol,
display form dummies up a symbol name
describing the problem.
This may be consider a kind of "soft failure".

The display form of an ISY
will be the display form of the XSY when
possible,
and a "soft failure" otherwise.
Because of this,
code which requires the display form of the XSY
corresponding to an ISY
will usually just ask for the display form of the ISY.

## Kollos object

`warn` is for a warning callback -- it's not
currently used.
`buffer` is used by kollos internally, usually
for buffering symbol ID's in the libmarpa wrappers.
`buffer_capacity` is its current capacity.

The buffer strategy currently is to set its capacity to the
maximum symbol count of any of the grammars in the Kollos
interpreter.

```
    -- miranda: section utility function definitions

    /* I assume this will be inlined by the compiler */
    static Marpa_Symbol_ID *shared_buffer_get(lua_State* L)
    {
        Marpa_Symbol_ID* buffer;
        const int base_of_stack = marpa_lua_gettop(L);
        marpa_lua_pushvalue(L, marpa_lua_upvalueindex(1));
        if (!marpa_lua_istable(L, -1)) {
            (void) internal_error_handle(L, "missing upvalue table",
            __PRETTY_FUNCTION__, __FILE__, __LINE__);
        }
        marpa_lua_getfield(L, -1, "buffer");
        buffer = marpa_lua_touserdata(L, -1);
        marpa_lua_settop(L, base_of_stack);
        return buffer;
    }

    -- miranda: section C function declarations
    /* I probably will, in the final version, want this to be a
     * static utility, internal to Kollos
     */
    int kollos_shared_buffer_resize(
        lua_State* L,
        size_t desired_capacity);
```

Not Lua C API.
Manipulates Lua stack,
leaving it as is.

```
    -- miranda: section external C function definitions
    int kollos_shared_buffer_resize(
        lua_State* L,
        size_t desired_capacity)
    {
        size_t buffer_capacity;
        const int base_of_stack = marpa_lua_gettop(L);
        const int upvalue_ix = base_of_stack + 1;

        marpa_lua_pushvalue(L, marpa_lua_upvalueindex(1));
        if (!marpa_lua_istable(L, -1)) {
            return internal_error_handle(L, "missing upvalue table",
            __PRETTY_FUNCTION__, __FILE__, __LINE__);
        }
        marpa_lua_getfield(L, upvalue_ix, "buffer_capacity");
        buffer_capacity = (size_t)marpa_lua_tointeger(L, -1);
        /* Is this test needed after development? */
        if (buffer_capacity < 1) {
            return internal_error_handle(L, "bad buffer capacity",
            __PRETTY_FUNCTION__, __FILE__, __LINE__);
        }
        if (desired_capacity > buffer_capacity) {
            /* TODO: this optimizes for space, not speed.
             * Insist capacity double on each realloc()?
             */
            (void)marpa_lua_newuserdata (L,
                desired_capacity * sizeof (Marpa_Symbol_ID));
            marpa_lua_setfield(L, upvalue_ix, "buffer");
            marpa_lua_pushinteger(L, (lua_Integer)desired_capacity);
            marpa_lua_setfield(L, upvalue_ix, "buffer_capacity");
        }
        marpa_lua_settop(L, base_of_stack);
        return 0;
    }

```

## Kollos registry objects

A Kollos registry object is an object kept in the Lua
registry.
The registry references allow the objects to be identified
safely to non-Lua code.

Only upper layer code should use the references.
Internal Lua code should not use them,
because it cannot know whether the upper layer
has released it.

Kollos does not do reference counting of its registry
objects.
If the upper layer wants the equivalent of reference counting,
it should register the same object multiple times.
This is at least as efficient as reference counting,
and Lua's GC will do the right thing.

TODO -- rather than use the Lua registry, perhaps Kollos
should have its own registry.
Right now the only upper layer is Perl, and there is a
dedicated Lua interpreter, but when Kollos is a library
it may be in an interpreter which does not follow the
standard registry conventions,
perhaps because it is buggy.

```
    -- miranda: section+ kollos table methods
    static int
    lca_register(lua_State* L)
    {
        marpa_luaL_checktype(L, 1, LUA_TTABLE);
        marpa_luaL_checkany(L, 2);
        marpa_lua_pushinteger(L, marpa_luaL_ref(L, 1));
        return 1;
    }

    static int
    lca_unregister(lua_State* L)
    {
        marpa_luaL_checktype(L, 1, LUA_TTABLE);
        marpa_luaL_checkinteger(L, 2);
        marpa_luaL_unref(L, 1, (int)marpa_lua_tointeger(L, 2));
        return 0;
    }

    -- miranda: section+ luaL_Reg definitions
    static const struct luaL_Reg kollos_funcs[] = {
      { "memcmp", lca_memcmp },
      { "from_vlq", lca_from_vlq },
      { "to_vlq", lca_to_vlq },
      { "register", lca_register },
      { "unregister", lca_unregister },
      { NULL, NULL },
    };

```

## SLIF grammar (SLG) class

### SLG fields

```
    -- miranda: section+ class_slg field declarations
    class_slg_fields.regix = true
    class_slg_fields.g1 = true
    class_slg_fields.l0 = true

    class_slg_fields.completion_event_by_isy = true
    class_slg_fields.completion_event_by_name = true
    class_slg_fields.discard_event_by_irl = true
    class_slg_fields.discard_event_by_name = true
    class_slg_fields.lexeme_event_by_isy = true
    class_slg_fields.lexeme_event_by_name = true
    class_slg_fields.nulled_event_by_isy = true
    class_slg_fields.nulled_event_by_name = true
    class_slg_fields.prediction_event_by_isy = true
    class_slg_fields.prediction_event_by_name = true

    class_slg_fields.exhaustion_action = true
    class_slg_fields.rejection_action = true

    class_slg_fields.nulling_semantics = true
    class_slg_fields.per_codepoint = true
    class_slg_fields.ranking_method = true
    class_slg_fields.if_inaccessible = true

    class_slg_fields.rule_semantics = true
    class_slg_fields.token_semantics = true

    class_slg_fields.xrls = true
    class_slg_fields.xprs = true
    class_slg_fields.xsys = true

```

A flag intended for developer use.
It might be used,
for example
to turn 'hi there' messages
on for particular grammars and not others

```
    -- miranda: section+ class_slg field declarations
    class_slg_fields.debug_level = true
```

Maps `(irlid, irl_dot)` pairs to external rule/symbol
information: `(xrlid, xrl_dot, predot_xsy)`.

```
    -- miranda: section+ class_slg field declarations
    class_slg_fields.preglade_sets = true
```

The "blessing" facility provide strings
to the upper layer.
The upper layer may interpret these as it wishes.
The Perl upper layer uses the "blessing" strings
to bless Kollos results into Perl packages,
whence the name.

```
    -- miranda: section+ class_slg field declarations
    class_slg_fields.default_blessing = true
```

```
    -- miranda: section+ populate metatables
    local class_slg_fields = {}
    -- miranda: insert class_slg field declarations
    declarations(_M.class_slg, class_slg_fields, 'slg')
```

This is a registry object.

```
    -- miranda: section+ luaL_Reg definitions
    static const struct luaL_Reg slg_methods[] = {
      { NULL, NULL },
    };

```

### SLG constructor

```
    -- miranda: section+ most Lua function definitions
    function _M.slg_new(flat_args)
        local slg = {}
        setmetatable(slg, _M.class_slg)
        local regix = _M.register(_M.registry, slg)
        slg.regix = regix

        slg.exhaustion_action = 'fatal'
        slg.rejection_action = 'fatal'

        slg.nulling_semantics = {}
        slg.rule_semantics = {}
        slg.token_semantics = {}

        slg.discard_event_by_irl = {}
        slg.discard_event_by_name = {}

        -- The codepoint data is populated, as needed, by the recognizers but,
        -- once populated, depends only on the codepoint and the
        -- grammar.
        slg.per_codepoint = {}

        slg.ranking_method = 'none'
        slg.if_inaccessible = 'warn'

        slg.debug_level = 0

        return slg
    end
```

```
    -- miranda: section+ forward declarations
    local precompute_cycles
    -- miranda: section+ most Lua function definitions
    function precompute_cycles(slg, subg)
        local loop_rule_count = 0
        local lmw_g = subg.lmw_g
        local events = lmw_g:events()
        for i = 1, #events, 2 do
            local event_type = events[i]
            if event_type == _M.event["LOOP_RULES"] then
                error(string.format(
                   "Unknown grammar precomputation event; type=%q"))
            end
            loop_rule_count = events[i+1]
        end
        if loop_rule_count > 0 then
            for rule_id = 0, lmw_g:highest_rule_id() do
                local lmw_g = subg.lmw_g
                local is_loop = lmw_g:rule_is_loop(rule_id)
                if is_loop then
                    local rule_desc = subg:rule_show(rule_id)
                    local message = string.format(
                        "Cycle found involving rule: %s\n", rule_desc
                    )
                    coroutine.yield('trace', message)
                end
            end
            error('Cycles in grammar, fatal error')
        end
    end
```

```
    -- miranda: section+ forward declarations
    local precompute_inaccessibles
    -- miranda: section+ most Lua function definitions
    function precompute_inaccessibles(slg, subg)
        local default_treatment = slg.if_inaccessible
        local lmw_g = subg.lmw_g

        -- This logic assumes that Marpa's logic
        -- is correct and that its rewrites are
        -- not creating inaccessible ISYs from
        -- accessible XSYs.

        for isyid = 0, lmw_g:highest_symbol_id() do
            local is_accessible = lmw_g:symbol_is_accessible(isyid) ~= 0
            if is_accessible then goto NEXT_SYMBOL end
            local xsy = subg.xsys[isyid]
            if not xsy then goto NEXT_SYMBOL end
            local treatment = xsy.if_inaccessible or default_treatment
            if treatment == 'ok' then goto NEXT_SYMBOL end
            -- return 'ok', 'ok', treatment
            local symbol_name = subg:symbol_name(isyid)
            local message = string.format(
                "Inaccessible %s symbol: %s", subg.name, symbol_name
            )
            if treatment == 'fatal' then _M.userX(message) end
            coroutine.yield('trace', message)
            ::NEXT_SYMBOL::
        end
    end
```

Assumes that caller has found an error code
in `lmw_g`.

```
    -- miranda: section+ forward declarations
    local do_precompute_errors
    -- miranda: section+ most Lua function definitions
    function do_precompute_errors(slg, lmw_g, error_object)

        if not error_object then
            local error_code, libmarpa_message = lmw_g:error_code()
            _M._internal_error('Precompute failed with no error object; code=%d; message=%q',
                error_code, libmarpa_message)
        end

        local error_code = error_object.code

        if not error_object then
            local error_code, libmarpa_message = lmw_g:error_code()
            _M._internal_error(
                'Precompute failed -- error object had no code',
                inspect(error_object, {depth=2} ))
        end

        if error_code == _M.err.LUA_INTERNAL then
            -- iprint('LUA_INTERNAL', inspect(error_object))
            error( error_object )
        end
        if error_code == _M.err.INTERNAL then
            error( error_object )
        end

        -- We do not handle cycles here -- we catch the
        -- events later and process them
        if error_code == _M.err.GRAMMAR_HAS_CYCLE then
            return
        end
        if error_code == _M.err.NO_RULES then
            _M.userX('Attempted to precompute grammar with no rules')
        end
        if error_code == _M.err.NULLING_TERMINAL then
            local msgs = {}
            local events = lmw_g:events()
            for i = 1, #events, 2 do
                local event_type = events[i]
                if event_type == _M.event.NULLING_TERMINAL then
                    msgs[#msgs+1] =
                       string.format("Nullable symbol %q is also a terminal\n",
                           lmw_g:symbol_name(events[i+1])
                       )
                end
            end
            msgs[#msgs+1] = 'A terminal symbol cannot also be a nulling symbol'
            _M.userX( '%s', table.concat(msgs) )
        end
        if error_code == _M.err.COUNTED_NULLABLE then
            local msgs = {}
            local events = lmw_g:events()
            for i = 1, #events, 2 do
                local event_type = events[i]
                if event_type == _M.event.COUNTED_NULLABLE then
                    msgs[#msgs+1] =
                       string.format("Nullable symbol %q is on RHS of counted rule\n",
                           lmw_g:symbol_name(events[i+1])
                       )
                end
            end
            msgs[#msgs+1] = 'Counted nullables confuse Marpa -- please rewrite the grammar\n'
            _M.userX( '%s', table.concat(msgs) )
        end
        if error_code == _M.err.START_NOT_LHS then
            _M.userX( "Start symbol %s not on LHS of any rule",
                lmw_g.start_name);
        end
        if error_code == _M.err.NO_START_SYMBOL then
                _M.userX('No start symbol')
        end
        if error_code == _M.err.UNPRODUCTIVE_START then
                _M.userX( 'Unproductive start symbol' )
        end

        error(error_object)
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slg.precompute(slg, subg)
        local lmw_g = subg.lmw_g
        if lmw_g:is_precomputed() ~= 0 then
            _M.userX('Attempted to precompute grammar twice')
        end
        if lmw_g:force_valued() < 0 then
            error( lmw_g:error_description() )
        end
        local start_name = lmw_g.start_name
        local start_id = lmw_g.isyid_by_name[start_name]
        if not start_id then
            error(string.format(
    "Internal error: Start symbol %q missing from grammar", start_name))
        end
        local result = lmw_g:start_symbol_set(start_id)
        if result < 0 then
            error(string.format(
                "Internal error: start_symbol_set() of %q failed; %s",
                    start_name,
                    lmw_g:error_description()
            ))
        end
        _M.throw = false
        local result, error_object = lmw_g:precompute()
        _M.throw = true
        if not result then
            do_precompute_errors(slg, lmw_g, error_object)
        end
        -- Above I went through the error events
        -- Now I go through the events for situations where there was no
        -- hard error returned from libmarpa
        precompute_cycles(slg, subg)
        precompute_inaccessibles(slg, subg)
        return
    end
```

```
    -- miranda: section+ forward declarations
    local precompute_g1
    -- miranda: section+ most Lua function definitions
    function precompute_g1(slg, source_hash)
        slg.g1 = _M.grammar_new(slg, 'g1')
        -- print(inspect(source_hash))
        -- Create the G1 grammar

        local g1g = slg.g1
        g1g.start_name = '[:start:]'

        do
           local g1_symbols = source_hash.symbols.g1
           local g1_symbol_names = {}
           for symbol_name, _ in pairs(g1_symbols) do
               g1_symbol_names[#g1_symbol_names+1] = symbol_name
           end
           table.sort(g1_symbol_names)
           for ix = 1,#g1_symbol_names do
               local symbol_name = g1_symbol_names[ix]
               local options = g1_symbols[symbol_name]
               g1_symbol_assign(slg, symbol_name, options)
           end
        end

        do
           local g1_rules = source_hash.rules.g1
           for ix = 1,#g1_rules do
               local options = g1_rules[ix]
               g1_rule_add(slg, options)
           end
        end

        local function event_setup(g1g, events, set_fn, activate_fn)
            local event_by_isy = {}
            local event_by_name = {}
            for isy_name, event in pairs(events) do
                local event_name = event[1]
                local is_active = (event[2] ~= "0")
                local isyid = g1g.isyid_by_name[isy_name]
                if not isyid then
                    error(string.format(
                        "Event defined for non-existent symbol: %s\n",
                        isy_name
                    ))
                end
                local event_desc = {
                   name = event_name,
                   isyid = isyid
                }
                event_by_isy[isyid] = event_desc
                event_by_isy[isy_name] = event_desc
                local name_entry = event_by_name[event_name]
                if not name_entry then
                    event_by_name[event_name] = { event_desc }
                else
                    name_entry[#name_entry+1] = event_desc
                end


                --  NOT serializable
                --  Must be done before precomputation
                set_fn(g1g, isyid, 1)
                if not is_active then activate_fn(g1g, isyid, 0) end
            end
        return event_by_isy, event_by_name
        end

        slg.completion_event_by_isy, slg.completion_event_by_name
            = event_setup(g1g,
                (source_hash.completion_events or {}),
                g1g.symbol_is_completion_event_set,
                g1g.completion_symbol_activate
            )

        slg.nulled_event_by_isy, slg.nulled_event_by_name
            = event_setup(g1g,
                (source_hash.nulled_events or {}),
                g1g.symbol_is_nulled_event_set,
                g1g.nulled_symbol_activate
            )

        slg.prediction_event_by_isy, slg.prediction_event_by_name
            = event_setup(g1g,
                (source_hash.prediction_events or {}),
                g1g.symbol_is_prediction_event_set,
                g1g.prediction_symbol_activate
            )

        slg:precompute(g1g)
    end
```

```
    -- miranda: section+ forward declarations
    local precompute_l0
    -- miranda: section+ most Lua function definitions
    function precompute_l0(slg, source_hash)
        local l0g = _M.grammar_new(slg, 'l0')
        slg.l0 = l0g
        l0g.start_name = '[:lex_start:]'

        local g1g = slg.g1

        do
           local l0_symbols = source_hash.symbols.l0
           local l0_symbol_names = {}
           for symbol_name, _ in pairs(l0_symbols) do
               l0_symbol_names[#l0_symbol_names+1] = symbol_name
           end
           table.sort(l0_symbol_names)
           for ix = 1,#l0_symbol_names do
               local symbol_name = l0_symbol_names[ix]
               local options = l0_symbols[symbol_name]
               l0_symbol_assign(slg, symbol_name, options)
           end
        end

        do
           local l0_rules = source_hash.rules.l0
           for ix = 1,#l0_rules do
               local options = l0_rules[ix]
               l0_rule_add(slg, options)
           end
        end

        for g1_isyid = 0, g1g:highest_symbol_id() do
            local is_terminal = 0 ~= g1g:symbol_is_terminal(g1_isyid)

            if not is_terminal then goto NEXT_G1_ISY end

            local g1_isy = g1g.isys[g1_isyid]
            local lexeme = setmetatable( {}, _M.class_lexeme )
            lexeme.g1_isy = g1_isy
            g1g.isys[g1_isyid].lexeme = lexeme
            local xsy = g1g:_xsy(g1_isyid)
            if not xsy then goto NEXT_G1_ISY end

            -- TODO delete this check after development ?
            if xsy.lexeme then
                local g1_isyid2 = xsy.lexeme.g1_isy.id
                _M._internal_error(
                    "Xsymbol %q (id=%d) has 2 g1 lexemes: \n\z
                    \u{20}   %q (id=%d), and\n\z
                    \u{20}   %q (id=%d)\n",
                    xsy:display_name(), xsy.id,
                    g1g:symbol_name(g1_isyid), g1_isyid,
                    g1g:symbol_name(g1_isyid2), g1_isyid2
                 )
            end

            lexeme.xsy = xsy
            xsy.lexeme = lexeme

            local lexeme_name = xsy.name

            -- TODO delete this check after development ?
            if lexeme_name ~= slg.g1:symbol_name(g1_isyid) then
                _M._internal_error(
                    "1: Lexeme name mismatch xsy=%q, g1 isy = %q",
                    lexeme_name,
                    slg.g1:symbol_name(g1_isyid)
                 )
            end

            local l0_isyid = slg:l0_symbol_by_name(lexeme_name)
            local l0_isy = l0g.isys[l0_isyid]
            lexeme.l0_isy = l0_isy
            l0_isy.lexeme = lexeme

            ::NEXT_G1_ISY::
        end

        local l0_discard_isyid = slg:l0_symbol_by_name('[:discard:]')
        local l0_target_isyid = slg:l0_symbol_by_name('[:target:]')
        for l0_irlid = 0, l0g:highest_rule_id() do
            local irl = l0g.irls[l0_irlid]
            local lhs_id = l0g:rule_lhs(l0_irlid)
            -- a discard rule
            if lhs_id == l0_discard_isyid then
                irl.g1_lexeme = 'discard'
                goto NEXT_L0_IRL
            end
            -- not a target or discard rule
            if lhs_id ~= l0_target_isyid then
                irl.g1_lexeme = 'ignore'
                goto NEXT_L0_IRL
            end
            -- a target rule
            local l0_rhs_id = l0g:rule_rhs(l0_irlid, 0)

            -- the rule '[:target:] ::= [:discard:]'
            if l0_rhs_id == l0_discard_isyid then
                irl.g1_lexeme = 'ignore'
                goto NEXT_L0_IRL
            end

            -- a lexeme rule?
            local l0_rhs_lexeme = l0g.isys[l0_rhs_id].lexeme
            if not l0_rhs_lexeme then
                irl.g1_lexeme = 'ignore'
                goto NEXT_L0_IRL
            end
            irl.g1_lexeme = l0_rhs_lexeme.g1_isy.id

            ::NEXT_L0_IRL::
    end

    -- Add ZWAs for assertions
    for l0_irlid = 0, l0g:highest_rule_id() do
        local lhs_id = l0g:rule_lhs(l0_irlid)
        if lhs_id ~= l0_target_isyid then
            goto NEXT_IRL
        end
        local l0_lexeme_id = l0g:rule_rhs(l0_irlid, 0)
        if l0_lexeme_id == l0_discard_isyid then
            goto NEXT_IRL
        end
        local lexeme = l0g.isys[l0_lexeme_id].lexeme
        if lexeme then
            local g1_lexeme_id = lexeme.g1_isy.id
            local assertion_id = slg.g1.isys[g1_lexeme_id].assertion
            if not assertion_id then
                assertion_id = l0g:zwa_new(0)
            end
            slg.g1.isys[g1_lexeme_id].assertion = assertion_id
            l0g:zwa_place(assertion_id, l0_irlid, 0)
        end
        ::NEXT_IRL::
    end

    slg:precompute(l0g)

    end
```

```
    -- miranda: section+ forward declarations
    local precompute_preglade_sets
    -- miranda: section+ most Lua function definitions
    function precompute_preglade_sets(slg)
        local g1g = slg.g1
        local xsys = slg.xsys
        local preglade_sets = {}
        local ahm_count = g1g:_ahm_count()
        for ahm_id = 0, ahm_count -1 do
            local preglades = {}
            local nrl_id = g1g:_ahm_nrl(ahm_id)
            local nrl_dot = g1g:_ahm_raw_position(ahm_id)
            local null_count = g1g:_ahm_null_count(ahm_id)
            local rhs_ix1 = nrl_dot - 1 - null_count
            if rhs_ix1 >= 0 then
                local nsy_id = g1g:_nrl_rhs(nrl_id, rhs_ix1)
                if g1g:_nsy_is_semantic(nsy_id) then
                    local xsy_id = g1g:xsyid_by_nsy(nsy_id)
                    if xsy_id then
                        local xsy = xsys[xsy_id]
                        local is_terminal = xsy.lexeme
                        local xsy_type = is_terminal and 'token' or 'brick'
                        preglades[#preglades+1] = { xsy_type, xsy_id }
                    end
                end
            end
            -- Add the null ur-glades
            for null_ix = 1, null_count do
                local nsy_id = g1g:_nrl_rhs(nrl_id, rhs_ix1 + null_ix)
                if g1g:_nsy_is_semantic(nsy_id) or g1g:_start_nsy() == nsy_id then
                    local xsy_id = g1g:xsyid_by_nsy(nsy_id)
                    if xsy_id then
                        preglades[#preglades+1] = { 'null', xsy_id }
                    end
                end
            end
            for ix = 1, #preglades do
               local item = preglades[ix]
               local key = item[1]
               local xsy_id = item[2]
               local by_key = preglades[key] or {}
               by_key[#by_key+1] = xsy_id
               preglades[key] = by_key
               preglades[ix] = nil
            end
            preglade_sets[ahm_id] = preglades
        end
        slg.preglade_sets = preglade_sets
    end
```

```
    -- miranda: section+ forward declarations
    local precompute_discard_events
    -- miranda: section+ most Lua function definitions
    function precompute_discard_events(slg, source_hash)
        local l0g = slg.l0
        local l0_discard_isyid = slg:l0_symbol_by_name('[:discard:]')

        for irlid = 0, l0g:highest_rule_id() do
            local irl = l0g.irls[irlid]
            local lhs_id = l0g:rule_lhs(irlid)
            local xpr = irl.xpr
            if not xpr then
                goto NEXT_IRL_ID
            end
            if lhs_id ~= l0_discard_isyid then
                goto NEXT_IRL_ID
            end
            do
                local is_active
                local event_name = xpr.event_name
                if event_name then
                     is_active = xpr.event_starts_active
                     goto EVENT_FOUND
                end
                do
                    local adverbs = source_hash.discard_default_adverbs
                    local event = adverbs and adverbs.event
                    if not event then return '' end
                    event_name, is_active = table.unpack(event)
                    is_active = is_active == '1'
                end
                ::EVENT_FOUND::
                if event_name == "'symbol" then
                    local irl = l0g.irls[irlid]
                    -- at this point, xpr must be defined
                    local xpr = irl.xpr
                    event_name = xpr.symbol_as_event
                end
                if event_name:sub(1, 1) == "'" then
                    _M.userX("Discard event has unknown name: %q", event_name)
                end

                    local event_desc = {
                       name = event_name,
                       irlid = irlid
                    }
                    slg.discard_event_by_irl[irlid] = event_desc
                    local name_entry = slg.discard_event_by_name[event_name]
                    if not name_entry then
                        slg.discard_event_by_name[event_name] = { event_desc }
                    else
                        name_entry[#name_entry+1] = event_desc
                    end

                    irl.event_on_discard = true
                    irl.event_on_discard_active = is_active

            end
            ::NEXT_IRL_ID::
        end
    end
```

```
    -- miranda: section+ forward declarations
    local precompute_lexeme_adverbs
    -- miranda: section+ most Lua function definitions
    function precompute_lexeme_adverbs(slg, source_hash)
        local lexeme_declarations = source_hash.lexeme_declarations or {}
        local g1g = slg.g1
        local l0g = slg.l0

        local lexeme_event_by_isy = {}
        slg.lexeme_event_by_isy = lexeme_event_by_isy
        local lexeme_event_by_name = {}
        slg.lexeme_event_by_name = lexeme_event_by_name

        for g1_lexeme_id = 0, g1g:highest_symbol_id() do
            local isy = g1g.isys[g1_lexeme_id]
            local lexeme_name = isy.name
            local declarations = lexeme_declarations[lexeme_name]
            if not isy.lexeme then
                goto NEXT_SYMBOL
            end
            declarations = declarations or {}

            local g1_isy = slg.g1.isys[g1_lexeme_id]
            local priority = 0
            if declarations.priority then
                priority = declarations.priority + 0
            end
            g1_isy.priority = priority
            if declarations.eager then g1_isy.eager = true end
            if slg.completion_event_by_isy[lexeme_name] then
                error(string.format(
                    "A completion event is declared for <%s>, but it is a lexeme.\n\z
                    \u{20}    Completion events are only valid for symbols on the LHS of G1 rules.\n",
                    lexeme_name
                ))
            end
            if slg.nulled_event_by_isy[lexeme_name] then
                error(string.format(
                    "A nulled event is declared for <%s>, but it is a lexeme.\n\z
                    \u{20}    Nulled events are only valid for symbols on the LHS of G1 rules.\n",
                    lexeme_name
                ))
            end

            local pause_value = declarations.pause
            if pause_value then
                pause_value = math.tointeger(pause_value)
                local g1_isy = slg.g1.isys[g1_lexeme_id]
                if pause_value == 1 then
                     g1_isy.pause_after = true
                elseif pause_value == -1 then
                     g1_isy.pause_before = true
                end
                local event = declarations.event
                local is_active = 1
                if event then
                    local event_name = event[1]
                    local is_active = event[2] ~= '0'

                    local event_desc = {
                       name = event_name,
                       isyid = g1_lexeme_id,
                    }
                    lexeme_event_by_isy[g1_lexeme_id] = event_desc
                    lexeme_event_by_isy[lexeme_name] = event_desc
                    local name_entry = lexeme_event_by_name[event_name]
                    if not name_entry then
                        lexeme_event_by_name[event_name] = { event_desc }
                    else
                        name_entry[#name_entry+1] = event_desc
                    end

                    local g1_isy = slg.g1.isys[g1_lexeme_id]
                    if is_active then
                        -- activate only if event is enabled
                        g1_isy.pause_after_active = g1_isy.pause_after
                        g1_isy.pause_before_active = g1_isy.pause_before
                    else
                        g1_isy.pause_after_active = nil
                        g1_isy.pause_before_active = nil
                    end

                end
            end
            ::NEXT_SYMBOL::
        end

        for irlid = 0, l0g:highest_rule_id() do
            local irl = l0g.irls[irlid]
            if l0g:rule_length(irlid) > 0 then
                local discard_symbol_id = l0g:rule_rhs(irlid, 0)
                local g1_lexeme_id = irl.g1_lexeme
                if type(g1_lexeme_id) == 'number' then
                    local eager = g1g.isys[g1_lexeme_id].eager
                    if eager then irl.eager = true end
                end
                local eager = l0g.isys[discard_symbol_id].eager
                if eager then
                    irl.eager = true
                end
            end
        end

        local lexeme_default_adverbs = source_hash.lexeme_default_adverbs or {}
        local default_lexeme_action = lexeme_default_adverbs.action
        if default_lexeme_action then
            for _, xsy in pairs(slg.xsys) do
                local name_source = xsy.name_source
                if name_source == 'lexical' and not xsy.lexeme_semantics then
                    xsy.lexeme_semantics = default_lexeme_action
                end
            end
        end
    end
```

This information is serializable, I think,
and so this function should really be run in
the AST-to-serializable phase.

```
    -- miranda: section+ forward declarations
    local precompute_xsy_blessings
    -- miranda: section+ most Lua function definitions
    function precompute_xsy_blessings(slg, source_hash)
        local lexeme_default_adverbs = source_hash.lexeme_default_adverbs or {}
        local default_blessing = lexeme_default_adverbs.bless or '::undef'
        local xsys = slg.xsys
        for xsyid = 1, #xsys do
            local xsy = xsys[xsyid]
            do
                if not xsy then
                    goto NEXT_XSYID
                end
                local lexeme = xsy.lexeme
                if not lexeme then
                    xsy.blessing = default_blessing
                    goto NEXT_XSYID
                end
                local name_source = xsy.name_source
                if name_source ~= 'lexical' then
                    xsy.blessing = default_blessing
                    goto NEXT_XSYID
                end
                if not xsy.blessing then
                    xsy.blessing = default_blessing
                    goto NEXT_XSYID
                end

                -- TODO delete the following check after development
                local g1_lexeme_id = lexeme.g1_isy.id
                if xsy.name ~= slg.g1:symbol_name(g1_lexeme_id) then
                    _M._internal_error(
                        "Lexeme name mismatch xsy=%q, g1 isy = %q",
                        xsy.name,
                        slg.g1:symbol_name(g1_lexeme_id)
                    )
                end
            end
            ::NEXT_XSYID::
        end
    end
```

This information is serializable, I think,
and so this function should really be run in
the AST-to-serializable phase.

```
    -- miranda: section+ forward declarations
    local precompute_character_classes
    -- miranda: section+ most Lua function definitions
    function precompute_character_classes(slg, source_hash)
        local character_class_hash = source_hash.character_classes
        local isys = slg.l0.isys
        for symbol_name, components in pairs(character_class_hash) do
            local char_class, flags = table.unpack(components)
            local isyid = slg:l0_symbol_by_name(symbol_name)
            local isy = isys[isyid]
            isy.character_class = char_class
            if flags then
                isy.character_flags = flags
            end
        end
    end
```

Takes the serializable form of the grammar as an argument,
and creates the "runtime" version, as a side effect.

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slg.seriable_to_runtime(slg, source_hash)
        xsys_populate(slg, source_hash)
        xrls_populate(slg, source_hash)
        xprs_populate(slg, source_hash)

        local if_inaccessible = slg.if_inaccessible
        do
            local defaults = source_hash.defaults
            if defaults then
                if_inaccessible = defaults.if_inaccessible or if_inaccessible
            end
        end
        slg.if_inaccessible = if_inaccessible

        precompute_g1(slg, source_hash);
        precompute_l0(slg, source_hash);
        precompute_preglade_sets(slg);
        precompute_discard_events(slg, source_hash)
        precompute_lexeme_adverbs(slg, source_hash)
        precompute_xsy_blessings(slg, source_hash)
        precompute_character_classes(slg, source_hash)
    end
```

```
    -- miranda: section+ forward declarations
    local l0_rule_add
    -- miranda: section+ most Lua function definitions
    function l0_rule_add(slg, options)
        local l0g = slg.l0

        local xpr_name = options.xprid or ''
        local default_rank = l0g:default_rank()
        -- io.stderr:write('xpr_name: ', inspect(xpr_name), '\n')
        local xpr_id = -1
        if #xpr_name > 0 then
            xpr_id = slg.xprs[xpr_name].id
        end

        local rhs_names = options.rhs or {}
        local min = options.min
        local separator_name = options.separator
        local is_ordinary_rule = not min or #rhs_names == 0
        if separator_name and is_ordinary_rule then
            error( 'separator defined for rule without repetitions')
        end

        local lhs_name = options.lhs
        local lhs_id = l0_symbol_assign(slg, lhs_name)
        local rhs_ids = {}
        local rule = { lhs_id }
        for ix = 1, #rhs_names do
            local rhs_id = l0_symbol_assign(slg, rhs_names[ix])
            rhs_ids[ix] = rhs_id
            rule[ix+1] = rhs_id
        end

        local base_irl_id
        if is_ordinary_rule then
            -- remove the test for nil or less than zero
            -- once refactoring is complete?
            _M.throw = false
            base_irl_id = l0g:rule_new(rule)
            _M.throw = true
            if not base_irl_id or base_irl_id < 0 then return -1 end
            l0g.irls[base_irl_id] = { id = base_irl_id }
        else
            if #rhs_names ~= 1 then
                error('Only one rhs symbol allowed for counted rule')
            end
            local sequence_options = {
                lhs = rule[1],
                rhs = rule[2],
                min = min
            }
            sequence_options.proper = (options.proper ~= 0)
            if separator_name then
                sequence_options.separator = l0_symbol_assign(slg, separator_name)
            end
            _M.throw = false
            base_irl_id = l0g:sequence_new(sequence_options)
            _M.throw = true
            -- remove the test for nil or less than zero
            -- once refactoring is complete?
            if not base_irl_id or base_irl_id < 0 then return end
            local l0_rule = setmetatable({}, _M.class_irl)
            l0_rule.id = base_irl_id
            l0g.irls[base_irl_id] = l0_rule
        end

        if not base_irl_id and base_irl_id < 0 then
            local rule_description = _M._raw_rule_show(lhs_id, rhs_ids)
            local error_code = l0g:error_code()
            local problem_description
            if error_code == _M.err.DUPLICATE_RULE then
                problem_description = "Duplicate rule"
            else
                problem_description = _M.err[error_code].description
            end
            error(problem_description .. ': ' .. rule_description)
        end

        local null_ranking_is_high = options.null_ranking == 'high' and 1 or 0
        local rank = options.rank or default_rank

        l0g:rule_null_high_set(base_irl_id, null_ranking_is_high)
        l0g:rule_rank_set(base_irl_id, rank)
        if xpr_id >= 0 then
            local xpr = slg.xprs[xpr_id]
            local irl = l0g.irls[base_irl_id]
            irl.xpr = xpr
            -- right now, the action & mask of an irl
            -- is always the action/mask of its xpr.
            -- But some day each irl may need its own.
            irl.action = xpr.action
            irl.mask = xpr.mask
        end
    end
```

### SLG accessors

Display any XPR

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slg.xpr_show_o(slg, xpr, options)
        options = options or {}
        local name_fn = options.diag and _M.class_xsy.diag_form
            or _M.class_xsy.display_form
        local pieces = {}
        local subg = xpr.subgrammar
        pieces[#pieces+1] = name_fn(xpr.lhs)
        pieces[#pieces+1] = subg == 'g1' and '::=' or '~'
        local rhs = xpr.rhs
        for ix = 1, #rhs do
            pieces[#pieces+1] = name_fn(rhs[ix])
        end
        local minimum = xpr.min
        if minimum then
            pieces[#pieces+1] =
                minimum <= 0 and '*' or '+'
        end
        local precedence = xpr.precedence
        if precedence then
            -- add a semi-colon to the most recent piece
            pieces[#pieces] = pieces[#pieces] .. ';'
            pieces[#pieces+1] = 'prec=' .. precedence
        end
        return table.concat(pieces, ' ')
    end
    function _M.class_slg.xpr_show(slg, xprid, options)
        local xprs = slg.xprs
        local xpr = xprs[xprid]
        return slg:xpr_show_o(xpr, options)
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slg.xpr_dotted_show(slg, xprid, dot_arg)
        local xprs = slg.xprs
        local xpr = xprs[xprid]
        local name_fn = _M.class_xsy.display_form
        local pieces = {}
        local subg = xpr.subgrammar
        pieces[#pieces+1] = name_fn(xpr.lhs)
        pieces[#pieces+1] = subg == 'g1' and '::=' or '~'
        local rhs = xpr.rhs
        local dot_used
        local dot = dot_arg == -1 and #rhs + 1 or dot_arg
        if dot == 0 then
          pieces[#pieces+1] = "."
          dot_used = true
        end
        for ix = 1, #rhs do
            pieces[#pieces+1] = name_fn(rhs[ix])
            if dot == ix then
              pieces[#pieces+1] = "."
              dot_used = true
            end
        end
        if not dot_used then
             _M.userX(
                 "xpr_dotted_show(%s, %s): dot is %s; must be -1, or 0-%d",
                 xprid, dot_arg, dot_arg, #rhs + 1
             )
        end
        local minimum = xpr.min
        if minimum then
            pieces[#pieces+1] =
                minimum <= 0 and '*' or '+'
        end
        local precedence = xpr.precedence
        if precedence then
            -- add a semi-colon to the most recent piece
            pieces[#pieces] = pieces[#pieces] .. ';'
            pieces[#pieces+1] = 'prec=' .. precedence
        end
        return table.concat(pieces, ' ')
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slg.xpr_expand_o(slg, xpr)
        local xsys = { xpr.lhs.id }
        local rhs = xpr.rhs
        for ix = 1, #rhs do
            local xsyid = rhs[ix].id
            xsys[#xsys+1] = xsyid
        end
        return xsys
    end
    function _M.class_slg.xpr_expand(slg, xprid)
        local xprs = slg.xprs
        local xpr = xprs[xprid]
        if not xpr then
            _M.userX(
                "xpr_expand(): %s is not a valid irlid",
                inspect(xprid)
            )
        end
        return slg:xpr_expand_o(xpr)
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slg.xpr_length_o(slg, xpr)
        return #xpr.rhs
    end
    function _M.class_slg.xpr_length(slg, xprid)
        local xprs = slg.xprs
        local xpr = xprs[xprid]
        if not xpr then
            _M.userX(
                "xpr_length(): %s is not a valid irlid",
                inspect(xprid)
            )
        end
        return slg:xpr_length_o(xpr)
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slg.xprs_show(slg, options)
        local verbose = options.verbose or 0
        local xprs = slg.xprs
        local xpr_descs = {}
        for xprid = 1, slg:highest_xprid() do
            local xpr = xprs[xprid]
            local subg = xpr.subgrammar

            local pcs = {}
            local pcs2 = {}
            pcs2[#pcs2+1] = 'R' .. xprid
            pcs2[#pcs2+1] = slg:xpr_show(xprid, options)
            pcs[#pcs+1] = table.concat(pcs2, ' ')
            pcs[#pcs+1] = "\n"

            local lhsid
            local rhsids = {}
            local rhs_length
            if verbose >= 2 then
                lhsid = xpr.lhs.id
                rhsids = {}
                local rhs = xpr.rhs
                rhs_length = #rhs
                local comments = {}
                if rhs_length == 0 then
                    comments[#comments+1] = 'empty'
                end
                if xpr.discard_separation then
                    comments[#comments+1] = 'discard_sep'
                end
                if #comments > 0 then
                    local pcs3 = {}
                    for ix = 1, #comments do
                        pcs3[#pcs3+1] = "/*" .. comments[ix] .. "*/"
                    end
                    pcs[#pcs+1] = table.concat(pcs3, ' ')
                    pcs[#pcs+1] = "\n"
                end
                pcs2 = {}
                pcs2[#pcs2+1] = '  Symbol IDs:'
                for ix = 1, rhs_length - 1 do
                   rhsids[ix] = xpr.rhs[ix].id
                end
                pcs2[#pcs2+1] = '<' .. lhsid .. '>'
                pcs2[#pcs2+1] = '::='
                for ix = 1, rhs_length - 1 do
                    pcs2[#pcs2+1] = '<' .. rhsids[ix] .. '>'
                end
                pcs[#pcs+1] = table.concat(pcs2, ' ')
                pcs[#pcs+1] = "\n"
            end
            if verbose >= 3 then
                local pcs2 = {}
                pcs2[#pcs2+1] = '  Canonical names:'
                pcs2[#pcs2+1] = slg:symbol_diag_form(lhsid)
                pcs2[#pcs2+1] = '::='
                for ix = 1, rhs_length - 1 do
                    pcs2[#pcs2+1] = slg:symbol_diag_form(rhsids[ix])
                end
                pcs[#pcs+1] = table.concat(pcs2, ' ')
                pcs[#pcs+1] = "\n"
            end
            xpr_descs[#xpr_descs+1] = {
                (subg == 'g1' and 0 or 1), xprid, table.concat(pcs)
            }
        end
        table.sort(xpr_descs, _M.cmp_seq)
        for ix = 1, #xpr_descs do
            local old_desc = xpr_descs[ix]
            xpr_descs[ix] = old_desc[#old_desc]
        end
        return table.concat(xpr_descs)
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slg.highest_xprid(slg)
        return #slg.xprs
    end
    function _M.class_slg.lmg_highest_rule_id(slg, subg_name)
        local subg = slg[subg_name]
        return subg:highest_rule_id();
    end
    function _M.class_slg.g1_highest_rule_id(slg, rule_id)
        return slg:lmg_highest_rule_id('g1')
    end
    function _M.class_slg.l0_highest_rule_id(slg, rule_id)
        return slg:lmg_highest_rule_id('l0')
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slg.xpr_name_o(slg, xpr)
        local name = xpr.name
        if name then return name end
        local lhs = xpr.lhs
        return lhs.name
    end
    function _M.class_slg.xpr_name(slg, xprid)
        local xpr = slg.xprs[xprid]
        if xpr then return slg:xpr_name_o(xpr) end
        return _M._internal_error('xpr_name(), bad argument = %d', xprid)
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slg.lmg_rule_to_xprid(slg, subg_name, irlid)
        local subg = slg[subg_name]
        local irl = subg.irls[irlid]
        if not irl then
            return _M._internal_error('lmg_rule_to_xprid(), bad argument = %d', irlid)
        end
        local xpr = irl.xpr
        if xpr then return xpr.id end
    end
    function _M.class_slg.g1_rule_to_xprid(slg, irlid)
        return slg:lmg_rule_to_xprid('g1', irlid)
    end
    function _M.class_slg.l0_rule_to_xprid(slg, irlid)
        return slg:lmg_rule_to_xprid('l0', irlid)
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slg.lmg_rule_to_xpr_dots(slg, subg_name, irlid)
        local subg = slg[subg_name]
        local irl = subg.irls[irlid]
        if not irl then
            return _M._internal_error('lmg_rule_to_xprid(), bad argument = %d', irlid)
        end
        -- print(inspect(irl, {depth=1}))
        return irl.xpr_dot
    end
    function _M.class_slg.g1_rule_to_xpr_dots(slg, irlid)
        return slg:lmg_rule_to_xpr_dots('g1', irlid)
    end
    function _M.class_slg.l0_rule_to_xpr_dots(slg, irlid)
        return slg:lmg_rule_to_xpr_dots('l0', irlid)
    end
```

TODO: Do I need xpr_top?

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slg.lmg_rule_is_xpr_top(slg, subg_name, irlid)
        local subg = slg[subg_name]
        local irl = subg.irls[irlid]
        if not irl then
            return _M._internal_error('lmg_rule_to_xprid(), bad argument = %s', inspect(irlid))
        end
        -- print(inspect(irl, {depth=1}))
        return irl.xpr_top
    end
    function _M.class_slg.g1_rule_is_xpr_top(slg, irlid)
        return slg:lmg_rule_is_xpr_top('g1', irlid)
    end
    function _M.class_slg.l0_rule_is_xpr_top(slg, irlid)
        return slg:lmg_rule_is_xpr_top('l0', irlid)
    end
```

TODO -- Turn lmg_*() forms into local functions?

TODO -- Census all Lua and perl symbol name functions, including
but not limited to lmg_*(), *_name(), *_{dsl,display}_form()
and eliminate the redundant ones.

Lowest XSYID is 1.
Lowest ISYID is 0.

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slg.highest_symbol_id(slg)
        return #slg.xsys
    end
    function _M.class_slg.lmg_highest_symbol_id(slg, subg_name)
        local subg = slg[subg_name]
        return subg:highest_symbol_id();
    end
    function _M.class_slg.g1_highest_symbol_id(slg, symbol_id)
        return slg:lmg_highest_symbol_id('g1')
    end
    function _M.class_slg.l0_highest_symbol_id(slg, symbol_id)
        return slg:lmg_highest_symbol_id('l0')
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slg.start_symbol_id(slg)
        return slg:symbol_by_name('[:start:]')
    end
    function _M.class_slg.lmg_start_symbol_id(slg, subg_name)
        local subg = slg[subg_name]
        return subg:start_symbol();
    end
    function _M.class_slg.g1_start_symbol_id(slg)
        return slg:lmg_start_symbol_id('g1')
    end
    function _M.class_slg.l0_start_symbol_id(slg)
        return slg:lmg_start_symbol_id('l0')
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slg.symbol_name(slg, xsyid)
        local xsy = slg.xsys[xsyid]
        if not xsy then return end
        return xsy.name
    end
    function _M.class_slg.lmg_symbol_name(slg, subg_name, symbol_id)
        local subg = slg[subg_name]
        return subg:symbol_name(symbol_id)
    end
    function _M.class_slg.g1_symbol_name(slg, symbol_id)
        return slg:lmg_symbol_name('g1', symbol_id)
    end
    function _M.class_slg.l0_symbol_name(slg, symbol_id)
        return slg:lmg_symbol_name('l0', symbol_id)
    end

    function _M.class_slg.symbol_by_name(slg, xsy_name)
        local xsy = slg.xsys[xsy_name]
        if not xsy then return end
        return xsy.id
    end
    function _M.class_slg.lmg_symbol_by_name(slg, symbol_name, subg_name)
        local subg = slg[subg_name]
        return subg.isyid_by_name[symbol_name]
    end
    function _M.class_slg.g1_symbol_by_name(slg, symbol_name)
        return slg:lmg_symbol_by_name(symbol_name, 'g1')
    end
    function _M.class_slg.l0_symbol_by_name(slg, symbol_name)
        return slg:lmg_symbol_by_name(symbol_name, 'l0')
    end

    function _M.class_slg.symbol_dsl_form(slg, xsyid)
        local xsy = slg.xsys[xsyid]
        if not xsy then
            _M.userX(
                "slg.symbol_dsl_form(): %s is not a valid xsyid",
                inspect(xsyid)
            )
        end
        return xsy.dsl_form
    end
    function _M.class_slg.lmg_symbol_dsl_form(slg, subg_name, symbol_id)
        local subg = slg[subg_name]
        return subg:symbol_dsl_form(symbol_id)
    end
    function _M.class_slg.g1_symbol_dsl_form(slg, symbol_id)
        return slg:lmg_symbol_dsl_form('g1', symbol_id)
    end
    function _M.class_slg.l0_symbol_dsl_form(slg, symbol_id)
        return slg:lmg_symbol_dsl_form('l0', symbol_id)
    end

    function _M.class_slg.symbol_display_form(slg, xsyid)
        local xsy = slg.xsys[xsyid]
        if not xsy then
            return '<bad xsyid ' .. xsyid .. '>'
        end
        return xsy:display_form();
    end
    function _M.class_slg.lmg_symbol_display_form(slg, subg_name, symbol_id)
        local subg = slg[subg_name]
        return subg:symbol_display_form(symbol_id)
    end
    function _M.class_slg.g1_symbol_display_form(slg, symbol_id)
        return slg:lmg_symbol_display_form('g1', symbol_id)
    end
    function _M.class_slg.l0_symbol_display_form(slg, symbol_id)
        return slg:lmg_symbol_display_form('l0', symbol_id)
    end

    function _M.class_slg.symbol_angled_form(slg, xsyid)
        local xsy = slg.xsys[xsyid]
        if not xsy then
            return '<bad xsyid ' .. xsyid .. '>'
        end
        return xsy:angled_form();
    end
    function _M.class_slg.lmg_symbol_angled_form(slg, subg_name, symbol_id)
        local subg = slg[subg_name]
        return subg:symbol_angled_form(symbol_id)
    end
    function _M.class_slg.g1_symbol_angled_form(slg, symbol_id)
        return slg:lmg_symbol_angled_form('g1', symbol_id)
    end
    function _M.class_slg.l0_symbol_angled_form(slg, symbol_id)
        return slg:lmg_symbol_angled_form('l0', symbol_id)
    end

    function _M.class_slg.symbol_diag_form(slg, xsyid)
        local xsy = slg.xsys[xsyid]
        if not xsy then
            return '[bad xsyid ' .. xsyid .. ']'
        end
        return xsy:diag_form();
    end
    function _M.class_slg.lmg_symbol_diag_form(slg, subg_name, symbol_id)
        local subg = slg[subg_name]
        return subg:symbol_diag_form(symbol_id)
    end
    function _M.class_slg.g1_symbol_diag_form(slg, symbol_id)
        return slg:lmg_symbol_diag_form('g1', symbol_id)
    end
    function _M.class_slg.l0_symbol_diag_form(slg, symbol_id)
        return slg:lmg_symbol_diag_form('l0', symbol_id)
    end

```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slg.g1_symbol_is_accessible(slg, symbol_id)
        local g1g = slg.g1
        return g1g:symbol_is_accessible(symbol_id) ~= 0
    end
    function _M.class_slg.g1_symbol_is_nulling(slg, symbol_id)
        local g1g = slg.g1
        return g1g:symbol_is_nulling(symbol_id) ~= 0
    end
    function _M.class_slg.g1_symbol_is_productive(slg, symbol_id)
        local g1g = slg.g1
        return g1g:symbol_is_productive(symbol_id) ~= 0
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slg.symbol_show(slg, symbol_id, options)
        local verbose = options.verbose or 0
        local symbol_id = math.tointeger(symbol_id)
        local max_symbol_id = #slg.xsys
        if not symbol_id or symbol_id < 1 or symbol_id > max_symbol_id then
            error(string.format('slg:symbol_show(): symbol_id is %s; must be an integer from 1 to %d',
                inspect(symbol_id, {depth = 1}),
                max_symbol_id
            ))
        end
        local pieces = { }
        pieces[#pieces+1] = table.concat (
            { 'S' .. symbol_id, slg:symbol_display_form( symbol_id ) },
            " ")
        pieces[#pieces+1] = "\n"
        if verbose >= 2 then
            pieces[#pieces+1] =  "  Canonical name: "
            pieces[#pieces+1] =  symbol_diag_form(slg:symbol_name(symbol_id))
            pieces[#pieces+1] =  "\n"
        end
        if verbose >= 3 then
            local dsl_form =  slg:symbol_dsl_form( symbol_id )
            if dsl_form then
                pieces[#pieces+1] =  '  DSL name: '
                pieces[#pieces+1] =  dsl_form
                pieces[#pieces+1] =  "\n"
            end
        end
        return table.concat(pieces)
    end
    function _M.class_slg.symbols_show(slg, options)
        local pieces = { }
        for symbol_id = 1, slg:highest_symbol_id() do
            pieces[#pieces+1] = slg:symbol_show(symbol_id, options)
        end
        return table.concat(pieces)
    end
    function _M.class_slg.lmg_symbols_show(slg, subg_name, verbose)
        local pieces = { }
        local lmw_g = slg[subg_name]
        verbose = verbose or 0
        for isyid = 0, lmw_g:highest_symbol_id() do
            pieces[#pieces+1] = table.concat (
                { subg_name, 'S' .. isyid, lmw_g:symbol_display_form( isyid ) },
                " ")
            pieces[#pieces+1] = "\n"
            if verbose >= 2 then
                local tags = { ' /*' }
                local nsyid = lmw_g:_isy_nsy(isyid)
                tags[#tags+1] = 'nsyid=' .. inspect(nsyid)
                if lmw_g:symbol_is_productive(isyid) == 0 then
                    tags[#tags+1] = 'unproductive'
                end
                if lmw_g:symbol_is_accessible(isyid) == 0 then
                    tags[#tags+1] = 'inaccessible'
                end
                if lmw_g:symbol_is_nulling(isyid) ~= 0 then
                    tags[#tags+1] = 'nulling'
                end
                if lmw_g:symbol_is_terminal(isyid) ~= 0 then
                    tags[#tags+1] = 'terminal'
                end
                if #tags >= 2 then
                    tags[#tags+1] = '*/'
                    pieces[#pieces+1] = " "
                    pieces[#pieces+1] = table.concat(tags, ' ')
                    pieces[#pieces+1] =  '\n'
                end
                pieces[#pieces+1] =  "  Canonical name: "
                pieces[#pieces+1] =  lmw_g:symbol_diag_form(isyid)
                pieces[#pieces+1] =  "\n"
            end
            if verbose >= 3 then
                local dsl_form =  slg:lmg_symbol_dsl_form( subg_name, isyid )
                if dsl_form then
                    pieces[#pieces+1] =  '  DSL name: '
                    pieces[#pieces+1] =  dsl_form
                    pieces[#pieces+1] =  "\n"
                end
            end
        end
        return table.concat(pieces)
    end
    function _M.class_slg.g1_symbols_show(slg, symbol_id, verbose)
        return slg:lmg_symbols_show('g1', symbol_id, verbose)
    end
    function _M.class_slg.l0_symbols_show(slg, symbol_id, verbose)
        return slg:lmg_symbols_show('l0', symbol_id, verbose)
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slg.lmg_xsyid(slg, subg_name, isy_key)
        local subg = slg[subg_name]
        return subg:xsyid(isy_key)
    end
    function _M.class_slg.g1_xsyid(slg, isy_key)
        return slg:lmg_xsyid('g1', isy_key)
    end
    function _M.class_slg.l0_xsyid(slg, isy_key)
        return slg:lmg_xsyid('l0', isy_key)
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slg.lmg_irl_isyids(slg, subg_name, irlid)
        local subg = slg[subg_name]
        return subg:irl_isyids(irlid)
    end
    function _M.class_slg.g1_irl_isyids(slg, irlid)
        return slg:lmg_irl_isyids('g1', irlid)
    end
    function _M.class_slg.l0_irl_isyids(slg, irlid)
        return slg:lmg_irl_isyids('l0', irlid)
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slg.lmg_rule_show(slg, subg_name, irlid, options)
        options = options or {}
        local name_fn = options.diag and _M.class_grammar.symbol_diag_form
            or _M.class_grammar.symbol_display_form
        local subg = slg[subg_name]
        local irl_isyids = subg:irl_isyids(irlid)
        local pieces = {}
        local symbol_name
            = name_fn(subg, irl_isyids[1])
        pieces[#pieces+1] = symbol_name
        pieces[#pieces+1] = subg_name == 'g1' and '::=' or '~'
        for ix = 2, #irl_isyids do
            symbol_name = name_fn(subg, irl_isyids[ix])
            pieces[#pieces+1] = symbol_name
        end
        local minimum = subg:sequence_min(irlid)
        if minimum then
            pieces[#pieces+1] =
                minimum <= 0 and '*' or '+'
        end
        return table.concat(pieces, ' ')
    end
    function _M.class_slg.g1_rule_show(slg, irlid, options)
        return slg:lmg_rule_show('g1', irlid, options)
    end
    function _M.class_slg.l0_rule_show(slg, irlid, options)
        return slg:lmg_rule_show('l0', irlid, options)
    end

    -- library IF
    function _M.class_slg.lmg_dotted_rule_show(slg, subg_name, irlid, dot_arg)
        local subg = slg[subg_name]
        local irl_isyids = subg:irl_isyids(irlid)
        if not irl_isyids then
             _M.userX(
                 "dotted_rule_show(%s, %s, %s): %s is not a valid irlid",
                 irlid, dot_arg, subg_name, irlid
             )
        end
        local pieces = {}
        pieces[#pieces+1]
            = subg:symbol_display_form(irl_isyids[1])
        pieces[#pieces+1] = '::='
        local dot_used
        local dot = dot_arg == -1 and #irl_isyids + 1 or dot_arg + 2
        for ix = 2, #irl_isyids do
            if dot == ix then
                pieces[#pieces+1] = '.'
                dot_used = true
            end
            pieces[#pieces+1]
                = subg:symbol_display_form(irl_isyids[ix])
        end
        local minimum = subg:sequence_min(irlid)
        if minimum then
            pieces[#pieces+1] =
                minimum <= 0 and '*' or '+'
        end
        if dot == #irl_isyids + 1 then
            pieces[#pieces+1] = '.'
            dot_used = true
        end
        if not dot_used then
             _M.userX(
                 "dotted_rule_show(%s, %s, %s): dot is %s; must be -1, or 0-%d",
                 irlid, dot_arg, subg_name, dot_arg, #irl_isyids + 1
             )
        end
        return table.concat(pieces, ' ')
    end
    -- library IF
    function _M.class_slg.g1_dotted_rule_show(slg, irlid, dot)
        return slg:lmg_dotted_rule_show('g1', irlid, dot)
    end
    -- library IF
    function _M.class_slg.l0_dotted_rule_show(slg, irlid, dot)
        return slg:lmg_dotted_rule_show('l0', irlid, dot)
    end

    function _M.class_slg.lmg_rules_show(slg, subg_name, options)
        local options = options or {}
        local verbose = options.verbose or 0
        local lmw_g = slg[subg_name]
        local pcs = {}
        for irlid = 0, lmw_g:highest_rule_id() do

            local pcs2 = {}
            pcs2[#pcs2+1] = 'R' .. irlid
            pcs2[#pcs2+1] = slg:lmg_rule_show(subg_name, irlid, options)
            pcs[#pcs+1] = table.concat(pcs2, ' ')
            pcs[#pcs+1] = "\n"

            local lhsid
            local rhsids = {}
            local rule_length
            if verbose >= 2 then
                lhsid = lmw_g:rule_lhs(irlid)
                rhsids = {}
                rule_length = lmw_g:rule_length(irlid)
                local comments = {}
                if lmw_g:rule_length(irlid) == 0 then
                    comments[#comments+1] = 'empty'
                end
                if lmw_g:_rule_is_used(irlid) == 0 then
                    comments[#comments+1] = '!used'
                end
                if lmw_g:rule_is_productive(irlid) == 0 then
                    comments[#comments+1] = 'unproductive'
                end
                if lmw_g:rule_is_accessible(irlid) == 0 then
                    comments[#comments+1] = 'inaccessible'
                end
                local irl = slg[subg_name].lmw_g.irls[irlid]
                local xpr = irl.xpr
                if xpr then
                    if xpr.discard_separation then
                        comments[#comments+1] = 'discard_sep'
                    end
                end
                if #comments > 0 then
                    local pcs3 = {}
                    for ix = 1, #comments do
                        pcs3[#pcs3+1] = "/*" .. comments[ix] .. "*/"
                    end
                    pcs[#pcs+1] = table.concat(pcs3, ' ')
                    pcs[#pcs+1] = "\n"
                end
                pcs2 = {}
                pcs2[#pcs2+1] = '  Symbol IDs:'
                for ix = 0, rule_length - 1 do
                   rhsids[ix] = lmw_g:rule_rhs(irlid, ix)
                end
                pcs2[#pcs2+1] = '<' .. lhsid .. '>'
                pcs2[#pcs2+1] = '::='
                for ix = 0, rule_length - 1 do
                    pcs2[#pcs2+1] = '<' .. rhsids[ix] .. '>'
                end
                pcs[#pcs+1] = table.concat(pcs2, ' ')
                pcs[#pcs+1] = "\n"
            end
            if verbose >= 3 then
                local pcs2 = {}
                pcs2[#pcs2+1] = '  Canonical symbols:'
                pcs2[#pcs2+1] = symbol_diag_form(slg:lmg_symbol_name(subg_name, lhsid))
                pcs2[#pcs2+1] = '::='
                for ix = 0, rule_length - 1 do
                    pcs2[#pcs2+1]
                        = symbol_diag_form(slg:lmg_symbol_name(subg_name, rhsids[ix]))
                end
                pcs[#pcs+1] = table.concat(pcs2, ' ')
                pcs[#pcs+1] = "\n"
            end
        end
        return table.concat(pcs)
    end
    function _M.class_slg.g1_rules_show(slg, options)
        return slg:lmg_rules_show('g1', options)
    end
    function _M.class_slg.l0_rules_show(slg, options)
        return slg:lmg_rules_show('l0', options)
    end
```

```
    -- miranda: section+ forward declarations
    local dotted_irl_to_xpr
    -- miranda: section+ most Lua function definitions
    local function dotted_irl_to_xpr(slg, irl_id, irl_dot)
        local xpr_id = slg:g1_rule_to_xprid(irl_id)
        local xpr_dots = slg:g1_rule_to_xpr_dots(irl_id)
        local xpr_dot
        if irl_dot == -1 then
            return xpr_id, xpr_dots[#xpr_dots]
        end
        return xpr_id, xpr_dots[irl_dot+1]
    end
```

### Mutators

```
    -- miranda: section+ forward declarations
    local g1_symbol_assign
    -- miranda: section+ most Lua function definitions
    function g1_symbol_assign(slg, symbol_name, options)
        local isyid = slg:g1_symbol_by_name(symbol_name)
        if isyid then
            -- symbol already exists
            return isyid
        end

        local g1g = slg.g1
        local isy = g1g:symbol_new(symbol_name)
        isyid = isy.id
        g1g.isys[isyid] = isy
        -- Assuming order does not matter
        if not options then options = {} end
        options.wsyid = nil

        local precedence = options.precedence
        if precedence then
            local int_value = math.tointeger(precedence)
            if not int_value then
                error(string.format('Symbol %q": precedence is %s; must be an integer',
                    symbol_name,
                    inspect(value, {depth = 1})
                ))
            end
            precedence = int_value
            isy.xsy_precedence = precedence
            options.precedence = nil
        end
        if precedence and precedence < 0 then
            precedence = nil
        end

        local is_terminal = options.terminal
        if is_terminal then
            g1g:symbol_is_terminal_set(isyid, value)
            options.terminal = nil
        end

        local rank = options.rank
        if rank then
            local int_value = math.tointeger(rank)
            if not int_value then
                error(string.format('Symbol %q": rank is %s; must be an integer',
                    symbol_name,
                    inspect(value, {depth = 1})
                ))
            end
            g1g:symbol_rank_set(isyid, int_value)
            options.rank = nil
        end

        local xsyid = options.xsy
        if xsyid then
            local xsy = slg.xsys[xsyid]
            g1g.xsys[isyid] = xsy
            -- G1 ISY of XSY set only to unprecedenced ISY
            -- print('xsy:', inspect(xsy, {depth=3}))
            if not precedence then
                if xsy.g1_nsyid then
                    print('xsy:', inspect(xsy, {depth=3}))
                    error(string.format('NSY is already set for symbol %q',
                        symbol_name
                    ))
                end
                local nsyid = g1g:_isy_nsy(isyid)
                if not nsyid then
                -- print('setting xsy nsyid:', inspect(nsyid))
                end
                xsy.g1_nsyid = nsyid
            end
            options.xsy = nil
        end

        for property, value in pairs(options) do
            error(string.format('Internal error: Symbol %q has unknown property %s with value %s',
                symbol_name,
                inspect(property),
                inspect(value)
            ))
            -- loop abends on 1st pass; this point is never reached --
        end

        return isyid
    end

```

```
    -- miranda: section+ forward declarations
    local l0_symbol_assign
    -- miranda: section+ most Lua function definitions
    function l0_symbol_assign(slg, symbol_name, options)
        local isyid = slg:l0_symbol_by_name(symbol_name)
        if isyid then
            -- symbol already exists
            return isyid
        end

        local l0g = slg.l0
        local isy = l0g:symbol_new(symbol_name)
        local isyid = isy.id
        l0g.isys[isyid] = isy

        local properties = {}
        -- Assuming order does not matter
        for property, value in pairs(options or {}) do
            if property == 'wsyid' then
                goto NEXT_PROPERTY
            end
            if property == 'xsy' then
                local xsy = slg.xsys[value]
                l0g.xsys[isyid] = xsy
                -- TODO remove this check after development
                if xsy.l0_nsyid then
                    error(string.format('ISY is already set for symbol %q has unknown property %q',
                        symbol_name
                    ))
                end
                local nsyid = l0g:_isy_nsy(isyid)
                xsy.l0_nsyid = nsyid
                goto NEXT_PROPERTY
            end
            if property == 'terminal' then
                g1g:symbol_is_terminal_set(isyid, value)
                goto NEXT_PROPERTY
            end
            if property == 'rank' then
                local int_value = math.tointeger(value)
                if not int_value then
                    error(string.format('Symbol %q": rank is %s; must be an integer',
                        symbol_name,
                        inspect(value, {depth = 1})
                    ))
                end
                l0g:symbol_rank_set(isyid, value)
                goto NEXT_PROPERTY
            end
            if property == 'eager' then
                local int_value = math.tointeger(value)
                if not int_value or (int_value > 1 and int_value < 0) then
                    error(string.format('Symbol %q": eager is %s; must be a boolean',
                        symbol_name,
                        inspect(value, {depth = 1})
                    ))
                end
                if int_value == 1 then
                    l0g.isys[isyid].eager = true
                end
                goto NEXT_PROPERTY
            end
            error(string.format('Internal error: Symbol %q has unknown property %q',
                symbol_name,
                property
            ))
            ::NEXT_PROPERTY::
        end
        return isyid
    end
```

### Hash to runtime processing

The object, in computing the hash, is to get as much
precomputation in as possible, without using undue space.
That means CPU-intensive processing should tend to be done
before or during hash creation, and space-intensive processing
should tend to be done here, in the code that converts the
hash to its runtime equivalent.

Populate the `xsys` table.

```
    -- miranda: section+ forward declarations
    local xsys_populate
    -- miranda: section+ most Lua function definitions
    function xsys_populate(slg, source_hash)
        local xsys = {}
        slg.xsys = xsys

        -- io.stderr:write(inspect(source_hash))
        local xsy_names = {}
        local hash_xsy_data = source_hash.xsy
        for xsy_name, _ in pairs(hash_xsy_data) do
             xsy_names[#xsy_names+1] = xsy_name
        end
        table.sort(xsy_names, _M.memcmp)
        for xsy_id = 1, #xsy_names do
            local xsy_name = xsy_names[xsy_id]

            local runtime_xsy = setmetatable({}, _M.class_xsy)
            local xsy_source = hash_xsy_data[xsy_name]

            runtime_xsy.id = xsy_id
            runtime_xsy.name = xsy_name
            -- copy, so that we can destroy `source_hash`
            runtime_xsy.lexeme_semantics = xsy_source.action
            runtime_xsy.blessing = xsy_source.blessing
            runtime_xsy.new_blessing = xsy_source.new_blessing
            runtime_xsy.dsl_form = xsy_source.dsl_form
            runtime_xsy.if_inaccessible = xsy_source.if_inaccessible
            runtime_xsy.name_source = xsy_source.name_source
            runtime_xsy.is_start = xsy_name == '[:start:]'

            xsys[xsy_name] = runtime_xsy
            xsys[xsy_id] = runtime_xsy
        end
    end
```

Populate the `xrls` table.
The contents of this table are not used,
currently,
but Jeffrey thinks they might be used someday,
for example in error messages.

```
    -- miranda: section+ forward declarations
    local xrls_populate
    -- miranda: section+ most Lua function definitions
    function xrls_populate(slg, source_hash)
        local xrls = {}
        slg.xrls = xrls

        -- io.stderr:write(inspect(source_hash))
        local xrl_names = {}
        local hash_xrl_data = source_hash.xrl
        for xrl_name, _ in pairs(hash_xrl_data) do
             xrl_names[#xrl_names+1] = xrl_name
        end
        table.sort(xrl_names,
           function(a, b)
                if a ~= b then return a < b end
                local start_a = hash_xrl_data[a].start
                local start_b = hash_xrl_data[b].start
                return start_a < start_b
           end
        )
        for xrl_id = 1, #xrl_names do
            local xrl_name = xrl_names[xrl_id]
            local runtime_xrl = setmetatable({}, _M.class_xrl)
            local xrl_source = hash_xrl_data[xrl_name]

            runtime_xrl.id = xrl_id
            runtime_xrl.name = xrl_name
            -- copy, so that we can destroy `source_hash`
            runtime_xrl.precedence_count = xrl_source.precedence_count
            runtime_xrl.lhs = xrl_source.lhs
            runtime_xrl.start = xrl_source.start
            runtime_xrl.length = xrl_source.length

            xrls[xrl_name] = runtime_xrl
            xrls[xrl_id] = runtime_xrl
        end
    end
```

Populate xprs.
"xprs" are eXternal productions.
They are actually not fully external,
but are a first translation of the XRLs into
BNF form --
they are as "external" as possible for pure
BNF.
One symptom of their less-than-fully external
nature is that there are two `xprs` tables,
one for each subgrammar.
(The subgrammars are only visible internally.)

```
    -- miranda: section+ forward declarations
    local xprs_subg_populate
    -- miranda: section+ most Lua function definitions
    function xprs_subg_populate(slg, source_hash, subgrammar)
        local subg = slg[subgrammar]
        local xprs = slg.xprs
        -- io.stderr:write(inspect(source_hash))
        local xpr_names = {}
        local xsys = slg.xsys
        local hash_xpr_data = source_hash.xpr[subgrammar]
        local hash_symbols = source_hash.symbols[subgrammar]
        for xpr_name, _ in pairs(hash_xpr_data) do
             xpr_names[#xpr_names+1] = xpr_name
        end
        table.sort(xpr_names,
           function(a, b)
                local start_a = hash_xpr_data[a].start
                local start_b = hash_xpr_data[b].start
                if start_a ~= start_b then return start_a < start_b end
                local subkey_a = hash_xpr_data[a].subkey
                local subkey_b = hash_xpr_data[b].subkey
                if subkey_a ~= subkey_b then return subkey_a < subkey_b end
                local lhs_a = hash_xpr_data[a].lhs
                local lhs_b = hash_xpr_data[b].lhs
                local cmp = _M.memcmp(lhs_a, lhs_b)
                if cmp ~= nil then return cmp end
                -- rules as of this writing are (I think) unique by start/subkey/LHS
                --    so that the logic from here on is probably not tested

                -- we only want an arbitrary order, and it is convenient to
                -- test on length first
                local rhs_a = hash_xpr_data[a].rhs
                local rhs_b = hash_xpr_data[b].rhs
                if #rhs_a ~= #rhs_b then return #rhs_a < #rhs_b end
                -- we now know that both RHS lengths are the same
                for ix = 1, #rhs_a do
                    local sym_a = rhs_a[ix]
                    local sym_b = rhs_b[ix]
                    local cmp = _M.memcmp(sym_a, sym_b)
                    if cmp ~= nil then return cmp end
                end
                return false
           end
        )
        for ix = 1, #xpr_names do
            local xpr_name = xpr_names[ix]
            local runtime_xpr = setmetatable({}, _M.class_xpr)

            local xpr_source = hash_xpr_data[xpr_name]

            -- copy, so that we can destroy `source_hash`

            runtime_xpr.xrl_name = xpr_source.xrlid
            runtime_xpr.name = xpr_source.name
            runtime_xpr.subgrammar = xpr_source.subgrammar
            runtime_xpr.lhs
                = xsys[xpr_source.lhs] or
                    xsys[hash_symbols[xpr_source.lhs].xsy]

            -- TODO delete after development
            if not runtime_xpr.lhs then
                print('missing xpr_source.lhs: %s', inspect( hash_symbols[xpr_source.lhs]))
                print('missing xpr_source.lhs: %s', inspect( subg:_xsy(xpr_source.lhs)))
                _M._internal_error('missing xpr_source.lhs: %s', inspect(xpr_source))
            end

            local to_rhs = {}
            local from_rhs = xpr_source.rhs
            for ix = 1, #from_rhs do
                local rh_sym = xpr_source.rhs[ix]
                to_rhs[ix] = xsys[rh_sym] or
                    xsys[hash_symbols[rh_sym].xsy]
            end
            runtime_xpr.rhs = to_rhs
            runtime_xpr.rank = xpr_source.rank
            runtime_xpr.precedence = xpr_source.precedence
            runtime_xpr.null_ranking = xpr_source.null_ranking

            runtime_xpr.symbol_as_event = xpr_source.symbol_as_event
            local source_event = xpr_source.event
            if source_event then
                runtime_xpr.event_name = source_event[1]
                -- TODO revisit type (boolean? string? integer?)
                --   once conversion to Lua is complete
                runtime_xpr.event_starts_active
                    = (math.tointeger(source_event[2]) ~= 0)
            end

            if xpr_source.min then
                runtime_xpr.min = math.tointeger(xpr_source.min)
            end
            runtime_xpr.separator = xpr_source.separator
            runtime_xpr.proper = xpr_source.proper
            runtime_xpr.bless = xpr_source.bless
            runtime_xpr.new_blessing = xpr_source.new_blessing
            runtime_xpr.action = xpr_source.action
            runtime_xpr.start = xpr_source.start
            runtime_xpr.length = xpr_source.length

            runtime_xpr.discard_separation =
                xpr_source.separator and
                    not xpr_source.keep

            local rhs_length = #xpr_source.rhs

            -- min defined if sequence rule
            if not xpr_source.min or rhs_length == 0 then
                if xpr_source.mask then
                    runtime_xpr.mask = xpr_source.mask
                else
                    local mask = {}
                    for i = 1, rhs_length do
                        mask[i] = 1
                    end
                    runtime_xpr.mask = mask
                end
            end

            local next_xpr_id = #xprs + 1
            runtime_xpr.id = next_xpr_id
            xprs[xpr_name] = runtime_xpr
            xprs[next_xpr_id] = runtime_xpr

            -- print('runtime_xpr:', inspect(runtime_xpr))
        end
    end
    -- miranda: section+ forward declarations
    local xprs_populate
    -- miranda: section+ most Lua function definitions
    function xprs_populate(slg, source_hash)
        slg.xprs = {}
        xprs_subg_populate(slg, source_hash, 'g1')
        return xprs_subg_populate(slg, source_hash, 'l0')
    end
```

#### Add a G1 rule

```
    -- miranda: section+ forward declarations
    local g1_rule_add
    -- miranda: section+ most Lua function definitions
    function g1_rule_add(slg, options)
        local g1g = slg.g1
        local allowed = {
            action = true,
            lhs = true,
            rhs = true,
            rank = true,
            null_ranking = true,
            precedence = true,
            min = true,
            separator = true,
            proper = true,
            subgrammar = true,
            xprid = true,
            xpr_dot = true,
            xpr_top = true
        }
        for key, _ in pairs(options) do
           if not allowed[key] then
               _M._internal_error("Unknown user rule option: %q",
                   key)
           end
        end
        local rank = options.rank or g1g:default_rank()
        local xpr_id = slg.xprs[options.xprid].id
        local rhs_names = options.rhs or {}

        local function rule_error()
            local error_code = g1g:error_code()
            local rule_description =_M._raw_rule_show(options.lhs, rhs_names)
            local problem_description
            if error_code == _M.err.DUPLICATE_RULE then
                problem_description = "Duplicate rule"
            else
                problem_description = _M.err[error_code].description
            end
            _M.userX("Error in rule: %s; rule was\n\z
            \u{20}   %s",
            problem_description, rule_description)
        end

        local is_ordinary = #rhs_names == 0 or not options.min
        local separator = options.separator
        local base_irl_id
        local rule_symids = { g1_symbol_assign(slg, options.lhs) }
        for ix = 1, #rhs_names do
            rule_symids[ix+1] = g1_symbol_assign(slg, rhs_names[ix])
        end
        if is_ordinary then
            if separator then
                _M._internal_error(
                    "Separator defined for rule without repetitions:\n    %s",
                    _M._raw_rule_show(options.lhs, rhs_names))
            end
            _M.throw = false
            base_irl_id = g1g:rule_new(rule_symids)
            _M.throw = true
            if not base_irl_id or base_irl_id < 0 then rule_error() end
            g1g.irls[base_irl_id] = { id = base_irl_id }
        else
            if #rhs_names ~= 1 then
                _M._internal_error(
                    "Rule has %d symbols on RHS\n\z
                    \u{20}   Only one RHS symbol is allowed for a rule counted rule:\n\z
                    \u{20}   %s",
                    #rhs_names,
                    _M._raw_rule_show(options.lhs, rhs_names))
            end
            local separator_id
                = separator and g1_symbol_assign(slg, separator)
                  or -1
            local proper = (options.proper and options.proper ~= 0)
            _M.throw = false
            base_irl_id = g1g:sequence_new{
                lhs = rule_symids[1],
                rhs = rule_symids[2],
                separator = separator_id,
                proper = proper,
                min = math.tointeger(options.min)
            }
            _M.throw = true
            -- remove the test for nil or less than zero
            -- once refactoring is complete?
            if not base_irl_id or base_irl_id < 0 then rule_error() end

            local g1_rule = setmetatable({}, _M.class_irl)
            g1_rule.id = base_irl_id
            g1g.irls[base_irl_id] = g1_rule
        end
        local ranking_is_high = options.null_ranking == 'high' and 1 or 0
        g1g:rule_null_high_set(base_irl_id, ranking_is_high)
        g1g:rule_rank_set(base_irl_id, rank)

        local xpr = slg.xprs[xpr_id]
        local irl = slg.g1.irls[base_irl_id]
        irl.xpr = xpr

        -- TODO: Normalization to boolean may not be needed after
        --       conversion to Perl
        irl.xpr_top = options.xpr_top and true or nil

        -- TODO: Remove math.tointeger() conversion after conversion from Perl
        irl.xpr_dot = {}
        for ix = 1, #options.xpr_dot do
           irl.xpr_dot[ix] = math.tointeger(options.xpr_dot[ix])
        end

        -- right now, the action & mask of an irl
        -- is always the action/mask of its xpr.
        -- But some day each irl may need its own.
        irl.action = xpr.action
        irl.mask = xpr.mask
    end
```

## Lexeme class

```
    -- miranda: section+ class_lexeme field declarations
    class_lexeme_fields.g1_isy = true
    class_lexeme_fields.xsy = true
    class_lexeme_fields.l0_isy = true
    class_lexeme_fields.l0_irl = true
```

```
    -- miranda: section+ create nonmetallic metatables
    _M.class_lexeme = {}
    -- miranda: section+ populate metatables
    local class_lexeme_fields = {}
    -- miranda: insert class_lexeme field declarations
    declarations(_M.class_lexeme, class_lexeme_fields, 'lexeme')
    do
        local old_new_index = _M.class_lexeme.__newindex
        -- allow integer keys
        _M.class_lexeme.__newindex = function (t, n, v)
            if type(n) == 'number' then return rawset(t, n, v) end
            return old_new_index(t, n, v)
            end
    end
```

## Block class

### SLR fields

```
    -- miranda: section+ class_blk field declarations
    class_blk_fields.text = true
    class_blk_fields.index = true
    class_blk_fields.offset = true
    class_blk_fields.eoread = true
```

```
    -- miranda: section+ create nonmetallic metatables
    _M.class_blk = {}
    -- miranda: section+ populate metatables
    local class_blk_fields = {}
    -- miranda: insert class_blk field declarations
    declarations(_M.class_blk, class_blk_fields, 'blk')
    do
        local old_new_index = _M.class_blk.__newindex
        -- allow integer keys
        _M.class_blk.__newindex = function (t, n, v)
            if type(n) == 'number' then return rawset(t, n, v) end
            return old_new_index(t, n, v)
            end
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.literal(slr, block_ix, l0_start, l0_length)
        if not block_ix then block_ix = slr.current_block.index end
        local block = slr.inputs[block_ix]
        local start_byte_p = slr:per_pos(block_ix, l0_start)
        local end_byte_p = slr:per_pos(block_ix, l0_start + l0_length)
        local text = block.text
        return text:sub(start_byte_p, end_byte_p - 1)
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.g1_literal(slr, g1_start, g1_count)
        -- io.stderr:write(string.format("g1_literal(%d, %d)\n",
            -- g1_start, g1_count
        -- ))

        if g1_count <= 0 then return '' end
        local pieces = {}
        local inputs = slr.inputs
        for block_ix, start, len in
            sweep_range(slr, g1_start, g1_start+g1_count-1)
        do
            local start_byte_p = slr:per_pos(block_ix, start)
            local end_byte_p = slr:per_pos(block_ix, start + len)
            local block = inputs[block_ix]
            local text = block.text
            local piece = text:sub(start_byte_p, end_byte_p - 1)
            pieces[#pieces+1] = piece
        end
        return table.concat(pieces)
    end

    function _M.class_slr.g1_span_l0_length(slr, g1_start, g1_count)
        if g1_count == 0 then return 0 end
        local inputs = slr.inputs
        local length = 0;
        for _, _, sweep_length in
            sweep_range(slr, g1_start, g1_start+g1_count-1)
        do
            length = length + sweep_length
        end
        return length
    end
```

The current position in L0 terms -- a kind of "end of parse" location.

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.l0_current_pos(slr)
        local per_es = slr.per_es
        local this_sweep = per_es[#per_es]
        local end_block = this_sweep[#this_sweep-2]
        local end_pos = this_sweep[#this_sweep-1]
        local end_len = this_sweep[#this_sweep]
        return end_block, end_pos + end_len
    end
```

Takes a g1 position,
call it `g1_pos`,
and returns
L0 position where the g1 position starts
as a `block, pos` duple.
As a special case,
when `g1_pos` is one after the last actual
g1 position,
treats it as
an "end of parse" location,
and returns the current L0 position.

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.g1_to_block_first(slr, g1_pos)
        local per_es = slr.per_es
        if g1_pos == #per_es then
            return slr:l0_current_pos()
        end
        local this_sweep = per_es[g1_pos+1]
        if not this_sweep then
            error(string.format(
                "slr:g1_to_block_first(%d): bad argument\n\z
                \u{20}   Allowed values are %d-%d\n",
                g1_pos, 0, #per_es-1
            ))
        end
        local start_block = this_sweep[1]
        local start_pos = this_sweep[2]
        return start_block, start_pos
    end
```

Takes a g1 position and returns
the last actual L0 position of the g1 position
as a `block, pos` duple.
The position is "actual" in the sense that
there is actually a codepoint at that position.
Allows for the possible future, where the per-es sweep
contains more than one L0 span.

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.g1_to_block_last(slr, g1_pos)
        local per_es = slr.per_es
        local this_sweep = per_es[g1_pos+1]
        if not this_sweep then
            error(string.format(
                "slr:g1_to_block_last(%d): bad argument\n\z
                \u{20}   Allowed values are %d-%d\n",
                g1_pos, 0, #per_es-1
            ))
        end
        local end_block = this_sweep[#this_sweep-2]
        local end_pos = this_sweep[#this_sweep-1]
        local end_len = this_sweep[#this_sweep]
        return end_block, end_pos + end_len - 1
    end
```

### Brief description of block/line/column

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.lc_brief(slr, block, offset)
        if not block then block = slr.current_block.index end
        local _, line_no, column_no = slr:per_pos(block, offset)
        return string.format("B%dL%dc%d",
            block, line_no, column_no)
    end
}
```

### Brief description of block/line/column for an L0 range

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.lc_range_brief(slr, block1, offset1, block2, offset2)
        local _, line1, column1 = slr:per_pos(block1, offset1)
        local _, line2, column2 = slr:per_pos(block2, offset2)
        if block1 ~= block2 then
           return string.format("B%dL%dc%d-B%dL%dc%d",
             block1, line1, column1, block2, line2, column2)
        end
        if line1 ~= line2 then
           return string.format("B%dL%dc%d-L%dc%d",
             block1, line1, column1, line2, column2)
        end
        if column1 ~= column2 then
           return string.format("B%dL%dc%d-%d",
             block1, line1, column1, column2)
        end
       return string.format("B%dL%dc%d",
             block1, line1, column1)
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.lc_table_brief(slr, locations)
        table.sort(locations, _M.cmp_seq)
        local block1, offset1 = table.unpack(locations[1])
        local block2, offset2 = table.unpack(locations[#locations])
        return slr:lc_range_brief(block1, offset1, block2, offset2)
    end
```

`block_new` must be called in a coroutine which handles
the `codepoint` command.

```
    -- miranda: section+ most Lua function definitions
    local eols = {
        [0x0A] = 0x0A,
        [0x0D] = 0x0D,
        [0x0085] = 0x0085,
        [0x000B] = 0x000B,
        [0x000C] = 0x000C,
        [0x2028] = 0x2028,
        [0x2029] = 0x2029
    }

    function _M.class_slr.block_new(slr, input_string)
        local trace_terminals = slr.trace_terminals
        local inputs = slr.inputs
        local new_block = setmetatable({}, _M.class_blk)
        local this_index = #inputs + 1
        inputs[this_index] = new_block
        new_block.text = input_string
        new_block.index = #inputs
        local ix = 1

        local eol_seen = false
        local line_no = 1
        local column_no = 0
        local per_codepoint = slr.slg.per_codepoint
        for byte_p, codepoint in utf8.codes(input_string) do

            if not per_codepoint[codepoint] then
               local new_codepoint = {}
               per_codepoint[codepoint] = new_codepoint
               local codepoint_data = coroutine.yield('codepoint', codepoint, trace_terminals)
               -- print('coro_ret: ', inspect(codepoint_data) )
               if codepoint_data.is_graphic == 'true' then
                   new_codepoint.is_graphic = true
               end
               local symbols = codepoint_data.symbols or {}
               for ix = 1, #symbols do
                   new_codepoint[ix] = math.tointeger(symbols[ix])
               end
               -- print('new_codepoint:', inspect(new_codepoint))
            end

            -- line numbering logic
            if eol_seen and
               (eol_seen ~= 0x0D or codepoint ~= 0x0A) then
               eol_seen = false
               line_no = line_no + 1
               column_no = 0
            end
            column_no = column_no + 1
            eol_seen = eols[codepoint]

            local vlq = _M.to_vlq({ byte_p, line_no, column_no })
            new_block[#new_block+1] = vlq
        end
        new_block.offset = 0
        new_block.eoread = #new_block
        return this_index
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.block_progress(slr, block_ix)
        local block
        if block_ix then
            block = slr.inputs[block_ix]
            if not block then return end
        else
            block = slr.current_block
        end
        if not block then return 0, 0, 0 end
        return block.index, block.offset,
            block.eoread
    end
    function _M.class_slr.block_set(slr, block_ix)
        local block = slr.inputs[block_ix]
        slr.current_block = block
    end
    function _M.class_slr.block_move(slr, offset, eoread)
        local block = slr.current_block
        if offset then block.offset = offset end
        if eoread then block.eoread = eoread end
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.block_read(slr)
        local events = {}
        local event_status
        while true do
            local alive = slr:read()
            event_status, events = slr:convert_libmarpa_events()
            if not alive or #events > 0 or event_status == 'pause' then
                break
            end
        end
        local _, offset = slr:block_progress()
        return offset
    end
```

Returns byte position, line and column of `pos`
in block with index `block_ix`.
Caller must ensure `block` and `pos` are valid.

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.per_pos(slr, block_ix, pos)

        -- TODO
        if not pos then error(debug.traceback()) end

        local block = slr.inputs[block_ix]
        -- codepoints array is 1-based
        local codepoint_ix = pos+1
        local text = block.text
        if codepoint_ix > #block then
            -- It is useful to have an "end of block"
            -- position.  No codepoint, but line is
            -- last line and byte_p and column are one
            -- after the end
            if codepoint_ix == #block + 1 then
                local vlq = block[#block]
                local last_byte_p, last_line_no, last_column_no
                    = table.unpack(_M.from_vlq(vlq))
                return #text+1, last_line_no, last_column_no+1
            end
            error(string.format(
                "Internal error: invalid block,pos: %d, %d\n\z
                \u{20}   pos must be from 0-%d\n",
                block_ix, pos, #block))
        end
        local vlq = block[codepoint_ix]
        -- print(inspect(_M.from_vlq(vlq)))
        return table.unpack(_M.from_vlq(vlq))
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.codepoint_from_pos(slr, block, pos)
        local byte_p = slr:per_pos(block, pos)
        local input = slr.inputs[block]
        local text = input.text
        if byte_p > #text then return end
        return utf8.codepoint(text, byte_p)
    end

```

### Block checking methods

These methods are for checking arguments of block
and block-related methods.

```
    -- miranda: section+ most Lua function definitions
    -- returns: current block, if block_id_arg is nil,
    --    block_id_arg, if block_id_arg is non-nil and valid
    --    nil, message if block_id_arg is non-nil and invalid
    function _M.class_slr.block_check_id(slr, block_id_arg)
        local block_id, l0_pos, end_pos = slr:block_progress(block_id_arg)
        if not block_id then
            return nil, string.format('Bad block index ' .. block_id_arg)
        end
        return block_id
    end

    -- assumes block_id is valid or nil
    -- returns:
    --    current block offset of current block if block_offset_arg == nil
    --    block_offset_arg as an integer if block_offset_arg is non-nil
    --        and it converts to a valid integer
    --    nil, error-message otherwise
    -- note: negative block_offset_arg is converted as offset
    -- from physical end-of-block
    function _M.class_slr.block_check_offset_i(slr, block_id, block_offset_arg)
        local block_id, offset, end_pos = slr:block_progress(block_id)
        local block = slr.inputs[block_id]
        local block_length = #block
        if not block_offset_arg then return offset end
        local new_block_offset = math.tointeger(block_offset_arg)
        if not new_block_offset then
            return nil, string.format('Bad current position argument %s', block_offset_arg)
        end
        if new_block_offset < 0 then
            new_block_offset = block_length + new_block_offset + 1
        end
        if new_block_offset < 0 then
            return nil, string.format('Current position is before start of block: %s', block_offset_arg)
        end
        if new_block_offset > block_length then
            return nil, string.format('Current position is after end of block: %s', block_offset_arg)
        end
        return new_block_offset
    end

    -- assumes valid block_id, block_offset
    -- returns:
    --     current eoread, if length_arg == nil
    --     end-of-read, based on length_arg, if length_arg valid, non-nil, < #block
    --     eoblock, if eoread based on length_arg > eoblock
    --     nil, error-message, otherwise
    -- Note: negative block_offset is converted as offset
    --     from physical end-of-block
    -- Note: uses the `block_offset` in its arguments, *not* the one actually
    --     in the block
    function _M.class_slr.block_check_length(slr, block_id, block_offset, length_arg)
        local block = slr.inputs[block_id]
        local block_length = #block

        if not length_arg then
            local _, _, eoread = slr:block_progress(block_id)
            return eoread
        end
        local longueur = math.tointeger(length_arg)
        if not longueur then
            return nil, string.format('Bad length argument %s', length_arg)
        end
        local eoread = longueur >= 0 and block_offset + longueur or
            block_length + longueur + 1
        if eoread < 0 then
            return nil, string.format('Last position is before start of block: %s', length_arg)
        end
        if eoread > block_length then return block_length end
        return eoread
    end

    function _M.class_slr.block_check_offset(slr, block_id_arg, block_offset_arg)
        local block_id, erreur
            = _M.class_slr.block_check_id(slr, block_id_arg, block_offset_arg)
        if not block_id then return nil, erreur end
        local new_block_offset, erreur
            = _M.class_slr.block_check_offset_i(slr, block_id, block_offset_arg)
        if not new_block_offset then return nil, erreur end
        return block_id, new_block_offset
    end

    -- assumes nothing about arguments
    -- block_id specifies current block if block_id_arg == nil
    -- block_offset specifies current offset in specified block
    --     if block_offset_arg == nil
    -- eoread specifies end of specified block if length_arg == nil
    -- returns block_id, block_offset, eoread on success
    -- returns nil, error-message otherwise
    function _M.class_slr.block_check_range(slr, block_id_arg, block_offset_arg, length_arg)
        local block_id, block_offset
            = _M.class_slr.block_check_offset(slr, block_id_arg, block_offset_arg)
        -- block offset is error when block_id == nil
        if not block_id then return nil, block_offset end
        local eoread, erreur
            = _M.class_slr.block_check_length(slr, block_id, block_offset, length_arg)
        if not eoread then return nil, erreur end
        return block_id, block_offset, eoread
    end

```

## SLIF recognizer (SLR) class

This is a registry object.

### SLR fields

```
    -- miranda: section+ class_slr field declarations
    class_slr_fields.accept_queue = true
    class_slr_fields.codepoint = true
    class_slr_fields.current_block = true
    class_slr_fields.end_of_lexeme = true
    class_slr_fields.event_queue = true
    class_slr_fields.g1 = true
    class_slr_fields.inputs = true
    class_slr_fields.is_lo_level_scanning = true
    class_slr_fields.l0 = true
    class_slr_fields.l0_candidate = true
    class_slr_fields.g1_isys = true
    class_slr_fields.l0_irls = true
    class_slr_fields.irls = true
    class_slr_fields.lexeme_queue = true
    class_slr_fields.regix = true
    class_slr_fields.slg = true
    class_slr_fields.start_of_lexeme = true
    class_slr_fields.terminals_expected = true
    class_slr_fields.too_many_earley_items = true
    class_slr_fields.token_is_literal = true
    class_slr_fields.token_is_undef = true
    class_slr_fields.token_values = true
    class_slr_fields.trace_terminals = true
    class_slr_fields.trace_values = true
    class_slr_fields.trailers = true
    -- TODO delete after development
    class_slr_fields.has_event_handlers = true
```

*Per earley set* field:
A 1-based sequence of data for each earley set.
G1 positions are 0-based, and therefore
the sequence index of `per_es` is offset by 1.
That is, the data for G1 position 0 is in
`per_es[1]`

Each element of the sequence is the triple
`(block_ix, start, length)`
in the form of a Lua sequence.
`block_ix` is the index of the block,
`start` is the start of the lexeme literal
and `length` is the length of the lexeme literal.

A zero length is acceptable.
TODO: Zero length is acceptable but only lightly
tested so far.

```
    -- miranda: section+ class_slr field declarations
    class_slr_fields.per_es = true
```

*At end of input* field:
`true` when at "end of input",
nil otherwise.
It is "end of input" because
a Kollos input can traverse multiple
files.
This is a common requirement:
It is necessary, for example,
for parsing C language.

```
    -- miranda: section+ class_slr field declarations
    class_slr_fields.at_eoi = true
```

*Block mode*:
`true` in block mode,
`nil` otherwise.
Kollos read method and its block-by-block
methods are not compatible.
This boolean keeps them from being used
together.

```
    -- miranda: section+ class_slr field declarations
    class_slr_fields.block_mode = true
```

*Empty lexeme block*

```
    -- miranda: section+ class_slr field declarations
    class_slr_fields.no_literal_block = true
```

```
    -- miranda: section+ populate metatables
    local class_slr_fields = {}
    -- miranda: insert class_slr field declarations
    declarations(_M.class_slr, class_slr_fields, 'slr')
```

```
    -- miranda: section+ luaL_Reg definitions
    static const struct luaL_Reg slr_methods[] = {
      { NULL, NULL },
    };

```

### SLR constructors

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slg.slr_new(slg, flat_args)
        local slr = {}
        setmetatable(slr, _M.class_slr)
        slr.slg = slg
        slr.regix = _M.register(_M.registry, slr)

        local l0g = slg.l0
        slr.l0 = {}
        slr.l0_irls = {}

        local g1g = slg.g1
        slr.g1 = _M.recce_new(g1g)
        local g1r = slr.g1

        -- TODO Census, eliminate most (all?) references via lmw_g
        slr.g1_isys = {}

        slr.codepoint = nil
        slr.inputs = {}

        slr.per_es = {}
        slr.current_block = nil
        slr.no_literal_block = nil

        -- Trailing (that is, discarded) sweeps by
        -- G0 Earley set index.  Integer indices, but not
        -- necessarily a sequence.
        slr.trailers = {}

        slr.event_queue = {}

        slr.lexeme_queue = {}
        slr.accept_queue = {}

        slr.too_many_earley_items = -1
        slr.trace_terminals = 0
        slr.start_of_lexeme = 0
        slr.end_of_lexeme = 0
        slr.is_lo_level_scanning = false

        local g_l0_rules = slg.l0.irls
        local r_l0_rules = slr.l0_irls
        local max_l0_rule_id = l0g:highest_rule_id()
        for rule_id = 0, max_l0_rule_id do
            local r_l0_rule = {}
            local g_l0_rule = g_l0_rules[rule_id]
            if g_l0_rule then
                for field, value in pairs(g_l0_rule) do
                    r_l0_rule[field] = value
                end
            end
            r_l0_rules[rule_id] = r_l0_rule
        end
        local g_g1_symbols = slg.g1.isys
        local r_g1_symbols = slr.g1_isys
        local max_g1_symbol_id = g1g:highest_symbol_id()
        for symbol_id = 0, max_g1_symbol_id do
            r_g1_symbols[symbol_id] = {
                lexeme_priority =
                    g_g1_symbols[symbol_id].priority,
                pause_before_active =
                    g_g1_symbols[symbol_id].pause_before_active,
                pause_after_active =
                    g_g1_symbols[symbol_id].pause_after_active
            }
        end

        slr:common_set(flat_args, {'event_is_active',
            -- TODO delete after development
            'event_handlers'
        })
        local trace_terminals = slr.trace_terminals

        local start_input_return = g1r:start_input()
        if start_input_return == -1 then
            error( string.format('Recognizer start of input failed: %s',
                g1g.error_description()))
        end
        if start_input_return < 0 then
            error( string.format('Problem in start_input(): %s',
                g1g.error_description()))
        end
        slr:g1_convert_events()

        if trace_terminals > 1 then
            local terminals_expected = slr.g1:terminals_expected()
            table.sort(terminals_expected)
            for ix = 1, #terminals_expected do
                local terminal = terminals_expected[ix]
                coroutine.yield('trace',
                    string.format('Expecting %q at earleme 0',
                    slg:g1_symbol_name(terminal)))
            end
        end
        slr.token_values = {}
        slr.token_is_undef = 1
        slr.token_values[slr.token_is_undef] = '[undef]'

        -- token is literal is a pseudo-index, and the SV undef
        -- is just a place holder
        slr.token_is_literal = 2
        slr.token_values[slr.token_is_literal] = '[literal]'

        return slr
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.l0r_new(slr)
        local l0r = _M.recce_new(slr.slg.l0)

        local block_ix, offset = slr:block_progress()
        local g1g = slr.slg.g1

        if not l0r then
            error('Internal error: l0r_new() failed %s',
                slr.slg.l0:error_description())
        end
        slr.l0 = l0r
        -- reset the candidate in the lexer
        slr.l0_candidate = nil
        local too_many_earley_items = slr.too_many_earley_items
        if too_many_earley_items >= 0 then
            l0r:earley_item_warning_threshold_set(too_many_earley_items)
        end

         -- TODO: for now use a per-slr field
         -- later replace with a local
        slr.terminals_expected = slr.g1:terminals_expected()

        local count = #slr.terminals_expected
        if not count or count < 0 then
            local error_description = slr.g1:error_description()
            error('Internal error: terminals_expected() failed in u_l0r_new(); %s',
                    error_description)
        end

        for i = 0, count -1 do
            local ix = i + 1
            local terminal = slr.terminals_expected[ix]
            local assertion = slr.slg.g1.isys[terminal].assertion
            assertion = assertion or -1
            if assertion >= 0 then
                local result = l0r:zwa_default_set(assertion, 1)
                if result < 0 then
                    local error_description = l0r:error_description()
                    error('Problem in u_l0r_new() with assertion ID %ld and lexeme ID %ld: %s',
                        assertion, terminal, error_description
                    )
                end
            end

            if slr.trace_terminals >= 3 then
                local xsy = g1g:_xsy(terminal)
                if xsy then
                    local display_form = xsy:display_form()
                    coroutine.yield('trace', string.format(
                        "Expected lexeme %s at %s; assertion ID = %d",
                        display_form,
                        slr:lc_brief(nil, offset),
                        assertion
                    ))
                end
            end

        end
        local result = l0r:start_input()
        if result and result <= -2 then
            local error_description = l0r:error_description()
            error('Internal error: problem with slr:start_input(l0r): %s',
                error_description)
        end
    end

```

A common processor for
the recognizer's Lua-level settings.

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.common_set(slr, flat_args, extra_args)
        local ok_args = {
            trace_terminals = true,
            too_many_earley_items = true,
            trace_values = true
        }
        if extra_args then
            for ix = 1, #extra_args do
                ok_args[extra_args[ix]] = true
            end
        end

        for name, value in pairs(flat_args) do
           if not ok_args[name] then
               error(string.format(
                   'Bad slr named argument: %s => %s',
                   inspect(name),
                   inspect(value)
               ))
           end
        end

        local raw_value

        -- TODO delete after development
        raw_value = flat_args.event_handlers
        if raw_value then
            slr.has_event_handlers = true
        end

        -- trace_terminals named argument --
        raw_value = flat_args.trace_terminals
        if raw_value then
            local value = math.tointeger(raw_value)
            if not value then
               error(string.format(
                   'Bad value for "trace_terminals" named argument: %s',
                   inspect(raw_value)))
            end
            if value ~= 0 then
                coroutine.yield('trace', 'Setting trace_terminals option')
            end
            slr.trace_terminals = value
        end

        -- trace_values named argument --
        raw_value = flat_args.trace_values
        if raw_value then
            local value = math.tointeger(raw_value)
            if not value then
               error(string.format(
                   'Bad value for "trace_values" named argument: %s',
                   inspect(raw_value)))
            end
            if value ~= 0 then
                coroutine.yield('trace', 'Setting trace_values option to ' .. value)
            end
            slr.trace_values = value
        end

        -- too_many_earley_items named argument --
        raw_value = flat_args.too_many_earley_items
        if raw_value then
            local value = math.tointeger(raw_value)
            if not value then
               error(string.format(
                   'Bad value for "too_many_earley_items" named argument: %s',
                   inspect(raw_value)))
            end
            slr.too_many_earley_items = value
            slr.g1:earley_item_warning_threshold_set(value)
        end

        -- 'event_is_active' named argument --
        -- Completion/nulled/prediction events are always initialized by
        -- Libmarpa to 'on'.  So here we need to override that if and only
        -- if we in fact want to initialize them to 'off'.
        --
        -- Events are already initialized as described by
        -- the DSL.  Here we override that with the recce arg, if
        -- necessary.
        raw_value = flat_args.event_is_active
        if raw_value then
            if type(raw_value) ~= 'table' then
               error(string.format(
                   'Bad value for "event_is_active" named argument: %s',
                   inspect(raw_value,{depth=1})))
            end
            for event_name, raw_activate in pairs(raw_value) do
                local activate = math.tointeger(raw_activate)
                if not activate or activate > 1 or activate < 0 then
                   error(string.format(
                       'Bad activation value for %q event in\z
                       "event_is_active" named argument: %s',
                       inspect(raw_activate,{depth=1})))
                end
                activate = activate == 1 and true or false
                slr:activate_by_event_name(event_name, activate)
            end
        end
    end
```

### Internal reading

The top-level read function.
Returns `true` if the read is alive (this is,
if there is some way to continue it),
`false` otherwise.

```
    -- miranda: section+ forward declarations
    local error_lo_hi_scanning
    -- miranda: section+ most Lua function definitions
    function error_lo_hi_scanning(function_name)
        return _M.userX(
           'unpermitted mix of external and internal scanning\n\z
           \u{20}   "%s" called while doing low level scanning\n\z
           \u{20}   Did you want to call lexeme_complete first?\n',
        function_name)
    end
    function _M.class_slr.read(slr)
        if slr.is_lo_level_scanning then
           return error_lo_hi_scanning("slr.read()")
        end
        if slr.block_mode then
           _M.userX( 'unpermitted use of slr.read() in block mode' )
        end
        slr.event_queue = {}
        while true do
            local _, offset, eoread = slr:block_progress()
            if offset >= eoread then
                -- a 'normal' return
                return false
            end
            if offset >= 0 then
                slr.start_of_lexeme = offset
                slr.l0 = nil
                if slr.trace_terminals >= 1 then
                    coroutine.yield('trace', string.format(
                        'Restarted recognizer at %s',
                        slr:lc_brief(nil, offset)
                    ))
                end
            end
            local g1r = slr.g1

            slr:l0_read_lexeme()

            local discard_mode = (g1r:is_exhausted() ~= 0)
            -- TODO: work on this
            local alive = slr:alternatives(discard_mode)
            if not alive then return false end
            local event_count = #slr.event_queue
            if event_count >= 1 then return true end
        end
        error('Internal error: unexcepted end of read loop')
    end
```

`l0_earleme_complete()`
"completes" an earleme in L0.
Returns `true` if the parser is "alive",
that is, not exhausted.
Otherwise returns `false` and a status string.

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.l0_earleme_complete(slr)
        local l0r = slr.l0
        local complete_result = l0r:earleme_complete()
        if complete_result == -2 then
            if l0r:error_code() == _M.err.PARSE_EXHAUSTED then
                return false, 'exhausted on failure'
            end
        end
        if complete_result < 0 then
            error('Problem in r->l0_read(), earleme_complete() failed: ',
            l0r:error_description())
        end
        if complete_result > 0 then
            slr:l0_convert_events()
            local is_exhausted = l0r:is_exhausted()
            if is_exhausted ~= 0 then
                return false, 'exhausted on success'
            end
        end
        return true
    end

```

`l0_alternative()`
reads an alternative.
Returns the number of alternatives accepted,
which will be 1 or 0.

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.l0_alternative(slr, symbol_id)
        local l0r = slr.l0
        local l0g = slr.slg.l0
        local codepoint = slr.codepoint
        local result = l0r:alternative(symbol_id, 1, 1)
        local _, offset = slr:block_progress()
        if result == _M.err.UNEXPECTED_TOKEN_ID then
            if slr.trace_terminals >= 1 then
                coroutine.yield('trace', string.format(
                    'Codepoint %q 0x%04x rejected as %s at %s',
                    utf8.char(codepoint),
                    codepoint,
                    l0g:symbol_display_form(symbol_id),
                    slr:lc_brief(nil, offset)
                ))
            end
            return 0
        end
        if result == _M.err.NONE then
            if slr.trace_terminals >= 1 then
                coroutine.yield('trace', string.format(
                    'Codepoint %q 0x%04x accepted as %s at %s',
                    utf8.char(codepoint),
                    codepoint,
                    l0g:symbol_display_form(symbol_id),
                    slr:lc_brief(nil, offset)
                ))
            end
            return 1
        end
        error(string.format([[
             Problem alternative() failed at char ix %d; symbol id %d; codepoint 0x%x
             Problem in l0_read(), alternative() failed: %s
        ]],
            offset, symbol_id, codepoint, l0r:error_description()
        ))
    end


```

`l0_read_codepoint()`
reads the current codepoint in L0.
Returns `true` if the parser is "alive"
(not exhausted).
Otherwise returns `false` and a status string.

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.l0_read_codepoint(slr)
        local codepoint = slr.codepoint

        local ops = slr.slg.per_codepoint[codepoint]
        local op_count = ops and #ops or -1

        if op_count <= 0 then
            _M.userX( 'Character in input is not in alphabet of grammar: %s',
              slr:character_describe(codepoint))
        end

        if slr.trace_terminals >= 1 then
           local _, offset = slr:block_progress()
           coroutine.yield('trace', string.format(
               'Reading codepoint %q 0x%04x at %s',
               utf8.char(codepoint),
               codepoint,
               slr:lc_brief(nil, offset)
           ))
        end
        local tokens_accepted = 0
        for ix = 1, op_count do
            local symbol_id = ops[ix]
            tokens_accepted = tokens_accepted +
                 slr:l0_alternative(symbol_id)
        end

        if tokens_accepted < 1 then return false, 'rejected char' end
        return slr:l0_earleme_complete()
    end

```

`l0_read_lexeme()`
reads a lexeme from the L0 recognizer.
Returns `true` on success,
otherwise `false` and a status string.

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.l0_read_lexeme(slr)
        if not slr.l0 then
            slr:l0r_new()
        end
        while true do
            local block_ix, offset, eoread = slr:block_progress()
            if offset >= eoread then
                return true
            end
            -- +1 because codepoints array is 1-based
            slr.codepoint = slr:codepoint_from_pos(block_ix, offset)
            local alive, status = slr:l0_read_codepoint()
            local this_candidate, eager = slr:l0_track_candidates()
            if this_candidate then slr.l0_candidate = this_candidate end
            if eager then return true end
            if not alive then return false, status end
            slr:block_move(offset + 1)
        end
        error('Unexpected fall through in l0_read()')
    end

```

Determine which paths
and candidates
are active.

[ Right now this is a prototype:
Only LATM is implemented;
a candidate is an earley set ID;
candidates are moved
and seconded at once.
In fact, because we are switching to global lexeme priorities;
this mechanism may be replaced by multiple lexers.
There would be two lexers for each
priority -- eager first, then lazy. ]

The candidate eventually
chosen is the last one
moved, unless one of the candidates is
eager.
That is decided by the caller --
the candidate and an `eager` boolean are
returned  so it can make that decision.

Return earley set ID if we have the completion of a lexeme
rule, false otherwise.

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.l0_track_candidates(slr)
        local l0r = slr.l0
        local slg = slr.slg
        local l0g = slg.l0
        local l0_rules = slr.l0_irls
        local eager = false
        local complete_lexemes = false
        local es_id = l0r:latest_earley_set()
        -- Do we have a completion of a lexeme rule?
        local max_eim = l0r:_earley_set_size(es_id) - 1
        for eim_id = 0, max_eim do
            local trv = _M.traverser_new(l0r, es_id, eim_id)
            local irl_id = trv:rule_id()
            if not irl_id then goto NEXT_EIM end
            local dot = trv:dot()
            if dot >= 0 then goto NEXT_EIM end
            complete_lexemes = true
            -- TODO: when we expand this, the ID of the g1 lexeme
            -- may matter; right now it does not.
            eager = eager or l0_rules[irl_id].eager
            ::NEXT_EIM::
        end
        if complete_lexemes then return es_id, eager end
        return
    end
```

`no_lexeme_handle()` handles the situation where the recognizer does
not find an acceptable lexeme.

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.no_lexeme_handle(slr)
       if slr.slg.rejection_action == 'event' then
           local q = slr.event_queue
           q[#q+1] = { "'rejected" }
           return
       end
       return slr:throw_at_pos(string.format(
            "No lexeme found at %s",
            slr:lc_brief(nil, slr.start_of_lexeme))
            )
    end
```

```
    -- miranda: section exhausted(), nested function of slr:alternatives()
    local function exhausted()
        -- no accepted or discarded lexemes
        if discard_mode then
           if slr.slg.exhaustion_action == 'event' then
               local q = slr.event_queue
               q[#q+1] = { "'exhausted" }
               return
           end
           return slr:throw_at_pos(string.format(
                "Parse exhausted, but lexemes remain, at %s",
                slr:lc_brief(nil, slr.start_of_lexeme))
                )
        end
        local start_of_lexeme = slr.start_of_lexeme
        slr:block_move(start_of_lexeme)
        return slr:no_lexeme_handle()
    end
```

`alternatives()`
finds and reads the alternatives in the SLIF.
Returns `true`
if the parse is alive and not in "parse before" status.
Return `false` otherwise.
The intuitive idea behind the return codes is that

* `true` means the parse that can continue without special action
by the user.

* `false` means that either

    - the parse is in "pause before" status and special action
    might be required for it to continue; or

    - the parse is exhausted, and cannot continue under any
    circumstance.

When `false` is returned, `alternatives()`
may also return
a string indicating the status.

TODO: Is the status string needed/used?

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.alternatives(slr, discard_mode)

        -- miranda: insert exhausted(), nested function of slr:alternatives()

        slr.lexeme_queue = {}
        slr.accept_queue = {}
        local l0r = slr.l0
        if not l0r then
            _M._internal_error('No l0r in slr_alternatives(): %s',
                slr.slg.l0:error_description())
        end
        local elect_earley_set = slr.l0_candidate
        -- no zero-length lexemes, so Earley set 0 is ignored
        if not elect_earley_set then return false, exhausted() end
        local working_pos = slr.start_of_lexeme + elect_earley_set
        local return_value = l0r:progress_report_start(elect_earley_set)
        if return_value < 0 then
            _M._internal_error('Problem in slr:progress_report_start(...,%d): %s',
                elect_earley_set, l0r:error_description())
        end
        local discarded, high_lexeme_priority = slr:l0_earley_set_examine(working_pos)
        l0r:progress_report_finish()
        -- PASS 2 --
        slr:lexeme_queue_examine(high_lexeme_priority)
        local accept_q = slr.accept_queue
        if #accept_q <= 0 then
            if discarded <= 0 then return false, exhausted() end
            -- if here, no accepted lexemes, but discarded ones
            slr:block_move(working_pos)
            local latest_es = slr.g1:latest_earley_set()
            local trailers = slr.trailers
            trailers[latest_es] =
                _M.sweep_add(trailers[latest_es],
                    slr.current_block.index,
                    slr.start_of_lexeme,
                    working_pos - slr.start_of_lexeme
                )
            return true
        end
        -- PASS 3 --
        local result = slr:do_pause_before()
        if result then return false, 'pause before' end
        slr:g1_earleme_complete()
        return true
    end
```

A utility function to create discard traces,
because this is done in two different places.

```
    -- miranda: section+ forward declarations
    local discard_event_gen
    -- miranda: section+ most Lua function definitions
    function discard_event_gen(slr, irlid, lexeme_start, lexeme_end)
        local l0g = slr.slg.l0
        local discarded_isyid = l0g:rule_rhs(irlid, 0)
        local discard_desc =
            discarded_isyid and l0g:symbol_display_form(discarded_isyid)
                or '[Bad irlid ' .. irlid .. ']'
        local block = slr.current_block
        local block_ix = block.index
        local event = { 'discarded lexeme',
            irlid, block_ix, lexeme_start, lexeme_end }
        event.msg = string.format(
            'Discarded lexeme %s: %s',
            slr:lc_range_brief(block_ix, lexeme_start, block_ix, lexeme_end - 1),
            discard_desc
        )
        return event
    end

```

Determine which lexemes are acceptable or discards.

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.l0_earley_set_examine(slr, working_pos)
        local discarded = 0
        local high_lexeme_priority = math.mininteger
        local block = slr.current_block
        local block_ix = block.index
        while true do
            local g1_lexeme = 'ignore'
            local rule_id, dot_position, origin = slr.l0:progress_item()
            if not rule_id then
                return discarded, high_lexeme_priority
            end
            if rule_id <= -2 then
                error(string.format('Problem in slr:progress_item(): %s'),
                    slr.l0:error_description())
            end
            if origin ~= 0 then
               goto NEXT_EARLEY_ITEM
            end
            if dot_position ~= -1 then
               goto NEXT_EARLEY_ITEM
            end
            g1_lexeme = slr.l0_irls[rule_id].g1_lexeme
            g1_lexeme = g1_lexeme or 'ignore'
            if g1_lexeme == 'ignore' then
               goto NEXT_EARLEY_ITEM
            end
            slr.end_of_lexeme = working_pos
            -- -2 means a discarded item
            if g1_lexeme == 'discard' then
               discarded = discarded + 1
               local q = slr.lexeme_queue
               q[#q+1] = discard_event_gen(slr, rule_id, slr.start_of_lexeme, slr.end_of_lexeme)
               goto NEXT_EARLEY_ITEM
            end
            -- if here, `g1_lexeme` must be a number
            -- this block hides the local's and allows the goto to work
            do
                local is_expected = slr.g1:terminal_is_expected(g1_lexeme)
                if not is_expected then
                    error(string.format('Internnal error: Marpa recognized unexpected token @%d-%d: lexme=%d',
                        slr.start_of_lexeme, slr.end_of_lexeme, g1_lexeme))
                end
                local this_lexeme_priority = slr.g1_isys[g1_lexeme].lexeme_priority
                if this_lexeme_priority > high_lexeme_priority then
                    high_lexeme_priority = this_lexeme_priority
                end
                local q = slr.lexeme_queue
                -- at this point we know the lexeme will be accepted by the grammar
                -- but we do not yet know about priority
                q[#q+1] = { 'acceptable lexeme',
                   block_ix, slr.start_of_lexeme, slr.end_of_lexeme,
                   g1_lexeme, this_lexeme_priority, this_lexeme_priority}
            end
            ::NEXT_EARLEY_ITEM::
        end
    end

```

Process the lexeme queue generated in pass 1.
In pass 1, we used a stack of tentative
trace events to record which lexemes
are acceptable, to be discarded, etc.
At this point, if we are tracing,
we convert the tentative trace
events into real trace events.

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.lexeme_queue_examine(slr, high_lexeme_priority)
        local lexeme_q = slr.lexeme_queue
        local block = slr.current_block
        local block_ix = block.index
        for ix = 1, #slr.lexeme_queue do
            local this_event = lexeme_q[ix]
            local event_type = this_event[1]
            if event_type == 'acceptable lexeme' then
                local event_type, lexeme_block, lexeme_start, lexeme_end,
                g1_lexeme, priority, required_priority =
                    table.unpack(this_event)
                if priority < high_lexeme_priority then
                    if slr.trace_terminals > 0 then
                        local xsy = g1g:_xsy(g1_lexeme)
                        if xsy then
                            coroutine.yield('trace', string.format(
                                "Outprioritized lexeme %s: %s; value=%q;\z
                                 priority was %d, but %d is required",
                                slr:lc_range_brief(block_ix, lexeme_start, block_ix, lexeme_end - 1),
                                xsy:display_form(),
                                slr:literal( block_ix, lexeme_start,  lexeme_end - lexeme_start ),
                                priority, high_lexeme_priority
                            ))
                        end
                    end
                    goto NEXT_LEXEME
                end
                -- TODO accept_queue
                local q = slr.accept_queue
                q[#q+1] = this_event
                goto NEXT_LEXEME
            end
            if event_type == 'discarded lexeme' then
                local event_type, rule_id,
                        lexeme_block, lexeme_start, lexeme_end
                    = table.unpack(this_event)
                -- we do not have the lexeme, only the lexer rule,
                -- so we will let the upper layer figure things out.
                if slr.trace_terminals > 0 then
                    local event = discard_event_gen(slr, rule_id, lexeme_start, lexeme_end)
                    coroutine.yield('trace', event.msg)
                end
                    local g1r = slr.g1
                    local event_on_discard_active =
                        slr.l0_irls[rule_id].event_on_discard_active
                    if event_on_discard_active then
                        local last_g1_location = g1r:latest_earley_set()
                        local q = slr.event_queue
                        q[#q+1] = { 'discarded lexeme',
                            rule_id,
                            lexeme_block, lexeme_start, lexeme_end - lexeme_start,
                            last_g1_location}
                     end
            end
            ::NEXT_LEXEME::
        end
        return
    end
```

Process any "pause before" lexemes.
Returns `true` is there was one,
`false` otherwise.

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.do_pause_before(slr)
        local accept_q = slr.accept_queue
        for ix = 1, #accept_q do
            local this_event = accept_q[ix]
                -- TODO accept_queue
            local event_type, lexeme_block, lexeme_start, lexeme_end,
                    g1_lexeme, priority, required_priority =
                table.unpack(this_event)
            local pause_before_active = slr.g1_isys[g1_lexeme].pause_before_active
            if pause_before_active then
                if slr.trace_terminals > 2 then
                    coroutine.yield('trace', 'g1 before lexeme event')
                end
                local q = slr.event_queue
                q[#q+1] = {
                    'before lexeme', g1_lexeme, lexeme_block, lexeme_start,
                    lexeme_end - lexeme_start
                }
                local start_of_lexeme = slr.start_of_lexeme
                slr:block_move(start_of_lexeme)
                return true
            end
        end
        return false
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.g1_earleme_complete(slr)
        slr:g1_alternatives()
        local g1r = slr.g1
        local result = g1r:earleme_complete()
        if result < 0 then
            error(string.format(
                'Problem in marpa_r_earleme_complete(): %s',
                g1r:error_description()
            ))
        end
        local end_of_lexeme = slr.end_of_lexeme
        slr:block_move(end_of_lexeme)
        if result > 0 then slr:g1_convert_events() end
        local start_of_lexeme = slr.start_of_lexeme
        local end_of_lexeme = slr.end_of_lexeme
        local lexeme_length = end_of_lexeme - start_of_lexeme
        local g1r = slr.g1
        local latest_earley_set = g1r:latest_earley_set()
        slr.per_es[latest_earley_set] =
            { slr.current_block.index, start_of_lexeme, lexeme_length }
    end
```

Read alternatives into the G1 grammar.

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.g1_alternatives(slr)
        local accept_q = slr.accept_queue
        local block = slr.current_block
        local block_ix = block.index
        local slg = slr.slg
        local g1g = slg.g1
        for ix = 1, #accept_q do
            local this_event = accept_q[ix]
                -- TODO accept_queue
            local event_type, lexeme_block, lexeme_start, lexeme_end,
                    g1_lexeme, priority, required_priority =
                table.unpack(this_event)

            if slr.trace_terminals > 2 then
                local xsy = g1g:_xsy(g1_lexeme)
                if xsy then
                    local working_earley_set = slr.g1:latest_earley_set() + 1
                    coroutine.yield('trace', string.format(
                        'Attempting to read lexeme %s e%d: %s; value=%q',
                        slr:lc_range_brief(block_ix, lexeme_start, block_ix, lexeme_end - 1),
                        working_earley_set,
                        xsy:display_form(),
                        slr:literal( block_ix, lexeme_start,  lexeme_end - lexeme_start )
                    ))
                end
            end
            local g1r = slr.g1
            local kollos = getmetatable(g1r).kollos
            local value_is_literal = _M.defines.TOKEN_VALUE_IS_LITERAL
            local return_value = g1r:alternative(g1_lexeme, value_is_literal, 1)
            if return_value == _M.err.UNEXPECTED_TOKEN_ID then
                error('Internal error: Marpa rejected expected token')
            end
            if return_value == _M.err.DUPLICATE_TOKEN then
                local xsy = g1g:_xsy(g1_lexeme)
                if xsy then
                    coroutine.yield('trace', string.format(
                        'Rejected as duplicate lexeme %s: %s; value=%q',
                        slr:lc_range_brief(block_ix, lexeme_start, block_ix, lexeme_end - 1),
                        xsy:display_form(),
                        slr:literal( block_ix, lexeme_start,  lexeme_end - lexeme_start )
                    ))
                end
                goto NEXT_EVENT
            end
            if return_value ~= _M.err.NONE then
                local l0r = slr.l0
                local _, offset  = slr:block_progress()
                error(string.format([[
                     'Problem SLR->read() failed on symbol id %d at position %d: %s'
                ]],
                    g1_lexeme, offset, l0r:error_description()
                ))
                goto NEXT_EVENT
            end
            do

                if slr.trace_terminals > 0 then
                    local xsy = g1g:_xsy(g1_lexeme)
                    if xsy then
                        local display_form = xsy:display_form()
                        local working_earley_set = slr.g1:latest_earley_set() + 1
                        coroutine.yield('trace', string.format(
                            "Accepted lexeme %s e%d: %s; value=%q",
                            slr:lc_range_brief(block_ix, lexeme_start, block_ix, lexeme_end - 1),
                            working_earley_set,
                            display_form,
                            slr:literal( block_ix, lexeme_start,  lexeme_end - lexeme_start )
                        ))
                    end
                end

                local pause_after_active = slr.g1_isys[g1_lexeme].pause_after_active
                if pause_after_active then
                    local display_form = g1g:symbol_display_form(g1_lexeme)
                    if slr.trace_terminals > 2 then
                        coroutine.yield('trace', string.format(
                            'Paused after lexeme %s: %s',
                            slr:lc_range_brief(block_ix, lexeme_start, block_ix, lexeme_end - 1),
                            display_form
                        ))
                    end
                    local q = slr.event_queue
                    q[#q+1] = { 'after lexeme', g1_lexeme, block_ix, lexeme_start, lexeme_end - lexeme_start}
                end
            end
            ::NEXT_EVENT::

        end
    end
```

### High-level external reading

Returns `nil` if `symbol_name` is rejected,
otherwise the new offset.

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.lexeme_read_string(slr, symbol_name, input_string)
        if slr.is_lo_level_scanning then
           return error_lo_hi_scanning("slr.lexeme_read_string()")
        end
        local ok = lexeme_alternative_i(slr, symbol_name, input_string, 1)
        if not ok then return end
        local save_block = slr:block_progress()
        local new_block_id = slr:block_progress(slr:block_new(input_string))
        local new_eoread
        local dummy
        dummy, dummy, new_eoread = slr:block_progress(new_block_id)
          -- print('new_eoread', new_eoread)
          -- print('input_string', input_string)
        local new_offset = slr:lexeme_complete(new_block_id, 0, new_eoread)
        slr:convert_libmarpa_events()
        slr:block_set(save_block)
        return new_offset
    end
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.lexeme_read_block(slr, symbol_name, token_sv,
            block_id_arg, offset_arg, length_arg)
        if slr.is_lo_level_scanning then
           return error_lo_hi_scanning("slr.lexeme_read_block()")
        end
        local block_id, offset, eoread
            = slr:block_check_range(block_id_arg, offset_arg, length_arg)
        local ok
        if token_sv then
            ok = lexeme_alternative_i(slr, symbol_name, token_sv, 1)
        else
            ok = lexeme_alternative_i2(slr, symbol_name,
                _M.defines.TOKEN_VALUE_IS_UNDEF, 1)
        end
        if not ok then return end
        local new_offset = slr:lexeme_complete(block_id, offset, eoread-offset)
        slr:convert_libmarpa_events()
        return new_offset
    end
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.lexeme_read_literal(slr, symbol_name,
            block_id_arg, offset_arg, length_arg)
        if slr.is_lo_level_scanning then
           return error_lo_hi_scanning("slr.lexeme_read_literal()")
        end
        local block_id, offset, eoread
            = slr:block_check_range(block_id_arg, offset_arg, length_arg)
        local ok = lexeme_alternative_i2(slr, symbol_name,
            _M.defines.TOKEN_VALUE_IS_LITERAL, 1)
        local new_offset = slr:lexeme_complete(block_id, offset, eoread-offset)
        slr:convert_libmarpa_events()
        return new_offset
    end
```

### Low-level external reading

These functions are for "external" reading of tokens --
that is reading tokens by a means other than Marpa's own
lexer.
It assumes that the caller checked the args.

`value_type` arg is "literal", "explicit" or "undef".
In the Perl interface, the `token` arg is always an SV.

Returns 1 if read OK, `nil` on soft error.
Other errors are thrown.

```
    -- miranda: section+ forward declarations
    local lexeme_alternative_i2
    -- miranda: section+ most Lua function definitions
    function lexeme_alternative_i2(slr, symbol_name, token_ix, length)
        local slg = slr.slg
        local xsy = slg.xsys[symbol_name]
        if not xsy then
            _M.userX(
                "slr->lexeme_alternative(): symbol %q does not exist",
                symbol_name)
        end
        local lexeme = xsy.lexeme
        if not lexeme then
            _M.userX(
                "slr->lexeme_alternative(): symbol %q is not a lexeme",
                symbol_name)
        end
        local symbol_id = lexeme.g1_isy.id
        local g1r = slr.g1

        local return_value = g1r:alternative(symbol_id, token_ix, length)
        if return_value == _M.err.NONE then return 1 end
        if return_value == _M.err.UNEXPECTED_TOKEN_ID then return end
        -- Soft failure on next two error codes
        -- is perhaps unnecessary or arguable,
        -- but it preserves compatibility with Marpa::XS
        if return_value == _M.err.NO_TOKEN_EXPECTED_HERE then return end
        if return_value == _M.err.INACCESSIBLE_TOKEN then return end
        local error_description = slg.g1:error_description()
        return _M.userX( 'Problem reading symbol "%s": %s',
            symbol_name, error_description
        );
    end
```

```
    -- miranda: section+ forward declarations
    local lexeme_alternative_i
    -- miranda: section+ most Lua function definitions
    function lexeme_alternative_i(slr, symbol_name, token, length)
        local token_ix = #slr.token_values + 1
        slr.token_values[token_ix] = token
        return lexeme_alternative_i2(slr, symbol_name, token_ix, length)
    end
```

These are the externally visible low-level `lexeme_alternative*()`
methods

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.lexeme_alternative_literal(slr, symbol_name, length)
        length = length or 1
        local accepted =
            lexeme_alternative_i2(slr, symbol_name, _M.defines.TOKEN_VALUE_IS_LITERAL, length)
        if not accepted then return end
        slr.is_lo_level_scanning = true
        return accepted
    end
    function _M.class_slr.lexeme_alternative_undef(slr, symbol_name, length)
        length = length or 1
        local accepted =
            lexeme_alternative_i2(slr, symbol_name, _M.defines.TOKEN_VALUE_IS_UNDEF, length)
        if not accepted then return end
        slr.is_lo_level_scanning = true
        return accepted
    end
    function _M.class_slr.lexeme_alternative(slr, symbol_name, token, length)
        length = length or 1
        local accepted = lexeme_alternative_i(slr, symbol_name, token, length)
        if not accepted then return end
        slr.is_lo_level_scanning = true
        return accepted
    end
```

Returns new block offset on success.
Always throws errors.

```
    -- miranda: section+ forward declarations
    local per_es_add
    -- miranda: section+ most Lua function definitions
    function per_es_add(slr, block_id, offset, longueur)
        local latest_earley_set = slr:latest_earley_set()
        slr.per_es[latest_earley_set] =
            { block_id, offset, longueur }
        local new_offset = offset + longueur
        slr:block_set(block_id)
        slr:block_move(new_offset)
        return new_offset
    end
    function _M.class_slr.lexeme_complete(slr, block_id, offset, longueur)
        local g1r = slr.g1
        slr.is_lo_level_scanning = false
        slr.event_queue = {}
        local this_earleme
        local closest_earleme = slr:closest_earleme()
        -- Loop until we create a new earley set
        while true do
            local event_count = g1r:earleme_complete()
            if event_count < 0 then
                return error('Problem in slr->lexeme_complete(): '
                    ..  slr.slg.g1:error_description())
            end
            this_earleme = slr:current_earleme()
            if this_earleme >= closest_earleme then break end
            -- Return early if events occured that are not at an Earley
            -- set.  As of this writing, there are no such events:
            -- recognizer events only occur when
            -- an earley set is created.
            if event_count > 0 then
                slr:g1_convert_events()
                local _, old_offset = slr:block_progress()
                return old_offset
            end
        end
        slr:g1_convert_events()
        return per_es_add(slr, block_id, offset, longueur)
    end
```

### Evaluation

```
    -- miranda: section+ class_slr C methods
    static int lca_slv_step_meth (lua_State * L)
    {
        Marpa_Value v;
        lua_Integer step_type;
        const int value_table = marpa_lua_gettop (L);
        int step_table;

        marpa_luaL_checktype (L, 1, LUA_TTABLE);
        /* Lua stack: [ value_table ] */

        marpa_lua_getfield(L, value_table, "lmw_v");
        /* Lua stack: [ value_table, lmw_v ] */
        marpa_luaL_argcheck (L, (LUA_TUSERDATA == marpa_lua_getfield (L,
                    -1, "_libmarpa")), 1,
            "Internal error: recce._libmarpa userdata not set");
        /* Lua stack: [ value_table, lmw_v, v_ud ] */
        v = *(Marpa_Value *) marpa_lua_touserdata (L, -1);
        /* Lua stack: [ value_table, lmw_v, v_ud ] */
        marpa_lua_settop (L, value_table);
        /* Lua stack: [ value_table ] */
        marpa_lua_newtable (L);
        /* Lua stack: [ value_table, step_table ] */
        step_table = marpa_lua_gettop (L);
        marpa_lua_pushvalue (L, -1);
        marpa_lua_setfield (L, value_table, "this_step");
        /* Lua stack: [ value_table, step_table ] */

        step_type = (lua_Integer) marpa_v_step (v);
        marpa_lua_pushstring (L, step_name_by_code (step_type));
        marpa_lua_setfield (L, step_table, "type");

        /* Stack indexes adjusted up by 1, because Lua arrays
         * are 1-based.
         */
        switch (step_type) {
        case MARPA_STEP_RULE:
            marpa_lua_pushinteger (L, marpa_v_result (v)+1);
            marpa_lua_setfield (L, step_table, "result");
            marpa_lua_pushinteger (L, marpa_v_arg_n (v)+1);
            marpa_lua_setfield (L, step_table, "arg_n");
            marpa_lua_pushinteger (L, marpa_v_rule (v));
            marpa_lua_setfield (L, step_table, "rule");
            marpa_lua_pushinteger (L, marpa_v_rule_start_es_id (v));
            marpa_lua_setfield (L, step_table, "start_es_id");
            marpa_lua_pushinteger (L, marpa_v_es_id (v));
            marpa_lua_setfield (L, step_table, "es_id");
            break;
        case MARPA_STEP_TOKEN:
            marpa_lua_pushinteger (L, marpa_v_result (v)+1);
            marpa_lua_setfield (L, step_table, "result");
            marpa_lua_pushinteger (L, marpa_v_token (v));
            marpa_lua_setfield (L, step_table, "symbol");
            marpa_lua_pushinteger (L, marpa_v_token_value (v));
            marpa_lua_setfield (L, step_table, "value");
            marpa_lua_pushinteger (L, marpa_v_token_start_es_id (v));
            marpa_lua_setfield (L, step_table, "start_es_id");
            marpa_lua_pushinteger (L, marpa_v_es_id (v));
            marpa_lua_setfield (L, step_table, "es_id");
            break;
        case MARPA_STEP_NULLING_SYMBOL:
            marpa_lua_pushinteger (L, marpa_v_result (v)+1);
            marpa_lua_setfield (L, step_table, "result");
            marpa_lua_pushinteger (L, marpa_v_token (v));
            marpa_lua_setfield (L, step_table, "symbol");
            marpa_lua_pushinteger (L, marpa_v_token_start_es_id (v));
            marpa_lua_setfield (L, step_table, "start_es_id");
            marpa_lua_pushinteger (L, marpa_v_es_id (v));
            marpa_lua_setfield (L, step_table, "es_id");
            break;
        }

        return 0;
    }

```

### Locations

A "sweep" is a set of trios represeenting spans in the input.
Each trio is `[block, start, length]`.
The trios are stored as a sequence in a table, so that,
if `n` is the number of trios,
the table's length is `3*n`.
Each trio represents a consecutive sequence of characters
in `block`.
A sweep can store other data in its non-numeric keys.

Factory to create iterator over the sweeps in a G1 range.
`g1_last` defaults to `g1_first`.

TODO: Allow for leading trailer, final trailer.


TODO: Assumes that the value is all on one block.
```
    -- miranda: section+ forward declarations
    local sweep_range
    -- miranda: section+ most Lua function definitions
    function sweep_range(slr, g1_first, g1_last)
         local function iter()
             if not g1_last then g1_last = g1_first end
             local g1_ix = g1_first+1
             while true do
                 local this_per_es = slr.per_es[g1_ix]
                 if not this_per_es then return end
                 for sweep_ix = 1, #this_per_es, 3 do
                      coroutine.yield(this_per_es[sweep_ix],
                          this_per_es[sweep_ix+1],
                          this_per_es[sweep_ix+2])
                 end
                 if g1_ix > g1_last then return end
                 local this_trailer = slr.trailers[g1_ix]
                 if this_trailer then
                     for sweep_ix = 1, #this_trailer, 3 do
                          coroutine.yield(this_trailer[sweep_ix],
                              this_trailer[sweep_ix+1],
                              this_trailer[sweep_ix+2])
                     end
                 end
                 g1_ix = g1_ix + 1
             end
         end
         return coroutine.wrap(iter)
    end

    -- miranda: section+ forward declarations
    local reverse_sweep_range
    -- miranda: section+ most Lua function definitions
    function reverse_sweep_range(slr, g1_last, g1_first)
         local function iter()
             if not g1_first then g1_first = g1_last end
             local g1_ix = g1_last+1
             while true do
                 local this_per_es = slr.per_es[g1_ix]
                 if not this_per_es then return end
                 local last_sweep_ix = 3*math.tointeger((#this_per_es-1)//3)+1
                 for sweep_ix = last_sweep_ix, 1, -3 do
                      coroutine.yield(this_per_es[sweep_ix],
                          this_per_es[sweep_ix+1],
                          this_per_es[sweep_ix+2])
                 end
                 if g1_ix <= g1_first then return end
                 local this_trailer = slr.trailers[g1_ix]
                 if this_trailer then
                     for sweep_ix = 1, #this_trailer, 3 do
                          coroutine.yield(this_trailer[sweep_ix],
                              this_trailer[sweep_ix+1],
                              this_trailer[sweep_ix+2])
                     end
                 end
                 g1_ix = g1_ix - 1
             end
         end
         return coroutine.wrap(iter)
    end

    function _M.sweep_add(sweep, block, start, len)
        -- io.stderr:write(string.format("Call: sweep, block,start,len = %s,%s,%s,%s\n",
            -- inspect(sweep), inspect(block), inspect(start), inspect(len)))

        if not sweep then
            return { block, start, len }
        end
        local last_block, last_start, last_len =
            table.unpack(sweep, (#sweep-2))

        -- io.stderr:write(string.format("sweep, last block,start,len = %s,%s,%s,%s\n",
            -- inspect(sweep), inspect(last_block), inspect(last_start), inspect(last_len)))

        -- As a special case, if the new sweep
        -- abuts the last one, we simply extend
        -- the last one
        if (block == last_block)
            and (last_start + last_len ==  start)
        then
            sweep[#sweep] = last_len + len
            -- io.stderr:write('Special case: ', inspect(sweep), "\n")
            return sweep
        end
        sweep[#sweep+1] = block
        sweep[#sweep+1] = start
        sweep[#sweep+1] = len
        -- io.stderr:write('Main case: ', inspect(sweep), "\n")
        return sweep
    end

```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.token_link_data(slr, lmw_r)
        local lmw_g = lmw_r.lmw_g
        local result = {}
        local token_id, value_ix = lmw_r:_source_token()
        local predecessor_ahm = lmw_r:_source_predecessor_state()
        local origin_set_id = lmw_r:_earley_item_origin()
        local origin_earleme = lmw_r:earleme(origin_set_id)
        local middle_earleme = origin_earleme
        local middle_set_id = lmw_r:_source_middle()
        if predecessor_ahm then
            middle_earleme = lmw_r:earleme(middle_set_id)
        end
        local token_name = lmw_g:nsy_name(token_id)
        result.predecessor_ahm = predecessor_ahm
        result.origin_earleme = origin_earleme
        result.middle_set_id = middle_set_id
        result.middle_earleme = middle_earleme
        result.token_name = token_name
        result.token_id = token_id
        result.value_ix = value_ix
        if value_ix ~= 2 then
            result.value = slr.token_values[value_ix]
        end
        return result
    end

```

### Events

```
    -- miranda: section+ most Lua function definitions

    function _M.class_slr.g1_convert_events(slr)
        local _, offset = slr:block_progress()
        local g1g = slr.slg.g1
        local q = slr.event_queue
        local events = g1g:events()
        for i = 1, #events, 2 do
            local event_type = events[i]
            local event_value = events[i+1]
            if event_type == _M.event["EXHAUSTED"] then
                goto NEXT_EVENT
            end
            if event_type == _M.event["SYMBOL_COMPLETED"] then
                q[#q+1] = { 'symbol completed', event_value}
                goto NEXT_EVENT
            end
            if event_type == _M.event["SYMBOL_NULLED"] then
                q[#q+1] = { 'symbol nulled', event_value}
                goto NEXT_EVENT
            end
            if event_type == _M.event["SYMBOL_PREDICTED"] then
                q[#q+1] = { 'symbol predicted', event_value}
                goto NEXT_EVENT
            end
            if event_type == _M.event["EARLEY_ITEM_THRESHOLD"] then
                coroutine.yield('trace', string.format(
                    'G1 exceeded earley item threshold at pos %d: %d Earley items',
                    offset, event_value))
                goto NEXT_EVENT
            end
            local event_data = _M.event[event_type]
            if not event_data then
                result_string = string.format(
                    'unknown event code, %d', event_type
                )
            else
                result_string = event_data.name
            end
            q[#q+1] = { 'unknown_event', result_string}
            ::NEXT_EVENT::
        end
    end

    function _M.class_slr.l0_convert_events(slr)
        local l0g = slr.slg.l0
        local _, offset = slr:block_progress()
        local q = slr.event_queue
        local events = l0g:events()
        for i = 1, #events, 2 do
            local event_type = events[i]
            local event_value = events[i+1]
            if event_type == _M.event["EXHAUSTED"] then
                goto NEXT_EVENT
            end
            if event_type == _M.event["EARLEY_ITEM_THRESHOLD"] then
                coroutine.yield('trace', string.format(
                    'L0 exceeded earley item threshold at pos %d: %d Earley items',
                    offset, event_value))
                goto NEXT_EVENT
            end
            local event_data = _M.event[event_type]
            if not event_data then
                result_string = string.format(
                    'unknown event code, %d', event_type
                )
            else
                result_string = event_data.name
            end
            q[#q+1] = { 'unknown_event', result_string}
            ::NEXT_EVENT::
        end
    end

```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.activate_by_event_name(slr, event_name, activate)
        local slg = slr.slg
        local events
        local active_flag = activate and 1 or 0

        events = slg.completion_event_by_name[event_name]
        if events then
            for ix = 1, #events do
                local event_data = events[ix]
                local isyid = event_data.isyid
                slr.g1:completion_symbol_activate(isyid, active_flag)
            end
        end

        events = slg.nulled_event_by_name[event_name]
        if events then
            for ix = 1, #events do
                local event_data = events[ix]
                local isyid = event_data.isyid
                slr.g1:nulled_symbol_activate(isyid, active_flag)
            end
        end

        events = slg.prediction_event_by_name[event_name]
        if events then
            for ix = 1, #events do
                local event_data = events[ix]
                local isyid = event_data.isyid
                slr.g1:prediction_symbol_activate(isyid, active_flag)
            end
        end

        events = slg.discard_event_by_name[event_name]
        if events then
            local g_l0_rules = slg.l0.irls
            local r_l0_rules = slr.l0_irls
            for ix = 1, #events do
                local event_data = events[ix]
                local irlid = event_data.irlid
                if activate
                    and not g_l0_rules[irlid].event_on_discard
                then
                    -- TODO: Can this be a user error?
                    error("Attempt to activate non-existent discard event")
                end
                r_l0_rules[irlid].event_on_discard_active = activate
            end
        end

        events = slg.lexeme_event_by_name[event_name]
        if events then
            local g_g1_symbols = slg.g1.isys
            local r_g1_symbols = slr.g1_isys
            for ix = 1, #events do
                    local event_data = events[ix]
                    local isyid = event_data.isyid
                    -- print(event_name, activate)
                    if activate then
                        r_g1_symbols[isyid].pause_after_active
                            = g_g1_symbols[isyid].pause_after
                        r_g1_symbols[isyid].pause_before_active
                            = g_g1_symbols[isyid].pause_before
                    else
                        r_g1_symbols[isyid].pause_after_active = nil
                        r_g1_symbols[isyid].pause_before_active = nil
                    end
            end
        end
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.convert_libmarpa_events(slr)
        local in_q = slr.event_queue
        local pause = false
        for event_ix = 1, #in_q do
            local event = in_q[event_ix]
            -- print(inspect(event))
            local event_type = event[1]

            if event_type == "'exhausted" then
                local yield_result = coroutine.yield ( 'event', event_type, event_type )
                pause = pause or yield_result == 'pause'
            end

            if event_type == "'rejected" then
                local yield_result = coroutine.yield ( 'event', event_type, event_type )
                pause = pause or yield_result == 'pause'
            end

            -- The code next set of events is highly similar -- an isyid at
            -- event[2] is looked up in a table of event names.  Does it
            -- make sense to share code, perhaps using closures?
            if event_type == 'symbol completed' then
                local completed_isyid = event[2]
                local slg = slr.slg
                local event_name = slg.completion_event_by_isy[completed_isyid].name
                local yield_result = coroutine.yield ( 'event', event_type, event_name )
                pause = pause or yield_result == 'pause'
            end

            if event_type == 'symbol nulled' then
                local nulled_isyid = event[2]
                local slg = slr.slg
                local event_name = slg.nulled_event_by_isy[nulled_isyid].name
                local yield_result = coroutine.yield ( 'event', event_type, event_name )
                pause = pause or yield_result == 'pause'
            end

            if event_type == 'symbol predicted' then
                local predicted_isyid = event[2]
                local slg = slr.slg
                local event_name = slg.prediction_event_by_isy[predicted_isyid].name
                local yield_result = coroutine.yield ( 'event', event_type, event_name )
                pause = pause or yield_result == 'pause'
            end

            if event_type == 'after lexeme'
                or event_type == 'before lexeme'
            then
                local lexeme_isyid = event[2]
                local slg = slr.slg
                local g1g = slg.g1
                local event_name = slg.lexeme_event_by_isy[lexeme_isyid].name
                -- there must be an XSY
                local lexeme_xsy = g1g:_xsy(lexeme_isyid)
                local lexeme_xsyid = lexeme_xsy.id
                local yield_result = coroutine.yield ( 'event', event_type,
                    event_name, lexeme_xsyid, table.unpack(event, 3) )
                pause = pause or yield_result == 'pause'
            end

            -- end of run of highly similar events

            if event_type == 'discarded lexeme' then
                local lexeme_irlid = event[2]
                local slg = slr.slg
                local event_name = slg.discard_event_by_irl[lexeme_irlid].name
                local new_event = { 'event', event_name }
                for ix = 4, #event do
                    new_event[#new_event+1] = event[ix]
                end
                local yield_result = coroutine.yield( 'event', event_type,
                    event_name, table.unpack(event, 3) )
                pause = pause or yield_result == 'pause'
            end
        end

        -- TODO -- after development, change to just return status?
        local event_status = pause and 'pause' or 'ok'
        -- print('event_status:', event_status)
        return event_status, {}
    end

```

### G1 location accessors

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.latest_earley_set(slr)
        local g1r = slr.g1
        return g1r:latest_earley_set()
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.current_earleme(slr)
        local g1r = slr.g1
        return g1r:current_earleme()
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.closest_earleme(slr)
        local g1r = slr.g1
        return g1r:closest_earleme()
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.furthest_earleme(slr)
        local g1r = slr.g1
        return g1r:furthest_earleme()
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.latest_earleme(slr)
        local g1r = slr.g1
        local latest_es = g1r:latest_earley_set()
        return g1r:earleme(latest_es)
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.earleme(slr, earley_set)
        local g1r = slr.g1
        return g1r:earleme(earley_set)
    end
```

### Progress reporting

Given a scanless
recognizer and a symbol,
`last_completed()`
returns the start earley set
and length
of the last such symbol completed,
or nil if there was none.

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.last_completed(slr, xsyid)
         local g1r = slr.g1
         local g1g = slr.slg.g1
         local latest_earley_set = g1r:latest_earley_set()
         local first_origin = latest_earley_set + 1
         local earley_set = latest_earley_set
         while earley_set >= 0 do
             g1r:progress_report_start(earley_set)
             while true do
                 local rule_id, dot_position, origin = g1r:progress_item()
                 if not rule_id then goto LAST_ITEM end
                 if dot_position ~= -1 then goto NEXT_ITEM end
                 local lhs_id = g1g:rule_lhs(rule_id)
                 local lhs_xsy = g1g:_xsy(lhs_id)
                 if not lhs_xsy then goto NEXT_ITEM end
                 local lhs_xsyid = lhs_xsy.id
                 if xsyid ~= lhs_xsyid then goto NEXT_ITEM end
                 if origin < first_origin then
                     first_origin = origin
                 end
                 ::NEXT_ITEM::
             end
             ::LAST_ITEM::
             g1r:progress_report_finish()
             if first_origin <= latest_earley_set then
                 goto LAST_EARLEY_SET
             end
             earley_set = earley_set - 1
         end
         ::LAST_EARLEY_SET::
         if earley_set < 0 then return end
         return first_origin, earley_set - first_origin
    end

```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.g1_progress(slr, ordinal_arg)
        local g1r = slr.g1
        local ordinal = ordinal_arg
        local latest_earley_set = g1r:latest_earley_set()
        if ordinal > latest_earley_set then
            error(string.format(
                "Argument out of bounds in slr->progress(%d)\n"
                .. "   Argument specifies Earley set after the latest Earley set 0\n"
                .. "   The latest Earley set is Earley set $latest_earley_set\n",
                ordinal_arg
                ))
        elseif ordinal < 0 then
            ordinal = latest_earley_set + 1 + ordinal
            if ordinal < 0 then
                error(string.format(
                    "Argument out of bounds in slr->progress(%d)\n"
                    .. "   Argument specifies Earley set before Earley set 0\n",
                    ordinal_arg
                ))
            end
            end
        local result = {}
        g1r:progress_report_start(ordinal)
        while true do
            local rule_id, dot_position, origin = g1r:progress_item()
            if not rule_id then goto LAST_ITEM end
            result[#result+1] = { rule_id, dot_position, origin }
        end
        ::LAST_ITEM::
        g1r:progress_report_finish()
        return result
    end

```

TODO: Make `collected_progress_items` a local, after development.

```
    -- miranda: section+ most Lua function definitions
        function _M.collected_progress_items(items)
            table.sort(items, _M.cmp_seq)
            return coroutine.wrap(function ()
                if #items < 1 then return end
                local this_item = items[1]
                local work_ordinal, _, work_rule_id, work_position, work_origin
                    = table.unpack(this_item)
                local origins = { work_origin }
                for ix = 2, #items do
                    local this_item = items[ix]
                    local this_ordinal, _, this_rule_id, this_position, this_origin
                        = table.unpack(this_item)
                    if work_ordinal ~= this_ordinal
                       or this_rule_id ~= work_rule_id
                       or this_position ~= work_position
                    then
                        coroutine.yield(work_ordinal, work_rule_id, work_position, origins)
                        work_ordinal = this_ordinal
                        work_rule_id = this_rule_id
                        work_position = this_position
                        origins = { this_origin }
                        goto NEXT_ITEM
                    end
                    if origins[#origins] ~= this_origin then
                        origins[#origins+1] = this_origin
                    end
                    ::NEXT_ITEM::
                end
                coroutine.yield(work_ordinal, work_rule_id, work_position, origins)
            end)
        end
    function _M.class_slr.g1_progress_show(slr, start_ordinal_arg, end_ordinal_arg)
        local slg = slr.slg
        local g1g = slg.g1
        local g1r = slr.g1
        local last_ordinal = g1r:latest_earley_set()
        local start_ordinal = math.tointeger(start_ordinal_arg) or last_ordinal
        if start_ordinal < 0 then start_ordinal = last_ordinal + 1 + start_ordinal end
        if start_ordinal > last_ordinal or start_ordinal < 0 then
             _M._internal_error(
                "Marpa::R3::Recognizer::g1_progress_show start index is %d, \z
                 must be in range 0-%d",
                 inspect(start_ordinal_arg, {depth=1}),
                 last_ordinal
             )
        end
        local end_ordinal = math.tointeger(end_ordinal_arg) or start_ordinal
        if end_ordinal < 0 then end_ordinal = last_ordinal + 1 + end_ordinal end
        if end_ordinal > last_ordinal or end_ordinal < 0 then
             _M._internal_error(
                "Marpa::R3::Recognizer::g1_progress_show start index is %d, \z
                 must be in range 0-%d",
                 inspect(end_ordinal_arg, {depth=1}),
                 last_ordinal
             )
        end
        local items = {}
        for current_ordinal = start_ordinal, end_ordinal do
            local current_items = slr:g1_progress(current_ordinal)
            for ix = 1, #current_items do
                -- item_type is 0 for prediction, 1 for medial, 2 for completed
                local item_type = 1
                local rule_id, position, origin = table.unpack(current_items[ix])
                local rule_length = g1g:rule_length(rule_id)
                if position == -1 then position = rule_length end
                if position == 0 then item_type = 0
                elseif position == rule_length then item_type = 2 end
                items[#items+1] = { current_ordinal, item_type, rule_id, position, origin }
            end
        end
        local lines = {}
        for current_ordinal, rule_id, position, origins in _M.collected_progress_items(items) do
            lines[#lines+1] = g1_progress_line_do(
                slr, current_ordinal, origins, rule_id, position
            )
        end
        -- io.stderr:write(inspect(lines, {depth=3}), "\n")
        lines[#lines+1] = '' -- to get a final "\n"
        return table.concat(lines, "\n")
    end
```

### SLR diagnostics


```
      -- miranda: section origins(), nested function of slr:progress()
      local function origins(traverser)
          local function origin_gen(base_trv, origin_trv)
              -- print(string.format('Calling origin gen %s %s', base_trv, origin_trv))
              -- print(string.format('Calling origin gen origin_trv = %s', inspect(origin_trv, {depth=2})))
              local nrl_id = origin_trv:nrl_id()
              -- print(string.format('nrl_id %s', nrl_id))
              local irl_id = origin_trv:rule_id()
              local origin = origin_trv:origin()
              if irl_id
                  and g1g:_nrl_semantic_equivalent(nrl_id)
                  and slg:g1_rule_is_xpr_top(irl_id)
              then
                  coroutine.yield(origin)
                  return
              end

              -- Do not recurse on sequence rules
              if g1g:sequence_min(irl_id) then return end

              -- If here, we are at a CHAF rule
              local lhs = g1g:_nrl_lhs(nrl_id)
              local ptrv = _M.ptraverser_new(g1r, origin, lhs)

              -- Do not recurse if there are any LIMs
              --    Note: LIM's are always first
              if ptrv:at_lim() then return end
              while true do
                  local trv = ptrv:eim_iter()
                  if not trv then break end
                  origin_gen(base_trv, trv)
              end

          end
          return coroutine.wrap(
                  function () origin_gen(traverser, traverser) end
          )
      end
```

```
    -- miranda: section+ most Lua function definitions
      function _M.class_slr.progress(slr, earley_set_id)
          local slg = slr.slg
          local g1g = slg.g1
          local g1r = slr.g1
          -- miranda: insert origins(), nested function of slr:progress()
          local uniq_items = {}
          if earley_set_id < 0 then
              earley_set_id = g1r:latest_earley_set() + earley_set_id + 1
          end
          local max_eim = g1r:_earley_set_size(earley_set_id) - 1

          for item_id = 0, max_eim do
              -- IRL data for debugging only -- delete
              local trv = _M.traverser_new(g1r, earley_set_id, item_id)
              local irl_id = trv:rule_id()
              if not irl_id then goto NEXT_ITEM end
              local irl_dot = trv:dot()
              -- io.stderr:write(string.format("item: %d R%d:%d@%d\n", earley_set_id,
                   -- irl_id, irl_dot, trv:origin()))
              local xpr_id, xpr_dot = dotted_irl_to_xpr(slg, irl_id, irl_dot)
              for origin in origins(trv) do
                  local vlq = _M.to_vlq{ xpr_id, xpr_dot, origin }
                  uniq_items[vlq] = true
              end
              -- No rewrite creates a right-recursion, so every
              -- Leo EIM is top-level
              local at_leo = trv:at_leo()
              while at_leo do
                  local ltrv = trv:lim()
                  while ltrv do
                      local irl_id, irl_dot, origin = ltrv:trailhead_eim()
                      local xpr_id, xpr_dot = dotted_irl_to_xpr(slg, irl_id, irl_dot)
                      local vlq = _M.to_vlq{ xpr_id, xpr_dot, origin }
                      uniq_items[vlq] = true
                      ltrv = ltrv:predecessor()
                  end
                  at_leo = trv:leo_next()
              end
              ::NEXT_ITEM::
          end
          local items = {}
          for vlq, _ in  pairs(uniq_items) do
              items[#items+1] = _M.from_vlq(vlq)
          end
          table.sort(items, _M.cmp_seq)
          return items
      end
```

```
    -- miranda: section progress_line_do(), local function of slr:progress_show
    local function progress_line_do(
      current_ordinal, origins, production_id, position
      )

      -- For origins[0], we apply
      --     -1 to convert earley set to G1, then
        --     +1 because it is an origin and the character
        --        doesn't begin until the next Earley set
        -- In other words, they balance and we do nothing
        local pcs = {}


        local dotted_type
        if position >= slg:xpr_length(production_id) then
          dotted_type = 'F'
          pcs[#pcs+1] = 'F' .. production_id
          goto TAG_FOUND
        end
        if position > 0 then
          dotted_type = 'R'
          pcs[#pcs+1] = 'R' .. production_id .. ':' .. position
          goto TAG_FOUND
        end
        dotted_type = 'P'
        pcs[#pcs+1] = 'P' .. production_id
        ::TAG_FOUND::

        if #origins > 1 then
          pcs[#pcs+1] = 'x' .. #origins
        end

        -- find the range
        if current_ordinal <= 0 then
          pcs[#pcs+1] = 'B1L1c1'
        else
            local l0_locations = {}
            for _, g1_location in pairs(origins) do
                l0_locations[#l0_locations+1] = { slr:g1_to_block_first(g1_location) }
            end
            pcs[#pcs+1] = slr:lc_table_brief(l0_locations)
        end

        pcs[#pcs+1] = slg:xpr_dotted_show(production_id, position)
        return table.concat(pcs, ' ')
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.progress_show(slr, start_ordinal_arg, end_ordinal_arg)
      local slg = slr.slg
      local g1g = slg.g1
      local g1r = slr.g1
      local last_ordinal = g1r:latest_earley_set()
      local start_ordinal = math.tointeger(start_ordinal_arg) or last_ordinal
      if start_ordinal < 0 then start_ordinal = last_ordinal + 1 + start_ordinal end
      if start_ordinal > last_ordinal or start_ordinal < 0 then
        _M._internal_error(
        "Marpa::R3::Recognizer::g1_progress_show start index is %d, \z
        must be in range 0-%d",
        inspect(start_ordinal_arg, {depth=1}),
        last_ordinal
        )
      end
      local end_ordinal = math.tointeger(end_ordinal_arg) or start_ordinal
      if end_ordinal < 0 then end_ordinal = last_ordinal + 1 + end_ordinal end
      if end_ordinal > last_ordinal or end_ordinal < 0 then
        _M._internal_error(
        "Marpa::R3::Recognizer::g1_progress_show start index is %d, \z
        must be in range 0-%d",
        inspect(end_ordinal_arg, {depth=1}),
        last_ordinal
        )
      end

      -- miranda: insert progress_line_do(), local function of slr:progress_show

      local function earley_set_header(earley_set_id)
        local location = 'B1L1c1'
        if earley_set_id > 0 then
          local block, pos = slr:g1_to_block_first(earley_set_id)
          location = slr:lc_brief(block, pos)
        end
        return string.format('=== Earley set %d at %s ===', earley_set_id, location)
      end

      local items = {}
      for current_ordinal = start_ordinal, end_ordinal do
        -- io.stderr:write(string.format("earley_set_display(%d)\n", current_ordinal))
        local current_items = slr:progress(current_ordinal)
        for ix = 1, #current_items do
          local xpr_id, xpr_dot, origin = table.unpack(current_items[ix])
          -- item_type is 0 for prediction, 1 for medial, 2 for completed
          local item_type = 1
          local xpr_length = slg:xpr_length(xpr_id)
          if xpr_dot == -1 then xpr_dot = rule_length end
          if xpr_dot == 0 then item_type = 0
          elseif xpr_dot == xpr_length then item_type = 2 end
          items[#items+1] = { current_ordinal, item_type, xpr_id, xpr_dot, origin }
        end
      end

      if #items == 0 then return earley_set_header(start_ordinal) .. '\n' end
      local lines = {}
      local last_ordinal
      for this_ordinal, rule_id, position, origins in _M.collected_progress_items(items) do
        if this_ordinal ~= last_ordinal then
          lines[#lines+1] = earley_set_header(this_ordinal)
          last_ordinal = this_ordinal
        end
        lines[#lines+1] = progress_line_do(this_ordinal, origins, rule_id, position )
      end
      lines[#lines+1] = '' -- to get a final "\n"
      return table.concat(lines, "\n")
    end
```

TODO -- after development, this should be a local function.

`throw_at_pos` appends a location description to `desc`
and throws the error.
It is designed to be convenient for use as a tail call.

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.throw_at_pos(slr, desc, block_ix, pos)
      desc = desc or ''
      local current_block_ix, offset = slr:block_progress()
      block_ix = block_ix or current_block_ix
      pos = pos or offset
      local codepoint = slr:codepoint_from_pos(block_ix, pos)
      return _M.userX(
             "Error in parse: %s\n\z
              * String before error: %s\n\z
              * The error was at %s, and at character %s, ...\n\z
              * here: %s\n",
              desc,
              slr:reversed_input_escape(block_ix, pos, 50),
              slr:lc_brief(block_ix, pos),
              slr:character_describe(codepoint),
              slr:input_escape(block_ix, pos, 50)
          )
    end
```

This is not currently used.
It was created for development,
and is being kept for use as
part of a "Pure Lua" implementation.

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.leo_item_show(slr)
        local g1r = slr.g1
        local g1g = slr.slg.g1
        local leo_base_state = g1r:_leo_base_state()
        if not leo_base_state then return '' end
        local trace_earley_set = g1r:_trace_earley_set()
        local trace_earleme = g1r:earleme(trace_earley_set)
        local postdot_symbol_id = g1r:_postdot_item_symbol()
        local postdot_symbol_name = g1g:nsy_name(postdot_symbol_id)
        local predecessor_symbol_id = g1r:_leo_predecessor_symbol()
        local base_origin_set_id = g1r:_leo_base_origin()
        local base_origin_earleme = g1r:earleme(base_origin_set_id)
        local link_texts = {
            string.format("%q", postdot_symbol_name)
        }
        if predecessor_symbol_id then
            link_texts[#link_texts+1] = string.format(
                'L%d@%d', predecessor_symbol_id, base_origin_earleme
            );
        end
        link_texts[#link_texts+1] = string.format(
            'S%d@%d-%d',
            leo_base_state,
            base_origin_earleme,
            trace_earleme
        );
        return string.format('L%d@%d [%s]',
             postdot_symbol_id, trace_earleme,
             table.concat(link_texts, '; '));
    end

```

```
    -- miranda: section+ forward declarations
    local g1_progress_line_do
    -- miranda: section+ most Lua function definitions
    function g1_progress_line_do(
        slr, current_ordinal, origins, rule_id, position
    )

        -- For origins[0], we apply
        --     -1 to convert earley set to G1, then
        --     +1 because it is an origin and the character
        --        doesn't begin until the next Earley set
        -- In other words, they balance and we do nothing

        local slg = slr.slg
        local g1g = slg.g1
        local pcs = {}

        local origins_desc = #origins <= 3
             and table.concat(origins, ',')
             or origins[1] .. '...' .. origins[#origins]

        local dotted_type
        if position >= g1g:rule_length(rule_id) then
            dotted_type = 'F'
            pcs[#pcs+1] = 'F' .. rule_id
            goto TAG_FOUND
        end
        if position > 0 then
            dotted_type = 'R'
            pcs[#pcs+1] = 'R' .. rule_id .. ':' .. position
            goto TAG_FOUND
        end
        dotted_type = 'P'
        pcs[#pcs+1] = 'P' .. rule_id
        ::TAG_FOUND::

        if #origins > 1 then
            pcs[#pcs+1] = 'x' .. #origins
        end

        local current_earleme = slr.g1:earleme(current_ordinal)
        pcs[#pcs+1] = '@' .. origins_desc .. '-' .. current_earleme

        -- find the range
        if current_ordinal <= 0 then
            pcs[#pcs+1] = 'B1L1c1'
            goto HAVE_RANGE
        end
        if dotted_type == 'P' then
            local block, pos = slr:g1_to_block_first(current_ordinal)
            pcs[#pcs+1] = slr:lc_brief(block, pos)
            goto HAVE_RANGE
        end
        do
            local l0_locations = {}
            for _, g1_location in pairs(origins) do
                l0_locations[#l0_locations+1] = { slr:g1_to_block_first(g1_location) }
            end
            pcs[#pcs+1] = slr:lc_table_brief(l0_locations)
            goto HAVE_RANGE
        end
        ::HAVE_RANGE::
        pcs[#pcs+1] = slg:g1_dotted_rule_show(rule_id, position)
        return table.concat(pcs, ' ')
    end
```

TODO: Move to SLV, then delete

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.trace_valuer_step ( slr )
        return slr.slv:trace_valuer_step()
    end
```

## SLIF valuer (SLV) class

This is a registry object.

### SLV fields

```
    -- miranda: section+ class_slv field declarations
    class_slv_fields.slr = true
    class_slv_fields.max_parses = true
    class_slv_fields.regix = true
    class_slv_fields.this_step = true
    class_slv_fields.lmw_b = true
    class_slv_fields.lmw_o = true
    class_slv_fields.lmw_t = true
    class_slv_fields.lmw_v = true
    class_slv_fields.end_of_parse = true
    class_slv_fields.trace_values = true
    -- underscore ("_") to prevent override of function of same name
    class_slv_fields._ambiguity_level = true
```

```
    -- miranda: section+ populate metatables
    local class_slv_fields = {}
    -- miranda: insert class_slv field declarations
    declarations(_M.class_slv, class_slv_fields, 'slv')
```

```
    -- miranda: section+ luaL_Reg definitions
    static const struct luaL_Reg slv_methods[] = {
      {"step", lca_slv_step_meth},
      { NULL, NULL },
    };

```
### SLV constructor

Temporary constructor for development.
Creates an "slr-internal" version of slv,
which is not kept in the registry.

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.slv_new(slr, flat_args)

        local function slv_register(slv)
            local regix = _M.register(_M.registry, slv)
            slv.regix = regix
            return slv
        end

        local function no_parse(slv)
            slv._ambiguity_level = 0
            -- clearing the libmarpa objects is
            -- not necessary, but may ease the burden on
            -- memory
            slv.lmw_b = nil
            slv.lmw_o = nil
            slv.lmw_t = nil
            slv.lmw_v = nil
            return slv_register(slv)
        end

        local slv = {}
        setmetatable(slv, _M.class_slv)
        slv.slr = slr
        local slg = slr.slg
        local g1r = slr.g1

        slv.trace_values = slr.trace_values or 0
        slv.max_parses = nil
        slv:common_set(flat_args, {'end'})

        local end_of_parse = slv.end_of_parse
        if not end_of_parse or end_of_parse < 0 then
            end_of_parse = g1r:latest_earley_set()
        end
        slv.end_of_parse = end_of_parse

        _M.throw = false
        local bocage = _M.bocage_new(g1r, end_of_parse)
        _M.throw = true
        slv.lmw_b = bocage
        if not bocage then return no_parse(slv) end

        local lmw_o = _M.order_new(bocage)
        slv.lmw_o = lmw_o

        local ranking_method = slg.ranking_method
        if ranking_method == 'high_rank_only' then
            lmw_o:high_rank_only_set(1)
            lmw_o:rank()
        end
        if ranking_method == 'rule' then
            lmw_o:high_rank_only_set(0)
            lmw_o:rank()
        end
        local ambiguity_level = lmw_o:ambiguity_metric()
        if ambiguity_level > 2 then ambiguity_level = 2 end
        slv._ambiguity_level = ambiguity_level
        slv.lmw_t = _M.tree_new(lmw_o)

        return slv_register(slv)

    end
```

A common processor for
the valuator's Lua-level settings.

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slv.common_set(slv, flat_args, extra_args)
        local ok_args = {
            ['end'] = true,
            max_parses = true,
            trace_values = true
        }
        if extra_args then
            for ix = 1, #extra_args do
                ok_args[extra_args[ix]] = true
            end
        end

        for name, value in pairs(flat_args) do
           if not ok_args[name] then
               error(string.format(
                   'Bad slv named argument: %s => %s',
                   inspect(name),
                   inspect(value)
               ))
           end
        end

        local raw_value

        -- trace_values named argument --
        raw_value = flat_args.trace_values
        if raw_value then
            local value = math.tointeger(raw_value)
            if not value then
               error(string.format(
                   'Bad value for "trace_values" named argument: %s',
                   inspect(raw_value)))
            end
            if value ~= 0 then
                coroutine.yield('trace', 'Setting trace_values option to ' .. value)
            end
            slv.trace_values = value
        end

        -- 'end' named argument --
        raw_value = flat_args["end"]
        if raw_value then
            local value = math.tointeger(raw_value)
            if not value then
               error(string.format(
                   'Bad value for "end" named argument: %s',
                   inspect(raw_value)))
            end
            slv.end_of_parse = value
        end

        -- 'max_parses' named argument --
        raw_value = flat_args.max_parses
        if raw_value then
            local value = math.tointeger(raw_value)
            if not value then
               error(string.format(
                   'Bad value for "max_parses" named argument: %s',
                   inspect(raw_value)))
            end
            slv.max_parses = value
        end

    end
```

### SLV mutators

```
    -- miranda: section+ valuator Libmarpa wrapper Lua functions
    function _M.class_slv.value(slv)
        _M.wrap(function ()
            if slv._ambiguity_level <= 0 then
                return 'ok', 'undef'
            end
            local slr = slv.slr
            local slg = slr.slg
            local g1r = slr.g1

            local lmw_t = slv.lmw_t
            local lmw_o = slv.lmw_o

            local max_parses = slv.max_parses
            local parse_count = lmw_t:parse_count()
            if max_parses and parse_count > max_parses then
                error(string.format("Maximum parse count (%d) exceeded", max_parses));
            end

            local result = lmw_t:next()
            if not result then return 'ok', 'undef' end
            slv.lmw_v = _M.value_new(lmw_t)

            local trace_values = slv.trace_values
            slv.lmw_v:_trace(trace_values)

            if trace_values > 0 then
              coroutine.yield('trace',
                "valuator trace level: " ..  slv.trace_values)
            end
            slv.lmw_v.stack = {}

            while true do
                local new_values = slv:do_steps()
                local this = slv.this_step
                local step_type = this.type
                if step_type == 'MARPA_STEP_RULE' then
                    -- print(inspect(new_values, {depth=2}))
                    local sv = coroutine.yield('perl_rule_semantics', this.rule, new_values)
                    local ix = slv:stack_top_index()
                    slv:stack_set(ix, sv)
                    if slv.trace_values > 0 then
                        local nook_ix = slv.lmw_v:_nook()
                        local or_node_id = lmw_t:_nook_or_node(nook_ix)
                        local choice = lmw_t:_nook_choice(nook_ix)
                        local and_node_id = lmw_o:_and_order_get(or_node_id, choice)
                        local msg = { 'Popping', tostring(#new_values), 'values to evaluate',
                           (slv:and_node_tag(and_node_id) .. ','),
                           'rule:',
                           slg:g1_rule_show(this.rule)
                        }
                        coroutine.yield('trace', table.concat(msg, ' '))
                        msg = { 'Calculated and pushed value:' }
                        msg[#msg+1] = coroutine.yield('terse_dump', sv)
                        coroutine.yield('trace', table.concat(msg, ' '))
                    end
                    goto NEXT_STEP
                end
                if step_type == 'MARPA_STEP_NULLING_SYMBOL' then
                    local sv = coroutine.yield('perl_nulling_semantics', this.symbol)
                    local ix = slv:stack_top_index()
                    slv:stack_set(ix, sv)
                    goto NEXT_STEP
                end
                if step_type == 'MARPA_STEP_TRACE' then
                    local msg = slv:trace_valuer_step()
                    if msg then coroutine.yield('trace', msg) end
                    goto NEXT_STEP
                end
                goto LAST_STEP
                ::NEXT_STEP::
            end
            ::LAST_STEP::
            local retour = slv:stack_get(1)
            return 'ok', 'ok', retour
        end)
    end
```

#### Set the value of a stack entry

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slv.stack_set(slv, ix, v)
        local slr = slv.slr
        local stack = slv.lmw_v.stack
        stack[ix+0] = v
    end

```

### SLV accessors

#### Return the value of a stack entry

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slv.stack_get(slv, ix)
        local slr = slv.slr
        local stack = slv.lmw_v.stack
        return stack[ix+0]
    end

```

#### Return the top index of the stack

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slv.stack_top_index(slv)
        local slr = slv.slr
        return slv.this_step.result
    end

```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slv.g1_pos(slv)
        return slv.end_of_parse
    end

```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slv.g1_range(slv)
        return slv.this_step.start_es_id, slv.this_step.es_id-1
    end

```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slv.ambiguity_level(slv)
         return slv._ambiguity_level
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slv.trace_valuer_step ( slv )
        local slr = slv.slr
        if not slv.trace_values or slv.trace_values < 2 then
            return
        end
        local nook_ix = slv.lmw_v:_nook()
        local b = slv.lmw_b
        local o = slv.lmw_o
        local t = slv.lmw_t
        local g1g = slr.slg.g1
        local or_node_id = t:_nook_or_node(nook_ix)
        local choice = t:_nook_choice(nook_ix)
        local and_node_id = o:_and_order_get(or_node_id, choice)
        local trace_irl_id = b:_or_node_nrl(or_node_id)
        local or_node_position = b:_or_node_position(or_node_id)
        local irl_length = g1g:_nrl_length(trace_irl_id)
        if irl_length ~= or_node_position then
            return
        end
        local is_virtual_rhs = g1g:_nrl_is_virtual_rhs(trace_irl_id) ~= 0
        local is_virtual_lhs = g1g:_nrl_is_virtual_lhs(trace_irl_id) ~= 0
        local real_symbol_count = g1g:_real_symbol_count(trace_irl_id)
        if is_virtual_rhs and not is_virtual_lhs then
            local msg = {'Head of Virtual Rule: '}
            msg[#msg+1] = slv:and_node_tag(and_node_id)
            msg[#msg+1] = ', rule: '
            msg[#msg+1] = g1g:brief_nrl(trace_irl_id)
            msg[#msg+1] = '\n'
            msg[#msg+1] = 'Incrementing virtual rule by '
            msg[#msg+1] = real_symbol_count
            msg[#msg+1] = ' symbols\n'
            return table.concat(msg)
        end
        if is_virtual_rhs and is_virtual_lhs then
            local msg = {'Virtual Rule: '}
            msg[#msg+1] = slv:and_node_tag(and_node_id)
            msg[#msg+1] = ', rule: '
            msg[#msg+1] = g1g:brief_nrl(trace_irl_id)
            msg[#msg+1] = '\n'
            msg[#msg+1] = 'Incrementing virtual rule by '
            msg[#msg+1] = real_symbol_count
            msg[#msg+1] = '\n'
            return table.concat(msg)
        end
        if not is_virtual_rhs and is_virtual_lhs then
            local msg = {'New Virtual Rule: '}
            msg[#msg+1] = slv:and_node_tag(and_node_id)
            msg[#msg+1] = ', rule: '
            msg[#msg+1] = g1g:brief_nrl(trace_irl_id)
            msg[#msg+1] = '\n'
            msg[#msg+1] = 'Real symbol count is '
            msg[#msg+1] = real_symbol_count
            msg[#msg+1] = '\n'
            return table.concat(msg)
        end
        return
    end
```

### SLV diagnostics

These diagnostics duplicate those
in the ASF code.
It is expected that the SLV object will be deleted in
favor of an ASF-based valuer,
at which point these diagnostics should be deleted.

```
    -- miranda: section+ diagnostics
    function _M.class_slv.and_node_tag(slv, and_node_id)
        local slr = slv.slr
        local bocage = slv.lmw_b
        local parent_or_node_id = bocage:_and_node_parent(and_node_id)
        local origin = bocage:_or_node_origin(parent_or_node_id)
        local origin_earleme = slr.g1:earleme(origin)

        local current_earley_set = bocage:_or_node_set(parent_or_node_id)
        local current_earleme = slr.g1:earleme(current_earley_set)

        local cause_id = bocage:_and_node_cause(and_node_id)
        local predecessor_id = bocage:_and_node_predecessor(and_node_id)

        local middle_earley_set = bocage:_and_node_middle(and_node_id)
        local middle_earleme = slr.g1:earleme(middle_earley_set)

        local position = bocage:_or_node_position(parent_or_node_id)
        local nrl_id = bocage:_or_node_nrl(parent_or_node_id)

        local tag = { string.format("R%d:%d@%d-%d",
            nrl_id,
            position,
            origin_earleme,
            current_earleme)
        }

        if cause_id then
            tag[#tag+1] = string.format("C%d", bocage:_or_node_nrl(cause_id))
        else
            tag[#tag+1] = string.format("S%d", bocage:_and_node_symbol(and_node_id))
        end
        tag[#tag+1] = string.format("@%d", middle_earleme)
        return table.concat(tag)
    end

    function _M.class_slv.and_nodes_show(slv)
        local slr = slv.slr
        local bocage = slv.lmw_b
        local g1r = slr.g1
        local data = {}
        local id = -1
        while true do
            id = id + 1
            local parent = bocage:_and_node_parent(id)
            -- print('parent:', parent)
            if not parent then break end
            local predecessor = bocage:_and_node_predecessor(id)
            local cause = bocage:_and_node_cause(id)
            local symbol = bocage:_and_node_symbol(id)
            local origin = bocage:_or_node_origin(parent)
            local set = bocage:_or_node_set(parent)
            local nrl_id = bocage:_or_node_nrl(parent)
            local position = bocage:_or_node_position(parent)
            local origin_earleme = g1r:earleme(origin)
            local current_earleme = g1r:earleme(set)
            local middle_earley_set = bocage:_and_node_middle(id)
            local middle_earleme = g1r:earleme(middle_earley_set)
            local desc = {string.format(
                "And-node #%d: R%d:%d@%d-%d",
                id,
                nrl_id,
                position,
                origin_earleme,
                current_earleme)}
            -- Marpa::R2's show_and_nodes() had a minor bug:
            -- cause_nrl_id was not set properly and therefore
            -- not used in the sort.  That problem is fixed
            -- here.
            local cause_nrl_id = -1
            if cause then
                cause_nrl_id = bocage:_or_node_nrl(cause)
                desc[#desc+1] = 'C' .. cause_nrl_id
            else
                desc[#desc+1] = 'S' .. symbol
            end
            desc[#desc+1] = '@' .. middle_earleme
            if not symbol then symbol = -1 end
            data[#data+1] = {
                origin_earleme,
                current_earleme,
                nrl_id,
                position,
                middle_earleme,
                symbol,
                cause_nrl_id,
                table.concat(desc)
            }
        end

        table.sort(data, _M.cmp_seq)
        local result = {}
        for _,datum in pairs(data) do
            result[#result+1] = datum[#datum]
        end
        result[#result+1] = '' -- so concat adds a final '\n'
        return table.concat(result, '\n')
    end

    function _M.class_slv.or_node_tag(slv, or_node_id)
        local slr = slv.slr
        local bocage = slv.lmw_b
        local set = bocage:_or_node_set(or_node_id)
        local nrl_id = bocage:_or_node_nrl(or_node_id)
        local origin = bocage:_or_node_origin(or_node_id)
        local position = bocage:_or_node_position(or_node_id)
        return string.format("R%d:%d@%d-%d",
            nrl_id,
            position,
            origin,
            set)
    end

    function _M.class_slv.or_nodes_show(slv)
        local slr = slv.slr
        local bocage = slv.lmw_b
        local g1r = slr.g1
        local data = {}
        local id = -1
        while true do
            id = id + 1
            local origin = bocage:_or_node_origin(id)
            if not origin then break end
            local set = bocage:_or_node_set(id)
            local nrl_id = bocage:_or_node_nrl(id)
            local position = bocage:_or_node_position(id)
            local origin_earleme = g1r:earleme(origin)
            local current_earleme = g1r:earleme(set)

            local desc = {string.format(
                "R%d:%d@%d-%d",
                nrl_id,
                position,
                origin_earleme,
                current_earleme)}
            data[#data+1] = {
                origin_earleme,
                current_earleme,
                nrl_id,
                table.concat(desc)
            }
        end

        local function cmp_data(i, j)
            for ix = 1, #i do
                if i[ix] < j[ix] then return true end
                if i[ix] > j[ix] then return false end
            end
            return false
        end

        table.sort(data, cmp_data)
        local result = {}
        for _,datum in pairs(data) do
            result[#result+1] = datum[#datum]
        end
        result[#result+1] = '' -- so concat adds a final '\n'
        return table.concat(result, '\n')
    end

```

`bocage_show` returns a string which describes the bocage.

```
    -- miranda: section+ diagnostics
    function _M.class_slv.bocage_show(slv)
        local slr = slv.slr
        local bocage = slv.lmw_b
        local data = {}
        local or_node_id = -1
        while true do
            or_node_id = or_node_id + 1
            local irl_id = bocage:_or_node_nrl(or_node_id)
            if not irl_id then goto LAST_OR_NODE end
            local position = bocage:_or_node_position(or_node_id)
            local or_origin = bocage:_or_node_origin(or_node_id)
            local origin_earleme = slr.g1:earleme(or_origin)
            local or_set = bocage:_or_node_set(or_node_id)
            local current_earleme = slr.g1:earleme(or_set)
            local and_node_ids = {}
            local first_and_id = bocage:_or_node_first_and(or_node_id)
            local last_and_id = bocage:_or_node_last_and(or_node_id)
            for and_node_id = first_and_id, last_and_id do
                local symbol = bocage:_and_node_symbol(and_node_id)
                local cause_tag
                if symbol then cause_tag = 'S' .. symbol end
                local cause_id = bocage:_and_node_cause(and_node_id)
                local cause_irl_id
                if cause_id then
                    cause_irl_id = bocage:_or_node_nrl(cause_id)
                    cause_tag = slv:or_node_tag(cause_id)
                end
                local parent_tag = slv:or_node_tag(or_node_id)
                local predecessor_id = bocage:_and_node_predecessor(and_node_id)
                local predecessor_tag = "-"
                if predecessor_id then
                    predecessor_tag = slv:or_node_tag(predecessor_id)
                end
                local tag = string.format(
                    "%d: %d=%s %s %s",
                    and_node_id,
                    or_node_id,
                    parent_tag,
                    predecessor_tag,
                    cause_tag
                )
                data[#data+1] = { and_node_id, tag }
            end
            ::LAST_AND_NODE::
        end
        ::LAST_OR_NODE::

        local function cmp_data(i, j)
            if i[1] < j[1] then return true end
            return false
        end

        table.sort(data, cmp_data)
        local result = {}
        for _,datum in pairs(data) do
            result[#result+1] = datum[#datum]
        end
        result[#result+1] = '' -- so concat adds a final '\n'
        return table.concat(result, '\n')

    end

```

## Abstract Syntax Forest (ASF) class

This is a registry object.

### ASF fields

```
    -- miranda: section+ class_asf field declarations
    class_asf_fields.slr = true
    class_asf_fields.regix = true

    class_asf_fields.top_xsyid = true
    class_asf_fields.top_nsyid = true
    class_asf_fields.g1_start = true
    class_asf_fields.g1_length = true

    -- underscore ("_") to prevent override of functions of same name
    class_asf_fields._peak = true
    class_asf_fields._ambiguity_level = true
```

`glades` is a sequence containing all glades.
The sequence maps glade indexes to glades.

There are also non-sequence elements.
The non-sequence elements are keys.
The keys are used when to find a glade using
its components and-ids or or-ids,
and to prevent the creation
of duplicate glades.
Every key is a comma-separated list of integers.

If the glade is a rule glade,
and therefore contains or-ids,
the first integer is the count of or-ids.
Subsequent integers are the or-ids.

If the glade is a token glade,
and therefore contains and-ids,
the first integer is negative --
the additive inverse of the
count of and-ids.
Subsequent integers are the and-ids.

```
    -- miranda: section+ class_asf field declarations
    class_asf_fields.glades = true
```

```
    -- miranda: section+ populate metatables
    local class_asf_fields = {}
    -- miranda: insert class_asf field declarations
    declarations(_M.class_asf, class_asf_fields, 'asf')
```

```
    -- miranda: section+ luaL_Reg definitions
    static const struct luaL_Reg asf_methods[] = {
      { NULL, NULL },
    };

```
### ASF constructor

Temporary constructor for development.
Creates an "slr-internal" version of asf,
which is not kept in the registry.

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.asf_new(slr, flat_args)

        local function asf_register(asf)
            local regix = _M.register(_M.registry, asf)
            asf.regix = regix
            return asf
        end

        local function no_parse(asf)
            asf._ambiguity_level = 0
            -- clearing the libmarpa objects is
            -- not necessary, but may ease the burden on
            -- memory
            return asf_register(asf)
        end

        local asf = {}
        setmetatable(asf, _M.class_asf)
        asf.slr = slr
        local slg = slr.slg
        local g1r = slr.g1
        local g1g = slg.g1
        local raw_arg
        local latest_earley_set = g1r:latest_earley_set()

        -- 'end' named argument --
        local end_of_parse
        local end_of_parse_arg = flat_args["end"]
        if end_of_parse_arg then
            local value = math.tointeger(raw_arg)
            if not value then
               error(string.format(
                   'Bad value for "end" named argument: %s',
                   inspect(raw_arg)))
            end
            end_of_parse = value
        else
            end_of_parse = latest_earley_set
        end
        flat_args["end"] = nil
        if end_of_parse < 0 then
            end_of_parse = latest_earley_set + end_of_parse + 1
        end
        if end_of_parse < 0 then
            _M.userX([[
                End of parse (%d) is before start of parse (0)\n\z
            ]], inspect(end_of_parse_arg))
            end_of_parse = latest_earley_set + end_of_parse + 1
        end

        -- 'start' named argument --
        local g1_start
        local start_of_parse_arg = flat_args.start
        if start_of_parse_arg then
            local value = math.tointeger(raw_arg)
            if not value then
              _M.userX('Bad value for "start" named argument: %s',
                   inspect(raw_arg))
            end
            g1_start = value
        else
            g1_start = 0
        end
        flat_args.start = nil
        if g1_start < 0 then
            g1_start = g1_start + latest_earley_set + 1
        end
        if g1_start < 0 then
              _M.userX('"start" named argument is less than zero: %s',
                   inspect(start_of_parse_arg))
        end

        asf.g1_start = g1_start
        local g1_length = end_of_parse - g1_start
        asf.g1_length = g1_length

        if g1_length == 0 then
            _M.userX([[
                Zero length parse NOT YET IMPLEMENTED\n\z
            ]])
        end
        if g1_length < 0 then
            _M.userX([[
                Negative length parse requested, start=%s end=%d\n\z
                \u{20}  Marpa cannot make sense of that\n\z
            ]],
                inspect(start_of_parse_arg),
                inspect(end_of_parse_arg)
            )
        end

        -- 'top' named argument --
        local top_nsyid
        local top_xsyid_arg = flat_args["top"]
        if top_xsyid_arg then
            local value = math.tointeger(top_xsyid_arg)
            if not value or value < 0 then
               error(string.format(
                   'Bad value for "top" named argument: %s',
                   inspect(top_xsyid_arg)))
            end
            top_xsyid = value
            asf.top_xsyid = top_xsyid
            local top_xsy = slg.xsys[top_xsyid]
            top_nsyid = top_xsy.g1_nsyid
        else
            top_nsyid = g1g:_start_nsy()
        end
        flat_args.top = nil
        asf.top_nsyid = top_nsyid

        -- There is at most one non-nullable G1 ISY for
        -- an XSY.

        asf:common_set(flat_args, {})

        asf.glades = {}

            -- print('peak cause_nsyid', inspect(top_nsyid))
            -- print(g1g:nsys_show())
            -- print(slg:g1_symbols_show(3))

        local peak = glade_from_instance(asf, top_nsyid, g1_start, g1_length)
        if not peak then
            _M.userX("No parse at G1 location %d", g1_end)
        end
        asf._peak = peak

        -- TODO Delete after development
        -- print('preglade_sets: ', inspect(slg.preglade_sets))

        return asf_register(asf)

    end
```

A common processor for
the ASF's Lua-level settings.
Right now most args are constructor-only,
so all this currently does is check for
illegal named arguments.

```
    -- miranda: section+ most Lua function definitions
    function _M.class_asf.common_set(asf, flat_args, extra_args)
        local ok_args = {
        }
        if extra_args then
            for ix = 1, #extra_args do
                ok_args[extra_args[ix]] = true
            end
        end

        for name, value in pairs(flat_args) do
           if not ok_args[name] then
               error(string.format(
                   'Bad asf named argument: %s => %s',
                   inspect(name),
                   inspect(value)
               ))
           end
        end

        local raw_value

    end
```

### ASF accessors

```
    -- miranda: section+ most Lua function definitions
    function _M.class_asf.ambiguity_level(asf)
         return asf._ambiguity_level
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_asf.peak(asf)
         return asf._peak
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_asf.dump(asf)
        local peak = asf._peak
        local slr = asf.slr
        local slg = slr.slg
        local g1g = slg.g1
        local function recursive_dump(dumpee, lines, indent)
            if type(dumpee) ~= 'table' then
                return table.insert(lines, string.rep(' ', indent) .. tostring(dumpee))
            end
            if dumpee.type == 'nglade' then
                local symch_count = #dumpee
                local glade = dumpee.glade
                local id = glade:id()
                local header = string.format("Glade %s; %s @%d-%d: %d symches", id,
                    g1g:nsy_name(glade.nsyid), glade.g1_start,
                    glade.g1_start + glade.g1_length,
                    symch_count)
                table.insert(lines, string.rep(' ', indent) .. header)
                if symch_count < 1 then
                   error(string.format("Bad symch count (%d) in nglade %s",
                      symch_count, inspect(dumpee, { depth = 3 } )))
                end
                for ix = 1, #dumpee do
                    local element = dumpee[ix]
                    if type(element) == 'table' then
                        recursive_dump(element, lines, indent+2)
                    else
                        table.insert(lines, string.rep(' ', indent+2) .. element)
                    end
                end
                return
            end
            if dumpee.type == 'nsymch' then
                local partition_count = #dumpee
                local glade = dumpee.glade
                local id = glade:id()
                local symch = dumpee.symch
                local nrlid = symch:nrl_id()
                local ix = dumpee.ix
                local header = string.format("Symch %s.%d: %s; %d partitions",
                   id, ix, slg:g1_nrl_show(nrlid), partition_count)
                table.insert(lines, string.rep(' ', indent) .. header)
                if partition_count < 1 then
                   error(string.format("Bad partition count (%d) in nsymch %s",
                      partition_count, inspect(dumpee, { depth = 3 } )))
                end
                for ix = 1, #dumpee do
                    local element = dumpee[ix]
                    if type(element) == 'table' then
                        recursive_dump(element, lines, indent+2)
                    else
                        table.insert(lines, string.rep(' ', indent+2) .. element)
                    end
                end
                return
            end
            for ix = 1, #dumpee do
                local element = dumpee[ix]
                recursive_dump(element, lines, indent+2)
            end
        end

        local lines = {}
        local top_cmd = { type = 'nglade', glade = peak }

        for v in glade_values(peak, {}) do
            table.insert(top_cmd, v)
        end

        -- print(inspect(top_cmd))

        recursive_dump(top_cmd, lines, 0)
        return table.concat(lines, "\n")
    end
```

## Abstract Syntax Forest (ASF2) class

This is a registry object.

### ASF2 fields

```
    -- miranda: section+ class_asf2 field declarations
    class_asf2_fields.slr = true
    class_asf2_fields.regix = true
    class_asf2_fields.lmw_b = true
    class_asf2_fields.lmw_o = true
    class_asf2_fields.end_of_parse = true
    -- underscore ("_") to prevent override of function of same name
    class_asf2_fields._ambiguity_level = true
```

```
    -- miranda: section+ populate metatables
    local class_asf2_fields = {}
    -- miranda: insert class_asf2 field declarations
    declarations(_M.class_asf2, class_asf2_fields, 'asf')
```

```
    -- miranda: section+ luaL_Reg definitions
    static const struct luaL_Reg asf2_methods[] = {
      { NULL, NULL },
    };

```
### ASF2 constructor

Temporary constructor for development.
Creates an "slr-internal" version of asf,
which is not kept in the registry.

```
    -- miranda: section+ most Lua function definitions
    function _M.class_slr.asf2_new(slr, flat_args)

        local function asf_register(asf)
            local regix = _M.register(_M.registry, asf)
            asf.regix = regix
            return asf
        end

        local function no_parse(asf)
            asf._ambiguity_level = 0
            -- clearing the libmarpa objects is
            -- not necessary, but may ease the burden on
            -- memory
            asf.lmw_b = nil
            asf.lmw_o = nil
            return asf_register(asf)
        end

        local asf = {}
        setmetatable(asf, _M.class_asf2)
        asf.slr = slr
        local slg = slr.slg
        local g1r = slr.g1
        local g1g = slg.g1

        asf:common_set(flat_args, {'end'})

        local end_of_parse = asf.end_of_parse
        if not end_of_parse or end_of_parse < 0 then
            end_of_parse = g1r:latest_earley_set()
        end
        asf.end_of_parse = end_of_parse

        _M.throw = false
        local bocage = _M.bocage_new(g1r, end_of_parse)
        _M.throw = true
        asf.lmw_b = bocage
        if not bocage then return no_parse(asf) end

        local lmw_o = _M.order_new(bocage)
        asf.lmw_o = lmw_o

        local ranking_method = slg.ranking_method
        if ranking_method == 'high_rank_only' then
            lmw_o:high_rank_only_set(1)
            lmw_o:rank()
        end
        if ranking_method == 'rule' then
            lmw_o:high_rank_only_set(0)
            lmw_o:rank()
        end
        local ambiguity_level = lmw_o:ambiguity_metric()
        if ambiguity_level > 2 then ambiguity_level = 2 end
        asf._ambiguity_level = ambiguity_level

        return asf_register(asf)

    end
```

A common processor for
the valuator's Lua-level settings.

```
    -- miranda: section+ most Lua function definitions
    function _M.class_asf2.common_set(asf, flat_args, extra_args)
        local ok_args = {
            ['end'] = true,
        }
        if extra_args then
            for ix = 1, #extra_args do
                ok_args[extra_args[ix]] = true
            end
        end

        for name, value in pairs(flat_args) do
           if not ok_args[name] then
               error(string.format(
                   'Bad asf named argument: %s => %s',
                   inspect(name),
                   inspect(value)
               ))
           end
        end

        local raw_value

        -- 'end' named argument --
        raw_value = flat_args["end"]
        if raw_value then
            local value = math.tointeger(raw_value)
            if not value then
               error(string.format(
                   'Bad value for "end" named argument: %s',
                   inspect(raw_value)))
            end
            asf.end_of_parse = value
        end

    end
```

### ASF2 mutators

### ASF2 accessors

```
    -- miranda: section+ most Lua function definitions
    function _M.class_asf2.g1_pos(asf)
        return asf.end_of_parse
    end

```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_asf2.ambiguity_level(asf)
         return asf._ambiguity_level
    end
```

### ASF2 diagnostics

```
    -- miranda: section+ diagnostics
    function _M.class_asf2.and_node_tag(asf, and_node_id)
        local slr = asf.slr
        local bocage = asf.lmw_b
        local parent_or_node_id = bocage:_and_node_parent(and_node_id)
        local origin = bocage:_or_node_origin(parent_or_node_id)
        local origin_earleme = slr.g1:earleme(origin)

        local current_earley_set = bocage:_or_node_set(parent_or_node_id)
        local current_earleme = slr.g1:earleme(current_earley_set)

        local cause_id = bocage:_and_node_cause(and_node_id)
        local predecessor_id = bocage:_and_node_predecessor(and_node_id)

        local middle_earley_set = bocage:_and_node_middle(and_node_id)
        local middle_earleme = slr.g1:earleme(middle_earley_set)

        local position = bocage:_or_node_position(parent_or_node_id)
        local nrl_id = bocage:_or_node_nrl(parent_or_node_id)

        local tag = { string.format("R%d:%d@%d-%d",
            nrl_id,
            position,
            origin_earleme,
            current_earleme)
        }

        if cause_id then
            tag[#tag+1] = string.format("C%d", bocage:_or_node_nrl(cause_id))
        else
            tag[#tag+1] = string.format("S%d", bocage:_and_node_symbol(and_node_id))
        end
        tag[#tag+1] = string.format("@%d", middle_earleme)
        return table.concat(tag)
    end

    function _M.class_asf2.and_nodes_show(asf)
        local slr = asf.slr
        local bocage = asf.lmw_b
        local g1r = slr.g1
        local data = {}
        local id = -1
        while true do
            id = id + 1
            local parent = bocage:_and_node_parent(id)
            if not parent then break end
            local predecessor = bocage:_and_node_predecessor(id)
            local cause = bocage:_and_node_cause(id)
            local symbol = bocage:_and_node_symbol(id)
            local origin = bocage:_or_node_origin(parent)
            local set = bocage:_or_node_set(parent)
            local nrl_id = bocage:_or_node_nrl(parent)
            local position = bocage:_or_node_position(parent)
            local origin_earleme = g1r:earleme(origin)
            local current_earleme = g1r:earleme(set)
            local middle_earley_set = bocage:_and_node_middle(id)
            local middle_earleme = g1r:earleme(middle_earley_set)
            local desc = {string.format(
                "And-node #%d: R%d:%d@%d-%d",
                id,
                nrl_id,
                position,
                origin_earleme,
                current_earleme)}
            -- Marpa::R2's show_and_nodes() had a minor bug:
            -- cause_nrl_id was not set properly and therefore
            -- not used in the sort.  That problem is fixed
            -- here.
            local cause_nrl_id = -1
            if cause then
                cause_nrl_id = bocage:_or_node_nrl(cause)
                desc[#desc+1] = 'C' .. cause_nrl_id
            else
                desc[#desc+1] = 'S' .. symbol
            end
            desc[#desc+1] = '@' .. middle_earleme
            if not symbol then symbol = -1 end
            data[#data+1] = {
                origin_earleme,
                current_earleme,
                nrl_id,
                position,
                middle_earleme,
                symbol,
                cause_nrl_id,
                table.concat(desc)
            }
        end

        table.sort(data, _M.cmp_seq)
        local result = {}
        for _,datum in pairs(data) do
            result[#result+1] = datum[#datum]
        end
        result[#result+1] = '' -- so concat adds a final '\n'
        return table.concat(result, '\n')
    end

    function _M.class_asf2.or_node_tag(asf, or_node_id)
        local slr = asf.slr
        local bocage = asf.lmw_b
        local set = bocage:_or_node_set(or_node_id)
        local nrl_id = bocage:_or_node_nrl(or_node_id)
        local origin = bocage:_or_node_origin(or_node_id)
        local position = bocage:_or_node_position(or_node_id)
        return string.format("R%d:%d@%d-%d",
            nrl_id,
            position,
            origin,
            set)
    end

    function _M.class_asf2.or_nodes_show(asf)
        local slr = asf.slr
        local bocage = asf.lmw_b
        local g1r = slr.g1
        local data = {}
        local id = -1
        while true do
            id = id + 1
            local origin = bocage:_or_node_origin(id)
            if not origin then break end
            local set = bocage:_or_node_set(id)
            local nrl_id = bocage:_or_node_nrl(id)
            local position = bocage:_or_node_position(id)
            local origin_earleme = g1r:earleme(origin)
            local current_earleme = g1r:earleme(set)

            local desc = {string.format(
                "R%d:%d@%d-%d",
                nrl_id,
                position,
                origin_earleme,
                current_earleme)}
            data[#data+1] = {
                origin_earleme,
                current_earleme,
                nrl_id,
                table.concat(desc)
            }
        end

        local function cmp_data(i, j)
            for ix = 1, #i do
                if i[ix] < j[ix] then return true end
                if i[ix] > j[ix] then return false end
            end
            return false
        end

        table.sort(data, cmp_data)
        local result = {}
        for _,datum in pairs(data) do
            result[#result+1] = datum[#datum]
        end
        result[#result+1] = '' -- so concat adds a final '\n'
        return table.concat(result, '\n')
    end

```

`bocage_show` returns a string which describes the bocage.

```
    -- miranda: section+ diagnostics
    function _M.class_asf2.bocage_show(asf)
        local slr = asf.slr
        local bocage = asf.lmw_b
        local data = {}
        local or_node_id = -1
        while true do
            or_node_id = or_node_id + 1
            local irl_id = bocage:_or_node_nrl(or_node_id)
            if not irl_id then goto LAST_OR_NODE end
            local position = bocage:_or_node_position(or_node_id)
            local or_origin = bocage:_or_node_origin(or_node_id)
            local origin_earleme = slr.g1:earleme(or_origin)
            local or_set = bocage:_or_node_set(or_node_id)
            local current_earleme = slr.g1:earleme(or_set)
            local and_node_ids = {}
            local first_and_id = bocage:_or_node_first_and(or_node_id)
            local last_and_id = bocage:_or_node_last_and(or_node_id)
            for and_node_id = first_and_id, last_and_id do
                local symbol = bocage:_and_node_symbol(and_node_id)
                local cause_tag
                if symbol then cause_tag = 'S' .. symbol end
                local cause_id = bocage:_and_node_cause(and_node_id)
                local cause_irl_id
                if cause_id then
                    cause_irl_id = bocage:_or_node_nrl(cause_id)
                    cause_tag = asf:or_node_tag(cause_id)
                end
                local parent_tag = asf:or_node_tag(or_node_id)
                local predecessor_id = bocage:_and_node_predecessor(and_node_id)
                local predecessor_tag = "-"
                if predecessor_id then
                    predecessor_tag = asf:or_node_tag(predecessor_id)
                end
                local tag = string.format(
                    "%d: %d=%s %s %s",
                    and_node_id,
                    or_node_id,
                    parent_tag,
                    predecessor_tag,
                    cause_tag
                )
                data[#data+1] = { and_node_id, tag }
            end
            ::LAST_AND_NODE::
        end
        ::LAST_OR_NODE::

        local function cmp_data(i, j)
            if i[1] < j[1] then return true end
            return false
        end

        table.sort(data, cmp_data)
        local result = {}
        for _,datum in pairs(data) do
            result[#result+1] = datum[#datum]
        end
        result[#result+1] = '' -- so concat adds a final '\n'
        return table.concat(result, '\n')

    end

```

## Glade class

Not a registry object.
As of this writing,
I don't think I want to
make this a Perl object.

### Glade fields

```
    -- miranda: section+ class_glade field declarations
    -- TODO Do I need the `asf` field?
    class_glade_fields.asf = true
    class_glade_fields.regix = true
    class_glade_fields.token_symch = true
    class_glade_fields.rule_symches = true
```

With an asf, the triple of xsyid, g1_start,
and g1_length identifies a glade.
But glade objects are traversers and more
than one can be active at once.

```
    -- miranda: section+ class_glade field declarations
    class_glade_fields.nsyid = true
    class_glade_fields.g1_start = true
    class_glade_fields.g1_length = true
```

Glade type is "trivial" for the peak of a null parse;
"rule" for a normal rule glade;
"token" for a normal non-null token glade;
and "leo" for a Leo glade.

```
    -- miranda: section+ class_glade field declarations
    class_glade_fields.type = true
```

```
    -- miranda: section+ create nonmetallic metatables
    _M.class_glade = {}
    -- miranda: section+ populate metatables
    local class_glade_fields = {}

    -- miranda: insert class_glade field declarations
    declarations(_M.class_glade, class_glade_fields, 'glade')
```

### Glade accessors

```
    -- miranda: section+ most Lua function definitions
    function _M.class_glade.xsyid(glade)
        return glade.xsyid
    end
    function _M.class_glade.g1_span(glade)
        return glade.g1_start, glade.g1_length
    end
    function _M.class_glade.id(glade)
        return glade.g1_start
            .. '.' .. glade.g1_length
            .. '.' .. glade.nsyid
    end
```

#### Glade dump accessors

The glade dump accessors share the `seen` variable.
Where `id` is a glade ID, `seen[id]` is `true` if the
glade has already been dumped.

```
    -- miranda: section+ forward declarations
    local glade_partitions
    local glade_partition_gen
    -- miranda: section+ most Lua function definitions
    function glade_partitions(glade, symch, seen)
        return coroutine.wrap(function ()
            glade_partition_gen(glade, symch, seen)
        end)
    end
    function glade_partition_gen(glade, symch, seen)
        local asf = glade.asf
        local slr = asf.slr
        local slg = slr.slg
        local g1g = slg.g1
        local nsyid = glade.nsyid
        local id = glade:id()
        local g1_start = glade.g1_start
        local g1_length = glade.g1_length
        local g1_end = g1_start + g1_length

        local at_token = symch:at_token()
        while at_token do
            local literal = slr:g1_literal(g1_start, g1_length)
            local body = string.format('Token %s: "%s"',
                g1g:nsy_name(nsyid),
                literal)
            coroutine.yield{ body }
            at_token = symch:token_next()
        end

        -- Note: we never have both token and completion/Leo links

        local cause_eim_db = {} -- by origin and IRL ID
        local predecessor_eim_db = {} -- by origin
        local at_completion = symch:at_completion()
        while at_completion do
            do
                local predecessor_trv = symch:completion_predecessor()
                -- TODO optimize by memoization?  Special C call?
                --   look at this after NRLs are eliminated
                local cause_trv = symch:completion_cause()
                local cause_nrlid = cause_trv:nrl_id()
                local cause_origin = cause_trv:origin()
                local eims_by_nrlid = cause_eim_db[cause_origin]
                if not eims_by_nrlid then
                    eims_by_nrlid = {}
                    cause_eim_db[cause_origin] = eims_by_nrlid
                    -- predecessor is determined by AHM, origin and "middle"
                    -- so there is only one
                    predecessor_eim_db[cause_origin] = predecessor_trv
                end
                local cause_eim = eims_by_nrlid[cause_nrlid]
                if cause_eim then goto NEXT_COMPLETION end
                eims_by_nrlid[cause_nrlid] = cause_trv
            end
            ::NEXT_COMPLETION::
            at_completion = symch:completion_next()
        end

        -- create down glades
        local downglades = {}
        for origin, eims_by_nrlid in pairs(cause_eim_db) do
            local symch = {}
            local cause_nsyid
            for cause_nrlid, eim_trv in pairs(eims_by_nrlid) do
                if not cause_nsyid then cause_nsyid = g1g:_nrl_lhs(cause_nrlid) end
                symch[#symch+1] = eim_trv
            end
            local g1_length = g1_end - origin
            local predecessor_eim = predecessor_eim_db[origin]
            local glade = glade_from_instance(asf, cause_nsyid, origin, g1_length, symch)
            local downglade = { predecessor_eim, glade }
            downglades[#downglades+1] = downglade
        end

        for ix = 1,#downglades do
           local predecessor_eim, glade = table.unpack(downglades[ix])
           local nglade = { type = 'nglade', glade = glade }
           for v in glade_values(glade, seen) do
               table.insert(nglade, v)
           end
           coroutine.yield(nglade)
        end

        -- will I need to mix Leo and completion causes in the same
        -- glade/downglade?
        local at_leo = symch:at_leo()
        while at_leo do
            coroutine.yield{ "leo links NOT YET IMPLEMENTED" }
            at_leo = symch:leo()
        end
    end
```

```
    -- miranda: section+ forward declarations
    local glade_symch_values
    local glade_symch_values_gen
    -- miranda: section+ most Lua function definitions
    function glade_symch_values(glade, symch, seen)
        return coroutine.wrap(
            function () glade_symch_values_gen(glade, symch, seen)
        end)
    end
    function glade_symch_values_gen(glade, symch, seen)
        -- a stack containing the current RHS
        for partition in glade_partitions(glade, symch, seen) do
            coroutine.yield(partition)
        end
    end
```

```
    -- miranda: section+ forward declarations
    local glade_values
    local glade_values_gen
    -- miranda: section+ most Lua function definitions
    function glade_values(glade, seen)
        return coroutine.wrap(
            function () glade_values_gen(glade, seen)
        end)
    end
    function glade_values_gen(glade, seen)

        local function form_symch_choice(parent, ix)
           if not parent then return ix end
           return parent .. "." .. ix
        end

        local id = glade:id()
        local symch_indent = 0
        local asf = glade.asf
        local slr = asf.slr
        local slg = slr.slg
        local g1g = slg.g1

        if seen[id] == true then
            local body = string.format("Glade %s already displayed", id)
            coroutine.yield{ body }
            return
        end
        local rule_symches = glade.rule_symches
        if rule_symches then
            for ix = 1, #rule_symches do
               local symch = rule_symches[ix]
               local nsymch = { type = 'nsymch', glade = glade, symch = symch, ix = ix }
               for partition in glade_symch_values(glade, symch, seen) do
                   table.insert(nsymch, partition)
               end
               coroutine.yield(nsymch)
               ::NEXT_RULE_SYMCH::
            end
           seen[id] = true
           return
        end
        coroutine.yield{ "dump of non-rule glade NOT YET IMPLEMENTED" }
        seen[id] = true
    end
```

### Glade mutators

`glade_from_token` assumes that the caller ensured its
arguments are correct.

```
    -- miranda: section+ forward declarations
    local glade_from_token
    -- miranda: section+ most Lua function definitions
    function glade_from_token(asf, nsyid, g1_start, g1_length)
        -- TODO hash glades per asf

        local id = g1_start
            .. '.' .. g1_length
            .. '.' .. isyid
        local glade = asf.glades[id]
        if glade then return glade end

        local slr = asf.slr
        local slg = slr.slg
        local g1g = slg.g1
        glade = setmetatable({}, _M.class_glade)
        local isy = g1g.isys[isyid]
        glade.asf = asf
        glade.nsyid = nsyid
        glade.g1_start = g1_start
        glade.g1_length = g1_length
        glade.type = 'token'
        local regix = _M.register(_M.registry, glade)
        glade.regix = regix
        return glade
    end
```

`glade_from_instance` assumes that the caller ensured its
arguments are correct.

```
    -- miranda: section+ forward declarations
    local glade_from_instance
    -- miranda: section+ most Lua function definitions
    local function eimset_from_instance(asf, nsyid, g1_start, g1_length)
        local slr = asf.slr
        local g1r = slr.g1
        local slg = slr.slg
        local g1g = slg.g1
        local g1_end = g1_start + g1_length
        local max_eim = g1r:_earley_set_size(g1_end) - 1
        local eimset = {}

        for eim_id = 0, max_eim do
            -- io.stderr:write('= trying eim_id: ', eim_id, "\n")
            local trv = _M.traverser_new(g1r, g1_end, eim_id)
            local origin = trv:origin()
            if origin ~= g1_start then goto NEXT_EIM end
            local dot = trv:nrl_dot()
            if dot >= 0 then return end
            local nrlid = trv:nrl_id()
            local lh_nsyid = g1g:_nrl_lhs(nrlid)
            if lh_nsyid ~= nsyid then goto NEXT_EIM end
            eimset[#eimset+1] = trv
            ::NEXT_EIM::
        end
        return eimset
    end

    function glade_from_instance(asf, nsyid, g1_start, g1_length, eimset)
        -- TODO what if xsyid is token?
        -- TODO what if g1_start, g1_length invalid?
        -- TODO hash glades per asf

        local id = g1_start
            .. '.' .. g1_length
            .. '.' .. nsyid
        local glade = asf.glades[id]
        if glade then return glade end
        glade = setmetatable({}, _M.class_glade)
        asf.glades[id] = glade

        local slr = asf.slr
        local slg = slr.slg
        local g1g = slg.g1
        local isyid = g1g:_source_isy(nsyid)
        local isy = g1g.isys[isyid]
        local is_terminal = isy.lexeme
        if is_terminal then
            return glade_from_token(asf, nsyid, g1_start, g1_length)
        end
        if not eimset then
            eimset = eimset_from_instance(asf, nsyid, g1_start, g1_length)
        end
        -- soft failure if no match
        if #eimset < 0 then return end
        table.sort(eimset,
           function (i,j)
               return i:rule_id() < j:rule_id()
           end
        )
        glade.asf = asf
        glade.nsyid = nsyid
        glade.g1_start = g1_start
        glade.g1_length = g1_length
        glade.rule_symches = eimset
        glade.type = 'rule'
        local regix = _M.register(_M.registry, glade)
        glade.regix = regix
        return glade
    end
```

## Kollos semantics

Initially, Marpa's semantics were performed using a VM (virtual machine)
of about two dozen
operations.  I am converting them to Lua, one by one.  Once they are in
Lua, the flexibility in defining operations becomes much greater than when
they were in C/XS.  The set of operations which can be defined becomes
literally open-ended.

With Lua replacing C, the constraints which dictated the original design
of this VM are completely altered.
It remains an open question what becomes of this VM and its operation
set as Marpa evolves.
For example,
at the extreme end, every program in the old VM could be replaced with
one that is a single instruction long, with that single instruction
written entirely in Lua.
If this were done, there no longer would be a VM, in any real sense of the
word.

### VM operations

A return value of -1 indicates this should be the last VM operation.
A return value of 0 or greater indicates this is the last VM operation,
and that there is a Perl callback with the contents of the values array
as its arguments.
A return value of -2 or less indicates that the reading of VM operations
should continue.

Note the use of tails calls in the Lua code.
Maintainers should be aware that these are finicky.
In particular, while `return f(x)` is turned into a tail call,
`return (f(x))` is not.

The next function is a utility to set
up the VM table.

#### VM add op utility

```
    -- miranda: section VM utilities

        _M.vm_ops = {}
        _M.vm_op_names = {}
        _M.vm_op_keys = {}
        local function op_fn_add(name, fn)
            local ops = _M.vm_ops
            local new_ix = #ops + 1
            ops[new_ix] = fn
            ops[name] = fn
            _M.vm_op_names[new_ix] = name
            _M.vm_op_keys[name] = new_ix
            return new_ix
        end

```


#### VM debug operation

Was used for development.
Perhaps I should delete this.

```
    -- miranda: section VM operations

    local function op_fn_debug (slv)
        local slr = slv.slr
        for k,v in pairs(slr) do
            print(k, v)
        end
        mt = debug.getmetatable(slr)
        print([[=== metatable ===]])
        for k,v in pairs(mt) do
            print(k, v)
        end
        return
    end
    op_fn_add("debug", op_fn_debug)

```

#### VM no-op operation

This is to be kept after development,
even if not used.
It may be useful in debugging.

```
    -- miranda: section+ VM operations

    local function op_fn_noop (slv)
        return
    end
    op_fn_add("noop", op_fn_noop)

```

#### VM bail operation

This is to used for development.
Its intended use is as a dummy argument,
which, if it is used by accident
as a VM operation,
fast fails with a clear message.

```
    -- miranda: section+ VM operations

    local function op_fn_bail (slv)
        error('executing VM op "bail"')
    end
    op_fn_add("bail", op_fn_bail)

```

#### VM result operations

If an operation in the VM returns -1, it is a
"result operation".
The actual result is expected to be in the stack
at index `slv.this_step.result`.

The result operation is not required to be the
last operation in a sequence,
and
a sequence of operations does not have to contain
a result operation.
If there are
other operations after the result operation,
they will not be performed.
If a sequence ends without encountering a result
operation, an implicit "no-op" result operation
is assumed and, as usual,
the result is the value in the stack
at index `slv.this_step.result`.

#### VM "result is undef" operation

Perhaps the simplest operation.
The result of the semantics is a Perl undef.

```
    -- miranda: section+ VM operations

    local function op_fn_result_is_undef(slv)
        local slr = slv.slr
        local stack = slv.lmw_v.stack
        stack[slv.this_step.result] = coroutine.yield('perl_undef')
        return 'continue'
    end
    op_fn_add("result_is_undef", op_fn_result_is_undef)

```

#### VM "result is token value" operation

The result of the semantics is the value of the
token at the current location.
It's assumed to be a MARPA_STEP_TOKEN step --
if not the value is an undef.

```
    -- miranda: section+ VM operations

    local function op_fn_result_is_token_value(slv)
        local slr = slv.slr
        if slv.this_step.type ~= 'MARPA_STEP_TOKEN' then
          return op_fn_result_is_undef(slv)
        end
        local stack = slv.lmw_v.stack
        local result_ix = slv.this_step.result
        if slv.this_step.value == slr.token_is_undef then
          return op_fn_result_is_undef(slv)
        end
        stack[result_ix] = slv:current_token_value()
        return 'continue'
    end
    op_fn_add("result_is_token_value", op_fn_result_is_token_value)

```

#### VM "result is N of RHS" operation

```
    -- miranda: section+ VM operations
    local function op_fn_result_is_n_of_rhs(slv, rhs_ix)
        local slr = slv.slr
        if slv.this_step.type ~= 'MARPA_STEP_RULE' then
          return op_fn_result_is_undef(slv)
        end
        local stack = slv.lmw_v.stack
        local result_ix = slv.this_step.result
        repeat
            if rhs_ix == 0 then break end
            local fetch_ix = result_ix + rhs_ix
            if fetch_ix > slv.this_step.arg_n then
                stack[result_ix] = coroutine.yield('perl_undef')
                break
            end
            stack[result_ix] = stack[fetch_ix]
        until 1
        return 'continue'
    end
    op_fn_add("result_is_n_of_rhs", op_fn_result_is_n_of_rhs)

```

#### VM "result is N of sequence" operation

In `stack`,
set the result to the `item_ix`'th item of a sequence.
`stack` is a 0-based Perl AV.
Here "sequence" means a sequence in which the separators
have been kept.
For those with separators discarded,
the "N of RHS" operation should be used.

```
    -- miranda: section+ VM operations
    local function op_fn_result_is_n_of_sequence(slv, item_ix)
        local slr = slv.slr
        if slv.this_step.type ~= 'MARPA_STEP_RULE' then
          return op_fn_result_is_undef(slv)
        end
        local result_ix = slv.this_step.result
        local fetch_ix = result_ix + item_ix * 2
        if fetch_ix > slv.this_step.arg_n then
          return op_fn_result_is_undef(slv)
        end
        local stack = slv.lmw_v.stack
        if item_ix > 0 then
            stack[result_ix] = stack[fetch_ix]
        end
        return 'continue'
    end
    op_fn_add("result_is_n_of_sequence", op_fn_result_is_n_of_sequence)

```

#### VM operation: result is constant

Returns a constant result.

```
    -- miranda: section+ VM operations
    local function op_fn_result_is_constant(slv, constant_ix)
        local slr = slv.slr
        local stack = slv.lmw_v.stack
        local result_ix = slv.this_step.result
        stack[result_ix] = coroutine.yield('constant', constant_ix)
        if slv.trace_values > 0 and slv.this_step.type == 'MARPA_STEP_TOKEN' then
            coroutine.yield('trace',
                table.concat(
                    { "valuator unknown step", slv.this_step.type, slr.token, constant},
                    ' '))
        end
        return 'continue'
    end
    op_fn_add("result_is_constant", op_fn_result_is_constant)

```

#### Operation of the values array

The following operations add elements to the `values` array.
This is a special array which may eventually be the result of the
sequence of operations.

#### VM "push undef" operation

Push an undef on the values array.

```
    -- miranda: section+ VM operations

    local function op_fn_push_undef(slv, dummy, new_values)
        local slr = slv.slr
        local next_ix = #new_values + 1;
        local constant = coroutine.yield('perl_undef')
        new_values[next_ix] = constant
        return
    end
    op_fn_add("push_undef", op_fn_push_undef)

```

#### VM "push one" operation

Push one of the RHS child values onto the values array.

```
    -- miranda: section+ VM operations

    local function op_fn_push_one(slv, rhs_ix, new_values)
        local slr = slv.slr
        if slv.this_step.type ~= 'MARPA_STEP_RULE' then
          return op_fn_push_undef(slv, nil, new_values)
        end
        local stack = slv.lmw_v.stack
        local result_ix = slv.this_step.result
        local next_ix = #new_values + 1;
        new_values[next_ix] = stack[result_ix + rhs_ix]
        return
    end
    op_fn_add("push_one", op_fn_push_one)

```

#### Find current token literal

`current_token_value` returns the value of the current token, calculating
its literal equivalent if that is what is called for.  It assumes that
there *is* a current token, that is, it assumes that the caller has
ensured that `slv.this_step.type ~= 'MARPA_STEP_TOKEN'`.

```
    -- miranda: section+ VM operations
    function _M.class_slv.current_token_value(slv)
      local slr = slv.slr
      if slr.token_is_literal == slv.this_step.value then
          local start_es = slv.this_step.start_es_id
          local end_es = slv.this_step.es_id
          local g1_count = end_es - start_es
          local literal = slr:g1_literal(start_es, g1_count)
          return literal
      end
      return slr.token_values[slv.this_step.value]
    end

```

#### VM "push values" operation

Push the child values onto the `values` list.
If it is a token step, then
the token at the current location is pushed onto the `values` list.
If it is a nulling step, the nothing is pushed.
Otherwise the values of the RHS children are pushed.

`increment` is 2 for sequences where separators must be discarded,
1 otherwise.

```
    -- miranda: section+ VM operations

    local function op_fn_push_values(slv, increment, new_values)
        local slr = slv.slr
        if slv.this_step.type == 'MARPA_STEP_TOKEN' then
            local next_ix = #new_values + 1;
            new_values[next_ix] = slv:current_token_value()
            return
        end
        if slv.this_step.type == 'MARPA_STEP_RULE' then
            local stack = slv.lmw_v.stack
            local arg_n = slv.this_step.arg_n
            local result_ix = slv.this_step.result
            local to_ix = #new_values + 1;
            for from_ix = result_ix,arg_n,increment do
                new_values[to_ix] = stack[from_ix]
                to_ix = to_ix + 1
            end
            return
        end
        -- if 'MARPA_STEP_NULLING_SYMBOL', or unrecogized type
        return
    end
    op_fn_add("push_values", op_fn_push_values)

```

#### VM operation: push start location

The current start location in input location terms -- that is,
in terms of the input string.

```
    -- miranda: section+ VM operations
    local function op_fn_push_start(slv, dummy, new_values)
        local slr = slv.slr
        local start_es = slv.this_step.start_es_id
        local per_es = slr.per_es
        local l0_start
        start_es = start_es + 1
        if start_es > #per_es then
             local es_entry = per_es[#per_es]
             l0_start = es_entry[2] + es_entry[3]
        elseif start_es < 1 then
             l0_start = 0
        else
             local es_entry = per_es[start_es]
             l0_start = es_entry[2]
        end
        local next_ix = #new_values + 1;
        new_values[next_ix] = l0_start
        return
    end
    op_fn_add("push_start", op_fn_push_start)


```

#### VM operation: push length

The length of the current step in input location terms --
that is, in terms of the input string

TODO: Assumes that the value is all on one block.
Use `g1_span_l0_length()`?

```
    -- miranda: section+ VM operations
    local function op_fn_push_length(slv, dummy, new_values)
        local slr = slv.slr
        local start_es = slv.this_step.start_es_id
        local end_es = slv.this_step.es_id
        local per_es = slr.per_es
        local l0_length = 0
        start_es = start_es + 1
        local start_es_entry = per_es[start_es]
        if start_es_entry then
            local l0_start = start_es_entry[2]
            local end_es_entry = per_es[end_es]
            l0_length =
                end_es_entry[2] + end_es_entry[3] - l0_start
        end
        local next_ix = #new_values + 1;
        new_values[next_ix] = l0_length
        return
    end
    op_fn_add("push_length", op_fn_push_length)

```

#### VM operation: push G1 start location

The current start location in G1 location terms -- that is,
in terms of G1 Earley sets.

```
    -- miranda: section+ VM operations
    local function op_fn_push_g1_start(slv, dummy, new_values)
        local slr = slv.slr
        local next_ix = #new_values + 1;
        new_values[next_ix] = slv.this_step.start_es_id
        return
    end
    op_fn_add("push_g1_start", op_fn_push_g1_start)

```

#### VM operation: push G1 length

The length of the current step in G1 terms --
that is, in terms of G1 Earley sets.

```
    -- miranda: section+ VM operations
    local function op_fn_push_g1_length(slv, dummy, new_values)
        local slr = slv.slr
        local next_ix = #new_values + 1;
        new_values[next_ix] = (slv.this_step.es_id
            - slv.this_step.start_es_id) + 1
        return
    end
    op_fn_add("push_g1_length", op_fn_push_g1_length)

```

#### VM operation: push constant onto values array

```
    -- miranda: section+ VM operations
    local function op_fn_push_constant(slv, constant_ix, new_values)
        local slr = slv.slr
        -- io.stderr:write('constant_ix: ', constant_ix, "\n")
        local next_ix = #new_values + 1;
        local constant = coroutine.yield('constant', constant_ix)

        new_values[next_ix] = constant
        return
    end
    op_fn_add("push_constant", op_fn_push_constant)

```

#### VM operation: set the array blessing

The blessing is registered in a constant, and this operation
lets the VM know its index.  The index is cleared at the beginning
of every sequence of operations

```
    -- miranda: section+ VM operations
    local function op_fn_bless(slv, blessing_ix)
        local slr = slv.slr
        slv.this_step.blessing_ix = blessing_ix
        return
    end
    op_fn_add("bless", op_fn_bless)

```

#### VM operation: result is array

This operation tells the VM that the current `values` array
is the result of this sequence of operations.

```
    -- miranda: section+ VM operations
    local function op_fn_result_is_array(slv, dummy, new_values)
        local slr = slv.slr
        local blessing_ix = slv.this_step.blessing_ix
        if blessing_ix then
          new_values = coroutine.yield('bless', new_values, blessing_ix)
        end
        local stack = slv.lmw_v.stack
        local result_ix = slv.this_step.result
        stack[result_ix] = new_values
        return 'continue'
    end
    op_fn_add("result_is_array", op_fn_result_is_array)

```

#### VM operation: callback

Tells the VM to create a callback to Perl, with
the `values` array as an argument.
The return value of 3 is a vestige of an earlier
implementation, which returned the size of the
`values` array.

```
    -- miranda: section+ VM operations
    local function op_fn_callback(slv, dummy, new_values)
        local slr = slv.slr
        local blessing_ix = slv.this_step.blessing_ix
        local step_type = slv.this_step.type
        if step_type ~= 'MARPA_STEP_RULE'
            and step_type ~= 'MARPA_STEP_NULLING_SYMBOL'
        then
            io.stderr:write(
                'Internal error: callback for wrong step type ',
                step_type
            )
            os.exit(false)
        end
        local blessing_ix = slv.this_step.blessing_ix
        if blessing_ix then
          new_values = coroutine.yield('bless', new_values, blessing_ix)
        end
        return 'callback'
    end
    op_fn_add("callback", op_fn_callback)

```

### Run the virtual machine

Return `true` if the caller should continue reading ops,
`false` if it should call back to Perl.

```
    -- miranda: section+ VM operations
    function _M.class_slv.do_ops(slv, ops, new_values)
        local slr = slv.slr
        local op_ix = 1
        while op_ix <= #ops do
            local op_code = ops[op_ix]
            if op_code == 0 then return true end
            if op_code ~= _M.defines.op_lua then
            end
            local fn_key = ops[op_ix+1]
            local arg = ops[op_ix+2]
            if slv.trace_values >= 3 then
              local tag = 'starting lua op'
              coroutine.yield('trace',
                  table.concat({'starting op', slv.this_step.type, 'lua'}, ' '))
              coroutine.yield('trace',
                  table.concat({tag, slv.this_step.type, _M.vm_op_names[fn_key]}, ' '))
            end
            -- io.stderr:write('ops: ', inspect(_M.vm_ops), '\n')
            -- io.stderr:write('fn_key: ', inspect(fn_key), '\n')
            local op_fn = _M.vm_ops[fn_key]
            local result = op_fn(slv, arg, new_values)
            if result then return result == 'continue' end
            op_ix = op_ix + 3
        end
        return true
    end

```

### Find and perform the VM operations

Determine the appropriate VM operations for this
step, and perform them.

```
    -- miranda: section+ VM operations
    function _M.class_slv.do_steps(slv)
        local slr = slv.slr
        local grammar = slr.slg
        while true do
            local new_values = {}
            local ops = {}
            slv:step()
            if slv.this_step.type == 'MARPA_STEP_INACTIVE' then
                return new_values
            end
            if slv.this_step.type == 'MARPA_STEP_RULE' then
                ops = grammar.rule_semantics[slv.this_step.rule]
                if not ops then
                    ops = _M.rule_semantics_default
                end
                goto DO_OPS
            end
            if slv.this_step.type == 'MARPA_STEP_TOKEN' then
                ops = grammar.token_semantics[slv.this_step.symbol]
                if not ops then
                    ops = _M.token_semantics_default
                end
                goto DO_OPS
            end
            if slv.this_step.type == 'MARPA_STEP_NULLING_SYMBOL' then
                ops = grammar.nulling_semantics[slv.this_step.symbol]
                if not ops then
                    ops = _M.nulling_semantics_default
                end
                goto DO_OPS
            end
            if true then return new_values end
            ::DO_OPS::
            if not ops then
                error(string.format('No semantics defined for %s', slv.this_step.type))
            end
            local do_ops_result = slv:do_ops(ops, new_values)
            local stack = slv.lmw_v.stack
            -- truncate stack
            local above_top = slv.this_step.result + 1
            for i = above_top,#stack do stack[i] = nil end
            if not do_ops_result then return new_values end
        end
    end

```

Set up the default VM operations

```
    -- miranda: section+ VM default operations
    do
        -- we record these values to set the defaults, below
        local op_lua =  _M.defines.MARPA_OP_LUA
        local op_bail_key = _M.vm_op_keys["bail"]
        local result_is_constant_key = _M.vm_op_keys["result_is_constant"]
        local result_is_undef_key = _M.vm_op_keys["result_is_undef"]
        local result_is_token_value_key = _M.vm_op_keys["result_is_token_value"]

        _M.nulling_semantics_default = { op_lua, result_is_undef_key, op_bail_key, 0 }
        _M.token_semantics_default = { op_lua, result_is_token_value_key, op_bail_key, 0 }
        _M.rule_semantics_default = { op_lua, result_is_undef_key, op_bail_key, 0 }

    end


```

### VM-related utilities for use in the Perl code

The following operations are used by the higher-level Perl code
to set and discover various Lua values.

#### Return operation key given its name

```
    -- miranda: section Utilities for semantics
    function _M.get_op_fn_key_by_name(op_name)
        return _M.vm_op_keys[op_name]
    end

```

#### Return operation name given its key

```
    -- miranda: section+ Utilities for semantics
    function _M.get_op_fn_name_by_key(op_key)
        return _M.vm_op_names[op_key]
    end

```

```

    -- miranda: section+ most Lua function definitions
    function _M.class_slr.earley_item_data(slr, lmw_r, set_id, item_id)
        local item_data = {}
        local lmw_g = lmw_r.lmw_g

        local result = lmw_r:_earley_set_trace(set_id)
        if not result then return end

        local ahm_id_of_yim = lmw_r:_earley_item_trace(item_id)
        if not ahm_id_of_yim then return end

        local origin_set_id  = lmw_r:_earley_item_origin()
        local origin_earleme = lmw_r:earleme(origin_set_id)
        local current_earleme = lmw_r:earleme(set_id)

        local nrl_id = lmw_g:_ahm_nrl(ahm_id_of_yim)
        local dot_position = lmw_g:_ahm_position(ahm_id_of_yim)

        item_data.current_set_id = set_id
        item_data.current_earleme = current_earleme
        item_data.ahm_id_of_yim = ahm_id_of_yim
        item_data.origin_set_id = origin_set_id
        item_data.origin_earleme = origin_earleme
        item_data.nrl_id = nrl_id
        item_data.dot_position = dot_position

        do -- token links
            local symbol_id = lmw_r:_first_token_link_trace()
            local links = {}
            while symbol_id do
                links[#links+1] = slr:token_link_data(lmw_r)
                symbol_id = lmw_r:_next_token_link_trace()
            end
            item_data.token_links = links
        end

        do -- completion links
            local ahm_id = lmw_r:_first_completion_link_trace()
            local links = {}
            while ahm_id do
                links[#links+1] = lmw_r:completion_link_data(ahm_id)
                ahm_id = lmw_r:_next_completion_link_trace()
            end
            item_data.completion_links = links
        end

        do -- leo links
            local ahm_id = lmw_r:_first_leo_link_trace()
            local links = {}
            while ahm_id do
                links[#links+1] = lmw_r:leo_link_data(ahm_id)
                ahm_id = lmw_r:_next_leo_link_trace()
            end
            item_data.leo_links = links
        end

        return item_data
    end

    function _M.class_slr.earley_set_data(slr, lmw_r, set_id)
        -- print('earley_set_data(', set_id, ')')
        local lmw_g = lmw_r.lmw_g
        local data = {}

        local result = lmw_r:_earley_set_trace(set_id)
        if not result then return end

        local earleme = lmw_r:earleme(set_id)
        data.earleme = earleme

        local item_id = 0
        while true do
            local item_data = slr:earley_item_data(lmw_r, set_id, item_id)
            if not item_data then break end
            data[#data+1] = item_data
            item_id = item_id + 1
        end
        data.leo = {}
        local postdot_symbol_id = lmw_r:_first_postdot_item_trace();
        while postdot_symbol_id do
            -- If there is no base Earley item,
            -- then this is not a Leo item, so we skip it
            local leo_item_data = lmw_r:leo_item_data()
            if leo_item_data then
                data.leo[#data.leo+1] = leo_item_data
            end
            postdot_symbol_id = lmw_r:_next_postdot_item_trace()
        end
        return data
    end

    function _M.class_slr.g1_earley_set_data(slr, set_id)
        local lmw_r = slr.g1
        local result = slr:earley_set_data(lmw_r, set_id)
        return result
    end

```

```
    -- miranda: section+ various Kollos lua defines
    _M.defines.TOKEN_VALUE_IS_UNDEF = 1
    _M.defines.TOKEN_VALUE_IS_LITERAL = 2

    _M.defines.MARPA_OP_LUA = 3
    _M.defines.MARPA_OP_NOOP = 4
    _M.op_names = {
        [_M.defines.MARPA_OP_LUA] = "lua",
        [_M.defines.MARPA_OP_NOOP] = "noop",
    }

    -- miranda: section+ temporary defines
    /* TODO: Delete after development */
    #define MARPA_OP_LUA 3
    #define MARPA_OP_NOOP 4

```

### Input

```
    -- miranda: section+ diagnostics
    function _M.class_slr.character_describe(slr, codepoint)
        return string.format('U+%04x %q',
           codepoint, utf8.char(codepoint))
    end
```

```
    -- miranda: section+ diagnostics
    function _M.class_slr.input_escape(slr, block_ix, start, max_length)
        local block = slr.inputs[block_ix]
        local pos = start
        local function codes()
            return function()
                if pos > #block then return end
                local codepoint = slr:codepoint_from_pos(block_ix, pos)
                pos = pos + 1
                return codepoint
            end
        end
        return _M.factory_escape(codes, max_length)
    end

    function _M.class_slr.g1_escape(slr, g1_pos, max_length)
        -- a worst case maximum, since each g1 location will have
        --   an L0 length of at least 1
        local g1_last = g1_pos + max_length
        local function factory()
            local function iter()
                for this_block, this_start, this_len in
                    sweep_range(slr, g1_pos, g1_last)
                do
                   for this_pos = this_start, this_start + this_len - 1 do
                       local codepoint = slr:codepoint_from_pos(this_block, this_pos)
                       coroutine.yield(codepoint)
                   end
                end
            end
            return coroutine.wrap(iter)
        end
        return _M.factory_escape(factory, max_length)
    end

    function _M.factory_escape(factory, max_length)
        local length_so_far = 0
        local escapes = {}

        for codepoint in factory() do
             local escape = _M.escape_codepoint(codepoint)
             length_so_far = length_so_far + #escape
             if length_so_far > max_length then
                 length_so_far = length_so_far - #escape
                 break
             end
             escapes[#escapes+1] = escape
        end

        -- trailing spaces get special treatment
        for i = #escapes, 1, -1 do
            if escapes[i] ~= ' ' then break end
            escapes[i] = '\\s'
            -- the escaped version is one character longer
            length_so_far = length_so_far + 1
        end

        -- trim back to adjust for escaped trailing spaces
        for i = #escapes, 1, -1 do
             -- print(length_so_far, max_length)
            if length_so_far <= max_length then break end
            escapes[i] = ''
            length_so_far = length_so_far - 2
        end

        return table.concat(escapes)
    end

    function _M.class_slr.reversed_input_escape(slr, block_ix, base_pos, max_length)
        local pos = base_pos - 1
        local function codes()
            return function()
                if pos < 0 then return end
                local codepoint = slr:codepoint_from_pos(block_ix, pos)
                pos = pos - 1
                return codepoint
            end
        end
        return reverse_factory_escape(codes, max_length)
    end

```

```
    -- miranda: section+ diagnostics
    function _M.class_slr.reverse_g1_escape(slr, g1_base, max_length)

        -- a worst case maximum, since each g1 location will have
        --   an L0 length of at least 1
        local g1_last = g1_base - (max_length + 1)
        if g1_last < 0 then g1_last = 0 end

        local function factory()
            local function iter()
                for this_block, this_start, this_len in
                    reverse_sweep_range(slr, g1_last, g1_base - 1)
                do
                   for this_pos = this_start + this_len - 1, this_start, -1 do
                       local codepoint = slr:codepoint_from_pos(this_block, this_pos)
                       coroutine.yield(codepoint)
                   end
                end
            end
            return coroutine.wrap(iter)
        end
        return reverse_factory_escape(factory, max_length)
    end

    -- miranda: section+ forward declarations
    local reverse_factory_escape
    -- miranda: section+ diagnostics
    function reverse_factory_escape(factory, max_length)
        local length_so_far = 0
        local reversed = {}
        for codepoint in factory() do
             local escape = _M.escape_codepoint(codepoint)
             length_so_far = length_so_far + #escape
             if length_so_far > max_length then
                 length_so_far = length_so_far - #escape
                 break
             end
             reversed[#reversed+1] = escape
        end

        -- trailing spaces get special treatment
        for i = 1, #reversed  do
            if reversed[i] ~= ' ' then break end
            reversed[i] = '\\s'
            -- the escaped version is one character longer
            length_so_far = length_so_far + 1
        end

        -- trim back to adjust for escaped trailing spaces
        local ix = #reversed
        while length_so_far > max_length do
            local this_escape = reversed[ix]
            reversed[ix] = ''
            length_so_far = length_so_far - #this_escape
            ix = ix - 1
        end

        local escapes = {}
        for i = #reversed, 1, -1 do
            escapes[#escapes+1] = reversed[i]
        end

        return table.concat(escapes)
    end

```

## Lexeme (LEX) class

### LEX fields

```
    -- miranda: section+ class_lex field declarations
    class_lex_fields.xsy = true
    class_lex_fields.g1_isy = true
    class_lex_fields.l0_isy = true
    class_lex_fields.l0_irl = true
    class_lex_fields.assertion_id = true
```

```
    -- miranda: section+ create nonmetallic metatables
    _M.class_lex = {}
    -- miranda: section+ populate metatables
    local class_lex_fields = {}

    class_lex_fields.id = true

    -- miranda: insert class_lex field declarations
    declarations(_M.class_lex, class_lex_fields, 'xrl')
```

## External rule (XRL) class

### XRL fields

```
    -- miranda: section+ class_xrl field declarations
    class_xrl_fields.id = true
    class_xrl_fields.name = true
    class_xrl_fields.assertion = true
    class_xrl_fields.precedence_count = true
    class_xrl_fields.lhs = true
    class_xrl_fields.start = true
    class_xrl_fields.length = true
```

```
    -- miranda: section+ create nonmetallic metatables
    _M.class_xrl = {}
    -- miranda: section+ populate metatables
    local class_xrl_fields = {}

    class_xrl_fields.id = true

    -- miranda: insert class_xrl field declarations
    declarations(_M.class_xrl, class_xrl_fields, 'xrl')
```

## External production (XPR) class

### XPR fields

```
    -- miranda: section+ class_xpr field declarations
    class_xpr_fields.action = true
    class_xpr_fields.discard_separation = true
    class_xpr_fields.event_name = true
    class_xpr_fields.event_starts_active = true
    class_xpr_fields.id = true
    class_xpr_fields.length = true
    class_xpr_fields.lhs = true
    class_xpr_fields.mask = true
    class_xpr_fields.min = true
    class_xpr_fields.name = true
    class_xpr_fields.null_ranking = true
    class_xpr_fields.precedence = true
    class_xpr_fields.proper = true
    class_xpr_fields.rank = true
    class_xpr_fields.rhs = true
    class_xpr_fields.separator = true
    class_xpr_fields.start = true
    class_xpr_fields.subgrammar = true
    class_xpr_fields.symbol_as_event = true
    class_xpr_fields.xrl_name = true
```

The "blessing" facility exists to provide strings
for interpretation by the upper layer.
The Perl upper layer uses these string to bless
Kollos results into Perl packages,
whence the name.

TODO: Delete `bless`.  Rename `new_blessing`.

```
    -- miranda: section+ class_xpr field declarations
    class_xpr_fields.bless = true
    class_xpr_fields.new_blessing = true
```

```
    -- miranda: section+ create nonmetallic metatables
    _M.class_xpr = {}
    -- miranda: section+ populate metatables
    local class_xpr_fields = {}

    class_xpr_fields.id = true

    -- miranda: insert class_xpr field declarations
    declarations(_M.class_xpr, class_xpr_fields, 'xpr')
```

## Internal rule (IRL) class

### IRL fields

```
    -- miranda: section+ class_irl field declarations
    class_irl_fields.id = true
    class_irl_fields.xpr = true
    class_irl_fields.action = true
    class_irl_fields.mask = true
    class_irl_fields.xpr_dot = true
    class_irl_fields.xpr_top = true
```

`g1_lexeme` records the G1 ISYID for the lexeme
if there is one.
If this is a "discard" rule, `g1_lexeme` is 'discard'.
If this is not a "discard" rule or a lexeme rule,
`g1_lexeme` is 'ignore'.

An alternative is to have a pointer to a lexeme object
here, but this would require having a way to deal with
the "no lexeme" and "discard" situations.

```
    -- miranda: section+ class_irl field declarations
    class_irl_fields.g1_lexeme = true
```

```
    -- miranda: section+ create nonmetallic metatables
    _M.class_irl = {}
    -- miranda: section+ populate metatables
    local class_irl_fields = {}
    -- miranda: insert class_irl field declarations
    declarations(_M.class_irl, class_irl_fields, 'irl')
```

## External symbol (XSY) class

### XSY fields

```
    -- miranda: section+ class_xsy field declarations
    class_xsy_fields.assertion = true
    class_xsy_fields.id = true
    class_xsy_fields.name = true
    class_xsy_fields.lexeme_semantics = true
    class_xsy_fields.dsl_form = true
    class_xsy_fields.if_inaccessible = true
    class_xsy_fields.name_source = true
    class_xsy_fields.lexeme = true
    class_xsy_fields.is_start = true
```

The ISYs for this XSY.
For each subgrammar, only
two ISYs are allowed -- nulling
and non-nullable.
That means that, for each subgrammar,
the ISY is unique by instance,
since each instance is either zero-length
or not.

Currently, there are no nulling ISYs but
that will change when the CHAF rewrite is
moved into the Lua layer.

Only the non-precedenced ISYs are memoized
here.

```
    -- miranda: section+ class_xsy field declarations
    class_xsy_fields.g1_nsyid = true
    class_xsy_fields.g1_null_nsyid = true
    class_xsy_fields.l0_nsyid = true
    class_xsy_fields.l0_null_nsyid = true
```

The "blessing" facility exists to provide strings
for interpretation by the upper layer.
The Perl upper layer uses these string to bless
Kollos results into Perl packages,
whence the name.

TODO: Delete `blessing`.  Rename `new_blessing`.

```
    -- miranda: section+ class_xsy field declarations
    class_xsy_fields.blessing = true
    class_xsy_fields.new_blessing = true
```

```
    -- miranda: section+ create nonmetallic metatables
    _M.class_xsy = {}
    -- miranda: section+ populate metatables
    local class_xsy_fields = {}

    -- miranda: insert class_xsy field declarations
    declarations(_M.class_xsy, class_xsy_fields, 'xsy')
```

### XSY accessors

```
    -- miranda: section+ most Lua function definitions
    function _M.class_xsy.angled_form(xsy)
        local form1 = xsy.dsl_form or xsy.name
        return '<' .. form1 .. '>'
    end
    function _M.class_xsy.display_form(xsy)
        local form1 = xsy.dsl_form or xsy.name
        if form1:find(' ', 1, true) then
            return '<' .. form1 .. '>'
        end
        return form1
    end
    function _M.class_xsy.diag_form(xsy)
        return symbol_diag_form(xsy.name)
    end
```

## Inner symbol (ISY) class

### ISY fields

```
    -- miranda: section+ class_isy field declarations
    class_isy_fields.id = true
    class_isy_fields.name = true
    -- fields for use by upper layers?
    class_isy_fields.assertion = true
    class_isy_fields.pause_after = true
    class_isy_fields.pause_after_active = true
    class_isy_fields.pause_before = true
    class_isy_fields.pause_before_active = true
    class_isy_fields.priority = true
    class_isy_fields.lexeme = true
    class_isy_fields.eager = true
    class_isy_fields.character_class = true
    class_isy_fields.character_flags = true
```

`xsy_precedence` is precedence of the base xsy.

```
    -- miranda: section+ class_isy field declarations
    class_isy_fields.xsy_precedence = true
```

```
    -- miranda: section+ create nonmetallic metatables
    _M.class_isy = {}
    -- miranda: section+ populate metatables
    local class_isy_fields = {}
    -- miranda: insert class_isy field declarations
    declarations(_M.class_isy, class_isy_fields, 'isy')
```

### ISY accessors

```
    -- miranda: section+ most Lua function definitions
    function _M.class_isy.diag_form(isy)
        return symbol_diag_form(isy.name)
    end
    function _M.class_isy.display_form(isy)
        return isy:diag_form()
    end
```

## Libmarpa grammar wrapper (LMG) class

### LMG fields

```
    -- miranda: section+ class_grammar field declarations
    class_grammar_fields._libmarpa = true
    class_grammar_fields.irls = true
    class_grammar_fields.isyid_by_name = true
    class_grammar_fields.isys = true
    class_grammar_fields.lmw_g = true
    class_grammar_fields.name_by_isyid = true
    class_grammar_fields.slg = true
    class_grammar_fields.start_name = true
    class_grammar_fields.xpr_by_irlid = true
```

The `name` is used for diagnostics and errors.

```
    -- miranda: section+ class_grammar field declarations
    class_grammar_fields.name = true
```

A per-grammar table of the XSY's,
indexed by isyid.

```
    -- miranda: section+ class_grammar field declarations
    class_grammar_fields.xsys = true
```

```
    -- miranda: section+ populate metatables
    local class_grammar_fields = {}
    -- miranda: insert class_grammar field declarations
    declarations(_M.class_grammar, class_grammar_fields, 'grammar')
```

### LMG constructor

```
    -- miranda: section+ adjust metal tables
    _M.metal.grammar_new = _M.grammar_new

    -- A Libmarpa "IRL" is a KOLLOS "NRL"
    -- Change the method names to follow Kollos terminology
    -- _M.metal._marpa_g_irl_count = _M.class_grammar._irl_count
    -- _M.metal._marpa_g_irl_is_virtual_lhs = _M.class_grammar._irl_is_virtual_lhs
    -- _M.metal._marpa_g_irl_is_virtual_rhs = _M.class_grammar._irl_is_virtual_rhs
    -- _M.metal._marpa_g_irl_length = _M.class_grammar._irl_length
    -- _M.metal._marpa_g_irl_lhs = _M.class_grammar._irl_lhs
    -- _M.metal._marpa_g_irl_rank = _M.class_grammar._irl_rank
    -- _M.metal._marpa_g_irl_rhs = _M.class_grammar._irl_rhs
    -- _M.metal._marpa_g_irl_semantic_equivalent = _M.class_grammar._irl_semantic_equivalent
    -- _M.metal._marpa_g_ahm_irl = _M.class_grammar._ahm_irl
    -- _M.class_grammar._nrl_is_virtual_lhs = _M.class_grammar._irl_is_virtual_lhs
    -- _M.class_grammar._nrl_is_virtual_rhs = _M.class_grammar._irl_is_virtual_rhs
    -- _M.class_grammar._nrl_length = _M.class_grammar._irl_length
    -- _M.class_grammar._nrl_lhs = _M.class_grammar._irl_lhs
    -- _M.class_grammar._nrl_rank = _M.class_grammar._irl_rank
    -- _M.class_grammar._nrl_rhs = _M.class_grammar._irl_rhs
    -- _M.class_grammar._nrl_semantic_equivalent = _M.class_grammar._irl_semantic_equivalent
    -- _M.class_grammar._ahm_nrl = _M.class_grammar._ahm_irl
    -- _M.class_grammar._irl_is_virtual_lhs = nil
    -- _M.class_grammar._irl_is_virtual_rhs = nil
    -- _M.class_grammar._irl_length = nil
    -- _M.class_grammar._irl_lhs = nil
    -- _M.class_grammar._irl_rank = nil
    -- _M.class_grammar._irl_rhs = nil
    -- _M.class_grammar._irl_semantic_equivalent = nil
    -- _M.class_grammar._ahm_irl = nil

    -- miranda: section+ most Lua function definitions
    function _M.grammar_new(slg, name)
        local grammar = _M.metal.grammar_new()
        setmetatable(grammar, _M.class_grammar)
        grammar.name = name
        grammar:force_valued()
        grammar.isyid_by_name = {}
        grammar.name_by_isyid = {}
        grammar.irls = {}
        grammar.isys = {}
        grammar.slg = slg
        grammar.xpr_by_irlid = {}
        grammar.xsys = {}

        return grammar
    end

```

```
    -- miranda: section+ adjust metal tables
    _M.metal_grammar.symbol_new = _M.class_grammar.symbol_new
    -- miranda: section+ most Lua function definitions
    function _M.class_grammar.symbol_new(grammar, symbol_name)
        local symbol_id = _M.metal_grammar.symbol_new(grammar)
        local symbol = setmetatable({}, _M.class_isy)
        symbol.id = symbol_id
        symbol.name = symbol_name
        grammar.isyid_by_name[symbol_name] = symbol_id
        grammar.name_by_isyid[symbol_id] = symbol_name
        return symbol
    end

```

```
    -- miranda: section+ grammar Libmarpa wrapper Lua functions

    function _M.class_grammar.symbol_name(lmw_g, symbol_id)
        local symbol_name = lmw_g.name_by_isyid[symbol_id]
        return symbol_name
    end

    function _M.class_grammar.irl_isyids(lmw_g, rule_id)
        local lhs = lmw_g:rule_lhs(rule_id)
        if not lhs then return end
        local symbols = { lhs }
        for rhsix = 0, lmw_g:rule_length(rule_id) - 1 do
             symbols[#symbols+1] = lmw_g:rule_rhs(rule_id, rhsix)
        end
        return symbols
    end

    function _M.class_grammar.ahm_describe(lmw_g, ahm_id)
        local irl_id = lmw_g:_ahm_nrl(ahm_id)
        local dot_position = lmw_g:_ahm_position(ahm_id)
        if dot_position < 0 then
            return string.format('R%d$', irl_id)
        end
        return string.format('R%d:%d', irl_id, dot_position)
    end

    function _M.class_grammar._dotted_nrl_show(lmw_g, nrl_id, dot_position)
        local lhs_id = lmw_g:_nrl_lhs(nrl_id)
        local nrl_length = lmw_g:_nrl_length(nrl_id)
        local lhs_name = lmw_g:nsy_name(lhs_id)
        local pieces = { lhs_name, '::=' }
        if dot_position < 0 then
            dot_position = nrl_length
        end
        for ix = 0, nrl_length - 1 do
            local rhs_nsy_id = lmw_g:_nrl_rhs(nrl_id, ix)
            local rhs_nsy_name = lmw_g:nsy_name(rhs_nsy_id)
            if ix == dot_position then
                pieces[#pieces+1] = '.'
            end
            pieces[#pieces+1] = rhs_nsy_name
        end
        if dot_position >= nrl_length then
            pieces[#pieces+1] = '.'
        end
        return table.concat(pieces, ' ')
    end
```

```
    -- miranda: section+ grammar Libmarpa wrapper Lua functions
    function _M.class_slg.lmg_nrl_show(slg, subg_name, nrl_id)
        local lmw_g = slg[subg_name]
        local lhs_id = lmw_g:_nrl_lhs(nrl_id)
        local nrl_length = lmw_g:_nrl_length(nrl_id)
        local lhs_name = lmw_g:nsy_name(lhs_id)
        local pieces = { lhs_name, '::=' }
        for ix = 0, nrl_length - 1 do
            local rhs_nsy_id = lmw_g:_nrl_rhs(nrl_id, ix)
            local rhs_nsy_name = lmw_g:nsy_name(rhs_nsy_id)
            pieces[#pieces+1] = rhs_nsy_name
        end
        return table.concat(pieces, ' ')
    end
    function _M.class_slg.g1_nrl_show(slg, nrlid)
        return slg:lmg_nrl_show('g1', nrlid)
    end
    function _M.class_slg.l0_nrl_show(slg, nrlid)
        return slg:lmg_nrl_show('l0', nrlid)
    end

```

```
    -- miranda: section+ grammar Libmarpa wrapper Lua functions
    function _M.class_slg.lmg_nrls_show(slg, subg_name)
        local lmw_g = slg[subg_name]
        local nrl_count = lmw_g:_nrl_count()
        local pieces = {}
        for nrlid = 0, nrl_count-1 do
            table.insert(pieces,
                nrlid .. ': ' .. slg:lmg_nrl_show(subg_name, nrlid))
        end
        return table.concat(pieces, '\n')
    end
    function _M.class_slg.g1_nrls_show(slg)
        return slg:lmg_nrls_show('g1')
    end
    function _M.class_slg.l0_nrls_show(slg)
        return slg:lmg_nrls_show('l0')
    end

```

```
    -- miranda: section+ grammar Libmarpa wrapper Lua functions

    function _M.class_grammar.nsy_name(grammar, nsy_id_arg)
        -- start symbol
        local nsy_id = math.tointeger(nsy_id_arg)
        -- print('nsy_id_arg =', inspect(nsy_id_arg))
        if not nsy_id then error('Bad nsy_name() symbol ID arg: ' .. inspect(nsy_id_arg)) end

        -- sequence LHS
        local lhs_xrl = grammar:_nsy_lhs_irl(nsy_id)
        if lhs_xrl and grammar:sequence_min(lhs_xrl) then
            local original_lhs_id = grammar:rule_lhs(lhs_xrl)
            local lhs_name = grammar:symbol_name(original_lhs_id)
            return lhs_name .. "[Seq]"
        end

        -- virtual symbol
        local xrl_offset = grammar:_nsy_irl_offset(nsy_id)
        if xrl_offset and xrl_offset > 0 then
            local original_lhs_id = grammar:rule_lhs(lhs_xrl)
            local lhs_name = grammar:symbol_name(original_lhs_id)
            return string.format("%s[R%d:%d]",
                lhs_name, lhs_xrl, xrl_offset)
        end

        local suffix = ''
        local is_nulling = grammar:_nsy_is_nulling(nsy_id)
        if is_nulling then suffix = suffix .. "[]" end

        local isy_id = grammar:_source_isy(nsy_id)
        if not isy_id then
            return string.format("[NO_ISYID:nsy=%d]%s", nsy_id, suffix)
        end

        local xsy_id = grammar:xsyid(isy_id)
        if not xsy_id then
            return string.format("[NO_XSYID:nsy=%d:isyid=%d]%s", nsy_id, isy_id, suffix)
        end

        local slg = grammar.slg

        local xsy_name = slg:symbol_name(xsy_id)
        if xsy_name then
            return xsy_name .. suffix
        end

        return string.format("[nsy=%d:xsy=%d]%s", nsy_id, xsy_id, suffix)
    end

    function _M.class_grammar.ahm_show(lmw_g, ahm_id, options)
        local options = options or {}
        local verbose = options.verbose or 0
        local postdot_id = lmw_g:_ahm_postdot(ahm_id)
        local pieces = { "AHM " .. ahm_id .. ': ' }
        local properties = {}
        if not postdot_id then
            properties[#properties+1] = 'completion'
        end
        local dot_position = lmw_g:_ahm_raw_position(ahm_id)
        properties[#properties+1] = 'dot=' .. dot_position
        local null_count = lmw_g:_ahm_null_count(ahm_id)
        properties[#properties+1] = 'nulls=' .. null_count
        pieces[#pieces+1] = table.concat(properties, '; ')
        pieces[#pieces+1] = "\n    "
        local irl_id = lmw_g:_ahm_nrl(ahm_id)
        pieces[#pieces+1] = lmw_g:_dotted_nrl_show(irl_id, dot_position)
        pieces[#pieces+1] = '\n'
        if verbose > 0 then
            local slg = lmw_g.slg
            local body = inspect(slg.preglade_sets[ahm_id],
               { indent = '        ' })
            local indented = body == '{}' and body or body:gsub('}$', '    }')
            pieces[#pieces+1] = '    ' .. indented .. '\n'
        end
        return table.concat(pieces)
    end

    function _M.class_grammar.ahms_show(lmw_g, options)
        local pieces = {}
        local count = lmw_g:_ahm_count()
        for i = 0, count -1 do
            pieces[#pieces+1] = lmw_g:ahm_show(i, options)
        end
        return table.concat(pieces)
    end

    function _M.class_grammar.nsy_show(lmw_g, nsy_id)
        local name = lmw_g:nsy_name(nsy_id)
        local pieces = { string.format("%d: %s", nsy_id, name) }
        local tags = {}
        local is_nulling = lmw_g:_nsy_is_nulling(nsy_id)
        if is_nulling then
        tags[#tags+1] = 'nulling'
        end
        if #tags > 0 then
            pieces[#pieces+1] = ', ' .. table.concat(tags, ' ')
        end
        pieces[#pieces+1] = '\n'
        return table.concat(pieces)
    end

    function _M.class_grammar.nsys_show(lmw_g)
        local nsy_count = lmw_g:_nsy_count()
        local pieces = {}
        for nsy_id = 0, nsy_count - 1 do
            pieces[#pieces+1] = lmw_g:nsy_show(nsy_id)
        end
        return table.concat(pieces)
    end

    function _M.class_grammar.brief_nrl(lmw_g, nrl_id)
        local pieces = { string.format("%d:", nrl_id) }
        local lhs_id = lmw_g:_nrl_lhs(nrl_id)
        pieces[#pieces+1] = lmw_g:nsy_name(lhs_id)
        pieces[#pieces+1] = "::="
        local rh_length = lmw_g:_nrl_length(nrl_id)
        if rh_length > 0 then
           for rhs_ix = 0, rh_length - 1 do
              local this_rhs_id = lmw_g:_nrl_rhs(nrl_id, rhs_ix)
              pieces[#pieces+1] = lmw_g:nsy_name(this_rhs_id)
           end
        end
        return table.concat(pieces, " ")
    end

```

### LMG accessors

`isy_key` is an ISY id.

TODO: Perhaps `isy_key` should also allow isy tables.

```
    -- miranda: section+ most Lua function definitions
    function _M.class_grammar._xsy(grammar, isy_key)
        return grammar.xsys[isy_key]
    end
    function _M.class_grammar.xsyid(grammar, isy_key)
        local xsy = grammar:_xsy(isy_key)
        if not xsy then
            _M._internal_error(
            -- _M.userX(
               "grammar:xsyid(%s): no such xsy",
               inspect(isy_key))
        end
        return xsy.id
    end
    function _M.class_grammar.xsyid_by_nsy(grammar, nsy_id)
        local isy_id = grammar:_source_isy(nsy_id)
        -- print(string.format("isy_id=%d nsy_id=%d", isy_id, nsy_id))
        if not isy_id then return end
        return grammar:xsyid(isy_id)
    end
```

```
    -- miranda: section+ most Lua function definitions
    function _M.class_grammar.symbol_dsl_form(grammar, xsyid)
        local xsy = grammar.xsys[xsyid]
        if xsy then return xsy.dsl_form end
        return
    end
```

Find a displayable
name for an ISYID.
If need be,
return a name that is actually a soft error
message.
The "forced" name is not
necessarily unique.

```
    -- miranda: section+ most Lua function definitions
    function _M.class_grammar.symbol_angled_form(grammar, isyid)
        local xsy = grammar.xsys[isyid]
        if xsy then return xsy:angled_form() end
        local isy = grammar.isys[isyid]
        if isy then return isy:angled_form() end
        return '<bad isyid ' .. isyid .. '>'
    end
    function _M.class_grammar.symbol_display_form(grammar, isyid)
        local xsy = grammar.xsys[isyid]
        if xsy then return xsy:display_form() end
        local isy = grammar.isys[isyid]
        if isy then return isy:display_form() end
        return '<bad isyid ' .. isyid .. '>'
    end
```

Given an ISYID,
display the ISY in diag form.
If need be,
return a name that is actually a soft error
message.
If the ISYID is valid, the name is unique.

```
    -- miranda: section+ most Lua function definitions
    function _M.class_grammar.symbol_diag_form(grammar, isyid)
        local isy = grammar.isys[isyid]
        if isy then return isy:diag_form() end
        return '[bad isyid ' .. isyid .. ']'
    end
```

## Libmarpa recognizer wrapper (LMR) class

### LMR fields

```
    -- miranda: section+ class_recce field declarations
    class_recce_fields._libmarpa = true
    class_recce_fields.lmw_g = true
```

```
    -- miranda: section+ populate metatables
    local class_recce_fields = {}
    -- miranda: insert class_recce field declarations
    declarations(_M.class_recce, class_recce_fields, 'recce')
```

### Functions for tracing Earley sets

```
    -- miranda: section+ recognizer Libmarpa wrapper Lua functions
    function _M.class_recce.leo_item_data(lmw_r)
        local lmw_g = lmw_r.lmw_g
        local leo_base_state = lmw_r:_leo_base_state()
        if not leo_base_state then return end
        local trace_earley_set = lmw_r:_trace_earley_set()
        local trace_earleme = lmw_r:earleme(trace_earley_set)
        local postdot_symbol_id = lmw_r:_postdot_item_symbol()
        local postdot_symbol_name = lmw_g:nsy_name(postdot_symbol_id)
        local predecessor_symbol_id = lmw_r:_leo_predecessor_symbol()
        local base_origin_set_id = lmw_r:_leo_base_origin()
        local base_origin_earleme = lmw_r:earleme(base_origin_set_id)
        return {
            postdot_symbol_name = postdot_symbol_name,
            postdot_symbol_id = postdot_symbol_id,
            predecessor_symbol_id = predecessor_symbol_id,
            base_origin_earleme = base_origin_earleme,
            leo_base_state = leo_base_state,
            trace_earleme = trace_earleme
        }
    end

    function _M.class_recce.completion_link_data(lmw_r, ahm_id)
        local result = {}
        local predecessor_state = lmw_r:_source_predecessor_state()
        local origin_set_id = lmw_r:_earley_item_origin()
        local origin_earleme = lmw_r:earleme(origin_set_id)
        local middle_set_id = lmw_r:_source_middle()
        local middle_earleme = lmw_r:earleme(middle_set_id)
        result.predecessor_state = predecessor_state
        result.origin_earleme = origin_earleme
        result.middle_earleme = middle_earleme
        result.middle_set_id = middle_set_id
        result.ahm_id = ahm_id
        return result
    end

    function _M.class_recce.leo_link_data(lmw_r, ahm_id)
        local result = {}
        local middle_set_id = lmw_r:_source_middle()
        local middle_earleme = lmw_r:earleme(middle_set_id)
        local leo_transition_symbol = lmw_r:_source_leo_transition_symbol()
        result.middle_earleme = middle_earleme
        result.leo_transition_symbol = leo_transition_symbol
        result.ahm_id = ahm_id
        return result
    end
```

## Libmarpa traverser wrapper class


```
    -- miranda: section+ class_traverser field declarations
    class_traverser_fields._libmarpa = true
    class_traverser_fields.lmw_g = true
```

```
    -- miranda: section+ populate metatables
    local class_traverser_fields = {}
    -- miranda: insert class_traverser field declarations
    declarations(_M.class_traverser, class_traverser_fields, 'traverser')
```

## Libmarpa valuer wrapper class

The "valuator" portion of Kollos produces the
value of a
Kollos parse.

TODO:
Currently this is part of SLIF recognizer.
Part of this will be broken out into a new "SLIF valuer"
object
and the rest will become a cleaner
Libmarpa wrapper class.

### Adjust metal tables

```
    -- miranda: section+ adjust metal tables
    -- _M.metal._marpa_b_or_node_irl = _M.class_bocage._or_node_irl
    -- _M.class_bocage._or_node_nrl = _M.class_bocage._or_node_irl
    -- _M.class_bocage._or_node_irl = nil
```

## Libmarpa interface

```
    --[==[ miranda: exec libmarpa interface globals

    function c_type_of_libmarpa_type(libmarpa_type)
        if (libmarpa_type == 'int') then return 'int' end
        if (libmarpa_type == 'Marpa_And_Node_ID') then return 'int' end
        if (libmarpa_type == 'Marpa_Assertion_ID') then return 'int' end
        if (libmarpa_type == 'Marpa_AHM_ID') then return 'int' end
        if (libmarpa_type == 'Marpa_Earley_Item_ID') then return 'int' end
        if (libmarpa_type == 'Marpa_Earley_Set_ID') then return 'int' end
        if (libmarpa_type == 'Marpa_NRL_ID') then return 'int' end
        if (libmarpa_type == 'Marpa_Nook_ID') then return 'int' end
        if (libmarpa_type == 'Marpa_NSY_ID') then return 'int' end
        if (libmarpa_type == 'Marpa_Or_Node_ID') then return 'int' end
        if (libmarpa_type == 'Marpa_Rank') then return 'int' end
        if (libmarpa_type == 'Marpa_Rule_ID') then return 'int' end
        if (libmarpa_type == 'Marpa_Symbol_ID') then return 'int' end
        return "!UNIMPLEMENTED!";
    end

    libmarpa_class_type = {
      g = "Marpa_Grammar",
      r = "Marpa_Recognizer",
      ltrv = "Marpa_LTraverser",
      ptrv = "Marpa_PTraverser",
      trv = "Marpa_Traverser",
      b = "Marpa_Bocage",
      o = "Marpa_Order",
      t = "Marpa_Tree",
      v = "Marpa_Value",
    };

    libmarpa_class_name = {
      g = "grammar",
      r = "recce",
      ltrv = "ltraverser",
      ptrv = "ptraverser",
      trv = "traverser",
      b = "bocage",
      o = "order",
      t = "tree",
      v = "value",
    };

    libmarpa_base_class = {
      r = "g",
      b = "r",
      o = "b",
      t = "o",
      v = "t",
    };

    function wrap_libmarpa_method(signature)
       local arg_count = math.floor(#signature/2)
       local function_name = signature[1]
       local unprefixed_name = string.gsub(function_name, "^[_]?marpa_", "");
       local class_letter = string.gsub(unprefixed_name, "_.*$", "");
       local wrapper_name = "wrap_" .. unprefixed_name;
       local result = {}
       result[#result+1] = "static int " .. wrapper_name .. "(lua_State *L)\n"
       result[#result+1] = "{\n"
       result[#result+1] = "  " .. libmarpa_class_type[class_letter] .. " self;\n"
       result[#result+1] = "  const int self_stack_ix = 1;\n"
       for arg_ix = 1, arg_count do
         local arg_type = signature[arg_ix*2]
         local arg_name = signature[1 + arg_ix*2]
         result[#result+1] = "  " .. arg_type .. " " .. arg_name .. ";\n"
       end
       result[#result+1] = "  int result;\n\n"

       result[#result+1] = "  if (1) {\n"
       result[#result+1] = "    marpa_luaL_checktype(L, self_stack_ix, LUA_TTABLE);"
       -- TODO: Should I get the values from the integer checks?
       for arg_ix = 1, arg_count do
           result[#result+1] = "    marpa_luaL_checkinteger(L, " .. (arg_ix+1) .. ");\n"
       end
       result[#result+1] = "  }\n"

       for arg_ix = arg_count, 1, -1 do
         local arg_type = signature[arg_ix*2]
         local arg_name = signature[1 + arg_ix*2]
         local c_type = c_type_of_libmarpa_type(arg_type)
         assert(c_type == "int", ("type " .. arg_type .. " not implemented"))
         result[#result+1] = "{\n"
         result[#result+1] = "  const lua_Integer this_arg = marpa_lua_tointeger(L, -1);\n"

         -- Each call checks that its arguments are in range
         -- the point of this check is to make sure that C's integer conversions
         -- do not change the value before the call gets it.
         -- We assume that all types involved are at least 32 bits and signed, so that
         -- values from -2^30 to 2^30 will be unchanged by any conversions.
         result[#result+1] = [[  marpa_luaL_argcheck(L, (-(1<<30) <= this_arg && this_arg <= (1<<30)), -1, "argument out of range");]], "\n"

         result[#result+1] = string.format("  %s = (%s)this_arg;\n", arg_name, arg_type)
         result[#result+1] = "  marpa_lua_pop(L, 1);\n"
         result[#result+1] = "}\n"
       end

       result[#result+1] = '  marpa_lua_getfield (L, -1, "_libmarpa");\n'
       -- stack is [ self, self_ud ]
       local cast_to_ptr_to_class_type = "(" ..  libmarpa_class_type[class_letter] .. "*)"
       result[#result+1] = "  self = *" .. cast_to_ptr_to_class_type .. "marpa_lua_touserdata (L, -1);\n"
       result[#result+1] = "  marpa_lua_pop(L, 1);\n"
       -- stack is [ self ]

       -- assumes converting result to int is safe and right thing to do
       -- if that assumption is wrong, generate the wrapper by hand
       result[#result+1] = "  result = (int)" .. function_name .. "(self\n"
       for arg_ix = 1, arg_count do
         local arg_name = signature[1 + arg_ix*2]
         result[#result+1] = "     ," .. arg_name .. "\n"
       end
       result[#result+1] = "    );\n"

       result[#result+1] = "  if (result < -1) {\n"
       result[#result+1] = string.format(
                            "   return libmarpa_error_handle(L, self_stack_ix, %q);\n",
                            wrapper_name .. '()')
       result[#result+1] = "  }\n"

       if signature.return_type == 'boolean' then
           result[#result+1] = "  if (result == -1) {\n"
           result[#result+1] = '      return internal_error_handle(L, "unexpected soft error", __PRETTY_FUNCTION__, __FILE__, __LINE__);'
           result[#result+1] = "  }\n"
           result[#result+1] = "  marpa_lua_pushboolean(L, (result ? 1 : 0));\n"
       else
           result[#result+1] = "  if (result == -1) { marpa_lua_pushnil(L); return 1; }\n"
           result[#result+1] = "  marpa_lua_pushinteger(L, (lua_Integer)result);\n"
       end
       result[#result+1] = "  return 1;\n"
       result[#result+1] = "}\n\n"

       return table.concat(result, '')

    end

    -- end of exec
    ]==]
```

### Standard template methods

Here are the meta-programmed wrappers --
This is Lua code which writes the C code based on
a "signature" for the wrapper

This meta-programming does not attempt to work for
all of the wrappers.  It works only when
1. The number of arguments is fixed.
2. Their type is from a fixed list.
3. Converting the return value to int is a good thing to do.
4. Non-negative return values indicate success
5. Return values less than -1 indicate failure
6. Return values less than -1 set the error code
7. Return value of -1 is "soft" and returning nil is
      the right thing to do

On those methods for which the wrapper requirements are "bent"
a little bit:

* marpa_r_alternative() -- generates events
Returns an error code.  Since these are always non-negative, from
the wrapper's point of view, marpa_r_alternative() always succeeds.

* marpa_r_earleme_complete() -- generates events

```

  -- miranda: section standard libmarpa wrappers
  --[==[ miranda: exec declare standard libmarpa wrappers
  signatures = {
    {"marpa_g_completion_symbol_activate", "Marpa_Symbol_ID", "sym_id", "int", "activate"},
    {"marpa_g_default_rank"},
    {"marpa_g_default_rank_set", "Marpa_Rank", "rank" },
    {"marpa_g_error_clear"},
    {"marpa_g_event_count"},
    {"marpa_g_force_valued"},
    {"marpa_g_has_cycle"},
    {"marpa_g_highest_rule_id"},
    {"marpa_g_highest_symbol_id"},
    {"marpa_g_is_precomputed"},
    {"marpa_g_nulled_symbol_activate", "Marpa_Symbol_ID", "sym_id", "int", "activate"},
    {"marpa_g_prediction_symbol_activate", "Marpa_Symbol_ID", "sym_id", "int", "activate"},
    {"marpa_g_rule_is_accessible", "Marpa_Rule_ID", "rule_id"},
    {"marpa_g_rule_is_loop", "Marpa_Rule_ID", "rule_id"},
    {"marpa_g_rule_is_nullable", "Marpa_Rule_ID", "rule_id"},
    {"marpa_g_rule_is_nulling", "Marpa_Rule_ID", "rule_id"},
    {"marpa_g_rule_is_productive", "Marpa_Rule_ID", "rule_id"},
    {"marpa_g_rule_is_proper_separation", "Marpa_Rule_ID", "rule_id"},
    {"marpa_g_rule_length", "Marpa_Rule_ID", "rule_id"},
    {"marpa_g_rule_lhs", "Marpa_Rule_ID", "rule_id"},
    {"marpa_g_rule_null_high", "Marpa_Rule_ID", "rule_id"},
    {"marpa_g_rule_null_high_set", "Marpa_Rule_ID", "rule_id", "int", "flag"},
    {"marpa_g_rule_rank", "Marpa_Rule_ID", "rule_id" },
    {"marpa_g_rule_rhs", "Marpa_Rule_ID", "rule_id", "int", "ix"},
    {"marpa_g_sequence_min", "Marpa_Rule_ID", "rule_id"},
    {"marpa_g_sequence_separator", "Marpa_Rule_ID", "rule_id"},
    {"marpa_g_start_symbol"},
    {"marpa_g_start_symbol_set", "Marpa_Symbol_ID", "id"},
    {"marpa_g_symbol_is_accessible", "Marpa_Symbol_ID", "symbol_id"},
    {"marpa_g_symbol_is_completion_event", "Marpa_Symbol_ID", "sym_id"},
    {"marpa_g_symbol_is_completion_event_set", "Marpa_Symbol_ID", "sym_id", "int", "value"},
    {"marpa_g_symbol_is_counted", "Marpa_Symbol_ID", "symbol_id"},
    {"marpa_g_symbol_is_nullable", "Marpa_Symbol_ID", "symbol_id"},
    {"marpa_g_symbol_is_nulled_event", "Marpa_Symbol_ID", "sym_id"},
    {"marpa_g_symbol_is_nulled_event_set", "Marpa_Symbol_ID", "sym_id", "int", "value"},
    {"marpa_g_symbol_is_nulling", "Marpa_Symbol_ID", "symbol_id"},
    {"marpa_g_symbol_is_prediction_event", "Marpa_Symbol_ID", "sym_id"},
    {"marpa_g_symbol_is_prediction_event_set", "Marpa_Symbol_ID", "sym_id", "int", "value"},
    {"marpa_g_symbol_is_productive", "Marpa_Symbol_ID", "symbol_id"},
    {"marpa_g_symbol_is_start", "Marpa_Symbol_ID", "symbol_id", return_type = 'boolean'},
    {"marpa_g_symbol_is_terminal", "Marpa_Symbol_ID", "symbol_id"},
    {"marpa_g_symbol_is_terminal_set", "Marpa_Symbol_ID", "symbol_id", "int", "boolean"},
    {"marpa_g_symbol_is_valued", "Marpa_Symbol_ID", "symbol_id"},
    {"marpa_g_symbol_is_valued_set", "Marpa_Symbol_ID", "symbol_id", "int", "boolean"},
    {"marpa_g_symbol_new"},
    {"marpa_g_symbol_rank", "Marpa_Symbol_ID", "symbol_id" },
    {"marpa_g_symbol_rank_set", "Marpa_Symbol_ID", "symbol_id", "Marpa_Rank", "rank" },
    {"marpa_g_zwa_new", "int", "default_value"},
    {"marpa_g_zwa_place", "Marpa_Assertion_ID", "zwaid", "Marpa_Rule_ID", "irl_id", "int", "rhs_ix"},
    {"marpa_r_completion_symbol_activate", "Marpa_Symbol_ID", "sym_id", "int", "reactivate"},
    {"marpa_r_alternative", "Marpa_Symbol_ID", "token", "int", "value", "int", "length"}, -- See above,
    {"marpa_r_current_earleme"},
    {"marpa_r_earleme_complete"}, -- See note above,
    {"marpa_r_earleme", "Marpa_Earley_Set_ID", "ordinal"},
    {"marpa_r_earley_item_warning_threshold"},
    {"marpa_r_earley_item_warning_threshold_set", "int", "too_many_earley_items"},
    {"marpa_r_earley_set_value", "Marpa_Earley_Set_ID", "ordinal"},
    {"marpa_r_expected_symbol_event_set", "Marpa_Symbol_ID", "isyid", "int", "value"},
    {"marpa_r_closest_earleme"},
    {"marpa_r_furthest_earleme"},
    {"marpa_r_is_exhausted"},
    {"marpa_r_latest_earley_set"},
    {"marpa_r_latest_earley_set_value_set", "int", "value"},
    {"marpa_r_nulled_symbol_activate", "Marpa_Symbol_ID", "sym_id", "int", "reactivate"},
    {"marpa_r_prediction_symbol_activate", "Marpa_Symbol_ID", "sym_id", "int", "reactivate"},
    {"marpa_r_progress_report_finish"},
    {"marpa_r_progress_report_start", "Marpa_Earley_Set_ID", "ordinal"},
    {"marpa_r_start_input"},
    {"marpa_r_terminal_is_expected", "Marpa_Symbol_ID", "isyid"},
    {"marpa_r_zwa_default", "Marpa_Assertion_ID", "zwaid"},
    {"marpa_r_zwa_default_set", "Marpa_Assertion_ID", "zwaid", "int", "default_value"},
    {"marpa_b_ambiguity_metric"},
    {"marpa_b_is_null"},
    {"marpa_o_ambiguity_metric"},
    {"marpa_o_high_rank_only_set", "int", "flag"},
    {"marpa_o_high_rank_only"},
    {"marpa_o_is_null"},
    {"marpa_o_rank"},
    {"marpa_t_next"},
    {"marpa_t_parse_count"},
    {"_marpa_t_size" },
    {"_marpa_t_nook_or_node", "Marpa_Nook_ID", "nook_id" },
    {"_marpa_t_nook_choice", "Marpa_Nook_ID", "nook_id" },
    {"_marpa_t_nook_parent", "Marpa_Nook_ID", "nook_id" },
    {"_marpa_t_nook_is_cause", "Marpa_Nook_ID", "nook_id" },
    {"_marpa_t_nook_cause_is_ready", "Marpa_Nook_ID", "nook_id" },
    {"_marpa_t_nook_is_predecessor", "Marpa_Nook_ID", "nook_id" },
    {"_marpa_t_nook_predecessor_is_ready", "Marpa_Nook_ID", "nook_id" },
    {"marpa_v_valued_force"},
    {"marpa_v_rule_is_valued_set", "Marpa_Rule_ID", "symbol_id", "int", "value"},
    {"marpa_v_symbol_is_valued_set", "Marpa_Symbol_ID", "symbol_id", "int", "value"},
    {"_marpa_v_nook"},
    {"_marpa_v_trace", "int", "flag"},
    {"_marpa_g_ahm_count"},
    {"_marpa_g_ahm_nrl", "Marpa_AHM_ID", "item_id"},
    {"_marpa_g_ahm_postdot", "Marpa_AHM_ID", "item_id"},
    {"_marpa_g_ahm_null_count", "Marpa_AHM_ID", "item_id"},
    {"_marpa_g_ahm_raw_position", "Marpa_AHM_ID", "item_id"},
    {"_marpa_g_nrl_count"},
    {"_marpa_g_nrl_is_virtual_lhs", "Marpa_NRL_ID", "nrl_id"},
    {"_marpa_g_nrl_is_virtual_rhs", "Marpa_NRL_ID", "nrl_id"},
    {"_marpa_g_nrl_length", "Marpa_NRL_ID", "nrl_id"},
    {"_marpa_g_nrl_lhs", "Marpa_NRL_ID", "nrl_id"},
    {"_marpa_g_nrl_rank", "Marpa_NRL_ID", "nrl_id"},
    {"_marpa_g_nrl_rhs", "Marpa_NRL_ID", "nrl_id", "int", "ix"},
    {"_marpa_g_nrl_semantic_equivalent", "Marpa_NRL_ID", "nrl_id"},
    {"_marpa_g_nsy_count"},
    {"_marpa_g_nsy_is_lhs", "Marpa_NSY_ID", "nsy_id"},
    {"_marpa_g_nsy_is_nulling", "Marpa_NSY_ID", "nsy_id", return_type='boolean'},
    {"_marpa_g_nsy_is_semantic", "Marpa_NSY_ID", "nsy_id", return_type='boolean'},
    {"_marpa_g_start_nsy" },
    {"_marpa_g_nsy_lhs_irl", "Marpa_NSY_ID", "nsy_id"},
    {"_marpa_g_nsy_rank", "Marpa_NSY_ID", "nsy_id"},
    {"_marpa_g_nsy_irl_offset", "Marpa_NSY_ID", "nsy_id"},
    {"_marpa_g_real_symbol_count", "Marpa_NRL_ID", "nrl_id"},
    {"_marpa_g_rule_is_keep_separation", "Marpa_Rule_ID", "rule_id"},
    {"_marpa_g_rule_is_used", "Marpa_Rule_ID", "rule_id"},
    {"_marpa_g_source_irl", "Marpa_NRL_ID", "nrl_id"},
    {"_marpa_g_source_isy", "Marpa_NSY_ID", "nsy_id"},
    {"_marpa_g_virtual_end", "Marpa_NRL_ID", "nrl_id"},
    {"_marpa_g_virtual_start", "Marpa_NRL_ID", "nrl_id"},
    {"_marpa_g_isy_nsy", "Marpa_Symbol_ID", "symid"},
    {"_marpa_g_isy_nulling_nsy", "Marpa_Symbol_ID", "symid"},
    {"_marpa_r_earley_item_origin"},
    {"_marpa_r_earley_item_trace", "Marpa_Earley_Item_ID", "item_id"},
    {"_marpa_r_earley_set_size", "Marpa_Earley_Set_ID", "set_id"},
    {"_marpa_r_earley_set_trace", "Marpa_Earley_Set_ID", "set_id"},
    {"_marpa_r_first_completion_link_trace"},
    {"_marpa_r_first_leo_link_trace"},
    {"_marpa_r_first_postdot_item_trace"},
    {"_marpa_r_first_token_link_trace"},
    {"_marpa_r_is_use_leo"},
    {"_marpa_r_is_use_leo_set", "int", "value"},
    {"_marpa_r_leo_base_origin"},
    {"_marpa_r_leo_base_state"},
    {"_marpa_r_leo_predecessor_symbol"},
    {"_marpa_r_next_completion_link_trace"},
    {"_marpa_r_next_leo_link_trace"},
    {"_marpa_r_next_postdot_item_trace"},
    {"_marpa_r_next_token_link_trace"},
    {"_marpa_r_postdot_item_symbol"},
    {"_marpa_r_postdot_symbol_trace", "Marpa_Symbol_ID", "symid"},
    {"_marpa_r_source_leo_transition_symbol"},
    {"_marpa_r_source_middle"},
    {"_marpa_r_source_predecessor_state"},
    {"_marpa_r_trace_earley_set"},
    {"_marpa_b_and_node_cause", "Marpa_And_Node_ID", "ordinal"},
    {"_marpa_b_and_node_count"},
    {"_marpa_b_and_node_middle", "Marpa_And_Node_ID", "and_node_id"},
    {"_marpa_b_and_node_parent", "Marpa_And_Node_ID", "and_node_id"},
    {"_marpa_b_and_node_predecessor", "Marpa_And_Node_ID", "ordinal"},
    {"_marpa_b_and_node_symbol", "Marpa_And_Node_ID", "and_node_id"},
    {"_marpa_b_or_node_and_count", "Marpa_Or_Node_ID", "or_node_id"},
    {"_marpa_b_or_node_first_and", "Marpa_Or_Node_ID", "ordinal"},
    {"_marpa_b_or_node_nrl", "Marpa_Or_Node_ID", "ordinal"},
    {"_marpa_b_or_node_is_semantic", "Marpa_Or_Node_ID", "or_node_id"},
    {"_marpa_b_or_node_is_whole", "Marpa_Or_Node_ID", "or_node_id"},
    {"_marpa_b_or_node_last_and", "Marpa_Or_Node_ID", "ordinal"},
    {"_marpa_b_or_node_origin", "Marpa_Or_Node_ID", "ordinal"},
    {"_marpa_b_or_node_position", "Marpa_Or_Node_ID", "ordinal"},
    {"_marpa_b_or_node_set", "Marpa_Or_Node_ID", "ordinal"},
    {"_marpa_b_top_or_node"},
    {"_marpa_o_and_order_get", "Marpa_Or_Node_ID", "or_node_id", "int", "ix"},
    {"_marpa_o_or_node_and_node_count", "Marpa_Or_Node_ID", "or_node_id"},
    {"_marpa_o_or_node_and_node_id_by_ix", "Marpa_Or_Node_ID", "or_node_id", "int", "ix"},
    {"marpa_ptrv_at_eim", return_type='boolean'},
    {"marpa_ptrv_at_lim", return_type='boolean'},
    {"marpa_ptrv_is_trivial"},
    {"marpa_trv_ahm_id"},
    {"marpa_trv_at_completion", return_type='boolean'},
    {"marpa_trv_at_leo", return_type='boolean'},
    {"marpa_trv_at_token", return_type='boolean'},
    {"marpa_trv_completion_next", return_type='boolean'},
    {"marpa_trv_current"},
    {"marpa_trv_is_trivial"},
    {"marpa_trv_leo_next", return_type='boolean'},
    {"marpa_trv_nrl_id"},
    {"marpa_trv_origin"},
    {"marpa_trv_rule_id"},
    {"marpa_trv_token_next", return_type='boolean'},
  }
  local result = {}
  for ix = 1,#signatures do
      result[#result+1] = wrap_libmarpa_method(signatures[ix])
  end
  return table.concat(result)
  -- end of exec
  ]==]

  -- miranda: section register standard libmarpa wrappers
  --[==[ miranda: exec register standard libmarpa wrappers
        local result = {}
        for ix = 1, #signatures do
           local signature = signatures[ix]
           local function_name = signature[1]
           local unprefixed_name = function_name:gsub("^[_]?marpa_", "", 1)
           local class_letter = unprefixed_name:gsub("_.*$", "", 1)
           local class_name = libmarpa_class_name[class_letter]
           local class_table_name = 'class_' .. class_name

           result[#result+1] = string.format("  marpa_lua_getfield(L, kollos_table_stack_ix, %q);\n", class_table_name)
           -- for example: marpa_lua_getfield(L, kollos_table_stack_ix, "class_grammar")

           result[#result+1] = "marpa_lua_pushvalue(L, upvalue_stack_ix);\n";

           local wrapper_name = "wrap_" .. unprefixed_name;
           result[#result+1] = string.format("  marpa_lua_pushcclosure(L, %s, 1);\n", wrapper_name)
           -- for example: marpa_lua_pushcclosure(L, wrap_g_highest_rule_id, 1)

           local classless_name = function_name:gsub("^[_]?marpa_[^_]*_", "")
           local initial_underscore = function_name:match('^_') and '_' or ''
           local field_name = initial_underscore .. classless_name
           result[#result+1] = string.format("  marpa_lua_setfield(L, -2, %q);\n", field_name)
           -- for example: marpa_lua_setfield(L, -2, "highest_rule_id")

           result[#result+1] = string.format("  marpa_lua_pop(L, 1);\n", field_name)

        end
        return table.concat(result)
  ]==]

```

```

  -- miranda: section create kollos libmarpa wrapper class tables
  --[==[ miranda: exec create kollos libmarpa wrapper class tables
        local result = {}
        for class_letter, class in pairs(libmarpa_class_name) do
           local class_table_name = 'class_' .. class
           local functions_to_register = class .. '_methods'
           -- class_xyz = {}
           result[#result+1] = string.format("  marpa_luaL_newlibtable(L, %s);\n", functions_to_register)
           -- add functions and upvalue to class_xyz
           result[#result+1] = "  marpa_lua_pushvalue(L, upvalue_stack_ix);\n"
           result[#result+1] = string.format("  marpa_luaL_setfuncs(L, %s, 1);\n", functions_to_register)
           -- class_xyz.__index = class_xyz
           result[#result+1] = "  marpa_lua_pushvalue(L, -1);\n"
           result[#result+1] = '  marpa_lua_setfield(L, -2, "__index");\n'
           -- kollos[class_xyz] = class_xyz
           result[#result+1] = "  marpa_lua_pushvalue(L, -1);\n"
           result[#result+1] = string.format("  marpa_lua_setfield(L, kollos_table_stack_ix, %q);\n", class_table_name);
           -- class_xyz[kollos] = kollos
           result[#result+1] = "  marpa_lua_pushvalue(L, kollos_table_stack_ix);\n"
           result[#result+1] = '  marpa_lua_setfield(L, -2, "kollos");\n'
        end
        return table.concat(result)
  ]==]

```

### Libmarpa class constructors

The standard constructors are generated indirectly, from a template.
This saves a lot of repetition, which makes for easier reading in the
long run.
In the short run, however, you may want first to look at the bocage
constructor.
It is specified directly, which can be easier for a first reading.


```
  -- miranda: section object constructors
  --[==[ miranda: exec object constructors
        local result = {}
        local template = [[
        |static int
        |wrap_!NAME!_new (lua_State * L)
        |{
        |  const int !BASE_NAME!_stack_ix = 1;
        |  int !NAME!_stack_ix;
        |
        |  if (0)
        |    printf ("%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);
        |  if (1)
        |    {
        |      marpa_luaL_checktype(L, !BASE_NAME!_stack_ix, LUA_TTABLE);
        |    }
        |
        |  marpa_lua_newtable(L);
        |  /* [ base_table, class_table ] */
        |  !NAME!_stack_ix = marpa_lua_gettop(L);
        |  marpa_lua_pushvalue(L, marpa_lua_upvalueindex(2));
        |  marpa_lua_setmetatable (L, !NAME!_stack_ix);
        |  /* [ base_table, class_table ] */
        |
        |  {
        |    !BASE_TYPE! *!BASE_NAME!_ud;
        |
        |    !TYPE! *!NAME!_ud =
        |      (!TYPE! *) marpa_lua_newuserdata (L, sizeof (!TYPE!));
        |    /* [ base_table, class_table, class_ud ] */
        |    marpa_lua_rawgetp (L, LUA_REGISTRYINDEX, &kollos_!LETTER!_ud_mt_key);
        |    /* [ class_table, class_ud, class_ud_mt ] */
        |    marpa_lua_setmetatable (L, -2);
        |    /* [ class_table, class_ud ] */
        |
        |    marpa_lua_setfield (L, !NAME!_stack_ix, "_libmarpa");
        |    marpa_lua_getfield (L, !BASE_NAME!_stack_ix, "lmw_g");
        |    marpa_lua_setfield (L, !NAME!_stack_ix, "lmw_g");
        |    marpa_lua_getfield (L, !BASE_NAME!_stack_ix, "_libmarpa");
        |    !BASE_NAME!_ud = (!BASE_TYPE! *) marpa_lua_touserdata (L, -1);
        |
        |    *!NAME!_ud = marpa_!LETTER!_new (*!BASE_NAME!_ud);
        |    if (!*!NAME!_ud)
        |      {
        |        return libmarpa_error_handle (L, !NAME!_stack_ix, "marpa_!LETTER!_new()");
        |      }
        |  }
        |
        |  if (0)
        |    printf ("%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);
        |  marpa_lua_settop(L, !NAME!_stack_ix );
        |  /* [ base_table, class_table ] */
        |  return 1;
        |}
        ]]
        -- for every class with a base,
        -- so that grammar constructor is special case
        -- grammar, bocage and traverser constructors are special cases
        local class_sequence = { 'r', 'o', 't', 'v'}
        for class_ix = 1, #class_sequence do
            local class_letter = class_sequence[class_ix]
            local class_name = libmarpa_class_name[class_letter]
            local class_type = libmarpa_class_type[class_letter]
            local base_class_letter = libmarpa_base_class[class_letter]
            local base_class_name = libmarpa_class_name[base_class_letter]
            local base_class_type = libmarpa_class_type[base_class_letter]
            local this_piece =
                pipe_dedent(template)
                   :gsub("!BASE_NAME!", base_class_name)
                   :gsub("!BASE_TYPE!", base_class_type)
                   :gsub("!BASE_LETTER!", base_class_letter)
                   :gsub("!NAME!", class_name)
                   :gsub("!TYPE!", class_type)
                   :gsub("!LETTER!", class_letter)
            result[#result+1] = this_piece
            ::NEXT_CLASS::
        end
        return table.concat(result, "\n")
  ]==]
```

The traverser constructors are special cases
because
traversers are not a "main sequence" class.

```
    -- miranda: section+ object constructors
    static int
    wrap_traverser_new (lua_State * L)
    {
      const int recce_stack_ix = 1;
      const int es_ordinal_stack_ix = 2;
      const int eim_ordinal_stack_ix = 3;
      int traverser_stack_ix;

      if (0)
        printf ("%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);
      if (1)
        {
          marpa_luaL_checktype(L, recce_stack_ix, LUA_TTABLE);
        }

      marpa_lua_newtable(L);
      traverser_stack_ix = marpa_lua_gettop(L);
      /* push "class_traverser" metatable */
      marpa_lua_pushvalue(L, marpa_lua_upvalueindex(2));
      marpa_lua_setmetatable (L, traverser_stack_ix);

      {
        Marpa_Recognizer *recce_ud;

        Marpa_Traverser *traverser_ud =
          (Marpa_Traverser *) marpa_lua_newuserdata (L, sizeof (Marpa_Traverser));
        /* [ base_table, class_table, class_ud ] */
        marpa_lua_rawgetp (L, LUA_REGISTRYINDEX, &kollos_trv_ud_mt_key);
        /* [ class_table, class_ud, class_ud_mt ] */
        marpa_lua_setmetatable (L, -2);
        /* [ class_table, class_ud ] */

        marpa_lua_setfield (L, traverser_stack_ix, "_libmarpa");
        marpa_lua_getfield (L, recce_stack_ix, "lmw_g");
        marpa_lua_setfield (L, traverser_stack_ix, "lmw_g");
        marpa_lua_getfield (L, recce_stack_ix, "_libmarpa");
        recce_ud = (Marpa_Recognizer *) marpa_lua_touserdata (L, -1);

        {
          int is_ok = 0;
          lua_Integer es_ordinal = -1;
          lua_Integer eim_ordinal = 0;
          if (marpa_lua_isnil(L, es_ordinal_stack_ix)) {
             is_ok = 1;
          } else {
             es_ordinal = marpa_lua_tointegerx(L, es_ordinal_stack_ix, &is_ok);
          }
          if (!is_ok) {
              marpa_luaL_error(L,
                  "problem with traverser_new() arg #2, type was %s",
                  marpa_luaL_typename(L, es_ordinal_stack_ix)
              );
          }
          is_ok = 0;
          if (marpa_lua_isnil(L, eim_ordinal_stack_ix)) {
             is_ok = 1;
          } else {
             eim_ordinal = marpa_lua_tointegerx(L, eim_ordinal_stack_ix, &is_ok);
          }
          if (!is_ok) {
              marpa_luaL_error(L,
                  "problem with traverser_new() arg #3, type was %s",
                  marpa_luaL_typename(L, eim_ordinal_stack_ix)
              );
          }
          *traverser_ud = marpa_trv_new (*recce_ud, (int)es_ordinal, (int)eim_ordinal);
        }

        if (!*traverser_ud)
          {
            return libmarpa_error_handle (L, traverser_stack_ix, "marpa_trv_new()");
          }
      }

      if (0)
        printf ("%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);
      marpa_lua_settop(L, traverser_stack_ix );
      /* [ base_table, class_table ] */
      return 1;
    }

```

```
    -- miranda: section+ non-standard wrappers
    static int
    lca_trv_completion_predecessor (lua_State * L)
    {
      const int base_traverser_stack_ix = 1;
      int traverser_stack_ix;
      Marpa_Traverser *base_traverser_ud;

      if (0)
        printf ("%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);
      if (1)
        {
          marpa_luaL_checktype(L, base_traverser_stack_ix, LUA_TTABLE);
        }
      marpa_lua_getfield (L, base_traverser_stack_ix, "_libmarpa");
      base_traverser_ud = marpa_lua_touserdata(L, -1);

      marpa_lua_newtable(L);
      traverser_stack_ix = marpa_lua_gettop(L);

      /* push "class_traverser" metatable */
      marpa_lua_getglobal(L, "_M");
      marpa_lua_getfield(L, -1, "class_traverser");

      marpa_lua_setmetatable (L, traverser_stack_ix);

      {
        Marpa_Traverser *traverser_ud =
          (Marpa_Traverser *) marpa_lua_newuserdata (L, sizeof (Marpa_Traverser));
        /* [ base_table, class_table, class_ud ] */
        marpa_lua_rawgetp (L, LUA_REGISTRYINDEX, &kollos_trv_ud_mt_key);
        /* [ class_table, class_ud, class_ud_mt ] */
        marpa_lua_setmetatable (L, -2);
        /* [ class_table, class_ud ] */

        marpa_lua_setfield (L, traverser_stack_ix, "_libmarpa");
        marpa_lua_getfield (L, base_traverser_stack_ix, "lmw_g");
        marpa_lua_setfield (L, traverser_stack_ix, "lmw_g");

        *traverser_ud = marpa_trv_completion_predecessor (*base_traverser_ud);
        if (!*traverser_ud)
          {
            if (marpa_trv_soft_error(*base_traverser_ud)) {
                marpa_lua_pushnil(L);
                return 1;
            }
            return libmarpa_error_handle (L, base_traverser_stack_ix, "marpa_trv_completion_predecessor()");
          }
      }

      if (0)
        printf ("%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);
      marpa_lua_settop(L, traverser_stack_ix );
      /* [ base_table, class_table ] */
      return 1;
    }

```

```
    -- miranda: section+ non-standard wrappers
    static int
    lca_trv_completion_cause (lua_State * L)
    {
      const int base_traverser_stack_ix = 1;
      int traverser_stack_ix;
      Marpa_Traverser *base_traverser_ud;

      if (0)
        printf ("%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);
      if (1)
        {
          marpa_luaL_checktype(L, base_traverser_stack_ix, LUA_TTABLE);
        }
      marpa_lua_getfield (L, base_traverser_stack_ix, "_libmarpa");
      base_traverser_ud = marpa_lua_touserdata(L, -1);

      marpa_lua_newtable(L);
      traverser_stack_ix = marpa_lua_gettop(L);

      /* push "class_traverser" metatable */
      marpa_lua_getglobal(L, "_M");
      marpa_lua_getfield(L, -1, "class_traverser");

      marpa_lua_setmetatable (L, traverser_stack_ix);

      {
        Marpa_Traverser *traverser_ud =
          (Marpa_Traverser *) marpa_lua_newuserdata (L, sizeof (Marpa_Traverser));
        /* [ base_table, class_table, class_ud ] */
        marpa_lua_rawgetp (L, LUA_REGISTRYINDEX, &kollos_trv_ud_mt_key);
        /* [ class_table, class_ud, class_ud_mt ] */
        marpa_lua_setmetatable (L, -2);
        /* [ class_table, class_ud ] */

        marpa_lua_setfield (L, traverser_stack_ix, "_libmarpa");
        marpa_lua_getfield (L, base_traverser_stack_ix, "lmw_g");
        marpa_lua_setfield (L, traverser_stack_ix, "lmw_g");

        *traverser_ud = marpa_trv_completion_cause (*base_traverser_ud);
        if (!*traverser_ud)
          {
            if (marpa_trv_soft_error(*base_traverser_ud)) {
                marpa_lua_pushnil(L);
                return 1;
            }
            return libmarpa_error_handle (L, base_traverser_stack_ix, "marpa_trv_completion_cause()");
          }
      }

      if (0)
        printf ("%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);
      marpa_lua_settop(L, traverser_stack_ix );
      /* [ base_table, class_table ] */
      return 1;
    }

```

```
    -- miranda: section+ non-standard wrappers
    static int
    lca_trv_token_predecessor (lua_State * L)
    {
      const int base_traverser_stack_ix = 1;
      int traverser_stack_ix;
      Marpa_Traverser *base_traverser_ud;

      if (0)
        printf ("%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);
      if (1)
        {
          marpa_luaL_checktype(L, base_traverser_stack_ix, LUA_TTABLE);
        }
      marpa_lua_getfield (L, base_traverser_stack_ix, "_libmarpa");
      base_traverser_ud = marpa_lua_touserdata(L, -1);

      marpa_lua_newtable(L);
      traverser_stack_ix = marpa_lua_gettop(L);
      /* push "class_traverser" metatable */
      marpa_lua_pushvalue(L, marpa_lua_upvalueindex(2));
      marpa_lua_setmetatable (L, traverser_stack_ix);

      {
        Marpa_Traverser *traverser_ud =
          (Marpa_Traverser *) marpa_lua_newuserdata (L, sizeof (Marpa_Traverser));
        /* [ base_table, class_table, class_ud ] */
        marpa_lua_rawgetp (L, LUA_REGISTRYINDEX, &kollos_trv_ud_mt_key);
        /* [ class_table, class_ud, class_ud_mt ] */
        marpa_lua_setmetatable (L, -2);
        /* [ class_table, class_ud ] */

        marpa_lua_setfield (L, traverser_stack_ix, "_libmarpa");
        marpa_lua_getfield (L, base_traverser_stack_ix, "lmw_g");
        marpa_lua_setfield (L, traverser_stack_ix, "lmw_g");

        *traverser_ud = marpa_trv_token_predecessor (*base_traverser_ud);
        if (!*traverser_ud)
          {
            if (marpa_trv_soft_error(*base_traverser_ud)) {
                marpa_lua_pushnil(L);
                return 1;
            }
            return libmarpa_error_handle (L, base_traverser_stack_ix, "marpa_trv_token_predecessor()");
          }
      }

      if (0)
        printf ("%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);
      marpa_lua_settop(L, traverser_stack_ix );
      /* [ base_table, class_table ] */
      return 1;
    }

```

```
    -- miranda: section+ non-standard wrappers
    static int lca_ltrv_trailhead_eim(lua_State *L)
    {
      /* [ recce_object ] */
      const int ltraverser_stack_ix = 1;
      Marpa_LTraverser ltrv;
      Marpa_Earley_Set_ID origin;
      int position;
      Marpa_Rule_ID rule_id;

      marpa_lua_getfield (L, ltraverser_stack_ix, "_libmarpa");
      /* [ recce_object, recce_ud ] */
      ltrv = *(Marpa_LTraverser *) marpa_lua_touserdata (L, -1);
      rule_id = marpa_ltrv_trailhead_eim (ltrv, &position, &origin);
      if (rule_id < -1)
        {
          return libmarpa_error_handle (L, ltraverser_stack_ix, "ltrv:trailhead_eim()");
        }
      if (rule_id == -1)
        {
          return 0;
        }
      marpa_lua_pushinteger (L, (lua_Integer) rule_id);
      marpa_lua_pushinteger (L, (lua_Integer) position);
      marpa_lua_pushinteger (L, (lua_Integer) origin);
      return 3;
    }
```

```
    -- miranda: section+ non-standard wrappers
    static int
    lca_ltrv_predecessor (lua_State * L)
    {
      const int base_ltraverser_stack_ix = 1;
      int ltraverser_stack_ix;
      Marpa_LTraverser *base_ltraverser_ud;

      if (0)
        printf ("%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);
      if (1)
        {
          marpa_luaL_checktype(L, base_ltraverser_stack_ix, LUA_TTABLE);
        }
      marpa_lua_getfield (L, base_ltraverser_stack_ix, "_libmarpa");
      base_ltraverser_ud = marpa_lua_touserdata(L, -1);

      marpa_lua_newtable(L);
      ltraverser_stack_ix = marpa_lua_gettop(L);
      /* push "class_ltraverser" metatable */
      marpa_lua_getglobal(L, "_M");
      marpa_lua_getfield(L, -1, "class_ltraverser");
      marpa_lua_setmetatable (L, ltraverser_stack_ix);

      {
        Marpa_LTraverser *ltraverser_ud =
          (Marpa_LTraverser *) marpa_lua_newuserdata (L, sizeof (Marpa_LTraverser));
        /* [ base_table, class_table, class_ud ] */
        marpa_lua_rawgetp (L, LUA_REGISTRYINDEX, &kollos_ltrv_ud_mt_key);
        /* [ class_table, class_ud, class_ud_mt ] */
        marpa_lua_setmetatable (L, -2);
        /* [ class_table, class_ud ] */

        marpa_lua_setfield (L, ltraverser_stack_ix, "_libmarpa");
        marpa_lua_getfield (L, base_ltraverser_stack_ix, "lmw_g");
        marpa_lua_setfield (L, ltraverser_stack_ix, "lmw_g");

        *ltraverser_ud = marpa_ltrv_predecessor (*base_ltraverser_ud);
        if (!*ltraverser_ud)
          {
            if (marpa_ltrv_soft_error(*base_ltraverser_ud)) {
                marpa_lua_pushnil(L);
                return 1;
            }
            return libmarpa_error_handle (L, base_ltraverser_stack_ix, "marpa_ltrv_predecessor()");
          }
      }

      if (0)
        printf ("%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);
      marpa_lua_settop(L, ltraverser_stack_ix );
      /* [ base_table, class_table ] */
      return 1;
    }

```

```
    -- miranda: section+ object constructors
    static int
    wrap_ptraverser_new (lua_State * L)
    {
      const int recce_stack_ix = 1;
      const int es_ordinal_stack_ix = 2;
      const int nsyid_ordinal_stack_ix = 3;
      int ptraverser_stack_ix;

      if (0)
        printf ("%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);
      if (1)
        {
          marpa_luaL_checktype(L, recce_stack_ix, LUA_TTABLE);
        }

      marpa_lua_newtable(L);
      ptraverser_stack_ix = marpa_lua_gettop(L);
      /* push "class_ptraverser" metatable */
      marpa_lua_pushvalue(L, marpa_lua_upvalueindex(2));
      marpa_lua_setmetatable (L, ptraverser_stack_ix);

      {
        Marpa_Recognizer *recce_ud;

        Marpa_PTraverser *ptraverser_ud =
          (Marpa_PTraverser *) marpa_lua_newuserdata (L, sizeof (Marpa_PTraverser));
        /* [ base_table, class_table, class_ud ] */
        marpa_lua_rawgetp (L, LUA_REGISTRYINDEX, &kollos_ptrv_ud_mt_key);
        /* [ class_table, class_ud, class_ud_mt ] */
        marpa_lua_setmetatable (L, -2);
        /* [ class_table, class_ud ] */

        marpa_lua_setfield (L, ptraverser_stack_ix, "_libmarpa");
        marpa_lua_getfield (L, recce_stack_ix, "lmw_g");
        marpa_lua_setfield (L, ptraverser_stack_ix, "lmw_g");
        marpa_lua_getfield (L, recce_stack_ix, "_libmarpa");
        recce_ud = (Marpa_Recognizer *) marpa_lua_touserdata (L, -1);

        {
          int is_ok = 0;
          lua_Integer es_ordinal = -1;
          lua_Integer nsyid_ordinal = 0;
          if (marpa_lua_isnil(L, es_ordinal_stack_ix)) {
             is_ok = 1;
          } else {
             es_ordinal = marpa_lua_tointegerx(L, es_ordinal_stack_ix, &is_ok);
          }
          if (!is_ok) {
              marpa_luaL_error(L,
                  "problem with ptraverser_new() arg #2, type was %s",
                  marpa_luaL_typename(L, es_ordinal_stack_ix)
              );
          }
          is_ok = 0;
          if (marpa_lua_isnil(L, nsyid_ordinal_stack_ix)) {
             is_ok = 1;
          } else {
             nsyid_ordinal = marpa_lua_tointegerx(L, nsyid_ordinal_stack_ix, &is_ok);
          }
          if (!is_ok) {
              marpa_luaL_error(L,
                  "problem with ptraverser_new() arg #3, type was %s",
                  marpa_luaL_typename(L, nsyid_ordinal_stack_ix)
              );
          }
          *ptraverser_ud = marpa_ptrv_new (*recce_ud, (int)es_ordinal, (int)nsyid_ordinal);
        }

        if (!*ptraverser_ud)
          {
            return libmarpa_error_handle (L, ptraverser_stack_ix, "marpa_ptrv_new()");
          }
      }

      if (0)
        printf ("%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);
      marpa_lua_settop(L, ptraverser_stack_ix );
      /* [ base_table, class_table ] */
      return 1;
    }

```

`lca_ptrv_eim_iter` returns an EIM iterator from an PIM
traverser.
It's slightly tricky -- it is called on a PIM Traverser,
but returns a EIM Traverser.

```
    -- miranda: section+ non-standard wrappers
    static int
    lca_ptrv_eim_iter (lua_State * L)
    {
      const int ptraverser_stack_ix = 1;
      int traverser_stack_ix;
      Marpa_PTraverser *ptraverser_ud;

      if (0)
        printf ("%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);
      if (1)
        {
          marpa_luaL_checktype(L, ptraverser_stack_ix, LUA_TTABLE);
        }
      marpa_lua_getfield (L, ptraverser_stack_ix, "_libmarpa");
      ptraverser_ud = marpa_lua_touserdata(L, -1);

      marpa_lua_newtable(L);
      traverser_stack_ix = marpa_lua_gettop(L);
      /* push "class_traverser" metatable */
      marpa_lua_getglobal(L, "_M");
      marpa_lua_getfield(L, -1, "class_traverser");
      marpa_lua_setmetatable (L, traverser_stack_ix);

      {
        Marpa_Traverser *traverser_ud =
          (Marpa_Traverser *) marpa_lua_newuserdata (L, sizeof (Marpa_Traverser));
        /* [ base_table, class_table, class_ud ] */
        marpa_lua_rawgetp (L, LUA_REGISTRYINDEX, &kollos_trv_ud_mt_key);
        /* [ class_table, class_ud, class_ud_mt ] */
        marpa_lua_setmetatable (L, -2);
        /* [ class_table, class_ud ] */

        marpa_lua_setfield (L, traverser_stack_ix, "_libmarpa");
        marpa_lua_getfield (L, ptraverser_stack_ix, "lmw_g");
        marpa_lua_setfield (L, traverser_stack_ix, "lmw_g");

        *traverser_ud = marpa_ptrv_eim_iter (*ptraverser_ud);
        if (!*traverser_ud)
          {
            if (marpa_ptrv_soft_error(*ptraverser_ud)) {
                marpa_lua_pushnil(L);
                return 1;
            }
            return libmarpa_error_handle (L, ptraverser_stack_ix, "marpa_ptrv_eim_iter()");
          }
      }

      if (0)
        printf ("%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);
      marpa_lua_settop(L, traverser_stack_ix );
      /* [ base_table, class_table ] */
      return 1;
    }

```

`lca_trv_lim()` is tricky -- it's called on an EIM
traverser, but returns a LIM traverser

```
    -- miranda: section+ non-standard wrappers
    static int
    lca_trv_lim (lua_State * L)
    {
      const int base_traverser_stack_ix = 1;
      int ltraverser_stack_ix;
      Marpa_Traverser *base_traverser_ud;

      if (0)
        printf ("%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);
      if (1)
        {
          marpa_luaL_checktype(L, base_traverser_stack_ix, LUA_TTABLE);
        }
      marpa_lua_getfield (L, base_traverser_stack_ix, "_libmarpa");
      base_traverser_ud = marpa_lua_touserdata(L, -1);

      marpa_lua_newtable(L);
      ltraverser_stack_ix = marpa_lua_gettop(L);
      /* push "class_ltraverser" metatable */
      marpa_lua_getglobal(L, "_M");
      marpa_lua_getfield(L, -1, "class_ltraverser");
      marpa_lua_setmetatable (L, ltraverser_stack_ix);

      {
        Marpa_LTraverser *ltraverser_ud =
          (Marpa_LTraverser *) marpa_lua_newuserdata (L, sizeof (Marpa_LTraverser));
        /* [ base_table, class_table, class_ud ] */
        marpa_lua_rawgetp (L, LUA_REGISTRYINDEX, &kollos_ltrv_ud_mt_key);
        /* [ class_table, class_ud, class_ud_mt ] */
        marpa_lua_setmetatable (L, -2);
        /* [ class_table, class_ud ] */

        marpa_lua_setfield (L, ltraverser_stack_ix, "_libmarpa");
        marpa_lua_getfield (L, base_traverser_stack_ix, "lmw_g");
        marpa_lua_setfield (L, ltraverser_stack_ix, "lmw_g");

        *ltraverser_ud = marpa_trv_lim (*base_traverser_ud);
        if (!*ltraverser_ud)
          {
            if (marpa_trv_soft_error(*base_traverser_ud)) {
                marpa_lua_pushnil(L);
                return 1;
            }
            return libmarpa_error_handle (L, base_traverser_stack_ix, "marpa_trv_lim()");
          }
      }

      if (0)
        printf ("%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);
      marpa_lua_settop(L, ltraverser_stack_ix );
      /* [ base_table, class_table ] */
      return 1;
    }

```

The bocage constructor takes an extra argument, so it's a special
case.
It's close to the standard constructor.
The standard constructors are generated indirectly, from a template.
The template saves repetition, but is harder on a first reading.
This bocage constructor is specified directly,
so you may find it easer to read it first.

```
    -- miranda: section+ object constructors
    static int
    wrap_bocage_new (lua_State * L)
    {
      const int recce_stack_ix = 1;
      const int ordinal_stack_ix = 2;
      int bocage_stack_ix;

      if (0)
        printf ("%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);
      if (1)
        {
          marpa_luaL_checktype(L, recce_stack_ix, LUA_TTABLE);
        }

      marpa_lua_newtable(L);
      bocage_stack_ix = marpa_lua_gettop(L);
      /* push "class_bocage" metatable */
      marpa_lua_pushvalue(L, marpa_lua_upvalueindex(2));
      marpa_lua_setmetatable (L, bocage_stack_ix);

      {
        Marpa_Recognizer *recce_ud;

        Marpa_Bocage *bocage_ud =
          (Marpa_Bocage *) marpa_lua_newuserdata (L, sizeof (Marpa_Bocage));
        /* [ base_table, class_table, class_ud ] */
        marpa_lua_rawgetp (L, LUA_REGISTRYINDEX, &kollos_b_ud_mt_key);
        /* [ class_table, class_ud, class_ud_mt ] */
        marpa_lua_setmetatable (L, -2);
        /* [ class_table, class_ud ] */

        marpa_lua_setfield (L, bocage_stack_ix, "_libmarpa");
        marpa_lua_getfield (L, recce_stack_ix, "lmw_g");
        marpa_lua_setfield (L, bocage_stack_ix, "lmw_g");
        marpa_lua_getfield (L, recce_stack_ix, "_libmarpa");
        recce_ud = (Marpa_Recognizer *) marpa_lua_touserdata (L, -1);

        {
          int is_ok = 0;
          lua_Integer ordinal = -1;
          if (marpa_lua_isnil(L, ordinal_stack_ix)) {
             is_ok = 1;
          } else {
             ordinal = marpa_lua_tointegerx(L, ordinal_stack_ix, &is_ok);
          }
          if (!is_ok) {
              marpa_luaL_error(L,
                  "problem with bocage_new() arg #2, type was %s",
                  marpa_luaL_typename(L, ordinal_stack_ix)
              );
          }
          *bocage_ud = marpa_b_new (*recce_ud, (int)ordinal);
        }

        if (!*bocage_ud)
          {
            return libmarpa_error_handle (L, bocage_stack_ix, "marpa_b_new()");
          }
      }

      if (0)
        printf ("%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);
      marpa_lua_settop(L, bocage_stack_ix );
      /* [ base_table, class_table ] */
      return 1;
    }

```

The grammar constructor is a special case, because its argument is
a special "configuration" argument.

```
    -- miranda: section+ object constructors
    static int
    lca_grammar_new (lua_State * L)
    {
        int grammar_stack_ix;

        if (0)
            printf ("%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);

        marpa_lua_newtable (L);
        /* [ grammar_table ] */
        grammar_stack_ix = marpa_lua_gettop (L);
        /* push "class_grammar" metatable */
        marpa_lua_pushvalue(L, marpa_lua_upvalueindex(2));
        marpa_lua_setmetatable (L, grammar_stack_ix);
        /* [ grammar_table ] */

        {
            Marpa_Config marpa_configuration;
            Marpa_Grammar *grammar_ud =
                (Marpa_Grammar *) marpa_lua_newuserdata (L,
                sizeof (Marpa_Grammar));
            /* [ grammar_table, class_ud ] */
            marpa_lua_rawgetp (L, LUA_REGISTRYINDEX, &kollos_g_ud_mt_key);
            /* [ grammar_table, class_ud ] */
            marpa_lua_setmetatable (L, -2);
            /* [ grammar_table, class_ud ] */

            marpa_lua_setfield (L, grammar_stack_ix, "_libmarpa");

            marpa_c_init (&marpa_configuration);
            *grammar_ud = marpa_g_new (&marpa_configuration);
            if (!*grammar_ud) {
                return libmarpa_error_handle (L, grammar_stack_ix, "marpa_g_new()");
            }
        }

        /* Set my "lmw_g" field to myself */
        marpa_lua_pushvalue (L, grammar_stack_ix);
        marpa_lua_setfield (L, grammar_stack_ix, "lmw_g");

        marpa_lua_settop (L, grammar_stack_ix);
        /* [ grammar_table ] */
        return 1;
    }

```

## Output

### The main Lua code file

```
  -- miranda: section create metal tables
  --[==[ miranda: exec create metal tables
        local result = { "  _M.metal = {}" }
        for _, class in pairs(libmarpa_class_name) do
           local metal_table_name = 'metal_' .. class
           result[#result+1] = string.format("  _M[%q] = {}", metal_table_name);
        end
       result[#result+1] = ""
       return table.concat(result, "\n")
  ]==]

```

```
    -- miranda: section main
    -- miranda: insert legal preliminaries
    -- miranda: insert luacheck declarations

    local _M = require "kollos.metal"
    _M.upvalues.kollos = _M
    _M.defines = {}
    _M.registry = {}

    -- miranda: insert forward declarations
    -- miranda: insert internal utilities

    -- miranda: insert create metal tables
    -- miranda: insert adjust metal tables
    -- miranda: insert create nonmetallic metatables
    -- miranda: insert populate metatables

    -- set up various tables

    -- miranda: insert constant Lua tables

    -- miranda: insert create sandbox table

    -- miranda: insert VM utilities
    -- miranda: insert VM operations
    -- miranda: insert VM default operations
    -- miranda: insert grammar Libmarpa wrapper Lua functions
    -- miranda: insert recognizer Libmarpa wrapper Lua functions
    -- miranda: insert valuator Libmarpa wrapper Lua functions
    -- miranda: insert diagnostics
    -- miranda: insert Utilities for semantics
    -- miranda: insert most Lua function definitions
    -- miranda: insert define Lua error codes
    -- miranda: insert define Lua event codes
    -- miranda: insert define Lua step codes
    -- miranda: insert various Kollos Lua defines

    return _M

    -- vim: set expandtab shiftwidth=4:
```

#### Preliminaries to the main code

Licensing, etc.

```

    -- miranda: section legal preliminaries

    -- Copyright 2018 Jeffrey Kegler
    -- Permission is hereby granted, free of charge, to any person obtaining a
    -- copy of this software and associated documentation files (the "Software"),
    -- to deal in the Software without restriction, including without limitation
    -- the rights to use, copy, modify, merge, publish, distribute, sublicense,
    -- and/or sell copies of the Software, and to permit persons to whom the
    -- Software is furnished to do so, subject to the following conditions:
    --
    -- The above copyright notice and this permission notice shall be included
    -- in all copies or substantial portions of the Software.
    --
    -- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    -- IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    -- FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    -- THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
    -- OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
    -- ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    -- OTHER DEALINGS IN THE SOFTWARE.
    --
    -- [ MIT license: http://www.opensource.org/licenses/mit-license.php ]
```

Luacheck declarations

```

    -- miranda: section luacheck declarations

    -- luacheck: std lua53
    -- luacheck: globals bit
    -- luacheck: globals __FILE__ __LINE__

```

### The Kollos C code file

```
    -- miranda: section kollos_c
    -- miranda: language c
    -- miranda: insert preliminaries to the c library code

    -- miranda: insert private error code declarations
    -- miranda: insert define error codes
    -- miranda: insert private event code declarations
    -- miranda: insert define event codes
    -- miranda: insert private step code declarations
    -- miranda: insert define step codes

    -- miranda: insert base error handlers

    -- miranda: insert utility function definitions

    -- miranda: insert event related code
    -- miranda: insert step structure code
    -- miranda: insert metatable keys
    -- miranda: insert non-standard wrappers
    -- miranda: insert object userdata gc methods
    -- miranda: insert kollos table methods
    -- miranda: insert class_slr C methods
    -- miranda: insert luaL_Reg definitions
    -- miranda: insert object constructors

    -- miranda: insert standard libmarpa wrappers
    -- miranda: insert define kollos_metal_loader method

    -- miranda: insert  external C function definitions
    /* vim: set expandtab shiftwidth=4: */
```

#### Stuff from okollos

```

    -- miranda: section+ utility function definitions

    /* For debugging */
    static void dump_stack (lua_State *L) UNUSED;
    static void dump_stack (lua_State *L) {
          int i;
          int top = marpa_lua_gettop(L);
          for (i = 1; i <= top; i++) {  /* repeat for each level */
            int t = marpa_lua_type(L, i);
            switch (t) {

              case LUA_TSTRING:  /* strings */
                printf("`%s'", marpa_lua_tostring(L, i));
                break;

              case LUA_TBOOLEAN:  /* booleans */
                printf(marpa_lua_toboolean(L, i) ? "true" : "false");
                break;

              case LUA_TNUMBER:  /* numbers */
                printf("%g", marpa_lua_tonumber(L, i));
                break;

              default:  /* other values */
                printf("%s", marpa_lua_typename(L, t));
                break;

            }
            printf("  ");  /* put a separator */
          }
          printf("\n");  /* end the listing */
    }

    -- miranda: section private error code declarations
    /* error codes */

    struct s_libmarpa_error_code {
       lua_Integer code;
       const char* mnemonic;
       const char* description;
    };

    -- miranda: section+ base error handlers

    /* error objects
     *
     * There are written in C, but not because of efficiency --
     * efficiency is not needed, and in any case, when the overhead
     * from the use of the debug calls is considered, is not really
     * gained.
     *
     * The reason for the use of C is that the error routines
     * must be available for use inside both C and Lua, and must
     * also be available as early as possible during set up.
     * It's possible to run Lua code both inside C and early in
     * the set up, but the added unclarity, complexity from issues
     * of error reporting for the Lua code, etc., etc. mean that
     * it actually is easier to write them in C than in Lua.
     */

    -- miranda: section+ base error handlers

    static inline const char *
    error_description_by_code (lua_Integer error_code)
    {
        if (error_code >= LIBMARPA_MIN_ERROR_CODE
            && error_code <= LIBMARPA_MAX_ERROR_CODE) {
            return marpa_error_codes[error_code -
                LIBMARPA_MIN_ERROR_CODE].description;
        }
        if (error_code >= KOLLOS_MIN_ERROR_CODE
            && error_code <= KOLLOS_MAX_ERROR_CODE) {
            return marpa_kollos_error_codes[error_code -
                KOLLOS_MIN_ERROR_CODE].description;
        }
        return (const char *) 0;
    }

    static inline void
    push_error_description_by_code (lua_State * L,
        lua_Integer error_code)
    {
        const char *description =
            error_description_by_code (error_code);
        if (description) {
            marpa_lua_pushstring (L, description);
        } else {
            marpa_lua_pushfstring (L, "Unknown error code (%d)",
                error_code);
        }
    }

    static inline int lca_error_description_by_code(lua_State* L)
    {
       const lua_Integer error_code = marpa_luaL_checkinteger(L, 1);
       if (marpa_lua_isinteger(L, 1)) {
           push_error_description_by_code(L, error_code);
           return 1;
       }
       marpa_luaL_tolstring(L, 1, NULL);
       return 1;
    }

    static inline const char* error_name_by_code(lua_Integer error_code)
    {
       if (error_code >= LIBMARPA_MIN_ERROR_CODE && error_code <= LIBMARPA_MAX_ERROR_CODE) {
           return marpa_error_codes[error_code-LIBMARPA_MIN_ERROR_CODE].mnemonic;
       }
       if (error_code >= KOLLOS_MIN_ERROR_CODE && error_code <= KOLLOS_MAX_ERROR_CODE) {
           return marpa_kollos_error_codes[error_code-KOLLOS_MIN_ERROR_CODE].mnemonic;
       }
       return (const char *)0;
    }

    static inline int lca_error_name_by_code(lua_State* L)
    {
       const lua_Integer error_code = marpa_luaL_checkinteger(L, 1);
       const char* mnemonic = error_name_by_code(error_code);
       if (mnemonic)
       {
           marpa_lua_pushstring(L, mnemonic);
       } else {
           marpa_lua_pushfstring(L, "Unknown error code (%d)", error_code);
       }
       return 1;
    }

    -- miranda: section private event code declarations

    struct s_libmarpa_event_code {
       lua_Integer code;
       const char* mnemonic;
       const char* description;
    };

    -- miranda: section+ event related code

    static inline const char* event_description_by_code(lua_Integer event_code)
    {
       if (event_code >= LIBMARPA_MIN_EVENT_CODE && event_code <= LIBMARPA_MAX_EVENT_CODE) {
           return marpa_event_codes[event_code-LIBMARPA_MIN_EVENT_CODE].description;
       }
       return (const char *)0;
    }

    static inline int lca_event_description_by_code(lua_State* L)
    {
       const lua_Integer event_code = marpa_luaL_checkinteger(L, 1);
       const char* description = event_description_by_code(event_code);
       if (description)
       {
           marpa_lua_pushstring(L, description);
       } else {
           marpa_lua_pushfstring(L, "Unknown event code (%d)", event_code);
       }
       return 1;
    }

    static inline const char* event_name_by_code(lua_Integer event_code)
    {
       if (event_code >= LIBMARPA_MIN_EVENT_CODE && event_code <= LIBMARPA_MAX_EVENT_CODE) {
           return marpa_event_codes[event_code-LIBMARPA_MIN_EVENT_CODE].mnemonic;
       }
       return (const char *)0;
    }

    static inline int lca_event_name_by_code(lua_State* L)
    {
       const lua_Integer event_code = marpa_luaL_checkinteger(L, 1);
       const char* mnemonic = event_name_by_code(event_code);
       if (mnemonic)
       {
           marpa_lua_pushstring(L, mnemonic);
       } else {
           marpa_lua_pushfstring(L, "Unknown event code (%d)", event_code);
       }
       return 1;
    }

    -- miranda: section private step code declarations

    /* step codes */

    struct s_libmarpa_step_code {
       lua_Integer code;
       const char* mnemonic;
    };

    -- miranda: section+ step structure code

    static inline const char* step_name_by_code(lua_Integer step_code)
    {
       if (step_code >= MARPA_MIN_STEP_CODE && step_code <= MARPA_MAX_STEP_CODE) {
           return marpa_step_codes[step_code-MARPA_MIN_STEP_CODE].mnemonic;
       }
       return (const char *)0;
    }

    static inline int l_step_name_by_code(lua_State* L)
    {
       const lua_Integer step_code = marpa_luaL_checkinteger(L, 1);
       const char* mnemonic = step_name_by_code(step_code);
       if (mnemonic)
       {
           marpa_lua_pushstring(L, mnemonic);
       } else {
           marpa_lua_pushfstring(L, "Unknown step code (%d)", step_code);
       }
       return 1;
    }

    -- miranda: section+ metatable keys

    /* userdata metatable keys
       The contents of these locations are never examined.
       These location are used as a key in the Lua registry.
       This guarantees that the key will be unique
       within the Lua state.
    */
    static char kollos_g_ud_mt_key;
    static char kollos_r_ud_mt_key;
    static char kollos_b_ud_mt_key;
    static char kollos_o_ud_mt_key;
    static char kollos_t_ud_mt_key;
    static char kollos_v_ud_mt_key;
    static char kollos_ltrv_ud_mt_key;
    static char kollos_ptrv_ud_mt_key;
    static char kollos_trv_ud_mt_key;

```

```

    -- miranda: section+ base error handlers

    /* Leaves the stack as before,
       except with the error object on top */
    static inline void push_error_object(lua_State* L,
        lua_Integer code,
        const char* message,
        const char* details)
    {
       const int error_object_stack_ix = marpa_lua_gettop(L)+1;
       marpa_lua_newtable(L);
       /* [ ..., error_object ] */
       marpa_lua_rawgetp(L, LUA_REGISTRYINDEX, &kollos_X_mt_key);
       /* [ ..., error_object, error_metatable ] */
       marpa_lua_setmetatable(L, error_object_stack_ix);
       /* [ ..., error_object ] */
       marpa_lua_pushinteger(L, code);
       marpa_lua_setfield(L, error_object_stack_ix, "code" );
      if (0) printf ("%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);
      if (0) printf ("%s code = %ld\n", __PRETTY_FUNCTION__, (long)code);
       /* [ ..., error_object ] */

       marpa_luaL_traceback(L, L, NULL, 1);
       marpa_lua_setfield(L, error_object_stack_ix, "where");

       if (message) {
           marpa_lua_pushstring(L, message);
           marpa_lua_setfield(L, error_object_stack_ix, "message" );
       }

       marpa_lua_pushstring(L, details);
       marpa_lua_setfield(L, error_object_stack_ix, "details" );
       /* [ ..., error_object ] */
    }

    -- miranda: section+ base error handlers

    /* grammar wrappers which need to be hand written */

    /* Get the throw flag from a libmarpa_wrapper.
     */
    static int get_throw_flag(lua_State* L, int lmw_stack_ix)
    {
        int result;
        const int base_of_stack = marpa_lua_gettop (L);
        marpa_luaL_checkstack (L, 10, "cannot grow stack");
        marpa_lua_pushvalue (L, lmw_stack_ix);
        if (!marpa_lua_getmetatable (L, lmw_stack_ix))
            goto FAILURE;
        if (marpa_lua_getfield (L, -1, "kollos") != LUA_TTABLE)
            goto FAILURE;
        if (marpa_lua_getfield (L, -1, "throw") != LUA_TBOOLEAN)
            goto FAILURE;
        result = marpa_lua_toboolean (L, -1);
        marpa_lua_settop (L, base_of_stack);
        return result;
      FAILURE:
        push_error_object (L, MARPA_ERR_DEVELOPMENT, 0, "Bad throw flag");
        return marpa_lua_error (L);
    }

    /* Development errors are always thrown.
     */
    static void
    development_error_handle (lua_State * L,
                            const char *details)
    {
      push_error_object(L, MARPA_ERR_DEVELOPMENT, 0, details);
      marpa_lua_pushvalue(L, -1);
      marpa_lua_setfield(L, marpa_lua_upvalueindex(1), "error_object");
      marpa_lua_error(L);
    }

```

Internal errors are those which "should not happen".
Often they were be caused by bugs.
Under the description, an exact and specific description
of the cause is not possible.
Instead,  information pinpointing the location in the
source code is provided.
The "throw" flag is ignored.

```
    -- miranda: section+ base error handlers
    static int
    internal_error_handle (lua_State * L,
                            const char *details,
                            const char *function,
                            const char *file,
                            int line
                            )
    {
      int error_object_ix;
      push_error_object(L, MARPA_ERR_LUA_INTERNAL, 0, details);
      error_object_ix = marpa_lua_gettop(L);
      marpa_lua_pushstring(L, function);
      marpa_lua_setfield(L, error_object_ix, "function");
      marpa_lua_pushstring(L, file);
      marpa_lua_setfield(L, error_object_ix, "file");
      marpa_lua_pushinteger(L, line);
      marpa_lua_setfield(L, error_object_ix, "line");
      marpa_lua_pushvalue(L, error_object_ix);
      marpa_lua_setfield(L, marpa_lua_upvalueindex(1), "error_object");
      marpa_lua_error(L);
      return 0; /* NOTREACHED -- to silence warning message */
    }

    static int out_of_memory(lua_State* L) UNUSED;
    static int out_of_memory(lua_State* L) {
        return marpa_luaL_error(L, "Kollos out of memory");
    }

    /* If error is not thrown, it leaves a nil, then
     * the error object, on the stack.
     */
    static int
    libmarpa_error_code_handle (lua_State * L,
                            int lmw_stack_ix,
                            int error_code,
                            const char *message,
                            const char *details)
    {
      int throw_flag = get_throw_flag(L, lmw_stack_ix);
      if (!throw_flag) {
          marpa_lua_pushnil(L);
      }
      push_error_object(L, error_code, message, details);
      if (0) fprintf (stderr, "%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);
      /* [ ..., nil, error_object ] */
      marpa_lua_pushvalue(L, -1);
      marpa_lua_setfield(L, marpa_lua_upvalueindex(1), "error_object");
      if (0) fprintf (stderr, "%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);
      if (throw_flag) return marpa_lua_error(L);
      if (0) fprintf (stderr, "%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);
      return 2;
    }

    /* Handle libmarpa errors in the most usual way.
       Uses 2 positions on the stack, and throws the
       error, if so desired.
       The error may be thrown or not thrown.
       The caller is expected to handle any non-thrown error.
    */
    static int
    libmarpa_error_handle (lua_State * L,
                            int stack_ix,
                            const char *details)
    {
      Marpa_Error_Code error_code;
      Marpa_Grammar *grammar_ud;
      const int base_of_stack = marpa_lua_gettop(L);
      const char* p_message;

      marpa_lua_getfield (L, stack_ix, "lmw_g");
      marpa_lua_getfield (L, -1, "_libmarpa");
      /* [ ..., grammar_ud ] */
      grammar_ud = (Marpa_Grammar *) marpa_lua_touserdata (L, -1);
      marpa_lua_settop(L, base_of_stack);
      error_code = marpa_g_error (*grammar_ud, &p_message);
      return libmarpa_error_code_handle(L,
          stack_ix, error_code, p_message, details);
    }

    /* A wrapper for libmarpa_error_handle to conform with the
     * Lua C API.  The only argument must be a Libmarpa wrapper
     * object.  These all define the `lmw_g` field.
     */
    static int
    lca_libmarpa_error(lua_State* L)
    {
       const int lmw_stack_ix = 1;
       const int details_stack_ix = 2;
       const char* details = marpa_lua_tostring (L, details_stack_ix);
       libmarpa_error_handle(L, lmw_stack_ix, details);
       /* Return only the error object,
        * not the nil on the stack
        * below it.
        */
       return 1;
    }

```

Return the current error_description
Lua C API.  The only argument must be a Libmarpa wrapper
object.
All such objects define the `lmw_g` field.
```
    -- miranda: section+ base error handlers

    static int
    lca_libmarpa_error_description(lua_State* L)
    {
        Marpa_Error_Code error_code;
        Marpa_Grammar *grammar_ud;
        const int lmw_stack_ix = 1;

        marpa_lua_getfield (L, lmw_stack_ix, "lmw_g");
        marpa_lua_getfield (L, -1, "_libmarpa");
        grammar_ud = (Marpa_Grammar *) marpa_lua_touserdata (L, -1);
        error_code = marpa_g_error (*grammar_ud, NULL);
        push_error_description_by_code(L, error_code);
        return 1;
    }

```

Return the current error data:
code, mnemonic and description.
Lua C API.  The only argument must be a Libmarpa wrapper
object.
All such objects define the `lmw_g` field.

```
    -- miranda: section+ base error handlers

    static int
    lca_libmarpa_error_code(lua_State* L)
    {
        Marpa_Error_Code error_code;
        Marpa_Grammar *grammar_ud;
        const int lmw_stack_ix = 1;
        const char *message;

        marpa_lua_getfield (L, lmw_stack_ix, "lmw_g");
        marpa_lua_getfield (L, -1, "_libmarpa");
        grammar_ud = (Marpa_Grammar *) marpa_lua_touserdata (L, -1);
        error_code = marpa_g_error (*grammar_ud, &message);
        marpa_lua_pushinteger (L, error_code);
        if (message) {
            marpa_lua_pushstring (L, message);
        } else {
            marpa_lua_pushnil (L);
        }
        return 2;
    }

    -- miranda: section+ non-standard wrappers

    /* The C wrapper for Libmarpa event reading.
       It assumes we just want all of them.
     */
    static int lca_grammar_events(lua_State *L)
    {
      /* [ grammar_object ] */
      const int grammar_stack_ix = 1;
      Marpa_Grammar *p_g;
      int event_count;

      marpa_lua_getfield (L, grammar_stack_ix, "_libmarpa");
      /* [ grammar_object, grammar_ud ] */
      p_g = (Marpa_Grammar *) marpa_lua_touserdata (L, -1);
      event_count = marpa_g_event_count (*p_g);
      if (event_count < 0)
        {
          return libmarpa_error_handle (L, grammar_stack_ix,
                                  "marpa_g_event_count()");
        }
      marpa_lua_pop (L, 1);
      /* [ grammar_object ] */
      marpa_lua_createtable (L, event_count, 0);
      /* [ grammar_object, result_table ] */
      {
        const int result_table_ix = marpa_lua_gettop (L);
        int event_ix;
        for (event_ix = 0; event_ix < event_count; event_ix++)
          {
            Marpa_Event_Type event_type;
            Marpa_Event event;
            /* [ grammar_object, result_table ] */
            event_type = marpa_g_event (*p_g, &event, event_ix);
            if (event_type <= -2)
              {
                return libmarpa_error_handle (L, grammar_stack_ix,
                                        "marpa_g_event()");
              }
            marpa_lua_pushinteger (L, event_ix*2 + 1);
            marpa_lua_pushinteger (L, event_type);
            /* [ grammar_object, result_table, event_ix*2+1, event_type ] */
            marpa_lua_settable (L, result_table_ix);
            /* [ grammar_object, result_table ] */
            marpa_lua_pushinteger (L, event_ix*2 + 2);
            marpa_lua_pushinteger (L, marpa_g_event_value (&event));
            /* [ grammar_object, result_table, event_ix*2+2, event_value ] */
            marpa_lua_settable (L, result_table_ix);
            /* [ grammar_object, result_table ] */
          }
      }
      /* [ grammar_object, result_table ] */
      return 1;
    }

    /* Another C wrapper for Libmarpa event reading.
       It assumes we want them one by one.
     */
    static int lca_grammar_event(lua_State *L)
    {
      /* [ grammar_object ] */
      const int grammar_stack_ix = 1;
      const int event_ix_stack_ix = 2;
      Marpa_Grammar *p_g;
      Marpa_Event_Type event_type;
      Marpa_Event event;
      const int event_ix = (Marpa_Symbol_ID)marpa_lua_tointeger(L, event_ix_stack_ix)-1;

      marpa_lua_getfield (L, grammar_stack_ix, "_libmarpa");
      /* [ grammar_object, grammar_ud ] */
      p_g = (Marpa_Grammar *) marpa_lua_touserdata (L, -1);
      /* [ grammar_object, grammar_ud ] */
      event_type = marpa_g_event (*p_g, &event, event_ix);
      if (event_type <= -2)
        {
          return libmarpa_error_handle (L, grammar_stack_ix, "marpa_g_event()");
        }
      marpa_lua_pushinteger (L, event_type);
      marpa_lua_pushinteger (L, marpa_g_event_value (&event));
      /* [ grammar_object, grammar_ud, event_type, event_value ] */
      return 2;
    }
```

-2 is a valid result, so `rule_rank_set()` is a special case.

TODO: Returning the rank from `marpa_g_rule_rank_set()`
was a bad idea.
Change the return value in libmarpa, so that
`marpa_g_rule_rank_set()` becomes a standard
function and does not require a special wrapper.

```
    -- miranda: section+ non-standard wrappers
    static int lca_grammar_rule_rank_set(lua_State *L)
    {
        Marpa_Grammar grammar;
        const int grammar_stack_ix = 1;
        Marpa_Rule_ID rule_id;
        Marpa_Rank rank;
        int result;

        if (1) {
            marpa_luaL_checktype (L, grammar_stack_ix, LUA_TTABLE);
            marpa_luaL_checkinteger (L, 2);
            marpa_luaL_checkinteger (L, 3);
        }
        {
            const lua_Integer this_arg = marpa_lua_tointeger (L, 2);
            marpa_luaL_argcheck (L, (-(1 << 30) <= this_arg
                    && this_arg <= (1 << 30)), -1, "argument out of range");
            rule_id = (Marpa_Rule_ID) this_arg;
        }
        {
            const lua_Integer this_arg = marpa_lua_tointeger (L, 3);
            marpa_luaL_argcheck (L, (-(1 << 30) <= this_arg
                    && this_arg <= (1 << 30)), -1, "argument out of range");
            rank = (Marpa_Rank) this_arg;
        }
        marpa_lua_getfield (L, grammar_stack_ix, "_libmarpa");
        grammar = *(Marpa_Grammar *) marpa_lua_touserdata (L, -1);
        marpa_lua_settop (L, grammar_stack_ix);
        result = (int) marpa_g_rule_rank_set (grammar, rule_id, rank);
        if (result == -2) {
            Marpa_Error_Code error_code = marpa_g_error (grammar, NULL);
            if (error_code != MARPA_ERR_NONE) {
                return libmarpa_error_handle (L, grammar_stack_ix,
                    "lca_grammar_rule_rank_set()");
            }
        }
        marpa_lua_pushinteger (L, (lua_Integer) result);
        return 1;
    }
```

`lca_grammar_rule_new` wraps the Libmarpa method `marpa_g_rule_new()`.
If the rule is 7 symbols or fewer, I put it on the stack.  As an old
kernel driver programmer, I was trained to avoid putting even small
arrays on the stack, but one of this size should be safe on anything
close to a modern architecture.

Perhaps I will eventually limit Libmarpa's
rule RHS to 7 symbols, 7 because I can encode dot position in 3 bit.

```
    -- miranda: section+ non-standard wrappers

    static int lca_grammar_rule_new(lua_State *L)
    {
        Marpa_Grammar g;
        Marpa_Rule_ID result;
        Marpa_Symbol_ID lhs;

        /* [ grammar_object, lhs, rhs ... ] */
        const int grammar_stack_ix = 1;
        const int args_stack_ix = 2;
        /* 7 should be enough, almost always */
        const int rhs_buffer_size = 7;
        Marpa_Symbol_ID rhs_buffer[rhs_buffer_size];
        Marpa_Symbol_ID *rhs;
        int overflow = 0;
        lua_Integer arg_count;
        lua_Integer table_ix;

        /* This will not be an external interface,
         * so eventually we will run unsafe.
         * This checking code is for debugging.
         */
        marpa_luaL_checktype(L, grammar_stack_ix, LUA_TTABLE);
        marpa_luaL_checktype(L, args_stack_ix, LUA_TTABLE);

        marpa_lua_len(L, args_stack_ix);
        arg_count = marpa_lua_tointeger(L, -1);
        if (arg_count > 1<<30) {
            marpa_luaL_error(L,
                "grammar:rule_new() arg table length too long");
        }
        if (arg_count < 1) {
            marpa_luaL_error(L,
                "grammar:rule_new() arg table length must be at least 1");
        }

        /* arg_count - 2 == rhs_ix
         * For example, arg_count of 3, has one arg for LHS,
         * and 2 for RHS, so max rhs_ix == 1
         */
        if (((size_t)arg_count - 2) >= (sizeof(rhs_buffer)/sizeof(*rhs_buffer))) {
           /* Treat "overflow" arg counts as freaks.
            * We do not optimize for them, but do a custom
            * malloc/free pair for each.
            */
           rhs = malloc(sizeof(Marpa_Symbol_ID) * (size_t)arg_count);
           overflow = 1;
        } else {
           rhs = rhs_buffer;
        }

        marpa_lua_geti(L, args_stack_ix, 1);
        lhs = (Marpa_Symbol_ID)marpa_lua_tointeger(L, -1);
        for (table_ix = 2; table_ix <= arg_count; table_ix++)
        {
            /* Calculated as above */
            const int rhs_ix = (int)table_ix - 2;
            marpa_lua_geti(L, args_stack_ix, table_ix);
            rhs[rhs_ix] = (Marpa_Symbol_ID)marpa_lua_tointeger(L, -1);
            marpa_lua_settop(L, args_stack_ix);
        }

        marpa_lua_getfield (L, grammar_stack_ix, "_libmarpa");
        /* [ grammar_object, grammar_ud ] */
        g = *(Marpa_Grammar *) marpa_lua_touserdata (L, -1);

        result = (Marpa_Rule_ID)marpa_g_rule_new(g, lhs, rhs, ((int)arg_count - 1));
        if (overflow) free(rhs);
        if (result <= -1) return libmarpa_error_handle (L, grammar_stack_ix,
                                "marpa_g_rule_new()");
        marpa_lua_pushinteger(L, (lua_Integer)result);
        return 1;
    }

```

`lca_grammar_sequence_new` wraps the Libmarpa method `marpa_g_sequence_new()`.
If the rule is 7 symbols or fewer, I put it on the stack.  As an old
kernel driver programmer, I was trained to avoid putting even small
arrays on the stack, but one of this size should be safe on anything
like close to a modern architecture.

Perhaps I will eventually limit Libmarpa's
rule RHS to 7 symbols, 7 because I can encode dot position in 3 bit.

```
    -- miranda: section+ non-standard wrappers

    static int lca_grammar_sequence_new(lua_State *L)
    {
        Marpa_Grammar *p_g;
        Marpa_Rule_ID result;
        lua_Integer lhs = -1;
        lua_Integer rhs = -1;
        lua_Integer separator = -1;
        lua_Integer min = 1;
        int proper = 0;
        const int grammar_stack_ix = 1;
        const int args_stack_ix = 2;

        marpa_luaL_checktype (L, grammar_stack_ix, LUA_TTABLE);
        marpa_luaL_checktype (L, args_stack_ix, LUA_TTABLE);

        marpa_lua_pushnil (L);
        /* [ ..., nil ] */
        while (marpa_lua_next (L, args_stack_ix)) {
            /* [ ..., key, value ] */
            const char *string_key;
            const int value_stack_ix = marpa_lua_gettop (L);
            const int key_stack_ix = value_stack_ix - 1;
            int is_int = 0;
            switch (marpa_lua_type (L, key_stack_ix)) {

            case LUA_TSTRING:      /* strings */
                /* lua_tostring() is safe because arg is always a string */
                string_key = marpa_lua_tostring (L, key_stack_ix);
                if (!strcmp (string_key, "min")) {
                    min = marpa_lua_tointegerx (L, value_stack_ix, &is_int);
                    if (!is_int) {
                        return marpa_luaL_error (L,
                            "grammar:sequence_new() value of 'min' must be numeric");
                    }
                    goto NEXT_ELEMENT;
                }
                if (!strcmp (string_key, "eager")) {
                    goto NEXT_ELEMENT;
                }
                if (!strcmp (string_key, "proper")) {
                    proper = marpa_lua_toboolean (L, value_stack_ix);
                    goto NEXT_ELEMENT;
                }
                if (!strcmp (string_key, "separator")) {
                    separator =
                        marpa_lua_tointegerx (L, value_stack_ix, &is_int);
                    if (!is_int) {
                        return marpa_luaL_error (L,
                            "grammar:sequence_new() value of 'separator' must be a symbol ID");
                    }
                    goto NEXT_ELEMENT;
                }
                if (!strcmp (string_key, "lhs")) {
                    lhs = marpa_lua_tointegerx (L, value_stack_ix, &is_int);
                    if (!is_int || lhs < 0) {
                        return marpa_luaL_error (L,
                            "grammar:sequence_new() LHS must be a valid symbol ID");
                    }
                    goto NEXT_ELEMENT;
                }
                if (!strcmp (string_key, "rhs")) {
                    rhs = marpa_lua_tointegerx (L, value_stack_ix, &is_int);
                    if (!is_int || rhs < 0) {
                        return marpa_luaL_error (L,
                            "grammar:sequence_new() RHS must be a valid symbol ID");
                    }
                    goto NEXT_ELEMENT;
                }
                return marpa_luaL_error (L,
                    "grammar:sequence_new() bad string key (%s) in arg table",
                    string_key);

            default:               /* other values */
                return marpa_luaL_error (L,
                    "grammar:sequence_new() bad key type (%s) in arg table",
                    marpa_lua_typename (L, marpa_lua_type (L, key_stack_ix))
                    );

            }

          NEXT_ELEMENT:

            /* [ ..., key, value, key_copy ] */
            marpa_lua_settop (L, key_stack_ix);
            /* [ ..., key ] */
        }


        if (lhs < 0) {
            return marpa_luaL_error (L,
                "grammar:sequence_new(): LHS argument is missing");
        }
        if (rhs < 0) {
            return marpa_luaL_error (L,
                "grammar:sequence_new(): RHS argument is missing");
        }

        marpa_lua_getfield (L, grammar_stack_ix, "_libmarpa");
        p_g = (Marpa_Grammar *) marpa_lua_touserdata (L, -1);

        result =
            (Marpa_Rule_ID) marpa_g_sequence_new (*p_g,
            (Marpa_Symbol_ID) lhs,
            (Marpa_Symbol_ID) rhs,
            (Marpa_Symbol_ID) separator,
            (int) min, (proper ? MARPA_PROPER_SEPARATION : 0)
            );
        if (result <= -1)
            return libmarpa_error_handle (L, grammar_stack_ix,
                "marpa_g_rule_new()");
        marpa_lua_pushinteger (L, (lua_Integer) result);
        return 1;
    }

    static int lca_grammar_precompute(lua_State *L)
    {
        Marpa_Grammar self;
        const int self_stack_ix = 1;
        int highest_symbol_id;
        int result;

        if (1) {
            marpa_luaL_checktype (L, self_stack_ix, LUA_TTABLE);
        }
        marpa_lua_getfield (L, -1, "_libmarpa");
        self = *(Marpa_Grammar *) marpa_lua_touserdata (L, -1);
        marpa_lua_pop (L, 1);
        result = (int) marpa_g_precompute (self);
        if (result == -1) {
            marpa_lua_pushnil (L);
            return 1;
        }
        if (result < -1) {
            return libmarpa_error_handle (L, self_stack_ix,
                "grammar:precompute; marpa_g_precompute");
        }

        highest_symbol_id = marpa_g_highest_symbol_id (self);
        if (highest_symbol_id < 0) {
            return libmarpa_error_handle (L, self_stack_ix,
                "grammar:precompute; marpa_g_highest_symbol_id");
            return 1;
        }

        if (0) {
            printf ("%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);
            printf ("About to resize buffer to %ld", (long) ( highest_symbol_id+1));
        }

        (void)kollos_shared_buffer_resize(L, (size_t) highest_symbol_id+1);
        marpa_lua_pushinteger (L, (lua_Integer) result);
        return 1;
    }
```

-1 is a valid result, so `ahm_position()` is a special case.

```
    -- miranda: section+ non-standard wrappers
    static int lca_grammar_ahm_position(lua_State *L)
    {
        Marpa_Grammar self;
        const int self_stack_ix = 1;
        Marpa_AHM_ID item_id;
        int result;

        if (1) {
            marpa_luaL_checktype (L, self_stack_ix, LUA_TTABLE);
            marpa_luaL_checkinteger (L, 2);
        }
        {
            const lua_Integer this_arg = marpa_lua_tointeger (L, -1);
            marpa_luaL_argcheck (L, (-(1 << 30) <= this_arg
                    && this_arg <= (1 << 30)), -1, "argument out of range");
            item_id = (Marpa_AHM_ID) this_arg;
            marpa_lua_pop (L, 1);
        }
        marpa_lua_getfield (L, -1, "_libmarpa");
        self = *(Marpa_Grammar *) marpa_lua_touserdata (L, -1);
        marpa_lua_pop (L, 1);
        result = (int) _marpa_g_ahm_position (self, item_id);
        if (result < -1) {
            return libmarpa_error_handle (L, self_stack_ix,
                "lca_grammar_ahm_position()");
        }
        marpa_lua_pushinteger (L, (lua_Integer) result);
        return 1;
    }
```

```
    -- miranda: section+ luaL_Reg definitions

    static const struct luaL_Reg grammar_methods[] = {
      { "error", lca_libmarpa_error },
      { "error_code", lca_libmarpa_error_code },
      { "error_description", lca_libmarpa_error_description },
      { "events", lca_grammar_events },
      { "precompute", lca_grammar_precompute },
      { "rule_rank_set", lca_grammar_rule_rank_set },
      { "rule_new", lca_grammar_rule_new },
      { "sequence_new", lca_grammar_sequence_new },
      { "_ahm_position", lca_grammar_ahm_position },
      { NULL, NULL },
    };

```

```
    -- miranda: section+ non-standard wrappers
    static int lca_recce_progress_item(lua_State *L)
    {
      /* [ recce_object ] */
      const int recce_stack_ix = 1;
      Marpa_Recce r;
      Marpa_Earley_Set_ID origin;
      int position;
      Marpa_Rule_ID rule_id;

      marpa_lua_getfield (L, recce_stack_ix, "_libmarpa");
      /* [ recce_object, recce_ud ] */
      r = *(Marpa_Recce *) marpa_lua_touserdata (L, -1);
      rule_id = marpa_r_progress_item (r, &position, &origin);
      if (rule_id < -1)
        {
          return libmarpa_error_handle (L, recce_stack_ix, "recce:progress_item()");
        }
      if (rule_id == -1)
        {
          return 0;
        }
      marpa_lua_pushinteger (L, (lua_Integer) rule_id);
      marpa_lua_pushinteger (L, (lua_Integer) position);
      marpa_lua_pushinteger (L, (lua_Integer) origin);
      return 3;
    }

    static int lca_recce_terminals_expected( lua_State *L )
    {
      /* [ recce_object ] */
      const int recce_stack_ix = 1;
      int count;
      int ix;
      Marpa_Recce r;

      /* The shared buffer is guaranteed to have space for all the symbol IDS
       * of the grammar.
       */
      Marpa_Symbol_ID* const buffer = shared_buffer_get(L);

      marpa_lua_getfield (L, recce_stack_ix, "_libmarpa");
      /* [ recce_object, recce_ud ] */
      r = *(Marpa_Recce *) marpa_lua_touserdata (L, -1);

      count = marpa_r_terminals_expected (r, buffer);
      if (count < 0) {
          return libmarpa_error_handle(L, recce_stack_ix, "grammar:terminals_expected; marpa_r_terminals_expected");
      }
      marpa_lua_newtable(L);
      for (ix = 0; ix < count; ix++) {
          marpa_lua_pushinteger(L, buffer[ix]);
          marpa_lua_rawseti(L, -2, ix+1);
      }
      return 1;
    }

    /* special-cased because two return values */
    static int lca_recce_source_token( lua_State *L )
    {
      Marpa_Recognizer self;
      const int self_stack_ix = 1;
      int result;
      int value;

      if (1) {
        marpa_luaL_checktype(L, self_stack_ix, LUA_TTABLE);  }
      marpa_lua_getfield (L, -1, "_libmarpa");
      self = *(Marpa_Recognizer*)marpa_lua_touserdata (L, -1);
      marpa_lua_pop(L, 1);
      result = (int)_marpa_r_source_token(self, &value);
      if (result == -1) { marpa_lua_pushnil(L); return 1; }
      if (result < -1) {
       return libmarpa_error_handle(L, self_stack_ix, "lca_recce_source_token()");
      }
      marpa_lua_pushinteger(L, (lua_Integer)result);
      marpa_lua_pushinteger(L, (lua_Integer)value);
      return 2;
    }

    -- miranda: section+ luaL_Reg definitions

    static const struct luaL_Reg recce_methods[] = {
      { "error", lca_libmarpa_error },
      { "error_code", lca_libmarpa_error_code },
      { "error_description", lca_libmarpa_error_description },
      { "terminals_expected", lca_recce_terminals_expected },
      { "progress_item", lca_recce_progress_item },
      { "_source_token", lca_recce_source_token },
      { NULL, NULL },
    };

```

For `trv_dot()`, -1 is a valid return value,
not a soft error.

```
    -- miranda: section+ non-standard wrappers
    static int wrap_trv_dot(lua_State *L)
    {
      Marpa_Traverser self;
      const int self_stack_ix = 1;
      int result;

      marpa_luaL_checktype(L, self_stack_ix, LUA_TTABLE);
      marpa_lua_getfield (L, -1, "_libmarpa");
      self = *(Marpa_Traverser*)marpa_lua_touserdata (L, -1);
      marpa_lua_pop(L, 1);
      result = marpa_trv_dot(self);
      if (0) fprintf (stderr, "%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);
      if (0) fprintf (stderr, "dot=%ld\n", (long)result);
      if (result < -1) {
       return libmarpa_error_handle(L, self_stack_ix, "wrap_trv_dot()");
      }
      marpa_lua_pushinteger(L, (lua_Integer)result);
      return 1;
    }
```

For `trv_nrl_dot()`, -1 is a valid return value,
not a soft error.

```
    -- miranda: section+ non-standard wrappers
    static int wrap_trv_nrl_dot(lua_State *L)
    {
      Marpa_Traverser self;
      const int self_stack_ix = 1;
      int result;

      marpa_luaL_checktype(L, self_stack_ix, LUA_TTABLE);
      marpa_lua_getfield (L, -1, "_libmarpa");
      self = *(Marpa_Traverser*)marpa_lua_touserdata (L, -1);
      marpa_lua_pop(L, 1);
      result = marpa_trv_nrl_dot(self);
      if (0) fprintf (stderr, "%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);
      if (0) fprintf (stderr, "dot=%ld\n", (long)result);
      if (result < -1) {
       return libmarpa_error_handle(L, self_stack_ix, "wrap_trv_nrl_dot()");
      }
      marpa_lua_pushinteger(L, (lua_Integer)result);
      return 1;
    }
```

```

    -- miranda: section+ luaL_Reg definitions
    static const struct luaL_Reg traverser_methods[] = {
      { "completion_cause", lca_trv_completion_cause },
      { "completion_predecessor", lca_trv_completion_predecessor },
      { "dot", wrap_trv_dot },
      { "error", lca_libmarpa_error },
      { "error_code", lca_libmarpa_error_code },
      { "error_description", lca_libmarpa_error_description },
      { "lim", lca_trv_lim },
      { "nrl_dot", wrap_trv_nrl_dot },
      { "token_predecessor", lca_trv_token_predecessor },
      { NULL, NULL },
    };

    static const struct luaL_Reg ltraverser_methods[] = {
      { "error", lca_libmarpa_error },
      { "error_code", lca_libmarpa_error_code },
      { "error_description", lca_libmarpa_error_description },
      { "trailhead_eim", lca_ltrv_trailhead_eim },
      { "predecessor", lca_ltrv_predecessor },
      { NULL, NULL },
    };

    static const struct luaL_Reg ptraverser_methods[] = {
      { "error", lca_libmarpa_error },
      { "error_code", lca_libmarpa_error_code },
      { "error_description", lca_libmarpa_error_description },
      { "eim_iter", lca_ptrv_eim_iter },
      { NULL, NULL },
    };

    static const struct luaL_Reg bocage_methods[] = {
      { "error", lca_libmarpa_error },
      { "error_code", lca_libmarpa_error_code },
      { "error_description", lca_libmarpa_error_description },
      { NULL, NULL },
    };

    /* order wrappers which need to be hand-written */

    static const struct luaL_Reg order_methods[] = {
      { "error", lca_libmarpa_error },
      { "error_code", lca_libmarpa_error_code },
      { "error_description", lca_libmarpa_error_description },
      { NULL, NULL },
    };

    static const struct luaL_Reg tree_methods[] = {
      { "error", lca_libmarpa_error },
      { "error_code", lca_libmarpa_error_code },
      { "error_description", lca_libmarpa_error_description },
      { NULL, NULL },
    };

    /* value wrappers which need to be hand-written */

    -- miranda: section+ non-standard wrappers

    /* Returns ok, result,
     * where ok is a boolean and
     * on failure, result is an error object, while
     * on success, result is an table
     */
    static int
    wrap_v_step (lua_State * L)
    {
      const char *result_string;
      Marpa_Value v;
      Marpa_Step_Type step_type;
      const int value_stack_ix = 1;

      marpa_luaL_checktype (L, value_stack_ix, LUA_TTABLE);

      marpa_lua_getfield (L, value_stack_ix, "_libmarpa");
      /* [ value_table, value_ud ] */
      v = *(Marpa_Value *) marpa_lua_touserdata (L, -1);
      step_type = marpa_v_step (v);

      if (0) printf("%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);

      if (step_type == MARPA_STEP_INACTIVE)
        {
          marpa_lua_pushboolean (L, 1);
          marpa_lua_pushnil (L);
          return 2;
        }

      if (step_type < 0)
        {
          return libmarpa_error_handle (L, value_stack_ix, "marpa_v_step()");
        }

      if (0) printf("%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);

      result_string = step_name_by_code (step_type);
      if (result_string)
        {

          int return_value_ix;

          /* The table containing the return value */
          marpa_lua_newtable (L);
          return_value_ix = marpa_lua_gettop(L);
          marpa_lua_pushstring (L, result_string);
          marpa_lua_seti (L, return_value_ix, 1);

          if (step_type == MARPA_STEP_TOKEN)
            {
              marpa_lua_pushinteger (L, marpa_v_token (v));
              marpa_lua_seti (L, return_value_ix, 2);
              marpa_lua_pushinteger (L, marpa_v_token_start_es_id (v));
              marpa_lua_seti (L, return_value_ix, 3);
              marpa_lua_pushinteger (L, marpa_v_es_id (v));
              marpa_lua_seti (L, return_value_ix, 4);
              marpa_lua_pushinteger (L, marpa_v_result (v));
              marpa_lua_seti (L, return_value_ix, 5);
              marpa_lua_pushinteger (L, marpa_v_token_value (v));
              marpa_lua_seti (L, return_value_ix, 6);
              marpa_lua_pushboolean (L, 1);
              marpa_lua_insert (L, -2);
              return 2;
            }

          if (step_type == MARPA_STEP_NULLING_SYMBOL)
            {
              marpa_lua_pushinteger (L, marpa_v_token (v));
              marpa_lua_seti (L, return_value_ix, 2);
              marpa_lua_pushinteger (L, marpa_v_rule_start_es_id (v));
              marpa_lua_seti (L, return_value_ix, 3);
              marpa_lua_pushinteger (L, marpa_v_es_id (v));
              marpa_lua_seti (L, return_value_ix, 4);
              marpa_lua_pushinteger (L, marpa_v_result (v));
              marpa_lua_seti (L, return_value_ix, 5);
              marpa_lua_pushboolean (L, 1);
              marpa_lua_insert (L, -2);
              return 2;
            }

          if (step_type == MARPA_STEP_RULE)
            {
              marpa_lua_pushinteger (L, marpa_v_rule (v));
              marpa_lua_seti (L, return_value_ix, 2);
              marpa_lua_pushinteger (L, marpa_v_rule_start_es_id (v));
              marpa_lua_seti (L, return_value_ix, 3);
              marpa_lua_pushinteger (L, marpa_v_es_id (v));
              marpa_lua_seti (L, return_value_ix, 4);
              marpa_lua_pushinteger (L, marpa_v_result (v));
              marpa_lua_seti (L, return_value_ix, 5);
              marpa_lua_pushinteger (L, marpa_v_arg_0 (v));
              marpa_lua_seti (L, return_value_ix, 6);
              marpa_lua_pushinteger (L, marpa_v_arg_n (v));
              marpa_lua_seti (L, return_value_ix, 7);
              marpa_lua_pushboolean (L, 1);
              marpa_lua_insert (L, -2);
              return 2;
            }
        }

      if (0) printf("%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);

      marpa_lua_pushfstring (L, "Problem in v->step(): unknown step type %d",
                             step_type);
      development_error_handle (L, marpa_lua_tostring (L, -1));
      marpa_lua_pushboolean (L, 0);
      marpa_lua_insert (L, -2);
      return 2;

    }

    /* Returns ok, result,
     * where ok is a boolean and
     * on failure, result is an error object, while
     * on success, result is an table
     */
    static int
    wrap_v_location (lua_State * L)
    {
      Marpa_Value v;
      Marpa_Step_Type step_type;
      const int value_stack_ix = 1;

      marpa_luaL_checktype (L, value_stack_ix, LUA_TTABLE);

      marpa_lua_getfield (L, value_stack_ix, "_libmarpa");
      /* [ value_table, value_ud ] */
      v = *(Marpa_Value *) marpa_lua_touserdata (L, -1);
      step_type = marpa_v_step_type (v);

      if (0) printf("%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);

      switch(step_type) {
      case MARPA_STEP_RULE:
          marpa_lua_pushinteger(L, marpa_v_rule_start_es_id (v));
          marpa_lua_pushinteger(L, marpa_v_es_id (v));
          return 2;
      case MARPA_STEP_NULLING_SYMBOL:
          marpa_lua_pushinteger(L, marpa_v_token_start_es_id (v));
          marpa_lua_pushinteger(L, marpa_v_es_id (v));
          return 2;
      case MARPA_STEP_TOKEN:
          marpa_lua_pushinteger(L, marpa_v_token_start_es_id (v));
          marpa_lua_pushinteger(L, marpa_v_es_id (v));
          return 2;
      }
      return 0;
    }

    -- miranda: section+ luaL_Reg definitions

    static const struct luaL_Reg value_methods[] = {
      { "location", wrap_v_location },
      { "step", wrap_v_step },
      { NULL, NULL },
    };

    -- miranda: section+ object userdata gc methods

    /*
     * Userdata metatable methods
     */

    --[==[ miranda: exec object userdata gc methods
        local result = {}
        local template = [[
        |static int l_!NAME!_ud_mt_gc(lua_State *L) {
        |  !TYPE! *p_ud;
        |  if (0) printf("%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);
        |  p_ud = (!TYPE! *) marpa_lua_touserdata (L, 1);
        |  if (*p_ud) marpa_!LETTER!_unref(*p_ud);
        |  *p_ud = NULL;
        |  return 0;
        |}
        ]]
        for letter, class_name in pairs(libmarpa_class_name) do
           local class_type = libmarpa_class_type[letter]
           result[#result+1] =
               pipe_dedent(template)
                   :gsub("!NAME!", class_name)
                   :gsub("!TYPE!", class_type)
                   :gsub("!LETTER!", letter)
        end
        return table.concat(result)
    ]==]

```

#### Kollos metal loader

To make this a real module, this fuction must be named "luaopen_kollos_metal".
The LUAOPEN_KOLLOS_METAL define allows us to override this for a declaration
compatible with static loading and namespace requirements like those of
Marpa::R3.

```
    -- miranda: section+ C function declarations
    #if !defined(LUAOPEN_KOLLOS_METAL)
    #define LUAOPEN_KOLLOS_METAL luaopen_kollos_metal
    #endif
    int LUAOPEN_KOLLOS_METAL(lua_State *L);
    -- miranda: section define kollos_metal_loader method
    int LUAOPEN_KOLLOS_METAL(lua_State *L)
    {
        /* The main kollos object */
        int kollos_table_stack_ix;
        int upvalue_stack_ix;

        /* Make sure the header is from the version we want */
        if (MARPA_MAJOR_VERSION != EXPECTED_LIBMARPA_MAJOR ||
            MARPA_MINOR_VERSION != EXPECTED_LIBMARPA_MINOR ||
            MARPA_MICRO_VERSION != EXPECTED_LIBMARPA_MICRO) {
            const char *message;
            marpa_lua_pushfstring
                (L,
                "Libmarpa header version mismatch: want %ld.%ld.%ld, have %ld.%ld.%ld",
                EXPECTED_LIBMARPA_MAJOR, EXPECTED_LIBMARPA_MINOR,
                EXPECTED_LIBMARPA_MICRO, MARPA_MAJOR_VERSION,
                MARPA_MINOR_VERSION, MARPA_MICRO_VERSION);
            message = marpa_lua_tostring (L, -1);
            return internal_error_handle (L, message,
                __PRETTY_FUNCTION__, __FILE__, __LINE__);
        }

        /* Now make sure the library is from the version we want */
        {
            int version[3];
            const Marpa_Error_Code error_code = marpa_version (version);
            if (error_code != MARPA_ERR_NONE) {
                const char *description =
                    error_description_by_code (error_code);
                const char *message;
                marpa_lua_pushfstring (L, "marpa_version() failed: %s",
                    description);
                message = marpa_lua_tostring (L, -1);
                return internal_error_handle (L, message, __PRETTY_FUNCTION__,
                    __FILE__, __LINE__);
            }
            if (version[0] != EXPECTED_LIBMARPA_MAJOR ||
                version[1] != EXPECTED_LIBMARPA_MINOR ||
                version[2] != EXPECTED_LIBMARPA_MICRO) {
                const char *message;
                marpa_lua_pushfstring
                    (L,
                    "Libmarpa library version mismatch: want %ld.%ld.%ld, have %ld.%ld.%ld",
                    EXPECTED_LIBMARPA_MAJOR, EXPECTED_LIBMARPA_MINOR,
                    EXPECTED_LIBMARPA_MICRO, version[0], version[1],
                    version[2]);
                message = marpa_lua_tostring (L, -1);
                return internal_error_handle (L, message, __PRETTY_FUNCTION__,
                    __FILE__, __LINE__);
            }
        }

        /* Create the kollos class */
        marpa_lua_newtable (L);
        kollos_table_stack_ix = marpa_lua_gettop (L);
        /* Create the main kollos_c object, to give the
         * C language Libmarpa wrappers their own namespace.
         *
         */
        /* [ kollos ] */

        /* _M.throw = true */
        marpa_lua_pushboolean (L, 1);
        marpa_lua_setfield (L, kollos_table_stack_ix, "throw");

        /* Create the shared upvalue table */
        {
            const size_t initial_buffer_capacity = 1;
            marpa_lua_newtable (L);
            upvalue_stack_ix = marpa_lua_gettop (L);
            marpa_lua_newuserdata (L,
                sizeof (Marpa_Symbol_ID) * initial_buffer_capacity);
            marpa_lua_setfield (L, upvalue_stack_ix, "buffer");
            marpa_lua_pushinteger (L, (lua_Integer) initial_buffer_capacity);
            marpa_lua_setfield (L, upvalue_stack_ix, "buffer_capacity");
        }

        /* Also keep the upvalues in an element of the class */
        marpa_lua_pushvalue (L, upvalue_stack_ix);
        marpa_lua_setfield (L, kollos_table_stack_ix, "upvalues");

        --miranda: insert create kollos libmarpa wrapper class tables

          marpa_lua_pushvalue(L, kollos_table_stack_ix);
          marpa_lua_pushvalue(L, upvalue_stack_ix);
          marpa_luaL_setfuncs(L, kollos_funcs, 1);

          /* Create the SLIF grammar metatable */
          marpa_luaL_newlibtable(L, slg_methods);
          marpa_lua_pushvalue(L, upvalue_stack_ix);
          marpa_luaL_setfuncs(L, slg_methods, 1);
          marpa_lua_pushvalue(L, -1);
          marpa_lua_setfield(L, -2, "__index");
          marpa_lua_pushvalue(L, -1);
          marpa_lua_setfield(L, kollos_table_stack_ix, "class_slg");
          marpa_lua_pushvalue(L, kollos_table_stack_ix);
          marpa_lua_setfield(L, -2, "kollos");

          /* Create the SLIF recce metatable */
          marpa_luaL_newlibtable(L, slr_methods);
          marpa_lua_pushvalue(L, upvalue_stack_ix);
          marpa_luaL_setfuncs(L, slr_methods, 1);
          marpa_lua_pushvalue(L, -1);
          marpa_lua_setfield(L, -2, "__index");
          marpa_lua_pushvalue(L, -1);
          marpa_lua_setfield(L, kollos_table_stack_ix, "class_slr");
          marpa_lua_pushvalue(L, kollos_table_stack_ix);
          marpa_lua_setfield(L, -2, "kollos");

          /* Create the SLIF value metatable */
          marpa_luaL_newlibtable(L, slv_methods);
          marpa_lua_pushvalue(L, upvalue_stack_ix);
          marpa_luaL_setfuncs(L, slv_methods, 1);
          marpa_lua_pushvalue(L, -1);
          marpa_lua_setfield(L, -2, "__index");
          marpa_lua_pushvalue(L, -1);
          marpa_lua_setfield(L, kollos_table_stack_ix, "class_slv");
          marpa_lua_pushvalue(L, kollos_table_stack_ix);
          marpa_lua_setfield(L, -2, "kollos");

          /* Create the ASF metatable */
          marpa_luaL_newlibtable(L, asf_methods);
          marpa_lua_pushvalue(L, upvalue_stack_ix);
          marpa_luaL_setfuncs(L, asf_methods, 1);
          marpa_lua_pushvalue(L, -1);
          marpa_lua_setfield(L, -2, "__index");
          marpa_lua_pushvalue(L, -1);
          marpa_lua_setfield(L, kollos_table_stack_ix, "class_asf");
          marpa_lua_pushvalue(L, kollos_table_stack_ix);
          marpa_lua_setfield(L, -2, "kollos");

          /* Create the ASF2 metatable */
          marpa_luaL_newlibtable(L, asf2_methods);
          marpa_lua_pushvalue(L, upvalue_stack_ix);
          marpa_luaL_setfuncs(L, asf2_methods, 1);
          marpa_lua_pushvalue(L, -1);
          marpa_lua_setfield(L, -2, "__index");
          marpa_lua_pushvalue(L, -1);
          marpa_lua_setfield(L, kollos_table_stack_ix, "class_asf2");
          marpa_lua_pushvalue(L, kollos_table_stack_ix);
          marpa_lua_setfield(L, -2, "kollos");

        /* Set up Kollos grammar userdata metatable */
        marpa_lua_newtable (L);
        /* [ kollos, mt_ud_g ] */
        marpa_lua_pushvalue (L, upvalue_stack_ix);
        marpa_lua_pushcclosure (L, l_grammar_ud_mt_gc, 1);
        /* [ kollos, mt_g_ud, gc_function ] */
        marpa_lua_setfield (L, -2, "__gc");
        /* [ kollos, mt_g_ud ] */
        marpa_lua_rawsetp (L, LUA_REGISTRYINDEX, &kollos_g_ud_mt_key);
        /* [ kollos ] */

        /* Set up Kollos recce userdata metatable */
        marpa_lua_newtable (L);
        /* [ kollos, mt_ud_r ] */
        marpa_lua_pushvalue (L, upvalue_stack_ix);
        marpa_lua_pushcclosure (L, l_recce_ud_mt_gc, 1);
        /* [ kollos, mt_r_ud, gc_function ] */
        marpa_lua_setfield (L, -2, "__gc");
        /* [ kollos, mt_r_ud ] */
        marpa_lua_rawsetp (L, LUA_REGISTRYINDEX, &kollos_r_ud_mt_key);
        /* [ kollos ] */

        /* Set up Kollos traverser userdata metatable */
        marpa_lua_newtable (L);
        /* [ kollos, mt_ud_traverser ] */
        marpa_lua_pushvalue (L, upvalue_stack_ix);
        marpa_lua_pushcclosure (L, l_traverser_ud_mt_gc, 1);
        /* [ kollos, mt_trv_ud, gc_function ] */
        marpa_lua_setfield (L, -2, "__gc");
        /* [ kollos, mt_trv_ud ] */
        marpa_lua_rawsetp (L, LUA_REGISTRYINDEX, &kollos_trv_ud_mt_key);
        /* [ kollos ] */

        /* Set up Kollos LIM traverser userdata metatable */
        marpa_lua_newtable (L);
        /* [ kollos, mt_ud_traverser ] */
        marpa_lua_pushvalue (L, upvalue_stack_ix);
        marpa_lua_pushcclosure (L, l_ltraverser_ud_mt_gc, 1);
        /* [ kollos, mt_trv_ud, gc_function ] */
        marpa_lua_setfield (L, -2, "__gc");
        /* [ kollos, mt_trv_ud ] */
        marpa_lua_rawsetp (L, LUA_REGISTRYINDEX, &kollos_ltrv_ud_mt_key);
        /* [ kollos ] */

        /* Set up Kollos PIM traverser userdata metatable */
        marpa_lua_newtable (L);
        /* [ kollos, mt_ud_traverser ] */
        marpa_lua_pushvalue (L, upvalue_stack_ix);
        marpa_lua_pushcclosure (L, l_ptraverser_ud_mt_gc, 1);
        /* [ kollos, mt_trv_ud, gc_function ] */
        marpa_lua_setfield (L, -2, "__gc");
        /* [ kollos, mt_trv_ud ] */
        marpa_lua_rawsetp (L, LUA_REGISTRYINDEX, &kollos_ptrv_ud_mt_key);
        /* [ kollos ] */

        /* Set up Kollos bocage userdata metatable */
        marpa_lua_newtable (L);
        /* [ kollos, mt_ud_bocage ] */
        marpa_lua_pushvalue (L, upvalue_stack_ix);
        marpa_lua_pushcclosure (L, l_bocage_ud_mt_gc, 1);
        /* [ kollos, mt_b_ud, gc_function ] */
        marpa_lua_setfield (L, -2, "__gc");
        /* [ kollos, mt_b_ud ] */
        marpa_lua_rawsetp (L, LUA_REGISTRYINDEX, &kollos_b_ud_mt_key);
        /* [ kollos ] */

        /* Set up Kollos order userdata metatable */
        marpa_lua_newtable (L);
        /* [ kollos, mt_ud_order ] */
        marpa_lua_pushvalue (L, upvalue_stack_ix);
        marpa_lua_pushcclosure (L, l_order_ud_mt_gc, 1);
        /* [ kollos, mt_o_ud, gc_function ] */
        marpa_lua_setfield (L, -2, "__gc");
        /* [ kollos, mt_o_ud ] */
        marpa_lua_rawsetp (L, LUA_REGISTRYINDEX, &kollos_o_ud_mt_key);
        /* [ kollos ] */

        /* Set up Kollos tree userdata metatable */
        marpa_lua_newtable (L);
        /* [ kollos, mt_ud_tree ] */
        marpa_lua_pushvalue (L, upvalue_stack_ix);
        marpa_lua_pushcclosure (L, l_tree_ud_mt_gc, 1);
        /* [ kollos, mt_t_ud, gc_function ] */
        marpa_lua_setfield (L, -2, "__gc");
        /* [ kollos, mt_t_ud ] */
        marpa_lua_rawsetp (L, LUA_REGISTRYINDEX, &kollos_t_ud_mt_key);
        /* [ kollos ] */

        /* Set up Kollos value userdata metatable */
        marpa_lua_newtable (L);
        /* [ kollos, mt_ud_value ] */
        marpa_lua_pushvalue (L, upvalue_stack_ix);
        marpa_lua_pushcclosure (L, l_value_ud_mt_gc, 1);
        /* [ kollos, mt_v_ud, gc_function ] */
        marpa_lua_setfield (L, -2, "__gc");
        /* [ kollos, mt_v_ud ] */
        marpa_lua_rawsetp (L, LUA_REGISTRYINDEX, &kollos_v_ud_mt_key);
        /* [ kollos ] */

        -- miranda: insert set up empty metatables

        /* In alphabetical order by field name */

        marpa_lua_pushvalue (L, upvalue_stack_ix);
        marpa_lua_pushcclosure (L, lca_error_description_by_code, 1);
        /* [ kollos, function ] */
        marpa_lua_setfield (L, kollos_table_stack_ix, "error_description");
        /* [ kollos ] */

        marpa_lua_pushvalue (L, upvalue_stack_ix);
        marpa_lua_pushcclosure (L, lca_error_name_by_code, 1);
        marpa_lua_setfield (L, kollos_table_stack_ix, "error_name");

        marpa_lua_pushvalue (L, upvalue_stack_ix);
        marpa_lua_pushcclosure (L, lca_event_name_by_code, 1);
        marpa_lua_setfield (L, kollos_table_stack_ix, "event_name");

        marpa_lua_pushvalue (L, upvalue_stack_ix);
        marpa_lua_pushcclosure (L, lca_event_description_by_code, 1);
        marpa_lua_setfield (L, kollos_table_stack_ix, "event_description");

        /* In Libmarpa object sequence order */

        marpa_lua_pushvalue (L, upvalue_stack_ix);
        marpa_lua_getfield (L, kollos_table_stack_ix, "class_grammar");
        marpa_lua_pushcclosure (L, lca_grammar_new, 2);
        marpa_lua_setfield (L, kollos_table_stack_ix, "grammar_new");

        marpa_lua_pushvalue (L, upvalue_stack_ix);
        marpa_lua_getfield (L, kollos_table_stack_ix, "class_recce");
        marpa_lua_pushcclosure (L, lca_grammar_event, 1);
        marpa_lua_setfield (L, kollos_table_stack_ix, "grammar_event");

        marpa_lua_pushvalue (L, upvalue_stack_ix);
        marpa_lua_getfield (L, kollos_table_stack_ix, "class_recce");
        marpa_lua_pushcclosure (L, wrap_recce_new, 2);
        marpa_lua_setfield (L, kollos_table_stack_ix, "recce_new");

        marpa_lua_pushvalue (L, upvalue_stack_ix);
        marpa_lua_getfield (L, kollos_table_stack_ix, "class_traverser");
        marpa_lua_pushcclosure (L, wrap_traverser_new, 2);
        marpa_lua_setfield (L, kollos_table_stack_ix, "traverser_new");

        marpa_lua_pushvalue (L, upvalue_stack_ix);
        marpa_lua_getfield (L, kollos_table_stack_ix, "class_ptraverser");
        marpa_lua_pushcclosure (L, wrap_ptraverser_new, 2);
        marpa_lua_setfield (L, kollos_table_stack_ix, "ptraverser_new");

        marpa_lua_pushvalue (L, upvalue_stack_ix);
        marpa_lua_getfield (L, kollos_table_stack_ix, "class_bocage");
        marpa_lua_pushcclosure (L, wrap_bocage_new, 2);
        marpa_lua_setfield (L, kollos_table_stack_ix, "bocage_new");

        marpa_lua_pushvalue (L, upvalue_stack_ix);
        marpa_lua_getfield (L, kollos_table_stack_ix, "class_order");
        marpa_lua_pushcclosure (L, wrap_order_new, 2);
        marpa_lua_setfield (L, kollos_table_stack_ix, "order_new");

        marpa_lua_pushvalue (L, upvalue_stack_ix);
        marpa_lua_getfield (L, kollos_table_stack_ix, "class_tree");
        marpa_lua_pushcclosure (L, wrap_tree_new, 2);
        marpa_lua_setfield (L, kollos_table_stack_ix, "tree_new");

        marpa_lua_pushvalue (L, upvalue_stack_ix);
        marpa_lua_getfield (L, kollos_table_stack_ix, "class_value");
        marpa_lua_pushcclosure (L, wrap_value_new, 2);
        marpa_lua_setfield (L, kollos_table_stack_ix, "value_new");

        marpa_lua_newtable (L);
        /* [ kollos, error_code_table ] */
        {
            const int name_table_stack_ix = marpa_lua_gettop (L);
            int error_code;
            for (error_code = LIBMARPA_MIN_ERROR_CODE;
                error_code <= LIBMARPA_MAX_ERROR_CODE; error_code++) {
                marpa_lua_pushinteger (L, (lua_Integer) error_code);
                marpa_lua_setfield (L, name_table_stack_ix,
                    marpa_error_codes[error_code -
                        LIBMARPA_MIN_ERROR_CODE].mnemonic);
            }
            for (error_code = KOLLOS_MIN_ERROR_CODE;
                error_code <= KOLLOS_MAX_ERROR_CODE; error_code++) {
                marpa_lua_pushinteger (L, (lua_Integer) error_code);
                marpa_lua_setfield (L, name_table_stack_ix,
                    marpa_kollos_error_codes[error_code -
                        KOLLOS_MIN_ERROR_CODE].mnemonic);
            }
        }

        /* [ kollos, error_code_table ] */
        marpa_lua_setfield (L, kollos_table_stack_ix, "error_code_by_name");

        marpa_lua_newtable (L);
        /* [ kollos, event_code_table ] */
        {
            const int name_table_stack_ix = marpa_lua_gettop (L);
            int event_code;
            for (event_code = LIBMARPA_MIN_EVENT_CODE;
                event_code <= LIBMARPA_MAX_EVENT_CODE; event_code++) {
                marpa_lua_pushinteger (L, (lua_Integer) event_code);
                marpa_lua_setfield (L, name_table_stack_ix,
                    marpa_event_codes[event_code -
                        LIBMARPA_MIN_EVENT_CODE].mnemonic);
            }
        }

        /* [ kollos, event_code_table ] */
        marpa_lua_setfield (L, kollos_table_stack_ix, "event_code_by_name");

        -- miranda: insert register standard libmarpa wrappers

            /* [ kollos ] */

        marpa_lua_settop (L, kollos_table_stack_ix);
        /* [ kollos ] */
        return 1;
    }

```

#### Preliminaries to the C library code
```
    -- miranda: section preliminaries to the c library code
    /*
    ** Permission is hereby granted, free of charge, to any person obtaining
    ** a copy of this software and associated documentation files (the
    ** "Software"), to deal in the Software without restriction, including
    ** without limitation the rights to use, copy, modify, merge, publish,
    ** distribute, sublicense, and/or sell copies of the Software, and to
    ** permit persons to whom the Software is furnished to do so, subject to
    ** the following conditions:
    **
    ** The above copyright notice and this permission notice shall be
    ** included in all copies or substantial portions of the Software.
    **
    ** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    ** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    ** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    ** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    ** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    ** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    ** SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
    **
    ** [ MIT license: http://www.opensource.org/licenses/mit-license.php ]
    */

    /* EDITS IN THIS FILE WILL BE LOST
     * This file is auto-generated.
     */

    #include "marpa.h"
    #include "kollos.h"

    #undef UNUSED
    #if     __GNUC__ >  2 || (__GNUC__ == 2 && __GNUC_MINOR__ >  4)
    #define UNUSED __attribute__((__unused__))
    #else
    #define UNUSED
    #endif

    #if defined(_MSC_VER)
    #define inline __inline
    #define __PRETTY_FUNCTION__ __FUNCTION__
    #endif

    #define EXPECTED_LIBMARPA_MAJOR 8
    #define EXPECTED_LIBMARPA_MINOR 6
    #define EXPECTED_LIBMARPA_MICRO 0

```

### The Kollos C header file

```
    -- miranda: section kollos_h
    -- miranda: language c
    -- miranda: insert preliminary comments of the c header file

    #ifndef KOLLOS_H
    #define KOLLOS_H

    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"

    -- miranda: insert temporary defines
    -- miranda: insert C extern variables
    -- miranda: insert C function declarations

    #endif

    /* vim: set expandtab shiftwidth=4: */
```

#### Preliminaries to the C header file
```
    -- miranda: section preliminary comments of the c header file

    /*
     * Copyright 2017 Jeffrey Kegler
     * Permission is hereby granted, free of charge, to any person obtaining a
     * copy of this software and associated documentation files (the "Software"),
     * to deal in the Software without restriction, including without limitation
     * the rights to use, copy, modify, merge, publish, distribute, sublicense,
     * and/or sell copies of the Software, and to permit persons to whom the
     * Software is furnished to do so, subject to the following conditions:
     *
     * The above copyright notice and this permission notice shall be included
     * in all copies or substantial portions of the Software.
     *
     * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
     * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
     * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
     * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
     * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
     * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
     * OTHER DEALINGS IN THE SOFTWARE.
     */

    /* EDITS IN THIS FILE WILL BE LOST
     * This file is auto-generated.
     */

```

## Internal utilities

Utilities used internally by Kollos and not visible to
Kollos users.

"Declare" the fields allowed in a table.
A variation on the `strict.lua` module, which
requires the fields to be declared in advance.
This is very helpful in development.

TODO -- Do I want to turn this off after developement?

```
    -- miranda: section+ internal utilities
    local function declarations(table, fields, name)
        table.__declared = fields

        table.__newindex = function (t, n, v)
          if not table.__declared[n] then
            error(
                "assign to undeclared member '"
                ..name
                .."."
                ..n
                .."'",
                2)
          end
          rawset(t, n, v)
        end

        table.__index = function (t, n)
          local v = rawget(t, n) or table[n]
          if v == nil and not table.__declared[n] then
            -- print(inspect(table.__declared, {depth=2}))
            error(string.format(
                "member %s.%s is not declared",
                name, n),
            2)
          end
          return v
        end
    end
```

This is
an exported internal, at least for now, so
that code inlined in Perl can use it.

```
    -- miranda: section+ internal utilities
    function _M._internal_error(...)
        local X = {
            code = _M.err.LUA_INTERNAL,
            where = debug.traceback(),
            msg = 'Kollos internal error: '
               .. string.format(...)
        }
        setmetatable(X, _M.mt_X)
        error(X)
    end
```

A utility to print a rule when all we have
are the names.
Primarily for error messages,
including internal ones,
so it cannot assume that its arguments are sane.

```
    -- miranda: section+ internal utilities
    function _M._raw_rule_show(lhs, rhs)
        local pcs = {}
        pcs[#pcs+1] = symbol_diag_form(tostring(lhs))
        pcs[#pcs+1] = '::='
        if type(rhs) ~= 'table' then
            pcs[#pcs+1] = symbol_diag_form(tostring(rhs))
        else
            for ix = 1, #rhs do
                local rhsym = rhs[ix]
                pcs[#pcs+1] = symbol_diag_form(tostring(rhsym))
            end
        end
        return table.concat(pcs, ' ')
    end
```

Eventually make this local.
Right now it's a static class method,
so that it can be used from the Lua inlined
in the Perl code.

Compares two sequences.
They must have the same length,
and every element at a given index
in one sequence must be comparable
to every elememt at the same index
in every other sequence.

```
    -- miranda: section+ internal utilities
    function _M.cmp_seq(i, j)
        for ix = 1, #i do
            if i[ix] < j[ix] then return true end
            if i[ix] > j[ix] then return false end
        end
        return false
    end
```

Combines `print` and `inspect`.

```
    -- miranda: section+ forward declarations
    local iprint
    -- miranda: section+ internal utilities
    function iprint(...)
        local args = {...}
        local results = {}
        for ix = 1, #args do
            local arg = args[ix]
            local arg_type = type(arg)
            if arg_type == 'string' then
                table.insert(results, arg)
                goto ARG
            end
            table.insert(results, inspect(args[ix], { depth=2 } ))
            ::ARG::
        end
        io.stderr:write( table.concat(results, '    ') .. "\n")
    end
    -- make it a static, so Lua inlined in Perl can also use it
    _M.iprint = iprint
```

Given a symbol name, convert it to a form
suitable for diagnostic messages.

```
    -- miranda: section+ forward declarations
    local symbol_diag_form
    -- miranda: section+ internal utilities
    function symbol_diag_form(name)
        if name:match('^%a[%w_-]*$') then
            return name
        end
        if name:sub(1, 1) == '[' and name:sub(-1, -1) == ']' then
            return name
        end
        return '<' .. name .. '>'
    end
```
### Coroutines

We use coroutines as "better callbacks".
They allow the upper layer to be called upon for processing
at any point in Kollos's Lua layers.
At this point, we allow the upper layers only one active coroutine
(though it may have child coroutines).
Also, this "upper layer child coroutine" must be run until
it returns before other processing is performed.
Obeying this constraint is currently up to the upper layer --
nothing in the code enforces it.

```
    -- miranda: section+ most Lua function definitions
    function _M.wrap(f)
        if _M.current_coro then
           -- error('Attempt to overwrite active Kollos coro')
        end
        _M.current_coro = coroutine.wrap(f)
    end

    function _M.resume(...)
        local coro = _M.current_coro
        if not coro then
           error('Attempt to resume non-existent Kollos coro')
        end
        local retours = {coro(...)}
        local cmd = table.remove(retours, 1)
        if not cmd or cmd == '' or cmd == 'ok' then
            cmd = false
            _M.current_coro = nil
        end
        return cmd, retours
    end
```

## Exceptions

```
    -- miranda: section+ C extern variables
    extern char kollos_X_fallback_mt_key;
    extern char kollos_X_proto_mt_key;
    extern char kollos_X_mt_key;
    -- miranda: section+ metatable keys
    char kollos_X_fallback_mt_key;
    char kollos_X_proto_mt_key;
    char kollos_X_mt_key;
    -- miranda: section+ set up empty metatables

    /* mt_X_fallback = {} */
    marpa_lua_newtable (L);
    marpa_lua_pushvalue (L, -1);
    marpa_lua_rawsetp (L, LUA_REGISTRYINDEX, &kollos_X_fallback_mt_key);
    /* kollos.mt_X_fallback = mt_X_fallback */
    marpa_lua_setfield (L, kollos_table_stack_ix, "mt_X_fallback");

    /* mt_X_proto = {} */
    marpa_lua_newtable (L);
    marpa_lua_pushvalue (L, -1);
    marpa_lua_rawsetp (L, LUA_REGISTRYINDEX, &kollos_X_proto_mt_key);
    /* kollos.mt_X_proto = mt_X_proto */
    marpa_lua_setfield (L, kollos_table_stack_ix, "mt_X_proto");

    /* Set up exception metatables, initially empty */
    /* mt_X = {} */
    marpa_lua_newtable (L);
    marpa_lua_pushvalue (L, -1);
    marpa_lua_rawsetp (L, LUA_REGISTRYINDEX, &kollos_X_mt_key);
    /* kollos.mt_X = mt_X */
    marpa_lua_setfield (L, kollos_table_stack_ix, "mt_X");

```

The "fallback" for converting an exception is make it part
of a table with a fallback __tostring method, which uses the
inspect package to dump it.

```
    -- miranda: section+ populate metatables

    -- `inspect` is used in our __tostring methods, but
    -- it also calls __tostring.  This global is used to
    -- prevent any recursive calls.
    _M.recursive_tostring = false

    local function X_fallback_tostring(self)
         -- print("in X_fallback_tostring")
         local desc
         if _M.recursive_tostring then
             desc = '[Recursive call of inspect]'
         else
             _M.recursive_tostring = 'X_fallback_tostring'
             desc = inspect(self, { depth = 3 })
             _M.recursive_tostring = false
         end
         local nl = ''
         local where = ''
         if type(self) == 'table' then
             local where = self.where
             if where and desc:sub(-1) ~= '\n' then
                 nl = '\n'
             end
         end
         local traceback = debug.traceback("Kollos internal error: bad exception object")
         return desc .. nl .. where .. '\n' .. traceback
    end

    local function X_tostring(self)
         -- print("in X_tostring")
         if type(self) ~= 'table' then
              return X_fallback_tostring(self)
         end
         local desc = self.msg
         local desc_type = type(desc)
         if desc_type == "string" then
             local nl = ''
             local where = self.where
             if where then
                 if desc:sub(-1) ~= '\n' then nl = '\n' end
             else
                 where = ''
             end
             return desc .. nl .. where
         end

         -- no `msg` so look for a code
         local error_code = self.code
         if error_code then
              local description = _M.error_description(error_code)
              local fields = {}
              local details = self.details
              if details then
                  table.insert(fields, details .. ':')
              end
              table.insert(fields, description)
              local message = self.message
              if message then
                  table.insert(fields, '("' .. message .. '")')
              end
              local pieces = { table.concat(fields, " ") }
              local where = self.where
              if where then
                  table.insert(pieces, where)
              end
              return table.concat(pieces, "\n")
         end

         -- no `msg` or `code` so we fall back
         return X_fallback_tostring(self)
    end

    local function error_tostring(self)
         print("Calling error_tostring")
         return '[error_tostring]'
    end

    _M.mt_X.__tostring = X_tostring
    _M.mt_X_proto.__tostring = X_tostring
    _M.mt_X_fallback.__tostring = X_fallback_tostring

```

A function to throw exceptions which do not carry a
traceback.  This is for "user" errors, where "user"
means the error can be explained in user-friendly terms
and things like stack traces are unnecessary.
(These errors are also usually "user" errors in the sense
that the user caused them,
but that is not necessarily the case.)

```
    -- miranda: section+ most Lua function definitions
    function _M.userX(format, ...)
        local message
        if format then
            message = string.format(format, ...)
        end
        local X = { msg = message, traceback = false }
        setmetatable(X, _M.mt_X)
        error(X)
    end
```

## Meta-coding

### Metacode execution sequence

```
    -- miranda: sequence-exec argument processing
    -- miranda: sequence-exec metacode utilities
    -- miranda: sequence-exec libmarpa interface globals
    -- miranda: sequence-exec declare standard libmarpa wrappers
    -- miranda: sequence-exec register standard libmarpa wrappers
    -- miranda: sequence-exec create kollos libmarpa wrapper class tables
    -- miranda: sequence-exec object userdata gc methods
    -- miranda: sequence-exec create metal tables
```

### Dedent method

A pipe symbol is used when inlining code to separate the code's indentation
from the indentation used to display the code in this document.
The `pipe_dedent` method removes the display indentation.

```
    --[==[ miranda: exec metacode utilities
    function pipe_dedent(code)
        return code:gsub('\n *|', '\n'):gsub('^ *|', '', 1)
    end
    ]==]
```

### `c_safe_string` method

```
    --[==[ miranda: exec metacode utilities
    local function c_safe_string (s)
        s = string.gsub(s, '"', '\\034')
        s = string.gsub(s, '\\', '\\092')
        s = string.gsub(s, '\n', '\\n')
        return '"' .. s .. '"'
    end
    ]==]

```

### Meta code argument processing

The arguments show where to find the files containing event
and error codes.

```
    -- assumes that, when called, out_file to set to output file
    --[==[ miranda: exec argument processing
    local error_file
    local event_file

    for _,v in ipairs(arg) do
       if not v:find("=")
       then return nil, "Bad options: ", arg end
       local id, val = v:match("^([^=]+)%=(.*)") -- no space around =
       if id == "out" then io.output(val)
       elseif id == "errors" then error_file = val
       elseif id == "events" then event_file = val
       else return nil, "Bad id in options: ", id end
    end
    ]==]
```

## Kollos non-locals

### Create a sandbox

Create a table, which can be used
as a "sandbox" for protect the global environment
from user code.
This code only creates the sandbox, it does not
set it as an environment -- it is assumed that
that will be done later,
after to-be-sandboxed Lua code is loaded,
but before it is executed.

```
    -- miranda: section create sandbox table

    local sandbox = {}
    _M.sandbox = sandbox
    sandbox.__index = _G
    setmetatable(sandbox, sandbox)

```

### Constants: Ranking methods

```
    -- miranda: section+ constant Lua tables
    _M.ranking_methods = { none = true, high_rank_only = true, rule = true }
```

## Kollos utilities

```
    -- miranda: section+ most Lua function definitions
    function _M.posix_lc(str)
       return str:gsub('[A-Z]', function(str) return string.char(string.byte(str)) end)
    end

    local escape_by_codepoint = {
       [string.byte("\a")] = "\\a",
       [string.byte("\b")] = "\\b",
       [string.byte("\f")] = "\\f",
       [string.byte("\n")] = "\\n",
       [string.byte("\r")] = "\\r",
       [string.byte("\t")] = "\\t",
       [string.byte("\v")] = "\\v",
       [string.byte("\\")] = "\\\\"
    }

    function _M.escape_codepoint(codepoint)
        local escape = escape_by_codepoint[codepoint]
        if escape then return escape end
        if 32 <= codepoint and codepoint <= 126 then
            return string.char(codepoint)
        end
        if codepoint < 255 then
            return string.format("\\x{%02x}", codepoint)
        end
        return string.format("\\x{%04x}", codepoint)
    end

```

### VLQ (Variable-Length Quantity)

This is an implementation of
[VLQ (Variable-Length Quantity)](https://en.wikipedia.org/wiki/Variable-length_quantity).

```
    -- miranda: section+ kollos table methods
    /*
    ** From the Lua code
    ** Check that 'arg' either is a table or can behave like one (that is,
    ** has a metatable with the required metamethods)
    */

    /*
    ** Operations that an object must define to mimic a table
    ** (some functions only need some of them)
    */
    #define TAB_R 1   /* read */
    #define TAB_W 2   /* write */
    #define TAB_L 4   /* length */
    #define TAB_RW (TAB_R | TAB_W)  /* read/write */
    #define UNSIGNED_VLQ_SIZE ((8*sizeof(lua_Unsigned))/7 + 1)

    #define aux_getn(L,n,w) (checktab(L, n, (w) | TAB_L), marpa_luaL_len(L, n))

    static int checkfield (lua_State *L, const char *key, int n) {
      marpa_lua_pushstring(L, key);
      return (marpa_lua_rawget(L, -n) != LUA_TNIL);
    }

    static void checktab (lua_State *L, int arg, int what) {
      if (marpa_lua_type(L, arg) != LUA_TTABLE) {  /* is it not a table? */
        int n = 1;  /* number of elements to pop */
        if (marpa_lua_getmetatable(L, arg) &&  /* must have metatable */
            (!(what & TAB_R) || checkfield(L, "__index", ++n)) &&
            (!(what & TAB_W) || checkfield(L, "__newindex", ++n)) &&
            (!(what & TAB_L) || checkfield(L, "__len", ++n))) {
          marpa_lua_pop(L, n);  /* pop metatable and tested metamethods */
        }
        else
          marpa_luaL_argerror(L, arg, "table expected");  /* force an error */
      }
    }

    static const unsigned char* uint_to_vlq(lua_Unsigned x, unsigned char *out)
    {
            unsigned char buf[UNSIGNED_VLQ_SIZE];
            unsigned char *p_buf = buf;
            unsigned char *p_out = out;
      if (0) printf("%s %s %d %lu\n", __PRETTY_FUNCTION__, __FILE__, __LINE__, (unsigned long)x);
            for (;;) {
                *p_buf = x & 0x7F;
      if (0) printf("%s %s %d byte = %lu\n", __PRETTY_FUNCTION__, __FILE__, __LINE__, (unsigned long)(*p_buf));
      if (0) printf("%s %s %d %lu\n", __PRETTY_FUNCTION__, __FILE__, __LINE__, (unsigned long)x);
                x >>= 7;
      if (0) printf("%s %s %d %lu\n", __PRETTY_FUNCTION__, __FILE__, __LINE__, (unsigned long)x);
                if (x == 0) break;
                p_buf++;
            }
            while (p_buf > buf) {
               unsigned char this_byte = *p_buf;
               p_buf--;
               *p_out++ = this_byte | 0x80;
            }
            *p_out++ = *p_buf;
            return p_out;
    }

    static const unsigned char* uint_from_vlq(
        const unsigned char *in, lua_Unsigned* p_x)
    {
            lua_Unsigned r = 0;
            unsigned char this_byte;

            do {
      if (0) printf("%s %s %d in byte = %lu\n", __PRETTY_FUNCTION__, __FILE__, __LINE__, (unsigned long)(*in));
                this_byte = *in++;
      if (0) printf("%s %s %d %lu\n", __PRETTY_FUNCTION__, __FILE__, __LINE__, (unsigned long)r);
                r = (r << 7) | (this_byte & 0x7F);
            } while (this_byte & 0x80);
            *p_x = r;
            return in;
    }

```

Kollos static function for creating VLQ strings.

```
    -- miranda: section+ kollos table methods
    static int lca_to_vlq (lua_State *L) {
      int i;
      unsigned char vlq_buf[UNSIGNED_VLQ_SIZE];
      luaL_Buffer b;
      lua_Integer last = aux_getn(L, 1, TAB_R);
      marpa_luaL_buffinit(L, &b);
      for (i = 1; i <= last; i++) {
        lua_Unsigned x;
        const unsigned char *eobuf;
        marpa_lua_geti(L, 1, i);
        x = (lua_Unsigned)marpa_lua_tointeger(L, -1);
        /* Stack must be restored before luaL_addlstring() */
        marpa_lua_pop(L, 1);
        eobuf = uint_to_vlq(x, vlq_buf);
        marpa_luaL_addlstring(&b, (char *)vlq_buf,
            (size_t)(eobuf - vlq_buf));
      }
      marpa_luaL_pushresult(&b);
      return 1;
    }

```

Kollos static function for unpacking VLQ strings
into integer sequences.

```
    -- miranda: section+ kollos table methods
    static int lca_from_vlq (lua_State *L) {
      int i;
      size_t vlq_len;
      const unsigned char *vlq
          = (unsigned char *)marpa_luaL_checklstring(L, 1, &vlq_len);
      const unsigned char *p = vlq;
      marpa_lua_newtable(L);
      if (0) printf("%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);
      for (i = 1; (size_t)(p - vlq) < vlq_len; i++) {
        lua_Unsigned x;
        p = uint_from_vlq(p, &x);
        marpa_lua_pushinteger(L, (lua_Integer)x);
        marpa_lua_rawseti(L, 2, i);
      }
      if (0) printf("%s %s %d\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);
      return 1;
    }

```

Kollos static function: a memcmp equivalent.
This avoids the use of locales.

If both arguments are not strings, it falls back
to the standard Lua compare.
Takes args `a` and `b`
Returns `true` if `a < b` for convenience with
`table.sort`.
Returns `nil` if `a == b`, and `false` if `a > b`.

```
    -- miranda: section+ kollos table methods
    static int
    lca_memcmp (lua_State * L)
    {
        int cmp;
        if (marpa_lua_type (L, 1) != LUA_TSTRING
            || marpa_lua_type (L, 2) != LUA_TSTRING) {
            if (marpa_lua_compare (L, 1, 2, LUA_OPLT)) {
                cmp = -1;
            } else if (marpa_lua_compare (L, 1, 2, LUA_OPEQ)) {
                cmp = 0;
            } else {
                cmp = 1;
            }
        } else {
            /* Both arguments are strings */
            size_t len;
            size_t len_b;
            const unsigned char *str_a
                = (unsigned char *) marpa_luaL_tolstring (L, 1, &len);
            const unsigned char *str_b
                = (unsigned char *) marpa_luaL_tolstring (L, 2, &len_b);
            if (len_b < len) {
                len = len_b;
            }
            cmp = memcmp (str_a, str_b, len);
            if (!cmp) {
                if (len < len_b) cmp = -1;
                else if (len > len_b) cmp = 1;
            }
        }
        if (cmp < 0) {
            marpa_lua_pushboolean (L, 1);
        } else if (cmp == 0) {
            marpa_lua_pushnil (L);
        } else {
            marpa_lua_pushboolean (L, 0);
        }
        return 1;
    }

```

<!--
vim: expandtab shiftwidth=4:
-->
