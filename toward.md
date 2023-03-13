<!--
Copyright 2022 Jeffrey Kegler
This file is part of Marpa::R2.  Marpa::R2 is free software: you can
redistribute it and/or modify it under the terms of the GNU Lesser
General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Marpa::R2 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser
General Public License along with Marpa::R2.  If not, see
http://www.gnu.org/licenses/.
-->

# Toward an on-line Marpa

An online parser parses
piece by piece, without waiting to have the whole input
available.
(Modern programmers often call this a SAX-ish parser.)
An offline parser assumes that
all of the input will be available before evaluation
begins.
(Modern programmers often call this a DOM-ish parser.)

The current Marpa implementation, while it does have
an event mechanism, is based on an offline parser.
This means that even an application using Marpa's event mechanism
to do online parsing,
must incur the storage costs of a non-online parser.

## The idea

An online parser needs is to track the causations (aka confluences)
of each Earley item (EIM).
Marpa currently does for most EIMs.
We mark those EIMs which are "upstream" from
any EIM in the "working Earley set window".
The working Earley set window is the Earley set (ES) currently
being worked on and one or more previous ESs.

An EIM `eimUp` is upstream from another EIM `eim1` iff it
* is `eim1`, or
* is an inflow (part of a confluence) of an EIM upstream from `eim1`.
Any EIM that is **not** marked upstream from an EIM in the working ES window
may have its memory released.
Almost all of the work of
marking of EIMs for release
can be done for Marpa
if the environment makes available a garbage collection mechanism
for managing memory.

Additionally,
applications may specify certain rules
("online evaluation rules" or "online rules")
to be evaluated as they are completed.
When an "online rule" is completed,
its completed EIM can stored as a special kind of EIM ---
an "evaluated EIM".
Evaluated EIMs do not have confluences,
and therefore a garbage collector will release all of their upstream storage
outside of the working ES window.
The use of evaluated EIMs can allow those application which lend themselves to
online parsing to parse arbitrary length inputs using a fixed amount of space.

## The working ES window

The working ES window should be at least two ESes in length.
If variable length tokens are used, it should be the size of the longest token,
plus one.
Additionally, retaining the EIMs in the last few ESes is useful for debugging
and tracing,
and a larger working ES window may be specified for those purposes.

## Complexity

The worst case is a highly ambiguous grammar with no
online evaluation rules.
In this case, the online version of Marpa offers no improvements.
But in many real-life cases, the savings in storage may make the
difference in whether an application is a practical candidate
for a Marpa implementation,
or not.
