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
(Modern programmers often call this SAX-ish parser.)
An offline parser assumes that
all of the input will be available before evaluation
begins.
(Modern programmers often call this DOM-ish parser.)
The current Marpa implementation, while it does have
an event mechanism, is based on an offline parser.
This means that even an application using Marpa's event mechanism
to do online parsing,
must incur the storage costs of a non-online parser.

The essential idea is to track the causations (aka confluences)
of each Earley item.
(Marpa currently does this.)
Any Earley item not "upstream" from an Earley item in the last two
Earley items,
may have its memory released.
This may easily be done if a garbage collection mechanism is available
for managing memory.

Additionally,
applications may specify certain rules
("online evaluation rules" or "online rules")
to be evaluated as they are completed.
When an "online rule" is completed,
it can converted from an Earley item to a "value node",
and all of its upstream storage can be completed.
This can allow those application which lend themselves to
online parsing to parse arbitrary length inputs in fixed storage.

