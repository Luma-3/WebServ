#!/bin/bash

launch() {
   local executable_path="./webserv"
   local config_path=$1

   $executable_path $config_path &
   SERVER_PID=$!

   if ps -p $SERVER_PID > /dev/null
   then
      echo "Le serveur a démarré avec succès."
   else
      echo "Échec du démarrage du serveur."
      exit 1
   fi
   echo "$SERVER_PID" > .server_pid

   echo
}


echo
echo "=============[Classic Return Code Test]============="
echo

touch tests/python/components/delete_me.html && touch tests/python/components/no_perm.html

chmod 000 tests/python/components/no_perm.html

launch "tests/python/conf/classic.conf"

echo 
echo "=============[Test Header Response]============="
echo





echo
echo "=============[Test Query string]============="
echo






echo
echo "=============[Test Cookies]============="
echo