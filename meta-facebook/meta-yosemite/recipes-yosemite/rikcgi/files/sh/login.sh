#!/bin/bash

logger "login.sh"

# echo "{\"login\":\"Root\",\"key\":\"1234abcd\"}"
exec /usr/bin/rikcgi-login
