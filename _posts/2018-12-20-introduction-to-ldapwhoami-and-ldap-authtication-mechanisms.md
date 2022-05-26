---
title: "Introduction to ldapwhoami and LDAP Authtication Mechanisms"
date: "2018-12-20"
categories:
  - "ldap"
tags:
  - "access-control"
  - "acl"
  - "authtication"
  - "cnconfig"
  - "it"
  - "ldapi"
  - "ldapwhoami"
  - "linux"
  - "openldap"
  - "rootdn"
  - "sasl"
  - "security"
---

The **ldapwhoami** command can tell you how the LDAP server sees you after authenticating.[<sup>\[1\]</sup>](#references) I found it is pretty helpful for understanding LDAP authentication mechanisms. In this post, we are going to present an introduction to **ldapwhoami** and discuss briefly about LDAP authentication mechanisms.


# Environment

All the code and commands, posted here, were tested in the following environment:

```bash
$ cat /etc/redhat-release
CentOS Linux release 7.3.1611 (Core)

$ ldapwhoami -VV
ldapwhoami: @(#) $OpenLDAP: ldapwhoami 2.4.40 (Mar 22 2017 06:29:57)
```

In additionally, Some commands require the LDAP servers have rootDN and suffix configured properly. To check if your environment meets the requirement, run the following command:

```bash
$ sudo ldapsearch -H ldapi:// -Y EXTERNAL -b "cn=config" "(olcSuffix=*)" olcSuffix olcRootDN olcRootPW -LLL -Q
dn: olcDatabase={2}hdb,cn=config
olcSuffix: dc=yyang_pplus,dc=github,dc=io
olcRootDN: cn=admin,dc=yyang_pplus,dc=github,dc=io
olcRootPW: {SSHA}8jr+DF1KahY4x1W0LlVHUFnKivxsoKLe
```

You should get similar outputs on your machine, if not, I recommend you follow the instructions of this post "[Step by Step OpenLDAP Server Configuration on CentOS 7 / RHEL 7](https://www.itzgeek.com/how-tos/linux/centos-how-tos/step-step-openldap-server-configuration-centos-7-rhel-7.html)" to setup the test bed properly.


# Discussion

Let's first start with an easy one:

```bash
$ ldapwhoami -H ldap:// -x
anonymous
```

This command shows it use anonymous authentication. The **\-H ldap://** specifies an unencrypted LDAP query on localhost. The **\-x** without any authentication information lets the server know you want an anonymous connection.[<sup>\[2\]</sup>](#references)

Similarly, for simple authentication, **ldapwhoami** simply shows the exact entry you are binding to.

```bash
$ ldapwhoami -H ldap:// -x -D cn=admin,dc=yyang_pplus,dc=github,dc=io -W
dn:cn=admin,dc=yyang_pplus,dc=github,dc=io
```

The **\-D** option is used to specify the bind DN. Binding to an entry often gives you additional privileges that are not available through an anonymous bind. Binding to the rootDN always gives you full access to the entire **DIT (Data Information Trees)**, since rootDN is not subject to access control or administrative limit restrictions for operations within the database. As a consequence, it's useless (and results in a performance penalty) to explicitly list the rootDN among the clauses. Also, rootDN need not refer to an entry in the database or even in the directory, and it may refer to a SASL identity. [<sup>\[1\]</sup>](#references)[<sup>\[5\]</sup>](#references)[<sup>\[6\]</sup>](#references) For an example of what this really means, feel free to check my other LDAP post: [{{page.next.title}}]({{page.next.url}}).

For the SASL authentication methods, the output is a bit more interesting, it provides more information on how the authentication mechanism is being seen.[<sup>\[1\]</sup>](#references)

```bash
$ sudo ldapwhoami -H ldapi:// -Y EXTERNAL -Q
dn:gidNumber=0+uidNumber=0,cn=peercred,cn=external,cn=auth
```

The **\-Y** option is used to indicate that we want to use a SASL authentication method. The **EXTERNAL** mechanism indicates that authentication and security is handled by some other means associated with the connection. For instance, it can be used with SSL to provide encryption and authentication. Most commonly, you will see it used with the **ldapi://** interface with the root or sudo users. The **ldapi://** indicates LDAP over an **IPC**. It communicates over a Unix socket instead of using an exposed network port, thus the user initiating the request can be obtained, and used to authenticate for certain operations. Note the **authentication DN**, i.e. the output, of the above command: **dn:gidNumber=0+uidNumber=0,cn=peercred,cn=external,cn=auth**. This is not an actual entry in our DIT, it is just how SASL authentication gets translated into a format that LDAP can understand.[<sup>\[1\]</sup>](#references)[<sup>\[2\]</sup>](#references)[<sup>\[3\]</sup>](#references)

By the way, Since the ldapi scheme requires a local connection, we never will have to specify a server name here. However, if you changed the socket-file location within the LDAP server configuration, you will need to specify the new socket location as part of the address.[<sup>\[1\]</sup>](#references)

In general, people use the binding to rootDN commands to maintain the content of LDAP databases in the **normal DIT**. And use the sudo with ldapi commands to manage the configuration of LDAP servers within the **administrative DIT**, also known as the **cn=config DIT**.

Seeing the authentication DN can be used to create mappings and access restrictions. Although a full discussion of access control is generally outside the scope of this article, I do want to show an example of how to query the **ACLs (Access control lists)** from LDAP servers.

```bash
$ sudo ldapsearch -H ldapi:// -Y EXTERNAL -b "cn=config" "olcAccess=*" olcAccess -LLL -Q
dn: olcDatabase={0}config,cn=config
olcAccess: {0}to *
  by dn.base="gidNumber=0+uidNumber=0,cn=peercred,cn=external,cn=auth" manage
  by * none

dn: olcDatabase={1}monitor,cn=config
olcAccess: {0}to *
  by dn.base="gidNumber=0+uidNumber=0,cn=peercred,cn=external,cn=auth" read
  by dn.base="cn=manager,dc=my-domain,dc=com" read
  by * none
```

The above command shows the current configuration on my machine. This is also the default configuration on CentOS. Notice how the same authentication DN for the localhost's superuser is mentioned in the output. This is how it is configured, by default, to allow administration for root or sudo users of the OS.[<sup>\[2\]</sup>](#references)


# Conclusion

In this article we barely scratched the surface of the LDAP authentication mechanisms. For the audiences who want to know more, I recommend refer to [OpenLDAP Software Administrator's Guide](https://www.openldap.org/doc/admin24/) or textbooks, such as [Understanding and Deploying LDAP Directory Services<sup>\[4\]</sup>](#references).


# References

1. [How To Manage and Use LDAP Servers with OpenLDAP Utilities](https://www.digitalocean.com/community/tutorials/how-to-manage-and-use-ldap-servers-with-openldap-utilities)
1. [How To Configure OpenLDAP and Perform Administrative LDAP Tasks](https://www.digitalocean.com/community/tutorials/how-to-configure-openldap-and-perform-administrative-ldap-tasks)
1. [Understanding the LDAP Protocol, Data Hierarchy, and Entry Components](https://www.digitalocean.com/community/tutorials/understanding-the-ldap-protocol-data-hierarchy-and-entry-components)
1. [Understanding and Deploying LDAP Directory Services (#ad)](https://www.amazon.com)
1. [OpenLDAP 2.4 Administrator's Guide: The slapd Configuration File](https://www.openldap.org/doc/admin24/slapdconfig.html)
1. [OpenLDAP 2.4 Administrator's Guide: Access Control](https://www.openldap.org/doc/admin24/access-control.html)
