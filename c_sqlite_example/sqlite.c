#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 

// https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm
// https://sqlite.org/cintro.html
// https://zetcode.com/db/sqlitec/
// https://medium.com/@duthujoe/sqlite-c-a-beginners-guide-243beb3fb3de
// https://www.openmymind.net/Writing-A-Custom-Sqlite-Function-Part-1/
// https://lloydrochester.com/post/c/sqlite-api/
// https://www.sqlitetutorial.net/
// https://sqlite.org/cli.html
// https://docs.sqlc.dev/en/v1.24.0/tutorials/getting-started-sqlite.html
// https://www.thoughtco.com/creating-populating-running-database-sql-query-958233
// https://www.linuxjournal.com/content/accessing-sqlite-c
// https://docs.python.org/3/library/sqlite3.html

// gcc sqlite.c -lsqlite3

static int callback_create(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for(i = 0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

static int callback_insert(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for(i = 0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

static int callback_select(void *data, int argc, char **argv, char **azColName){
	int i;
	fprintf(stderr, "%s: ", (const char*)data);
   
	for(i = 0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
   
	printf("\n");
   return 0;
}

int main(int argc, char* argv[]) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;

	/* Open database */
	rc = sqlite3_open("test.db", &db);
	if( rc ) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return(0);
	} else {
		fprintf(stdout, "Opened database successfully\n");
	}

	/* Create SQL statement */
	sql =	"CREATE TABLE COMPANY("  \
			"ID INT PRIMARY KEY	  NOT NULL," \
			"NAME			  TEXT	 NOT NULL," \
			"AGE				INT	  NOT NULL," \
			"ADDRESS		  CHAR(50)," \
			"SALARY			REAL );";
	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback_create, 0, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		fprintf(stdout, "Table created successfully\n");
	}

	/* Insert SQL statement */
	sql =	"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
			"VALUES (1, 'Paul', 32, 'California', 20000.00 ); " \
			"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
			"VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "     \
			"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
			"VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
			"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
			"VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";
	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback_insert, 0, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		fprintf(stdout, "Records created successfully\n");
	}

	printf("\n");
	const char* data = "Callback function called";
	/* Select SQL statement */
	sql = "SELECT * from COMPANY";
	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback_select, (void*)data, &zErrMsg);
	if( rc != SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
	fprintf(stdout, "Operation done successfully\n");
	}

	sqlite3_close(db);

	return 0;
}