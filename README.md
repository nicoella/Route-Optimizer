# Route Optimizer

A modified version of the Held-Karp algorithm to solve a modified Traveling Salesman problem. Finds the shortest route to visit exactly one node in each group and return to the origin.

# Usage

Make sure Boost.Beast and CMake are installed.

## Backend

Update CMakeLists.txt with file paths (if necessary).

Build and run the server:

```
$ cd backend
$ chmod +x run.sh
$ ./run.sh
```

## Frontend

Setup the frontend (first time):

```
cd frontend
chmod +x setup.sh
./setup.sh
```

Update your Google Cloud API key in `frontend/src/config.json`.

Run the frontend:

```
$ cd frontend
$ npm run serve
```
