# SafeRoute SOS – Project Explanation

Each item: **What** it is → **Why** we used it → **Where** in the project.

## A. Classes

| Class | Purpose | Key members |
|---|---|---|
| `User` | One account | private `name, username, email, password`; `checkPassword()` compares without exposing the password |
| `Contact` | One emergency contact | `name, phone (string), relation` |
| `SOS` | One emergency request | `id, username, emergencyType, location, priority, status`; `getPriorityLevel()` converts text to a number (Critical=4 … Low=1) |
| `Facility` | Hospital / police station (sample) | `id, name, type, area, phone` |
| `Graph` | Map of locations and roads | adjacency list, `bfs()`, `dijkstra()` |
| `SOSHistory` | Linked list of SOS records | `addRecord, displayForUser, findById, searchByStatus` |
| `EmergencyManager` | Controls everything | owns the vector, queue, stack, list and graph; one function per menu option |

`main.cpp` only shows menus and calls `EmergencyManager`. This separation is what we call **modular design**.

## B. Data structures

**Array / Vector** – Fixed-size array (`const string types[5]`, `priorities[4]`) holds the menu choices. `vector<Contact>` stores contacts because the number is not known in advance. *Where:* `createSOS()`, `contacts` in `EmergencyManager`. **Delete** = find index → shift later elements one step left → `pop_back()`.

**Linked List** – Nodes connected by `next` pointers. We use it for **SOS history** because history only grows at the end (add at tail, O(1)) and is read from start to end. *Where:* `struct Node`, `class SOSHistory` in `SOS.h/.cpp`.

**Stack (LIFO)** – `stack<int>` stores the IDs of SOS created in the current login. **Undo Last SOS** pops the most recent ID and cancels it. *Why a stack:* the last action must be undone first. *Where:* `undoStack`, `undoLastSOS()`.

**Queue (FIFO)** – `queue<int>` inside BFS holds locations waiting to be visited. *Where:* `Graph::bfs`.

**Priority Queue** – Like a queue, but the *most important* element leaves first. `priority_queue<SOS, vector<SOS>, CompareSOS>` stores pending SOS. *Where:* `sosQueue`, `processNextSOS()`.
- **Comparator `CompareSOS`:** `priority_queue` keeps the "largest" element on top. `operator()(a, b)` returns `true` if **a should come after b**. So lower priority level → after. If levels are equal, the bigger ID (newer SOS) → after, so older SOS is served first.
- Example: 101 Low, 102 Critical, 103 High → processed 102, 103, 101.

**Hash Table** – `unordered_map<string,int>` maps a location name (lower-case) to its index in the graph, so `findLocation()` is O(1) on average instead of scanning every name. *Where:* `Graph::indexOf`. (We did *not* force hash tables elsewhere.)

**Graph (adjacency list)** – Vertices = locations, edges = roads, weight = distance. `adj[u]` is a list of `(neighbour, distance)`. Chosen over a matrix because the map is sparse (few roads). Roads are two-way, so each road is stored twice. *Where:* `Graph.h/.cpp`.

## C. Algorithms

**BFS** – Uses a queue; visits all locations 1 road away, then 2 roads away, … (level by level). Used for **nearby facilities**: start location is level 0, and facilities are printed at the levels where they are found. *Note:* BFS counts roads, **not km**, so it is for "nearby by hops", not shortest distance.

**Dijkstra** – Finds the shortest distance on a weighted graph with **non-negative** weights.
1. `dist[start]=0`, others = infinity.
2. Take the location with the smallest `dist` (min-heap).
3. For each road, if `dist[u] + w < dist[v]`, update `dist[v]` and `parent[v] = u`.
4. Repeat until the heap is empty.
5. Rebuild the route from `parent[]` backwards and reverse it.
Weights must be non-negative because Dijkstra assumes a location, once finalised, can never get a shorter distance later.

**Searching (linear search)** – Look at items one by one. Used for contacts (partial, case-insensitive name), facilities (area/type), SOS (by ID/status). Fine for small data.

**Sorting (Bubble Sort)** – Compare neighbours, swap if in the wrong order, repeat. After every pass the weakest item moves to the end. Used in `sortSOS()` (highest priority first, then smaller ID). Works on a *copy*, so history and the queue are unchanged.

## D. OOP concepts
- **Class / Object:** blueprint / actual instance (`EmergencyManager manager;` in `main`).
- **Encapsulation:** data is `private`; outside code uses getters/setters (`user.checkPassword(p)`, not `user.password`).
- **Constructors:** default (`User()`) sets empty values; parameterized (`User(n,u,e,p)`) sets given values.
- **Member functions:** functions inside a class (`SOS::displayRow()`).
- **Modular programming:** each class has `.h` (declaration) and `.cpp` (definition).

## E. Time complexity (n = items, V = locations, E = roads)

| Operation | Complexity | Meaning in simple words |
|---|---|---|
| Linear search | O(n) | in the worst case look at every item |
| Array/vector delete (shift) | O(n) | up to n items move one place |
| Linked list traversal | O(n) | visit each node once |
| Linked list add at tail | O(1) | we keep a `tail` pointer |
| Stack push/pop/top | O(1) | only touches the top |
| Queue push/pop | O(1) | only touches front/back |
| Priority queue push/pop | O(log n) | heap re-arranges along one branch |
| Hash lookup (`unordered_map`) | O(1) average | direct jump by hash |
| BFS | O(V + E) | each location and road is processed once |
| Dijkstra (with heap) | O((V + E) log V) | like BFS but heap operations cost log |
| Bubble sort | O(n²) worst/avg, O(n) best | n passes × n comparisons; best case = already sorted (we stop early) |

## F. Files used
`users.txt` (accounts), `contacts_<username>.txt` (one file per user), `sos_history.txt` (all SOS records). Fields are separated by `|`. On start the history is loaded into the linked list and each user's **Pending** SOS are pushed back into the priority queue at sign-in.

## G. Design decisions (good to mention in viva)
- **SOS queue is per user:** after sign-in the queue holds only *your* pending SOS, so "Process Next SOS" is easy to demo.
- **Cancelled status for undo:** `priority_queue` cannot delete from the middle, so undo marks the SOS *Cancelled* and **rebuilds** the queue from history.
- **Usernames are lower-cased** and limited to letters/digits/underscore because they appear in a file name.
- **Phone as string:** keeps a leading `0`, and we never do arithmetic on phone numbers.
- **`getline(cin >> ws, s)`:** `ws` skips leftover whitespace/newline first, so an old `\n` in the buffer does not produce an empty input.
