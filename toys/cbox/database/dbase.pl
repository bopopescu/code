#!/usr/bin/perl

# https://www.tutorialspoint.com/sqlite/sqlite_perl.htm


use strict;
use warnings;
use DBI;

my $driver   = "SQLite"; 
my $database = "test.db";
my $dsn = "DBI:$driver:dbname=$database";
my $userid = "";
my $password = "";
our $dbh = DBI->connect($dsn, $userid, $password, { RaiseError => 1 }) 
   or die $DBI::errstr;

print "Opened database successfully\n";

sub make_table {
    our $dbh ;
    my $stmt = qq(CREATE TABLE COMPANY
       (ID INT PRIMARY KEY     NOT NULL,
          NAME           TEXT    NOT NULL,
          AGE            INT     NOT NULL,
          ADDRESS        CHAR(50),
          SALARY         REAL););

    my $rv = $dbh->do($stmt);
    if($rv < 0) {
       print $DBI::errstr;
    } else {
       print "Table created successfully\n";
    }
    $dbh->disconnect();
}

sub populate {
    our $dbh ;
    my $stmt = qq(INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)
               VALUES (1, 'Paul', 32, 'California', 20000.00 ));
    my $rv = $dbh->do($stmt) or die $DBI::errstr;

    $stmt = qq(INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)
                   VALUES (2, 'Allen', 25, 'Texas', 15000.00 ));
    $rv = $dbh->do($stmt) or die $DBI::errstr;

    $stmt = qq(INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)
                   VALUES (3, 'Teddy', 23, 'Norway', 20000.00 ));

    $rv = $dbh->do($stmt) or die $DBI::errstr;

    $stmt = qq(INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)
                   VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 ););

    $rv = $dbh->do($stmt) or die $DBI::errstr;

    print "Records created successfully\n";
    $dbh->disconnect();

}

sub show {
    our $dbh ;
    my $stmt = qq(SELECT id, name, address, salary from COMPANY;);
    my $sth = $dbh->prepare( $stmt );
    my $rv = $sth->execute() or die $DBI::errstr;

    if($rv < 0) {
       print $DBI::errstr;
    }

    while(my @row = $sth->fetchrow_array()) {
          print "ID = ". $row[0] . "\n";
          print "NAME = ". $row[1] ."\n";
          print "ADDRESS = ". $row[2] ."\n";
          print "SALARY =  ". $row[3] ."\n\n";
    }
    print "Operation done successfully\n";
    $dbh->disconnect();
    
}

#populate();
show();

