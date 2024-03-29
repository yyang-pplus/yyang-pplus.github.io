#pragma once

#include <string>

#include <ldap.h>

#include "ldap-debug-enums.hpp"


const std::string URI = "ldap://";
const std::string BASE = "dc=yyang-pplus,dc=github,dc=io";
constexpr auto SCOPE = LDAP_SCOPE_BASE;
constexpr auto DEBUG_LEVEL = LDAP_DEBUG_ANY;
