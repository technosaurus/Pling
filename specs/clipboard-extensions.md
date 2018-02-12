###  Selection target `_NET_MAX_SELECTION_SIZE`

When a client encounters target `_NET_MAX_SELECTION_SIZE` in `SelectionRequest`
event, it should use its value as a limit for the following targets in the same
request (i.e. the request should be of type `MULTIPLE`).
The `_NET_MAX_SELECTION_SIZE` target should be always the first target in the
`MULTIPLE` request.

The target `_NET_MAX_SELECTION_SIZE` is a side-effect target, as described in
ICCCM section 2.6.3.
When handling this target, the matching property on  the requestor window 
should be of type `XA_INTEGER`, format 32, with 2 elements.

If the selection owner's connection to the X server is local, the first element
is used as the limit, otherwise the second element is used as the limit.
A local connection is identified by a value of `XDisplayString()` with the first
character ':', for example, ":0.0". A value of -1 for either element means no
limit.

The value should limit the sum of the sizes in the request,
not separately each of them.
The client should reject subsequent targets if they would cause the total amount
of data transferred for all targets to exceed the limit.
Note that there is no requirement that if one target is rejected, all other
targets will be rejected or all that subsequent targets will be rejected.

Clients are not required to limit the data size precisely to the given value.
If the computation of the data size would be expensive for the client,
a certain reasonable inaccurancy is allowed.
In other words, if the client for various reasons doesn't know exact size of the
data to be transfered, it should try to estimate it, rather than transfering
data with size that is possibly magnitudes larger.

The intent of this target is to avoid unwanted large transfers, so it should be
requsted only by xclipboard-like tools.
They should use a request of type `MULTIPLE` with `_NET_MAX_SELECTION_SIZE` as
the first target in the `MULTIPLE` request, with the requested target(s) after
it.
