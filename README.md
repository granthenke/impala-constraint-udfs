impala-constraint-udfs
======================
Impala UDFs to help validate and constrain the data in your tables.


CHECK UDFs:
-----------
The CHECK function mimics CHECK constraints from other SQL databases. A more in depth description can be found [here](http://en.wikipedia.org/wiki/Check_constraint).

The main difference is that instead of enforcing the constraints at the table level, they are enforced at the statement level by adding them to the WHERE clause of a query.

The CHECK function can act as a guard clause to an insert statement causing the insert to fail if bad data is found. It can also be used for validation by failing a query if bad data is found to be in an existing table.


**CHECK Behavior:**

1. A query will fail immediately when the first row that fails is found.
2. For that row, if more than one check statement is in the query the first to fail will be reported.
3. For that check statement, if there is more than one expression all results be reported in the error message.


**CHECK Function Reference:**

    *CHECK(boolean constraints ...)*
       Purpose: Fails a query if any of the passed boolean expressions are false
       Return type: boolean

    *CHECK(string message, boolean constraints ...)*
       Purpose: Fails a query if any of the passed boolean expressions are false. Includes a custom message in the logged failure.
       Return type: boolean


*Example:*
```
#Constrain Inserts
INSERT INTO other_table
SELECT *
FROM some_table
WHERE CHECK(col1 IS NOT NULL);

#Constrain Inserts (with custom message)
INSERT INTO other_table
SELECT *
FROM some_table
WHERE CHECK("col2 must be less than 10", col2 < 10);

#Validate data
SELECT *
FROM other_table
WHERE CHECK(col1 IS NOT NULL, col2 < 10);
```


How To Build:
-------------
1. Install the required packages to build Impala UDFs ([Documentation](http://www.cloudera.com/content/cloudera/en/documentation/cloudera-impala/latest/topics/impala_udf.html#udf_building_unique_1))
2. cmake .
3. make

*Example:*
```
yum-config-manager --add-repo http://archive.cloudera.com/cdh5/redhat/5/x86_64/cdh/cloudera-cdh5.repo
yum-config-manager --enable cloudera-cdh5

sudo yum install gcc-c++ cmake boost-devel
sudo yum install impala-udf-devel

git clone git@github.com:granthenke/impala-constraint-udfs.git
cd impala-constraint-udfs
cmake .
make
```


How To Deploy:
--------------
1. Copy the built shared library to an HDFS location readable by Impala. ([Documentation](http://www.cloudera.com/content/cloudera/en/documentation/cloudera-impala/latest/topics/impala_udf.html#udf_building_unique_1))
2. In Impala execute "CREATE FUNCTION" calls to register the functions you would like to use.

*Example:*
```
# Deploy to HDFS
hadoop fs -mkdir /user/impala/udfs
hadoop fs -put ./build/libudfcheck.so /user/impala/udfs/libudfcheck.so

# Connect to Impala
impala-shell -i my.impalad.host.com

# Deploy Functions
DROP FUNCTION IF EXISTS check(string, boolean ...);
CREATE FUNCTION check(string, boolean ...) returns boolean location '/user/impala/udfs/libudfcheck.so' symbol='Check';

DROP FUNCTION IF EXISTS check(boolean ...);
CREATE FUNCTION check(boolean ...) returns boolean location '/user/impala/udfs/libudfcheck.so' symbol='Check';

# Exit Impala shell
quit;
```


Impala Bugs:
------------
There are a few bugs that may effect the use of these functions.

- [IMPALA-1756](https://issues.cloudera.org/browse/IMPALA-1756) - Errors in functions with constant arguments may not fail the query
- [IMPALA-1746](https://issues.cloudera.org/browse/IMPALA-1746) - Errors in functions in the select clause may not fail the query
- [IMPALA-1748](https://issues.cloudera.org/browse/IMPALA-1748) - Functions must be re-registered each time Impala is bounced
- [IMPALA-1407](https://issues.cloudera.org/browse/IMPALA-1407) - Functions are scoped to the default database by default.


Backlog:
--------
- Improve "CHECK" function docs and improve sample queries
- Add Deploy/Register scripts
- Add "show" functions to show rows fail check constraints
- Create parody UDFs for Hive

Useful Links:
-------------
- [Impala UDF Development Documentation](http://www.cloudera.com/content/cloudera/en/documentation/cloudera-impala/latest/topics/impala_udf.html)
- [Sample Impala UDF Project](https://github.com/cloudera/impala-udf-samples)
- [Impala Community](http://impala.io/community.html)
