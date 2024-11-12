#!/bin/bash
exit_status=0

launch() {
   local executable_path="./webserv"
   local config_path=$1

   $executable_path $config_path &
   SERVER_PID=$!

   if ps -p $SERVER_PID > /dev/null
   then
      echo "Le serveur a démarré avec succès."
      echo $SERVER_PID > .server_pid
   else
      echo "Échec du démarrage du serveur."
      exit 1
   fi
   echo
}

run_test() {
   local test_script=$1

   python3 $test_script &
   TEST_PID=$!

   wait $TEST_PID

   if [ $? -eq 0 ]
   then
      echo "Le test a réussi."
   else
      echo "Le test a échoué."
      exit_status=1
   fi
}

kill_server() {
   kill -9 $SERVER_PID
   rm -f .server_pid
   echo
   echo "Server killed."
   echo
}

echo
echo "=============[Classic Return Code Test]============="
echo

touch tests/python/components/delete_me.html && touch tests/python/components/no_perm.html
chmod 000 tests/python/components/no_perm.html

launch "tests/python/conf/classic.conf"
sleep 1
run_test "tests/python/request_return_code.py"


kill_server
rm -f tests/python/components/no_perm.html

echo
echo "=============[Cgi Body Cookies Query Strings]============="
echo

launch "tests/python/conf/test_website.conf"
sleep 1
run_test "tests/python/request_body.py"

kill_server



exit $exit_status