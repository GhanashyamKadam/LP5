import sqlite3
import concurrent.futures
import time

def exe_sql_qry(query):
    try:
        conn = sqlite3.connect('mydb.db')
        cursor = conn.cursor()
        cursor.execute(query)

        if query.strip().lower().startswith('select'):
            result = cursor.fetchall()
        else:
            conn.commit()
            result = 'Query Executed...'

        conn.close()
        return result

    except Exception as e:
        return f'error occurred: {str(e)}'

# Step 1: Setup the database with table and data
setup_queries = [
    "DROP TABLE IF EXISTS users;",
    "CREATE TABLE users (id INTEGER PRIMARY KEY, name TEXT, mono TEXT);",
    "INSERT INTO users VALUES (1, 'Sanket', '90000000');",
    "INSERT INTO users VALUES (2, 'Tanmay', '80000000');",
    "INSERT INTO users VALUES (3, 'Sanket', '90000001');",
    "INSERT INTO users VALUES (4, 'Aniket', '90000002');",
    "INSERT INTO users VALUES (5, 'Omkar', '90000003');"
]

print("Running setup queries...")
for q in setup_queries:
    print(exe_sql_qry(q))

# Step 2: Define SELECT queries to be executed
queries = [
    "SELECT * FROM users;",
    "SELECT name FROM users WHERE mono = '90000000';",
    "SELECT COUNT(*) FROM users;"
]

# Step 3: Run queries in parallel using ThreadPoolExecutor
s1time = time.time()
with concurrent.futures.ThreadPoolExecutor() as executor:
    results1 = list(executor.map(exe_sql_qry, queries))
e1time = time.time()

# Step 4: Run queries sequentially
s2time = time.time()
results2 = list(map(exe_sql_qry, queries))
e2time = time.time()

# Step 5: Display results
print("\nParallel Execution Results:")
for result in results1:
    print(result)

print("\nSequential Execution Results:")
for result in results2:
    print(result)

print(f"\nTime Taken (Parallel): {e1time - s1time:.6f} seconds")
print(f"Time Taken (Sequential): {e2time - s2time:.6f} seconds")
