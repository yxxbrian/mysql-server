# ==== Purpose ====
#
# This file restarts a server after replacing it relay log info repository file by a custom one
#
# ==== Usage ====
#
# --sub_testcase= <sub testcase name>
#
# Parameters:
#  $slave_server_num
#    The number of the slave server for which operations in this script
#    are to be performed.
#  $sub_testcase
#    The test case name for which file repo is to be prepared.

--write_file $MYSQL_TMP_DIR/create_file_repo_with_PCU_has_@.inc END_OF_PROCEDURE
  --write_file $MYSQLD_DATADIR/$filename
10
./slave-relay-bin.000001
4

0
0
0
1

replication_applier
l@calhost
EOF
END_OF_PROCEDURE

--write_file $MYSQL_TMP_DIR/create_file_repo_with_anon_PCU.inc END_OF_PROCEDURE
  --write_file $MYSQLD_DATADIR/$filename
10
./slave-relay-bin.000001
4

0
0
0
1


localhost
EOF
END_OF_PROCEDURE

--write_file $MYSQL_TMP_DIR/create_file_repo_with_nonexist_PCU.inc END_OF_PROCEDURE
  --write_file $MYSQLD_DATADIR/$filename
10
./slave-relay-bin.000001
4

0
0
0
1

replication_applier_nonexist
localhost
EOF
END_OF_PROCEDURE

--write_file $MYSQL_TMP_DIR/create_file_repo_with_PCU.inc END_OF_PROCEDURE
  --write_file $MYSQLD_DATADIR/$filename
10
./slave-relay-bin.000001
4

0
0
0
1

replication_applier
localhost
EOF
END_OF_PROCEDURE


--let $MYSQLD_DATADIR= `select @@datadir`
--let $filename = `SELECT @@relay_log_info_file`

--let $rpl_server_number = $slave_server_num
--let $rpl_force_stop=1
--source include/rpl_stop_server.inc
--remove_file $MYSQLD_DATADIR/$filename

if ($sub_testcase == default) {
  --source $MYSQL_TMP_DIR/create_file_repo_with_PCU.inc
}
if ($sub_testcase == host_name_has_@) {
  --source $MYSQL_TMP_DIR/create_file_repo_with_PCU_has_@.inc
}
if ($sub_testcase == anonymous_user) {
  --source $MYSQL_TMP_DIR/create_file_repo_with_anon_PCU.inc
}
if ($sub_testcase == nonexist_user) {
  --source $MYSQL_TMP_DIR/create_file_repo_with_nonexist_PCU.inc
}
--let $rpl_server_parameters=--skip_slave_start

--let $explicit_default_counter = 15000
--source include/rpl_start_server.inc

--remove_file $MYSQL_TMP_DIR/create_file_repo_with_PCU.inc
--remove_file $MYSQL_TMP_DIR/create_file_repo_with_PCU_has_@.inc
--remove_file $MYSQL_TMP_DIR/create_file_repo_with_anon_PCU.inc
--remove_file $MYSQL_TMP_DIR/create_file_repo_with_nonexist_PCU.inc
