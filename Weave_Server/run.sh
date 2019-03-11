## Run the newest build in the bin folder with valgrind
valgrind --leak-check=full --log-file=errors ./Weave_Server/bin/Weave_Server -c server.conf

echo "VALGRIND AT EXIT: "
grep 'at exit' errors

