# SafeRoute SOS – Test Cases

Tested with `g++ 13.3 -std=c++11 -Wall -Wextra` (no warnings) on Linux. Sessions were run with scripted
input and also with AddressSanitizer + UBSan (no errors reported).
"Actual result" = what the program really printed in testing.

| # | Test case | Input | Expected output | Actual result |
|---|---|---|---|---|
| 1 | Sign up (valid) | name `Rahul Sharma`, username `rahul_01`, `rahul@gmail.com`, `password123` | "Sign up successful!" | Same. Record saved in `users.txt` |
| 2a | Invalid email | `studentgmail.com` | Rejected, asks again | "Invalid email. Example: student@gmail.com" |
| 2b | Invalid email | `student@` | Rejected | Same message |
| 2c | Invalid email | `@gmail.com` | Rejected | Same message |
| 2d | Valid emails | `student@gmail.com`, `student123@yahoo.com` | Accepted | Both valid (checked with `isValidEmail`) |
| 3 | Short password | `short` (5 chars) | Rejected | "Password too short. It must have at least 8 characters." |
| 4a | Invalid phone | `98765abc10` | Rejected | "Invalid phone number. Enter exactly 10 digits (numbers only)." |
| 4b | Invalid phone | `987-654-3210` | Rejected | Same message |
| 4c | Invalid phone | `98765 43210` | Rejected | Same message |
| 4d | Valid phone | `9876543210` | Accepted | Contact added. Also `0123456789` kept with leading 0 |
| 4e | Phone length | `987654321` (9) / `98765432101` (11) | Rejected | Both invalid |
| 5 | Sign in (valid) | `rahul_01` / `password123` | Welcome + menu | "Welcome, Rahul Sharma!" and menu shown |
| 5b | Sign in, username in other case | `RAHUL_01` / `password123` | Accepted (not case-sensitive) | Welcome shown |
| 6a | Wrong password | `rahul_01` / `wrongpass` | Login refused, back to main menu | "Wrong username or password." |
| 6b | Unknown user | `nobody` / `password123` | Login refused | Same message |
| 6c | Duplicate username on sign up | `rahul_01` | Rejected | "Username already taken. Try another." |
| 6d | Invalid username | `x`, `Amit Kumar` | Rejected | "Invalid username. Use only letters, digits and underscore (3-20 characters)." |
| 6e | Invalid menu input | `abc`, `9` at main menu (1-3) | Asks again, no crash | "Invalid input. Please enter a number from 1 to 3." |
| 7 | Add contact | `Papa Sharma`, `9876543210`, `Father` (+ 2 more) | "Contact added successfully." | Same; saved to `contacts_rahul_01.txt` |
| 8a | Search contact (found) | `ri` | Riya Verma shown | Riya Verma row displayed |
| 8b | Search contact (missing) | `zzz` | "Contact Not Found." | Same |
| 9a | Delete contact | `riya verma` (case ignored) | Deleted, remaining shift left | "Contact deleted successfully."; list shows Papa, Mohit |
| 9b | Delete missing contact | `Riya Verma` again | "Contact Not Found." | Same |
| 10 | Create SOS | type 1, `Civil Lines`, priority 4 | SOS ID 101, status Pending | ID 101, Medical Emergency, Civil Lines, Low, Pending |
| 10b | Invalid priority | `9`, `0`, `abc` then `3` | "Invalid priority. Please try again." until valid | Message shown 3 times, then Medium accepted |
| 11 | Multiple SOS | 101 Low, 102 Medium, 103 Critical, 104 High | IDs increase by 1 | IDs 101-104 assigned |
| 12 | Priority queue order | Press "Process Next SOS" 5 times | 103 (Critical), 104 (High), 102 (Medium), 101 (Low), then "No emergency requests available." | Exactly this order; "Pending SOS left" counted 3,2,1,0 |
| 12b | Persistence of queue | Create Low (106) + Critical (107), exit, restart, sign in, Process Next | 107 first | 107 processed first; 106 remained Pending |
| 13 | SOS history | Menu 7 | All of this user's SOS in order of creation, with status | Table shown; statuses updated to Processed |
| 13b | Search SOS by ID | `107` / `999` | Found / "SOS not found." | Row shown / "SOS not found." |
| 13c | Search SOS by status | Pending / Processed / Cancelled | Matching rows | Correct rows for each status |
| 13d | Undo last SOS | Create 105 (Low), Undo | 105 Cancelled, removed from queue | "SOS 105 cancelled..."; history shows Cancelled |
| 13e | Undo on already processed SOS | Undo again (top = 104, processed) | Refused | "SOS 104 is already processed or cancelled, so it cannot be undone." |
| 14a | View facilities | Menu 9 | 5 sample facilities + sample-data note | Shown with note "predefined/sample project data" |
| 14b | Facility by area | `civil lines` | City Hospital | Shown (case ignored) |
| 14c | Facility unknown area | `Mars` | "No facilities found in this area." | Same |
| 14d | Hospitals / police | Options 2 and 3 | 3 hospitals / 2 police stations | Correct lists |
| 15a | BFS from Roorkee | `Roorkee` | Level 0: Roorkee; Level 1: Civil Lines, Railway Station, Dehradun; Level 2: University, Bus Stand; Level 3: Hospital, Police Station | Same; 5 facilities found |
| 15b | BFS from Dehradun | `dehradun` | Level 1: University, Roorkee | Same |
| 16a | Dijkstra | Roorkee → Hospital | 9 km, Roorkee → Civil Lines → University → Hospital | Same |
| 16b | Dijkstra (indirect better than direct) | Roorkee → Dehradun | 17 km via University, not direct 20 | "17 km", Roorkee → Civil Lines → University → Dehradun |
| 16c | Dijkstra other direction | Police Station → Roorkee | 8 km via Bus Stand, Railway Station | Same |
| 16d | Same start and end | Hospital → Hospital | 0 km | "0 km", route: Hospital |
| 17 | Sort SOS | Menu 13 with 101 Low, 102 Medium, 103 Critical, 104 High | 103, 104, 102, 101 | Same order |
| 18 | Invalid location | BFS: `Atlantis`; Dijkstra: `Roorkee` → `Paris` | "Location not found." | Same |
| 19a | Empty contacts | New user: view / search / delete | "No emergency contacts found." | Same |
| 19b | Empty SOS | New user: history, search, process, sort | "No emergency requests available." | Same |
| 19c | Empty undo | New login, Undo | "No recent SOS to undo." | Same |
| 19d | SOS isolation | Second user opens history | Does not see other user's SOS | "No emergency requests available." |
| 19e | Input ends suddenly | End of input (Ctrl+D / Ctrl+Z) | Program exits cleanly | "Input ended. Exiting program." |

**Note on the data:** Facilities, locations, distances and phone numbers are predefined sample data. Passwords are saved as plain text in `users.txt`.
