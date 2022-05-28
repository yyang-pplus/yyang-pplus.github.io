---
title: "Manage and Use LDAP Servers without RootDN"
date: "2018-12-21"
categories:
  - "ldap"
tags:
  - "access-control"
  - "cnconfig"
  - "it"
  - "ldapadd"
  - "ldapi"
  - "ldapmodify"
  - "linux"
  - "network"
  - "openldap"
  - "rootdn"
  - "sasl"
  - "security"
---

Are you tired of typing password when running **LDAP** commands with **-W**, but feel nervous about using plain text password with **-w**? Are you looking for a way to run **LDAP** commands that is both passwordless and secure? Then this post is for you.


# Introduction

The differences between the following two commands are pretty clear to people who are familiar with LDAP administration.

```bash
sudo ldapadd -H ldapi:// -Y EXTERNAL -Q
```

```bash
ldapadd -H ldap:// -x -D cn=admin,dc=yyang-pplus,dc=github,dc=io -W
```

As we have already discussed in [my previous post]({{page.previous.url}}), basically, the first command uses the **SASL authentication** method through **ldapi** interface to bind as the superuser of the localhost, while the second command utilizes **simple authentication** with normal **ldap** scheme to bind as the so-called **rootDN** of the **DIT**. For ease of discussion, in the rest part of the post, let's name the first command as the **ldapi command**, and call the second command as the **rootDN command**.[<sup>\[1\]</sup>](#references)

In general, people use the rootDN commands to maintain the content of LDAP databases in the **normal DIT**. And use the ldapi commands to manage the configuration of LDAP servers within the **administrative DIT**, which also known as the **cn=config DIT**.[<sup>\[1\]</sup>](#references) Those two kinds of commands exist because that's just how LDAP servers are configured by default.

In this post, we are going to present a way to manage both the administrative DIT and the normal DIT using only the ldapi commands. To be more specific, we want to be able to run the following test command successfully:

```bash
$ sudo ldapadd -H ldapi:// -Y EXTERNAL -Q <<EOF
dn: dc=yyang-pplus,dc=github,dc=io
objectClass: dcObject
objectClass: organization
o: yyang-pplus
EOF
```

Instead of getting an error message, says:

```bash
ldap_add: Insufficient access (50)
    additional info: no write access to parent
```


# Environment

All the code and commands, posted here, were tested in the following environment:

```bash
$ cat /etc/redhat-release
CentOS release 6.10 (Final)

$ ldapadd -VV
ldapadd: @(#) $OpenLDAP: ldapmodify 2.4.40 (Mar 22 2017 06:29:57)
```

Also, the test command, given in the [precious section](#introduction), require the LDAP server has domain suffix configured properly. To check if your environment meets the requirement, run the following command:

```bash
$ sudo ldapsearch -H ldapi:// -Y EXTERNAL -b "cn=config" "(olcSuffix=*)" olcSuffix -LLL -Q
dn: olcDatabase={2}bdb,cn=config
olcSuffix: dc=yyang-pplus,dc=github,dc=io
```

You should get similar outputs on your machine, if not, I recommend you follow the instructions of this post "[Configure LDAP Server](https://www.server-world.info/en/note?os=CentOS_6&p=ldap)" to setup the test bed properly.


# Solution

The solution is actually pretty simple, we just use the same **authentication DN** for root or sudo users of the OS in the place of where rootDN usually is.

```bash
$ sudo ldapmodify -Y EXTERNAL -H ldapi:// -Q <<EOF
dn: olcDatabase={2}bdb,cn=config
changetype: modify
replace: olcRootDN
olcRootDN: gidNumber=0+uidNumber=0,cn=peercred,cn=external,cn=auth
EOF
modifying entry "olcDatabase={2}bdb,cn=config"
```

Line 2 `dn: olcDatabase={2}bdb,cn=config` is the DN of the normal DIT you want the ldapi commands to have access to, and it may vary from server to server. Please refer to the last command in the [Environment section](#environment) for an example of how to get the normal DIT DN on your machine.

Line 5 `olcRootDN: gidNumber=0+uidNumber=0,cn=peercred,cn=external,cn=auth` is the authentication DN for the localhost's superuser. Please check [my previous post]({{page.previous.url}}) for how to query this information from the openLDAP servers.

This solution works, because the **olcRootDN** directive need not refer to an entry in the database or even in the directory, and it may refer to a **SASL identity**. Also, regardless of what access control policy is defined, the olcRootDN is always allowed full rights (i.e. auth, search, compare, read and write) on everything and anything within the DIT.[<sup>\[2\]</sup>](#references)


# Test

When we run our test command again, we should now get something like this:

```bash
adding new entry "dc=yyang-pplus,dc=github,dc=io"
```

Which means our solution works as expected. Just to double-check, let's query the entry we have just added:

```bash
$ ldapsearch -H ldap:// -x -b "dc=yyang-pplus,dc=github,dc=io" -LLL
dn: dc=yyang-pplus,dc=github,dc=io
objectClass: dcObject
objectClass: organization
o: yyang-pplus
dc: yyang-pplus
```

Easy. Right?

Also note how the **dc** attribute was added automatically to the entry by the LDAP server.


# Conclusion

Well, you may ask: why does anyone bother to do something like this?

Because it is quite convenient. In this way, you don't need the rootDN and its password anymore. I think having one less thing to remember is always good. Also, in the case where simple authentication is required, like by some LDAP-enabled applications, you can just assign a value to the **olcRootPW** directive, which will turn on simple authentication as well.


# References

1. [Introduction to ldapwhoami and LDAP Authtication Mechanisms]({{page.previous.url}})
1. [OpenLDAP 2.4 Administrator’s Guide: Access Control](https://www.openldap.org/doc/admin24/access-control.html).
