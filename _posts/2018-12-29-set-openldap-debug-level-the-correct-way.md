---
title: "Set OpenLDAP Debug Level The Correct Way"
date: "2018-12-29"
categories:
  - "ldap"
tags:
  - "c"
  - "c++"
  - "debug"
  - "ldap_initialize"
  - "ldap_opt_debug_level"
  - "ldap_search_ext_s"
  - "ldap_set_option"
  - "libldap"
  - "linux"
  - "openldap"
  - "programming"
  - "security"
---

Recently, I run into an issue with OpenLDAP client library's `ldap_set_option()` function, when I was trying to turn on the library's internal debug logging. Due to its poor documentation, I have to dig deeply into its source code to find out the right way of setting the debug level of OpenLDAP library. In this post, I'd like to share what I have found, so that other people, who have similar issues, may save some of their time.


# Synopsis

```cpp
{% include src/2018-12-29-LDAP-global-vs-handle-options/ldap-option-declaration.hpp %}
```

# Description

This pair of routines are used to get/set various options for a LDAP handle or global options. Passing a NULL LDAP handle as the first argument `ld` will set/retrieve global options.[<sup>\[1\]</sup>](#references)

The second argument `option` is the actual LDAP option enum that one's interested in. There are tons of available options, so I'm not going to cover all of them here. In fact, they are probably system-dependent anyway. Our main concern in this post is `LDAP_OPT_DEBUG_LEVEL` only. People who are curious about the other options may want to check the man page, by typing `man ldap_set_option`.

Based on the man page:[<sup>\[1\]</sup>](#references)

> LDAP_OPT_DEBUG_LEVEL: Sets/gets the debug level of the client library. invalue must be a const int *; outvalue must be a int *. Valid debug levels are LDAP_DEBUG_ANY, LDAP_DEBUG_ARGS, LDAP_DEBUG_BER, LDAP_DEBUG_CONNS, LDAP_DEBUG_NONE, LDAP_DEBUG_PACKETS, LDAP_DEBUG_PARSE, and LDAP_DEBUG_TRACE. This option is OpenLDAP specific.

Seems clear, however, if you try to use one of the above mentioned enums, it will probably fail. The reason is that those enums are only defined in `<ldap_log.h>`. Also, if you try to include `<ldap_log.h>`, it likely won't work either. As `<ldap_log.h>` is not shipped with the `libldap2-dev` package on Ubuntu (or `openldap-devel` on RedHat). Thus, the only proper thing you can do is just defining the same enums in your code where they are needed. So, for your convenient, here is a copy of all the debug levels defined in `<ldap_log.h>`:

```cpp
{% include src/2018-12-29-LDAP-global-vs-handle-options/ldap-debug-enums.hpp %}
```

Another quirky about setting LDAP debug level is that, apparently, it only works correctly if it is set to global options before opening any connections; In other words, it doesn't work if it is set to an individual LDAP handle which is got by calling `ldap_initialize()`.


# Solution

Putting it all together, here is an example of the basic steps about how to set OpenLDAP debug level correctly:

```cpp
{% include src/2018-12-29-LDAP-global-vs-handle-options/set-debug-level-example.hpp %}
```

Again, here I copied the debug level enums from `<ldap_log.h>` in order to use it in my own code, and I passed `nullptr` as the first argument of `ldap_set_option()` to indicate that I want to set the global options.


# Example

For a slightly more completed example, one may want to take a look at [the source code companions this post on Github]({{site.github.repository_url}}/tree/master/_includes/src/2018-12-29-LDAP-global-vs-handle-options).


# Conclusion

I'm surprised that how a seemingly simple task like this (setting debug level) turns out to be much more involved than I could imagine, mostly because of poor documentation. Especially, given how long has OpenLDAP been around.

Anyway, in this post, I have shown you the proper way to turn on OpenLDAP's internal debug logging. I would expect my post will fill some of the documentation gaps.


# References

1. [ldap_set_option(3) - Linux man page](https://linux.die.net/man/3/ldap_set_option)
1. [LDAP: Programming Directory-Enabled Apps (1st Edition) (#ad)](https://www.amazon.com) by Mark Smith.
