Basic idea: put libconfig into the compositor and expose a new global to communicate with.

How the protocol looks like:

| Client                           | Server                                          |                                                 |
|----------------------------------|-------------------------------------------------|:------------------------------------------------|
|                                  | wl_display_create                               |                                                 |
|                                  | wl_global_create : wl_compositor                |                                                 |
|                                  | [create other rendering stuff...]               |                                                 |
|                                  | wl_global_create : raven_config                 |                                                 |
| wl_display_connect               |                                                 |                                                 |
| wl_registry_add_listener         |                                                 | This causes the client to enumerate all globals |
| wl_registry_bind : wl_compositor |                                                 |                                                 |
|                                  | wl_ressource_create : wl_compositor             |                                                 |
|                                  | wl_ressource_set_implementation : wl_compositor | The server provides his implementation of the protocol |
| [bind other rendering stuff]     |                                                 |                                                 |
| wl_registry_bind : raven_config  |                                                 |                                                 |
|                                  | wl_ressource_create : raven_config              |                                                 |
|                                  | wl_ressource_set_implementation : raven_config  | The server provides his implementation of the protocol |

Beyond this point the client can do request to `$LIB_NAME`. We'll have to provide our own convenience methods (which is already the case for most other protocols).
The client doesn't have to bind on rendering related objects if no rendering is needed. The server would not propose them were `feathers` in headless.
