# header-only library containing prevailing generic datatypes and utility functions for smiley
 - Hashtable generic                                                        [x]
    - change these to rely on the linked-list implementation so that 
    you don't need to manually re-populate the hashtable whenever it
    goes over the capacity
 - String                                                                   [x]
 - Node (linked tree)                                                       [ ]
 - hashtable_get, _add, _init, \_free                                       [x]
 - hashtable_print                                                          [x]
 - hashfunction should be place-in-able per hashtable as a callback         [x]
 - Stack, push, pop                                                         [ ]
 - check_match                                                              [x]
 - match_until_but_better                                                   [x]
 - check_opts                                                               [x]
 - match_until_opts_but_better                                              [x]
 - crt_str                                                                  [x]
 - MAYBE string.h function replacements                                     [x]
    - consider actually just limiting the functions that get included in the 
    final executable:
    ```
    gcc [-0s] -ffunction-sections [-fdata-sections] -Wl,--gc-sections
    ```
