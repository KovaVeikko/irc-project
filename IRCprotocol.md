The first part of any IRC connection is sending the NICK and USER messages. The first of these is simple, just NICK name. The next is the USER message.

An example of a USER message is:
> USER username 0 * :Real name

The * part is a remnant of earlier days, and will not need to be changed. The 0 is a bitmask for the user's mode, but with just one switch. Change it to 8 to be invisible to those not in a channel with you.

The next part of the protocol is the PING message, because some servers need one immediately after these two messages. The server will send you a message in the format PING :message to which it needs a response of PONG :message. This is the most common case of a server not sending a source. Most servers use the server name as the message part, but this isn't consistent.
